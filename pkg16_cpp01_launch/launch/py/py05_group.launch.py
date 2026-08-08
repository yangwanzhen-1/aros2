from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.actions import PushRosNamespace
from launch.actions import GroupAction
"""
    4.2.5 分组设置
    需求：创建三个turtlesim_node，然后将前两个划分为一组，第三个单独一组
"""
def generate_launch_description():
    # 创建三个turtlesim_node
    turtle1 = Node(package="turtlesim",executable="turtlesim_node",name="t1")
    turtle2 = Node(package="turtlesim",executable="turtlesim_node",name="t2")
    turtle3 = Node(package="turtlesim",executable="turtlesim_node",name="t3")
    # 分组
    # 设置当前组的命名空间，以及包含的节点
    g1 = GroupAction(actions=[PushRosNamespace(namespace="g1"),turtle1, turtle2])
    g2 = GroupAction(actions=[PushRosNamespace(namespace="g2"),turtle3])
    return LaunchDescription([g1,g2])