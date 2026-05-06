# Copyright (c) 2025 Rafael Carbonell Lázaro (racarla96)
# Licensed under the Creative Commons Attribution 4.0 International License (CC BY 4.0)
# See: https://creativecommons.org/licenses/by/4.0/

cmake_minimum_required(VERSION 3.16)
project(caddy_ai2_ros2_robot_description_publisher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  std_msgs
)

find_package(ament_cmake REQUIRED)
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

add_executable(${PROJECT_NAME}
  src/robot_description_publisher.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

ament_target_dependencies(${PROJECT_NAME}
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)

install(TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY bringup
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
