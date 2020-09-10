#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "zzz_navigation_msgs/Map.h"
#include "zzz_driver_msgs/RigidBodyStateStamped.h"
#include "modules/map/hdmap/hdmap.h"
 
#include <visualization_msgs/MarkerArray.h>

// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <yaml-cpp/yaml.h>
// #include <fstream>

#include <cmath>
#include <map>
#include <vector>
#include <memory>
#include <iomanip> // for shared_ptr
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>

#define STDCOUT(express)  std::cout << express << std::endl
#define SIZECOUT(vec) STDCOUT(#vec << " size : " << vec.size() )
#define TRY_RUN(express)  if ( 0 == (express) ) { \
  std::cout << "run " << #express << " success." << std::endl; \
} else { \
  std::cout << "run " << #express << " failed." << std::endl; \
}

struct Iterm {
  int road_index;
  int lane_index;
};

apollo::hdmap::HDMap map;

double origin_x = 428191;
double origin_y = 4417667;
double lat0 = 39.90582185794227;
double lon0 = 116.14200170513072;
double alt0 = 74.7032012939;

apollo::common::PointENU last_point;
apollo::common::PointENU last_point_pose;

bool initialized = false;

ros::Subscriber pose_sub;
ros::Subscriber path_sub;
ros::Publisher map_output_pub;
ros::Publisher map_mark_pub;

std::map<std::string, Iterm> road_tab;  
std::vector<std::vector<zzz_navigation_msgs::Lane>> lanes;
std::vector<bool> road_in_junction;
std::vector<apollo::hdmap::JunctionInfoConstPtr> junction_tab;


uint8_t laneBoundryWrapper(const apollo::hdmap::LaneBoundaryType& t) {
  switch (t.types().Get(0)) {
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_UNKNOWN : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_UNKNOWN;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_DOTTED_YELLOW : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_DASHED_YELLOW;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_DOTTED_WHITE : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_DASHED_WHITE;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_SOLID_YELLOW : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_SOLID_YELLOW;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_SOLID_WHITE : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_SOLID_WHITE;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_DOUBLE_YELLOW : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_SOLID_YELLOW_TURN;
    case apollo::hdmap::LaneBoundaryType_Type::LaneBoundaryType_Type_CURB : 
      return zzz_navigation_msgs::LaneBoundary::BOUNDARY_CURB;
  }
}

bool sameDirection(const apollo::common::PointENU& drive_direction, 
    const apollo::hdmap::LaneInfoConstPtr& lane_ptr) {
  auto p1 = lane_ptr->lane().central_curve().segment().Get(0).line_segment().point().Get(0);
  auto& p2 = lane_ptr->lane().central_curve().segment().Get(0).line_segment().point().Get(1);
  p1.set_x(p2.x() - p1.x());
  p1.set_y(p2.y() - p1.y()); 
  return ( (drive_direction.x() * p1.x() + drive_direction.y() * p1.y()) > 0 );
}

bool junctionToMsg (const apollo::hdmap::JunctionInfoConstPtr& junction_ptr,
    const std_msgs::Header& header, zzz_navigation_msgs::Map& out_map, bool next) {
  if (next) {
    out_map.next_drivable_area.points.resize(
        junction_ptr->junction().polygon().point().size());
    for (auto pidx=0; pidx < 
        junction_ptr->junction().polygon().point().size(); pidx++) {
      out_map.next_drivable_area.points[pidx].x = 
          junction_ptr->junction().polygon().point().Get(pidx).x() - origin_x;
      out_map.next_drivable_area.points[pidx].y = 
          junction_ptr->junction().polygon().point().Get(pidx).y() - origin_y;
      out_map.next_drivable_area.points[pidx].z = 
          junction_ptr->junction().polygon().point().Get(pidx).z();
    }  
  } else {
    out_map.header = header;
    out_map.in_junction = true;
    out_map.lanes.clear();
    out_map.drivable_area.points.resize(
        junction_ptr->junction().polygon().point().size());
    for (auto pidx=0; pidx < 
        junction_ptr->junction().polygon().point().size(); pidx++) {
      out_map.drivable_area.points[pidx].x = 
          junction_ptr->junction().polygon().point().Get(pidx).x() - origin_x;
      out_map.drivable_area.points[pidx].y = 
          junction_ptr->junction().polygon().point().Get(pidx).y() - origin_y;
      out_map.drivable_area.points[pidx].z = 
          junction_ptr->junction().polygon().point().Get(pidx).z();
    }    
  }
  return true;
}
  static int count = 0;
