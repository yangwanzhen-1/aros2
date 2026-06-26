import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # 修改节点名称
    # return LaunchDescription([
    #     # Node(package='/* pkg_name */', executable='/* exec_name */', name='/* node_name */', output='screen'),
    #     Node(package='turtlesim', executable='turtlesim_node', name='turtle1', output='screen'),
    #     Node(package='turtlesim', executable='turtlesim_node', namespace='t1', output='screen'),
    #     Node(package='turtlesim', executable='turtlesim_node', namespace='t1', name="turtle1", output='screen'),
    # ])

    # 修改话题名称
    return LaunchDescription([
        Node(package="turtlesim",executable="turtlesim_node",namespace="t1"),
        Node(package="turtlesim",
            executable="turtlesim_node",
            remappings=[("/turtle1/cmd_vel","/cmd_vel")]
        )

    ])

    pass