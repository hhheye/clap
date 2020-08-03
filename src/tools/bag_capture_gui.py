#!/usr/bin/env python
# coding: utf-8
import os
import sys
import time
import datetime
import threading
import copy
import signal

import roslib
import rospy
import rosbag
from sensor_msgs.msg import Image, CompressedImage
from sensor_msgs.msg import PointCloud2
from nav_msgs.msg import Path
from cv_bridge import CvBridgeError, CvBridge
from visualization_msgs.msg import Marker, MarkerArray
from queue import Queue
import argparse
import utm
import numpy as np
from tf2_msgs.msg import TFMessage 

from python_qt_binding.QtGui import *
from python_qt_binding.QtCore import *
try:
    from python_qt_binding.QtWidgets import *
except ImportError:
    pass

import rviz

# zzz message reference
from xpmotors_can_msgs.msg import AutoStateEx
from zzz_driver_msgs.msg import RigidBodyStateStamped
from zzz_perception_msgs.msg import TrackingBox, TrackingBoxArray, DetectionBox, DetectionBoxArray, ObjectSignals

roslib.load_manifest('rosbag')

# parser = argparse.ArgumentParser(description='Reorder a bagfile based on header timestamps.')
# parser.add_argument('bagfile', nargs=1, help='input bag file')
# parser.add_argument('--window_seconds', nargs=1, help='max time offset (sec) to correct.', default='120', type=int)
# args = parser.parse_args()

traffic_publisher = None
traffic_light_topic = "/zzz/perception/traffic_lights"

# 2-autopilot, 0-manually
last_autostate  = 0
take_over_count = 0

# bag record default 1 minutes
window_seconds = 60*1.5

last_point = None
total_distance = 0.0

# all topics
auto_topic = '/xp/auto_state_ex'
ego_pose_topic  = '/zzz/navigation/ego_pose'
obs_topic = '/zzz/perception/objects_tracked'
left_cam_topic  = '/left_usb_cam/image_raw/compressed'
pcl_topic = '/middle/rslidar_points'
ego_marker_topic = '/zzz/cognition/ego_markerarray'
lanes_marker_topic = '/zzz/cognition/lanes_markerarray'
obstacles_marker_topic = '/zzz/cognition/obstacles_markerarray'
sent_ref_path_topic = '/zzz/cognition/sent_ref_path'
all_trajectory_path_topic = '/zzz/planning/all_trajectory_path'
decision_trajectory_path_topic = '/zzz/planning/decision_trajectory_path'
prepoint_topic = '/pre_point'
collision_topic = '/zzz/planning/collision_circle'
tf_topic = '/tf'

# queue
# autostate_mode, 50 hz
auto_queue = Queue(50 * window_seconds)
# ego_pose hz 100
pose_queue = Queue(100 * window_seconds)
# perception obstacle hz 10
obs_queue = Queue(10 * window_seconds)
# usb cam hz 10
image_queue = Queue(10 * window_seconds)
# pcl hz 10
pcl_queue = Queue(10 * window_seconds)
# 5 hz
ego_marker_queue = Queue(5 * window_seconds)
# 5 hz
lanes_marker_queue = Queue(5 * window_seconds)
# 5 hz
obstacles_marker_queue = Queue(5 * window_seconds)
# 20 hz
sent_ref_path_queue = Queue(20 * window_seconds)
# 5 hz
all_trajectory_path_queue = Queue(5 * window_seconds)
# 5 hz
decision_trajectory_path_queue = Queue(5 * window_seconds)
# 20 hz
prepoint_queue = Queue(20 * window_seconds)
# 5 hz
collision_queue = Queue(5 * window_seconds)
# 100 hz
tf_queue = Queue(100 * window_seconds)


def start_capture(topic_queue_pairs):
    t = threading.Thread(target=write2bag, args=(topic_queue_pairs, ))
    t.setDaemon(False)
    t.start()
    rospy.loginfo("### Launch Thread Record Bag File! ###")
    

