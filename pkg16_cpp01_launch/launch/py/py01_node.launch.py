from launch import LaunchDescription
from launch_ros.actions import Node
import os

# 封装终端指令相关类-----------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明由于获取--------------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关-----------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关--------------------------
# from launch_ros.actions import PushROSNamespace
# from launch.actions import GroupAction
# 事件相关--------------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo
# 获取功能包下的share目录路径---------
from ament_index_python.packages import get_package_share_directory

"""
    需求：演示 NOde 的使用

    构造函数参数说明：
        :param: package 被执行的程序所属的功能包；
        :param: executable 可执行程序；
        :param: name 设置节点名称；
        :param: namespace 设置命名空间的；
        :param: exec_name 设置程序标签；
        :param: parameters 设置参数的
        :param: remappings 用来实现话题重映射
        :param: arguments 为节点传参数-> xx yy zz --ros-args
        :param: ros_arguments 为节点传参-> --ros-args xx yy

"""

def generate_launch_description():
    turtle1 = Node(package="turtlesim", 
                   executable="turtlesim_node", 
                   name="t1",
                   exec_name="turtle_label", # 表示流程的标签
                   respawn=True     #节点杀敌之后，是否重启
                   )
    
    turtle2 = Node(package="turtlesim", 
                executable="turtlesim_node", 
                name="t2",
                # 参数设置方式1,直接设置参数
                # parameters=[{"background_r": 0,"background_g": 0,"background_b": 0}],
                # 参数设置方式2: 从 yaml 文件加载参数，yaml 文件所属目录需要在配置文件中安装。
                parameters=[os.path.join(get_package_share_directory("pkg16_cpp01_launch"),"config","t2.yaml")],
                )
    turtle3 = Node(package="turtlesim", 
                executable="turtlesim_node", 
                name="t3", 
                remappings=[("/turtle1/cmd_vel","/cmd_vel")] #话题重映射
                )    
    
    # 相当于：ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t4_ns --remap __node:=t4
    turtle4 = Node(package="turtlesim", 
                executable="turtlesim_node",
                # 节点启动时传参，相当于 arguments 传参时添加前缀 --ros-args 
                ros_arguments=["--remap", "__ns:=/t4_ns", "--remap", "__node:=t4"]
                )
    
    return LaunchDescription([turtle1,turtle2,turtle3, turtle4])