void pose_callback(const zzz_driver_msgs::RigidBodyStateStamped& msg) {
  // ROS_INFO("recv pose: %f  %f", msg.state.pose.pose.position.x, msg.state.pose.pose.position.y);


  apollo::common::PointENU point;
  point.set_x(msg.state.pose.pose.position.x + origin_x);
  point.set_y(msg.state.pose.pose.position.y + origin_y);
  if (!initialized) {
    last_point_pose = point;
    initialized = true;
    return;
  }
  apollo::common::PointENU drive_direction;
  drive_direction.set_x(point.x() - last_point_pose.x());
  drive_direction.set_y(point.y() - last_point_pose.y());
  last_point_pose = point;
  // std::cout << "get pose" << std::endl;
  if (count<10) {
    count++;
    return;
  } else {
    count = 0;
  }
  std::cout << "get map ... " << count << std::endl;
  double path_heading = std::atan2(drive_direction.y(), drive_direction.x());
  apollo::hdmap::LaneInfoConstPtr nearest_lane;
  double nearest_s, nearest_l;
  double radius = 4.0;
  double max_heading_difference = 0.5;
  // if ( 0 == map.GetNearestLaneWithHeading(point, radius, path_heading, 
  //   max_heading_difference, &nearest_lane, &nearest_s, &nearest_l) ) 
  if ( 0 == map.GetNearestLane(point,
      &nearest_lane, &nearest_s, &nearest_l) ) {
    zzz_navigation_msgs::Map out_map;   
    auto iter = road_tab.find(nearest_lane->road_id().id());
    std::cout << "road id  " << nearest_lane->road_id().id() << std::endl;    
    /** on road */
    if (iter != road_tab.end()) {

      int road_index = iter->second.road_index;
      out_map.header = msg.header;
      // out_map.target_lane_index = static_cast<char>(iter->second.lane_index);
 
      out_map.in_junction = road_in_junction[road_index];
      out_map.drivable_area.points.clear();
      if (out_map.in_junction) {
        apollo::hdmap::JunctionInfoConstPtr junction_ptr 
            = junction_tab[road_index]; 
        std::cout << "on road in junc  " << junction_ptr->id().id() << std::endl;          
        if (junction_ptr != nullptr)
          junctionToMsg(junction_ptr, msg.header, out_map, false);
      } 
      // else 
      {
        std::cout << "on road in lane  " << nearest_lane->id().id() << std::endl; 
        out_map.lanes.resize(lanes[road_index].size());
        for (int ii = 0; ii < out_map.lanes.size(); ii++) {
          out_map.lanes[ii] = lanes[road_index][ii];
          out_map.lanes[ii].speed_limit = 
              (float)nearest_lane->lane().speed_limit() * 3.6 * 1.0; // km/h
          out_map.lanes[ii].central_path_type = 
              zzz_navigation_msgs::Lane::CENTRAL_PATH_LINE;
          // std::cout << "speed limit : " << out_map.lanes[ii].speed_limit << std::endl; 
        }
      }

      int next_index = (road_index + 1) % lanes.size();
      if (road_in_junction[next_index]) {
        apollo::hdmap::JunctionInfoConstPtr junction_ptr 
            = junction_tab[next_index]; 
        if (junction_ptr != nullptr)
          junctionToMsg(junction_ptr, msg.header, out_map, true);
      } else {
        out_map.next_lanes.resize(lanes[next_index].size());
        for (int ii = 0; ii < out_map.next_lanes.size(); ii++) {
          out_map.next_lanes[ii] = lanes[next_index][ii];
          out_map.next_lanes[ii].speed_limit = 
              (float)nearest_lane->lane().speed_limit() * 3.6 * 1.0; // km/h
          out_map.next_lanes[ii].central_path_type = 
              zzz_navigation_msgs::Lane::CENTRAL_PATH_LINE;
        }
      }
      map_output_pub.publish(out_map);
    } else {
      /** in junction */
      if (nearest_lane->lane().has_junction_id()) {
        apollo::hdmap::JunctionInfoConstPtr junction_ptr 
            = map.GetJunctionById(nearest_lane->lane().junction_id());
        std::cout << "outside road in junc  " << junction_ptr->id().id() << std::endl; 
        if (junction_ptr != nullptr) {
          std::string junc_id = junction_ptr->junction().id().id();
          int junc_index = -1;
          for (int j=0; j<road_in_junction.size(); j++) {
            if (road_in_junction[j] && junction_tab[j]->id().id() == junc_id) {
              junc_index = j;
              break;
            }
          }
          if (junc_index == -1) {
            junctionToMsg(junction_ptr, msg.header, out_map, false);
            map_output_pub.publish(out_map);  
          } else {
            junctionToMsg(junction_ptr, msg.header, out_map, false);
            int next_index = (junc_index + 1) % lanes.size();
            if (!road_in_junction[next_index]) {
              out_map.next_lanes.resize(lanes[next_index].size());
              for (int ii = 0; ii < out_map.next_lanes.size(); ii++) {
                out_map.next_lanes[ii] = lanes[next_index][ii];
                out_map.next_lanes[ii].speed_limit = 
                    (float)nearest_lane->lane().speed_limit() * 3.6 * 1.0; // km/h
                out_map.next_lanes[ii].central_path_type = 
                    zzz_navigation_msgs::Lane::CENTRAL_PATH_LINE;
              }
            }
          }
          map_output_pub.publish(out_map);           
        } 
      }
    }      

  }

#if 0
  visualization_msgs::MarkerArray tmp_marker_array;
  for (int ind=0; ind<out_map.lanes.size(); ind++) {
    visualization_msgs::Marker lane_marker;
    lane_marker.header.frame_id = "map";
    lane_marker.header.stamp = msg.header.stamp;
    lane_marker.ns = "map_marker";
    lane_marker.id = 0;
    lane_marker.type = visualization_msgs::Marker::LINE_STRIP;
    lane_marker.action = visualization_msgs::Marker::ADD;
    lane_marker.scale.x = 0.2;
    lane_marker.scale.y = 0.2;
    lane_marker.scale.z = 0.2;
    lane_marker.color.r = 0.6;
    lane_marker.color.g = 0.4;
    lane_marker.color.b = 1.0;
    lane_marker.color.a = 1.0;
    lane_marker.frame_locked = true;

    for (unsigned int i = 0; 
        i < out_map.lanes[ind].central_path_points.size(); i++) {
      geometry_msgs::Point point;
      lane_marker.id = i;
      point = out_map.lanes[ind].central_path_points[i].position;
      lane_marker.points.push_back(point);
    }
    lane_marker.ns = "lane_" + std::to_string(ind);
    tmp_marker_array.markers.push_back(lane_marker);
  }
  map_mark_pub.publish(tmp_marker_array);  
#endif

}