def write2bag(topic_queue_pairs):

    file_name = time.strftime("%Y-%m-%d_%H-%M-%S.bag", time.localtime()) 
    # time.sleep(60)
    bag = rosbag.Bag(file_name, "w")
    for topic, que, _, _, _ in topic_queue_pairs:
        for msg, t in list(que.queue):
            bag.write(topic, msg, t)
    
    bag.flush()
    bag.close()
    print('### Bag Write {} Done! ###'.format(file_name))


auto_capture = False
def autostate_callback(msg):
    global pose_queue, obs_queue, image_queue, last_autostate
    
    if msg.CurDriveMode == 0 and last_autostate == 2:
        global auto_capture, global_topic_queue_pairs, take_over_count
        if auto_capture:
            start_capture(global_topic_queue_pairs)

        take_over_count = take_over_count + 1

    last_autostate = msg.CurDriveMode
    if not auto_queue.full():
        auto_queue.put((msg, rospy.Time.now()))
    else:
        auto_queue.get()


def ego_pose_callback(msg):
    if not pose_queue.full():
        pose_queue.put((msg, rospy.Time.now()))
    else:
        pose_queue.get()

    global last_point
    point = (msg.state.pose.pose.position.x,
            msg.state.pose.pose.position.y)
    if last_point is None:
        last_point = point

    vb = np.array([point[0], point[1]])
    va = np.array([last_point[0], last_point[1]])
    gap = np.linalg.norm(vb - va)
    last_point = point
    
    global total_distance, last_autostate
    if last_autostate == 2:
        total_distance = total_distance + gap


def obstacles_callback(msg):
    if not obs_queue.full():
        obs_queue.put((msg, rospy.Time.now()))
    else:
        obs_queue.get()


def image_callback(msg):
    if not image_queue.full():
        image_queue.put((msg, rospy.Time.now()))
    else:
        image_queue.get()


def pcl_callback(msg):
    if not pcl_queue.full():
        pcl_queue.put((msg, rospy.Time.now()))
    else:
        pcl_queue.get()


def ego_marker_callback(msg):
    global ego_marker_queue
    if not ego_marker_queue.full():
        ego_marker_queue.put((msg, rospy.Time.now()))
    else:
        ego_marker_queue.get()


def lanes_marker_callback(msg):
    global lanes_marker_queue
    if not lanes_marker_queue.full():
        lanes_marker_queue.put((msg, rospy.Time.now()))
    else:
        lanes_marker_queue.get()


def obstacles_marker_callback(msg):
    global obstacles_marker_queue
    if not obstacles_marker_queue.full():
        obstacles_marker_queue.put((msg, rospy.Time.now()))
    else:
        obstacles_marker_queue.get()


def sent_ref_path_callback(msg):
    global sent_ref_path_queue
    if not sent_ref_path_queue.full():
        sent_ref_path_queue.put((msg, rospy.Time.now()))
    else:
        sent_ref_path_queue.get()
    

def all_trajectory_path_callback(msg):
    global all_trajectory_path_queue
    if not all_trajectory_path_queue.full():
        all_trajectory_path_queue.put((msg, rospy.Time.now()))
    else:
        all_trajectory_path_queue.get()


def decision_trajectory_path_callback(msg):
    global decision_trajectory_path_queue
    if not decision_trajectory_path_queue.full():
        decision_trajectory_path_queue.put((msg, rospy.Time.now()))
    else:
        decision_trajectory_path_queue.get()


def prepoint_callback(msg):
    global prepoint_queue
    if not prepoint_queue.full():
        prepoint_queue.put((msg, rospy.Time.now()))
    else:
        prepoint_queue.get()


def collision_callback(msg):
    global collision_queue
    if not collision_queue.full():
        collision_queue.put((msg, rospy.Time.now()))
    else:
        collision_queue.get()


def tf_callback(msg):
    global tf_queue
    if not tf_queue.full():
        tf_queue.put((msg, rospy.Time.now()))
    else:
        tf_queue.get()


