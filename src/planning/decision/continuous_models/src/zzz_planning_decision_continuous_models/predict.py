import numpy as np
import rospy
import matplotlib.pyplot as plt
import copy
import math

from Werling.trajectory_structure import Frenet_path, Frenet_state
from zzz_common.kinematics import get_frenet_state
from zzz_driver_msgs.utils import get_speed, get_yaw
from common import rviz_display, convert_ndarray_to_pathmsg, convert_path_to_ndarray
from zzz_common.geometry import dense_polyline2d



class predict():
    def __init__(self, dynamic_map, considered_obs_num, maxt, dt, robot_radius, radius_speed_ratio, move_gap, ego_speed):
        self.considered_obs_num = considered_obs_num
        self.maxt = maxt
        self.dt = dt
        self.check_radius = robot_radius + radius_speed_ratio * ego_speed
        self.move_gap = move_gap

        self.dynamic_map = dynamic_map
        self.initialze_fail = False

        self.rviz_collision_checking_circle = None
        self.rviz_collision_checking_circle_ego = None
        self.rivz_element = rviz_display()

        try:
            self.reference_path = self.dynamic_map.jmap.reference_path.map_lane.central_path_points
            ref_path_ori = convert_path_to_ndarray(self.reference_path)
            self.ref_path = dense_polyline2d(ref_path_ori, 2)
            self.ref_path_tangets = np.zeros(len(self.ref_path))

            self.obs = self.found_closest_obstacles()
            self.obs_paths = self.prediction_obstacle(self.obs, self.maxt, self.dt)
        except:
            rospy.logdebug("continous module: fail to initialize prediction")
            self.obs_paths = []
        
        
    def check_collision(self, fp):
        if len(self.obs_paths) == 0 or len(fp.t) < 2 :
            return True
            
        # two circles for a vehicle
        fp_front = copy.deepcopy(fp)
        fp_back = copy.deepcopy(fp)

        try:
            # fp_front.x = (np.array(fp.x) + np.cos(np.array(fp.yaw)) * self.move_gap).tolist()
            # fp_front.y = (np.array(fp.y) + np.cos(np.array(fp.yaw)) * self.move_gap).tolist()
            # fp_back.x = (np.array(fp.x) - np.cos(np.array(fp.yaw)) * self.move_gap).tolist()
            # fp_back.y = (np.array(fp.y) - np.cos(np.array(fp.yaw)) * self.move_gap).tolist()

            for t in range(len(fp.yaw)):
                fp_front.x[t] = fp.x[t] + math.cos(fp.yaw[t]) * self.move_gap
                fp_front.y[t] = fp.y[t] + math.sin(fp.yaw[t]) * self.move_gap
                fp_back.x[t] = fp.x[t] - math.cos(fp.yaw[t]) * self.move_gap
                fp_back.y[t] = fp.y[t] - math.sin(fp.yaw[t]) * self.move_gap

            for obsp in self.obs_paths:
            #     mind = min((np.array(obsp.x) - np.array(fp_front.x)) ** 2 + (np.array(obsp.y) - np.array(fp_front.y)) ** 2)
            #     if mind <= self.check_radius**2:
            #         return False
            #     mind = min((np.array(obsp.x) - np.array(fp_back.x)) ** 2 + (np.array(obsp.y) - np.array(fp_back.y)) ** 2)
            #     if mind <= self.check_radius**2:
            #         return False
                
                for t in range(len(fp.t)):
                    d = (obsp.x[t] - fp_front.x[t])**2 + (obsp.y[t] - fp_front.y[t])**2
                    if d <= self.check_radius**2: 
                        return False
                    d = (obsp.x[t] - fp_back.x[t])**2 + (obsp.y[t] - fp_back.y[t])**2
                    if d <= self.check_radius**2: 
                        return False
        except:
            pass
            print("collision check fail",len(fp.yaw),len(fp_back.x),len(fp_front.x))

        self.rviz_collision_checking_circle_ego = self.rivz_element.draw_circles(fp_front, fp_back, self.check_radius)
        return True

    def found_closest_obstacles(self):
        closest_obs = []
        obs_tuples = []
        
        for obs in self.dynamic_map.jmap.obstacles: 
            # distance
            p1 = np.array([self.dynamic_map.ego_state.pose.pose.position.x , self.dynamic_map.ego_state.pose.pose.position.y])
            p2 = np.array([obs.state.pose.pose.position.x , obs.state.pose.pose.position.y])
            p3 = p2 - p1
            p4 = math.hypot(p3[0],p3[1])

            obs_ffstate = get_frenet_state(obs.state, self.ref_path, self.ref_path_tangets)
            obs_yaw = get_yaw(obs.state)
            one_obs = (obs.state.pose.pose.position.x , obs.state.pose.pose.position.y , obs.state.twist.twist.linear.x ,
                     obs.state.twist.twist.linear.y , p4 , obs_ffstate.s , -obs_ffstate.d , obs_ffstate.vs, obs_ffstate.vd, 
                     obs.state.accel.accel.linear.x, obs.state.accel.accel.linear.y, obs_yaw)
            obs_tuples.append(one_obs)
        
        #sorted by distance
        sorted_obs = sorted(obs_tuples, key=lambda obs: obs[4])   # sort by distance
        i = 0
        for obs in sorted_obs:
            if i < self.considered_obs_num:
                closest_obs.append(obs)
                i = i + 1
            else:
                break
        return np.array(closest_obs)

    def prediction_obstacle(self, ob, max_prediction_time, delta_t): # we should do prediciton in driving space
        
        obs_paths = []

        for one_ob in ob:
            obsp_front = Frenet_path()
            obsp_back = Frenet_path()
            obsp_front.t = [t for t in np.arange(0.0, max_prediction_time, delta_t)]
            obsp_back.t = [t for t in np.arange(0.0, max_prediction_time, delta_t)]
            ax = 0#one_ob[9]
            ay = 0#one_ob[10]

            # vx = one_ob[2] + ax * delta_t * np.linspace(0,len(obsp_front.t)-1,len(obsp_front.t)-1)
            # vy = one_ob[3] + ax * delta_t * np.linspace(0,len(obsp_front.t)-1,len(obsp_front.t)-1)

            # obspx = one_ob[0] + delta_t * vx * np.linspace(0,len(obsp_front.t)-1,len(obsp_front.t)-1)
            # obspy = one_ob[1] + delta_t * vy * np.linspace(0,len(obsp_front.t)-1,len(obsp_front.t)-1)

            # yaw = one_ob[11]
            # obsp_front.x = (obspx + math.cos(yaw) * self.move_gap).tolist()
            # obsp_front.y = (obspy + math.sin(yaw) * self.move_gap).tolist()

            # obsp_back.x = (obspx - math.cos(yaw) * self.move_gap).tolist()
            # obsp_back.x = (obspy - math.sin(yaw) * self.move_gap).tolist()

            for i in range(len(obsp_front.t)):
                vx = one_ob[2] + ax * delta_t * i
                vy = one_ob[3] + ax * delta_t * i
                yaw = one_ob[11]   #only for constant prediction

                obspx = one_ob[0] + i * delta_t * vx
                obspy = one_ob[1] + i * delta_t * vy

                obsp_front.x.append(obspx + math.cos(yaw) * self.move_gap)
                obsp_front.y.append(obspy + math.sin(yaw) * self.move_gap)
                obsp_back.x.append(obspx - math.cos(yaw) * self.move_gap)
                obsp_back.y.append(obspy - math.sin(yaw) * self.move_gap)
                
            obs_paths.append(obsp_front)
            obs_paths.append(obsp_back)

        self.rviz_collision_checking_circle = self.rivz_element.draw_obs_circles(obs_paths, self.check_radius)

        return obs_paths

    

