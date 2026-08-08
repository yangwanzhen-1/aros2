from pkg_resources import declare_namespace
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

"""
    需求：在launch文件启动的时候动态设置turtlesim_node的背景颜色
    实现：
        1.声明参数（变量）；
        2.调用参数（变量）；
        3.执行launch文件时动态导入参数。

"""

def generate_launch_description():
    # 1.声明参数（变量）；
    decl_bg_r = DeclareLaunchArgument(name="backg_r",default_value="255")
    decl_bg_g = DeclareLaunchArgument(name="backg_g",default_value="255")
    decl_bg_b = DeclareLaunchArgument(name="backg_b",default_value="255")
    # 2.调用参数（变量）；
    turtle = Node(package="turtlesim", 
            executable="turtlesim_node",
            parameters=[{"background_r": LaunchConfiguration("backg_r"), "background_g": LaunchConfiguration("backg_g"), "background_b": LaunchConfiguration("backg_b")}]
            )
    
    return LaunchDescription([decl_bg_r,decl_bg_g,decl_bg_b,turtle])

# 启动文件的时候，可以这传递参数 ros2 launch pkg16_cpp01_launch py03_args.launch.py backg_r:=125
