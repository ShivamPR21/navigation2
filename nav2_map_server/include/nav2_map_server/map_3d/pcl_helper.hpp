// Copyright (c) 2020 Shivam Pandey
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*Helper functions for PCL<->sensor_msg conversions*/

#ifndef NAV2_MAP_SERVER__MAP_3D__PCL_HELPER_HPP_
#define NAV2_MAP_SERVER__MAP_3D__PCL_HELPER_HPP_

#include <iostream>
#include <vector>
#include <string>

#include "sensor_msgs/msg/point_cloud2.hpp"
#include "geometry_msgs/msg/pose.hpp"

#include "pcl/PCLPointField.h"
#include "pcl_conversions/pcl_conversions.h"
#include "Eigen/Core"

namespace nav2_map_server
{

namespace  map_3d
{

/**
 * @brief Modifies message fields that describes how
 * the pointcloud data is arranged and type of each field.
 * @param msg Output PointCloud2 sensor message with modified fields.
 * @param fields Input PCLPointFields indicating field types used in pcd-v0.7
 */
inline void modifyMsgFields(
  sensor_msgs::msg::PointCloud2 & msg,
  const std::vector<pcl::PCLPointField> & fields)
{
  msg.fields.clear();
  for (auto & field : fields) {
    sensor_msgs::msg::PointField new_field;
    new_field.datatype = field.datatype;
    new_field.name = field.name;
    new_field.count = field.count;
    new_field.offset = field.offset;
    msg.fields.push_back(new_field);
  }
  std::cout << "[DEBUG] [pcl_helper]: Message field modification done" << std::endl;
}

/**
 * @brief Converts map from pcl::PointCloud2 to sensor_msgs::msg::PointCloud2
 * @param msg message object to be changed according to the input pointcloud
 * @param cloud pointcloud to be converted in message object
 */
inline void pclToMsg(
  sensor_msgs::msg::PointCloud2 & msg,
  const pcl::PCLPointCloud2::Ptr & cloud)
{
  msg.data.clear();
  modifyMsgFields(msg, cloud->fields);
  msg.data = cloud->data;
  msg.point_step = cloud->point_step;
  msg.row_step = cloud->row_step;
  msg.width = cloud->width;
  msg.height = cloud->height;
  msg.is_bigendian = cloud->is_bigendian;
  msg.is_dense = cloud->is_dense;
  msg.header = pcl_conversions::fromPCL(cloud->header);
  std::cout << "[DEBUG] [pcl_helper]: PCL to message conversion done" << std::endl;
}

/**
 * @brief Modifies the pointcloud2 fields in pcl scope
 * @param fields pointfields modified according to incoming message
 * @param msg message containing the pointfields to be converted
 */
inline void modifyPclFields(
  std::vector<pcl::PCLPointField> & fields,
  const sensor_msgs::msg::PointCloud2 & msg)
{
  fields.clear();
  for (auto & field : msg.fields) {
    pcl::PCLPointField new_field;
    new_field.datatype = field.datatype;
    new_field.name = field.name;
    new_field.count = field.count;
    new_field.offset = field.offset;
    fields.push_back(new_field);
  }
  std::cout << "[DEBUG] [pcl_helper]: PCL field modification done" << std::endl;
}

/**
 * @brief Converts pointcloud sensor_msgs::msg::PointCloud2 to  from pcl::PointCloud2
 * @param cloud pointcloud object to be changed according to the input message
 * @param msg message to be converted in pointcloud object
 */
inline void msgToPcl(
  pcl::PCLPointCloud2::Ptr & cloud,
  const sensor_msgs::msg::PointCloud2 & msg)
{
  cloud->data.clear();
  modifyPclFields(cloud->fields, msg);
  cloud->data = msg.data;
  cloud->point_step = msg.point_step;
  cloud->row_step = msg.row_step;
  cloud->width = msg.width;
  cloud->height = msg.height;
  cloud->is_bigendian = msg.is_bigendian;
  cloud->is_dense = msg.is_dense;
  cloud->header = pcl_conversions::toPCL(msg.header);
  std::cout << "[DEBUG] [pcl_helper]: message to PCL conversion done" << std::endl;
}

/**
 * @brief Helper function to match input string with the desired ending
 * @param value input string
 * @param ending desired ending
 * @return true or false
 */
inline bool ends_with(std::string const & value, std::string const & ending)
{
  if (ending.size() > value.size()) {
    return false;
  }
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

/**
 * @brief Converts position and orientation from PCL to geometry_msg format
 * @param origin desired Pose in geonetry_msg format
 * @param position input Eigen::Vector4f from pcl
 * @param orientation input Eigen::Quaternionf from pcl
 */
inline void viewPoint2Pose(
  geometry_msgs::msg::Pose & origin,
  const Eigen::Vector4f & position,
  const Eigen::Quaternionf & orientation)
{
  // Put position information to origin
  origin.position.x = static_cast<double>(position[0]);
  origin.position.y = static_cast<double>(position[1]);
  origin.position.z = static_cast<double>(position[2]);

  // Put orientation information to origin
  origin.orientation.w = static_cast<double>(orientation.w());
  origin.orientation.x = static_cast<double>(orientation.x());
  origin.orientation.y = static_cast<double>(orientation.y());
  origin.orientation.z = static_cast<double>(orientation.z());
}
//
}  // namespace map_3d

}  // namespace nav2_map_server

#endif  // NAV2_MAP_SERVER__MAP_3D__PCL_HELPER_HPP_