int main (int argc, char** argv) {

  ros::init(argc, argv, "baidu_static_map");
  ros::NodeHandle node;
  ros::NodeHandle priv_nh("~");

  std::string map_path;
  std::string pose_topic;
  std::string static_map_conf;
  std::string map_output_topic;
  // std::string map_file_type_topic;

  priv_nh.param<std::string>("map_path", map_path, "");
  priv_nh.param<std::string>("pose_topic", pose_topic, "/zzz/navigation/ego_pose");
  priv_nh.param<std::string>("map_output_topic", map_output_topic, "/zzz/navigation/local_static_map");
  priv_nh.param<std::string>("static_map_conf", static_map_conf, "");

  if (map_path.empty()) {
    ROS_ERROR("map_path is empty.");
    exit(-1);
  }
  if (static_map_conf.empty()) {
    ROS_ERROR("static_map_conf is empty.");
    exit(-1);
  }
  ROS_INFO("loading map...");
  map.LoadMapFromFile(map_path);
  

  YAML::Node yaml_node = YAML::LoadFile(static_map_conf);
  lanes.resize(yaml_node.size());
  road_in_junction.resize(yaml_node.size());
  junction_tab.resize(yaml_node.size());

  for (int idx=0; idx<yaml_node.size(); idx++) {
    std::string junction_id = yaml_node[idx]["Junction"].as<std::string>();
    if (!junction_id.empty() && junction_id != "~") {
      std::cout << junction_id << std::endl;
    }
  }
  for (int idx=0; idx<yaml_node.size(); idx++) {
    std::string junction_id = yaml_node[idx]["Junction"].as<std::string>();
    if (!junction_id.empty() && junction_id != "~") {
      road_in_junction[idx] = true;
      apollo::hdmap::Id junc_id;
      junc_id.set_id(junction_id);
      junction_tab[idx] = map.GetJunctionById(junc_id);
    } else {
      road_in_junction[idx] = false;
      junction_tab[idx] = nullptr;
    }

    YAML::Node n_road = yaml_node[idx]["Road"];
    YAML::Node n_lane_index = yaml_node[idx]["Target"];
    for (int n=0; n<n_road.size(); n++) {
      road_tab.insert(std::pair<std::string, Iterm>(n_road[n].as<std::string>(), 
          {idx, n_lane_index[n].as<int>()}));
    }

    YAML::Node n_lanes = yaml_node[idx]["Lane"];
    lanes[idx].resize(n_lanes.size());
    for (int n_l=0; n_l<n_lanes.size(); n_l++) {
      YAML::Node n_lane = n_lanes[n_l];
      lanes[idx][n_l].index = n_l;

      for (int n_s=0; n_s<n_lane.size(); n_s++) {
        apollo::hdmap::Id lane_id;
        std::string lane_str = n_lane[n_s].as<std::string>();
        lane_id.set_id(lane_str);
        apollo::hdmap::LaneInfoConstPtr lane_ptr = map.GetLaneById(lane_id); 
        if (lane_ptr == nullptr) {
          std::cout << lane_str << " is error" << std::endl;
        }
        /** lane central */
        for (auto& seg: lane_ptr->lane().central_curve().segment()) {
          for (auto& p: seg.line_segment().point()) {
            zzz_navigation_msgs::LanePoint pt;         
            pt.position.x = p.x() - origin_x;
            pt.position.y = p.y() - origin_y;
            pt.position.z = p.z();
            if (lanes[idx][n_l].central_path_points.empty()) {
              lanes[idx][n_l].central_path_points.emplace_back(pt);              
            } else {
              double lx = lanes[idx][n_l].central_path_points.back().position.x;
              double ly = lanes[idx][n_l].central_path_points.back().position.y;
              double dx = pt.position.x - lx;
              double dy = pt.position.y - ly;
              double dis = sqrt(dx*dx + dy*dy);
              double unit = 1.0;
              if (dis > 0.05 && dis <= unit) {
                lanes[idx][n_l].central_path_points.emplace_back(pt);  
              } else {
                int num = static_cast<int>(dis / unit);
                double ux = dx / (double)num;
                double uy = dy / (double)num;
                for (int ss=0; ss<num; ss++) {
                  zzz_navigation_msgs::LanePoint spt;
                  spt.position.x = lx + ux * static_cast<double>(ss+1);
                  spt.position.y = ly + uy * static_cast<double>(ss+1);
                  spt.position.z = 0;
                  lanes[idx][n_l].central_path_points.emplace_back(spt); 
                }
              }
            }
          }
        } 
#if 0
        /** left boundry */
        for (auto& seg: lane_ptr->lane().left_boundary().curve().segment()) {
          for (auto& p: seg.line_segment().point()) {
            zzz_navigation_msgs::LaneBoundary lb;          
            lb.boundary_point.position.x = p.x() - origin_y;
            lb.boundary_point.position.y = p.y() - origin_y;
            lb.boundary_point.position.z = p.z();
            lb.boundary_type = laneBoundryWrapper(lane_ptr->lane().left_boundary().boundary_type().Get(0));
            if (lanes[idx][n_l].left_boundaries.empty()) {
              lanes[idx][n_l].left_boundaries.emplace_back(lb);              
            } else {
              double lx = lanes[idx][n_l].left_boundaries.back().boundary_point.position.x;
              double ly = lanes[idx][n_l].left_boundaries.back().boundary_point.position.y;
              double dx = lb.boundary_point.position.x - lx;
              double dy = lb.boundary_point.position.y - ly;
              double dis = sqrt(dx*dx + dy*dy);
              double unit = 1.0;
              if (dis > 0.05 && dis <= unit) {
                lanes[idx][n_l].left_boundaries.emplace_back(lb);   
              } else {
                int num = static_cast<int>(dis / unit);
                double ux = dx / (double)num;
                double uy = dy / (double)num;
                for (int ss=0; ss<num; ss++) {
                  zzz_navigation_msgs::LaneBoundary slb;          
                  slb.boundary_point.position.x = lx + ux * static_cast<double>(ss+1);
                  slb.boundary_point.position.y = ly + uy * static_cast<double>(ss+1);
                  slb.boundary_point.position.z = p.z();
                  slb.boundary_type = laneBoundryWrapper(lane_ptr->lane().left_boundary().boundary_type().Get(0));
                  lanes[idx][n_l].left_boundaries.emplace_back(slb); 
                }
              }
            }
          }
        }
        /** right boundry */
        for (auto& seg: lane_ptr->lane().right_boundary().curve().segment()) {
          for (auto& p: seg.line_segment().point()) {
            zzz_navigation_msgs::LaneBoundary lb;          
            lb.boundary_point.position.x = p.x() - origin_y;
            lb.boundary_point.position.y = p.y() - origin_y;
            lb.boundary_point.position.z = p.z();
            lb.boundary_type = laneBoundryWrapper(lane_ptr->lane().right_boundary().boundary_type().Get(0));
            if (lanes[idx][n_l].right_boundaries.empty()) {
              lanes[idx][n_l].right_boundaries.emplace_back(lb);              
            } else {
              double lx = lanes[idx][n_l].right_boundaries.back().boundary_point.position.x;
              double ly = lanes[idx][n_l].right_boundaries.back().boundary_point.position.y;
              double dx = lb.boundary_point.position.x - lx;
              double dy = lb.boundary_point.position.y - ly;
              double dis = sqrt(dx*dx + dy*dy);
              double unit = 1.0;
              if (dis > 0.05 && dis <= unit) {
                lanes[idx][n_l].right_boundaries.emplace_back(lb);   
              } else {
                int num = static_cast<int>(dis / unit);
                double ux = dx / (double)num;
                double uy = dy / (double)num;
                for (int ss=0; ss<num; ss++) {
                  zzz_navigation_msgs::LaneBoundary slb;          
                  slb.boundary_point.position.x = lx + ux * static_cast<double>(ss+1);
                  slb.boundary_point.position.y = ly + uy * static_cast<double>(ss+1);
                  slb.boundary_point.position.z = p.z();
                  slb.boundary_type = laneBoundryWrapper(lane_ptr->lane().right_boundary().boundary_type().Get(0));
                  lanes[idx][n_l].right_boundaries.emplace_back(slb); 
                }
              }
            }
          }
        }
#endif
      }
    }
  }



  map_output_pub = node.advertise<zzz_navigation_msgs::Map>(map_output_topic, 1);
  map_mark_pub = node.advertise<visualization_msgs::MarkerArray>("map_mark", 10, true);
  pose_sub = node.subscribe(pose_topic, 5, &pose_callback);

  ros::spin();
  return 0;
}
