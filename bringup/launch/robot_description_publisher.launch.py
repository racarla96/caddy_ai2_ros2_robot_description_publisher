# Copyright (c) 2025 Rafael Carbonell Lázaro (racarla96)
# Licensed under the Creative Commons Attribution 4.0 International License (CC BY 4.0)
# See: https://creativecommons.org/licenses/by/4.0/

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    robot_description_arg = DeclareLaunchArgument(
        "robot_description",
        default_value="",
        description="URDF/XACRO robot description string",
    )

    robot_description_topic_arg = DeclareLaunchArgument(
        "robot_description_topic",
        default_value="robot_description",
        description="Name of the topic on which robot_description is published",
    )

    node = Node(
        package="caddy_ai2_ros2_robot_description_publisher",
        executable="caddy_ai2_ros2_robot_description_publisher",
        name="robot_description_publisher",
        parameters=[
            {
                "robot_description": LaunchConfiguration("robot_description"),
                "robot_description_topic": LaunchConfiguration("robot_description_topic"),
            }
        ],
        output="screen",
    )

    return LaunchDescription([
        robot_description_arg,
        robot_description_topic_arg,
        node,
    ])