global_topic_queue_pairs = [
    ###  (topic, queue, class, callback, hz)
    (auto_topic, auto_queue, AutoStateEx, autostate_callback, 20),
    (ego_pose_topic, pose_queue, RigidBodyStateStamped, ego_pose_callback, 100), 
    (obs_topic, obs_queue, TrackingBoxArray, obstacles_callback, 10),
    (left_cam_topic, image_queue, CompressedImage, image_callback, 10),
    (pcl_topic, pcl_queue, PointCloud2, pcl_callback, 10),
    (ego_marker_topic, ego_marker_queue, MarkerArray, ego_marker_callback, 5),
    (lanes_marker_topic, lanes_marker_queue, MarkerArray, lanes_marker_callback, 5),
    (obstacles_marker_topic, obstacles_marker_queue, MarkerArray, obstacles_marker_callback, 10),
    (sent_ref_path_topic, sent_ref_path_queue, Path, sent_ref_path_callback, 20),
    (all_trajectory_path_topic, all_trajectory_path_queue, MarkerArray, all_trajectory_path_callback, 5),
    (decision_trajectory_path_topic, decision_trajectory_path_queue, Path, decision_trajectory_path_callback, 5),
    (prepoint_topic, prepoint_queue, Marker, prepoint_callback, 100),
    (collision_topic, collision_queue, MarkerArray, collision_callback, 5), 
    (tf_topic, tf_queue, TFMessage, tf_callback, 100)]


ros_main_thread_pid = -1

def ros_main_thread():
    
    rospy.loginfo("### Ros Main Thread ID %d ###", os.getpid())
    global ros_main_thread_pid
    ros_main_thread_pid = os.getpid()
    
    try:
        # batch create sub
        for t, _, cls, cb, hz in global_topic_queue_pairs:
            rospy.Subscriber(t, cls, cb, queue_size=hz)
        
        global traffic_publisher
        traffic_publisher = rospy.Publisher(traffic_light_topic, DetectionBoxArray, queue_size=10)

        rospy.loginfo("*** Create Subscribers Done, Start Loop ***")
        rospy.spin()
    finally:
        rospy.loginfo("*** All Done ***")
        

