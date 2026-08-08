from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable

def generate_launch_description():
    turtle = Node(package="turtlesim", executable="turtlesim_node")
    spawn = ExecuteProcess(
        # cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{x: 8.0, y: 9.0, theta: 0.0, name: 'turtle2'}\""],
        # 或
        cmd = [
            FindExecutable(name = "ros2"), # 不可以有空格
            " service call",
            " /spawn turtlesim/srv/Spawn",
            " \"{x: 8.0, y: 9.0, theta: 1.0, name: 'turtle2'}\""
        ],
        output="both",
        shell=True)
    return LaunchDescription([turtle, spawn])