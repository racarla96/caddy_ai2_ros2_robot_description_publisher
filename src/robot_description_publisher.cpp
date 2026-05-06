// Copyright (c) 2025 Rafael Carbonell Lázaro (racarla96)
// Licensed under the Creative Commons Attribution 4.0 International License (CC BY 4.0)
// See: https://creativecommons.org/licenses/by/4.0/

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace caddy_ai2_ros2_robot_description_publisher
{

class RobotDescriptionPublisher : public rclcpp::Node
{
public:
  explicit RobotDescriptionPublisher(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : rclcpp::Node("robot_description_publisher", options)
  {
    // Read-only: the publisher is created at startup, so the topic name cannot change at runtime.
    const std::string topic_name = this->declare_parameter<std::string>(
      "robot_description_topic", "robot_description");

    // Transient local mirrors ROS 1 latching: late-joining subscribers receive the last message.
    description_pub_ = this->create_publisher<std_msgs::msg::String>(
      topic_name, rclcpp::QoS(1).transient_local());

    robot_description_ = this->declare_parameter<std::string>("robot_description", "");

    if (!robot_description_.empty()) {
      publish(robot_description_);
    } else {
      RCLCPP_WARN(this->get_logger(),
        "robot_description parameter is empty — nothing published yet");
    }

    param_cb_ = this->add_on_set_parameters_callback(
      [this](const std::vector<rclcpp::Parameter> & params)
      {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        for (const auto & param : params) {
          if (param.get_name() == "robot_description") {
            publish(param.as_string());
          }
        }
        return result;
      });
  }

private:
  void publish(const std::string & description)
  {
    auto msg = std::make_unique<std_msgs::msg::String>();
    msg->data = description;
    description_pub_->publish(std::move(msg));
    RCLCPP_INFO(this->get_logger(),
      "Published robot_description on '%s'", description_pub_->get_topic_name());
  }

  std::string robot_description_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr description_pub_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_cb_;
};

}  // namespace caddy_ai2_ros2_robot_description_publisher

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(
    std::make_shared<caddy_ai2_ros2_robot_description_publisher::RobotDescriptionPublisher>());
  rclcpp::shutdown();
  return 0;
}