class MyViz(QWidget):
    
    def __init__(self):
        QWidget.__init__(self)
        self.frame = rviz.VisualizationFrame()
        self.frame.initialize()
        self.setWindowTitle("bag-capture")
        self.frame.setMenuBar(None)
        self.frame.setStatusBar(None)
        self.frame.setHideButtonVisibility(False)
        self.statusBar = QStatusBar()
        # record number of current bag
        self.number_of_current_bag = 1

        self.manager = self.frame.getManager()
        ## Here we create the layout and other widgets in the usual Qt way.
        layout = QVBoxLayout()        
        save_layout = QHBoxLayout()
        traffic_signal_layout = QHBoxLayout()
        
        capture_question_button = QPushButton("BAD BAG")
        capture_question_button.setFixedSize(150,40)
        capture_question_button.setStyleSheet("color:red")
        capture_question_button.clicked.connect(self.onCaptureQuestionButtonClick)
        save_layout.addWidget(capture_question_button)
        
        capture_showcase_button = QPushButton("GOOD BAG")
        capture_showcase_button.setFixedSize(100,40)
        capture_showcase_button.setStyleSheet("color:green")
        capture_showcase_button.clicked.connect(self.onCaptureShowcaseButtonClick)
        save_layout.addWidget(capture_showcase_button)
        
        red_button = QPushButton("RedSignal")
        red_button.setStyleSheet("background:red")
        red_button.clicked.connect( self.onRedSignalButtonClick)
        traffic_signal_layout.addWidget(red_button)
        
        green_button = QPushButton("GreenSignal")
        green_button.setStyleSheet("background:green")
        green_button.clicked.connect(self.onGreenSignalButtonClick)
        traffic_signal_layout.addWidget(green_button)
        
        # ADD a case description 
        self.case_description = QTextEdit()
        layout.addWidget(self.case_description)
        
        layout.addLayout(save_layout)
        layout.addLayout(traffic_signal_layout)
        layout.addWidget(self.statusBar)
        self.setLayout(layout)

        self.statusBar.showMessage('Initialize Successfully')
    ## GUI button event handling
    def onCaptureQuestionButtonClick(self):
        global global_topic_queue_pairs
        start_capture(global_topic_queue_pairs)
        with open('Record.txt', 'a+') as f:
            f.write('BAG NO.{0} | Question bag | {1}'.format(str(self.number_of_current_bag), self.case_description.toPlainText()) + '\n')
        self.case_description.clear()
        self.statusBar.showMessage('No.{0} bag captured'.format(str(self.number_of_current_bag)), 5000)
        self.number_of_current_bag += 1
        print("### Capture Done! ###")
    
    def onCaptureShowcaseButtonClick(self):
        global global_topic_queue_pairs
        start_capture(global_topic_queue_pairs)
        with open('Record.txt', 'a+') as f:
            f.write('BAG NO.{0} | ShowCase bag | {1}'.format(str(self.number_of_current_bag), self.case_description.toPlainText()) + '\n')
        self.case_description.clear()
        self.statusBar.showMessage('No.{0} bag captured'.format(str(self.number_of_current_bag)), 5000)
        self.number_of_current_bag += 1
        print("### Capture Done! ###")
        
        
    def onRedSignalButtonClick(self):
        traffic_light_detection = DetectionBoxArray()
        traffic_light = DetectionBox()
        traffic_light.signal.flags = ObjectSignals.TRAFFIC_LIGHT_RED
        traffic_light_detection.detections.append(traffic_light)
        global traffic_publisher
        traffic_publisher.publish(traffic_light_detection)
        print('### Send RED Signal...')
    
        
    def onGreenSignalButtonClick(self):
        traffic_light_detection = DetectionBoxArray()
        traffic_light = DetectionBox()
        traffic_light.signal.flags = ObjectSignals.TRAFFIC_LIGHT_GREEN
        traffic_light_detection.detections.append(traffic_light)
        global traffic_publisher
        traffic_publisher.publish(traffic_light_detection)
        print('### Send GREEN Signal...')
        
    
## Start the Application
if __name__ == '__main__':
    # ros comm
    rospy.init_node("Bug-Capture-Node", anonymous=True)
    ros_thread = threading.Thread(
        target = ros_main_thread, args=())
    ros_thread.setDaemon(True)
    ros_thread.start()
    
    # open a new text file to record testing result
    with open('Record.txt', 'w+') as f:
        f.write('Today is :{0}\n'.format(datetime.date.today()))
        f.write('This testing begin from:{0}\n'.format(datetime.datetime.now().strftime('%H:%M:%S')))
        f.write('---------------------------\n')
    # qt-gui
    app = QApplication(sys.argv)
    myviz = MyViz()
    myviz.resize(250, 30)
    myviz.show()
    app.exec_()
    global total_distance, take_over_count
    print('### Total Distance - {} km, Take over {} times ###'.format(total_distance / 1000.0, take_over_count))
    # kill ros_main_thread
    with open('Record.txt', 'a+') as f:
        f.write('---------------------------\n')
        f.write('This testing end at:{0}\n'.format(datetime.datetime.now().strftime('%H:%M:%S')))
        f.write('Today we droved {}km, Take over {} times'.format(total_distance / 1000.0, take_over_count))
    
    global ros_main_thread_pid
    os.kill(ros_main_thread_pid, signal.SIGINT)
    time.sleep(5)
    
    cmd = "kill -9 %d" % int(ros_main_thread_pid)
    os.system(cmd)
    # os.kill(ros_main_thread_pid, signal.SIGTERM)
    print('### kill ros_main_thread {} ! ###'.format(ros_main_thread_pid))

