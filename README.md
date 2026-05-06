# caddy_ai2_ros2_robot_description_publisher

ROS 2 node that reads a robot description string from the `robot_description` parameter and republishes it as a `std_msgs/msg/String` topic with **transient local** QoS (equivalent to ROS 1 latching). Late-joining subscribers always receive the last published message.

The node is **format-agnostic**: it publishes the string as-is without validating its content, so it works with URDF, SDF, or any other text-based description format. The published topic name is configurable via a parameter.

> **Format note:** standard ROS 2 tools (`robot_state_publisher`, `rviz2`, etc.) expect URDF on the `robot_description` topic. When publishing SDF or other formats, use a different topic name (e.g. `robot_description_topic:=robot_description_sdf`) to avoid conflicts, and make sure the subscriber also uses **transient local** QoS.

## Parameters

| Parameter | Type | Default | Description |
|---|---|---|---|
| `robot_description` | `string` | `""` | Robot description string to publish (URDF, SDF, or any format). Supports dynamic updates at runtime. |
| `robot_description_topic` | `string` | `"robot_description"` | Name of the topic on which the description is published. Effectively read-only: the publisher is created at startup and cannot be moved to a different topic without restarting the node. |

## Published topics

| Topic | Type | QoS |
|---|---|---|
| `<robot_description_topic>` | `std_msgs/msg/String` | Reliable, transient local (depth 1) |

> Subscribers should also use **transient local** QoS to receive the message even after the node has already published it.

## Usage

### Command line — URDF

```bash
ros2 run caddy_ai2_ros2_robot_description_publisher caddy_ai2_ros2_robot_description_publisher \
  --ros-args \
  -p robot_description:="$(cat /path/to/robot.urdf)" \
  -p robot_description_topic:=robot_description
```

### Command line — SDF

```bash
ros2 run caddy_ai2_ros2_robot_description_publisher caddy_ai2_ros2_robot_description_publisher \
  --ros-args \
  -p robot_description:="$(cat /path/to/robot.sdf)" \
  -p robot_description_topic:=robot_description_sdf
```

### Launch file

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="caddy_ai2_ros2_robot_description_publisher",
            executable="caddy_ai2_ros2_robot_description_publisher",
            name="robot_description_publisher",
            parameters=[{
                "robot_description": "<robot>...</robot>",  # or SDF: "<sdf>...</sdf>"
                "robot_description_topic": "robot_description",
            }],
            output="screen",
        )
    ])
```

A ready-to-use launch file is provided at `bringup/launch/robot_description_publisher.launch.py`.

## Namespace and node name

Because `robot_description_topic` defaults to a **relative** name (no leading `/`), the node fully supports ROS 2 namespacing and node name remapping.

### Command line

```bash
ros2 run caddy_ai2_ros2_robot_description_publisher caddy_ai2_ros2_robot_description_publisher \
  --ros-args \
  -r __ns:=/my_robot \
  -r __node:=my_description_publisher \
  -p robot_description:="$(cat robot.urdf)"
# Publishes on /my_robot/robot_description
```

### Launch file

```python
Node(
    package="caddy_ai2_ros2_robot_description_publisher",
    executable="caddy_ai2_ros2_robot_description_publisher",
    name="my_description_publisher",   # node name
    namespace="my_robot",              # namespace
    parameters=[{
        "robot_description": ...,
        "robot_description_topic": "robot_description",  # → /my_robot/robot_description
    }],
)
```

> **Note:** if `robot_description_topic` is set to an absolute path (starting with `/`), the namespace is ignored for that topic — this is standard ROS 2 behaviour.

## Dynamic updates

The `robot_description` parameter supports runtime updates. Setting it via `ros2 param set` triggers an immediate re-publish:

```bash
ros2 param set /robot_description_publisher robot_description "$(cat new_robot.urdf)"
```
