#pragma once

#include "marker_manager.hpp"
#include "ogrid_manager.hpp"
#include "object_detector.hpp"
#include "pcodar_params.hpp"
#include "pcodar_types.hpp"
#include "input_cloud_filter.hpp"
#include "persistent_cloud_filter.hpp"
#include "object_associator.hpp"
#include "point_cloud_builder.hpp"

#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <mil_bounds/BoundsConfig.h>
#include <mil_msgs/ObjectDBQuery.h>
#include <dynamic_reconfigure/client.h>
#include <tf2/convert.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_eigen/tf2_eigen.h>

#include <boost/circular_buffer.hpp>

namespace pcodar
{
using point_cloud = pcl::PointCloud<pcl::PointXYZ>;
using point_cloud_ptr = pcl::PointCloud<pcl::PointXYZ>::Ptr;

class pcodar_controller
{
public:
  pcodar_controller(ros::NodeHandle nh);

  void velodyne_cb(const sensor_msgs::PointCloud2ConstPtr& pcloud);

  void odom_cb(const nav_msgs::OdometryConstPtr &odom);

  void initialize();
private:
  bool bounds_update_cb(const mil_bounds::BoundsConfig &config);
  bool DBQuery_cb(mil_msgs::ObjectDBQuery::Request &req,
                  mil_msgs::ObjectDBQuery::Response &res);
  bool transform_point_cloud(const sensor_msgs::PointCloud2& pcloud2, point_cloud& out);

private:
  ros::NodeHandle nh_;
  dynamic_reconfigure::Client<mil_bounds::BoundsConfig> bounds_client_;

  ros::ServiceServer modify_classification_service_;

  std::string global_frame_;

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener;

  // Publishers
  ros::Publisher pub_grid_;
  ros::Publisher pub_objects_;
  ros::Publisher pub_pcl_;
  ros::Publisher pub_pcl_old_;

  // Subscriber
  ros::Subscriber pc_sub;
  ros::Subscriber odom_sub;

  // Place to hold the latest message
  sensor_msgs::PointCloud2 latest_point_cloud_;
  nav_msgs::OdometryConstPtr latest_odom_;

  // Visualization
  marker_manager marker_manager_;
  ogrid_manager ogrid_manager_;

  // Model (It eventually will be obeject tracker, but for now just detections)
  InputCloudFilter input_cloud_filter_;
  PersistentCloudFilter persistent_cloud_filter_;
  point_cloud_builder persistent_cloud_builder_;
  object_detector detector_;

  id_object_map_ptr id_object_map_;
  id_label_map_ptr id_label_map_;
  uint32_t highest_id_;

  mil_msgs::PerceptionObjectArray old_objects_;

  associator ass;

};

}  // namespace pcodar
