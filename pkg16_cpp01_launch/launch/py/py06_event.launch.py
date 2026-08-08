from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
from launch.substitutions import FindExecutable
from launch.event_handlers import OnProcessStart, OnProcessExit

"""
    需求：为 turtlesim_node 绑定事件，节点启动对的时候，执行生成乌龟的程序，节点关闭的时候，执行日志输出的操作


"""
def generate_launch_description():
    turtle = Node(package="turtlesim", executable="turtlesim_node")
    spawn = ExecuteProcess(
        # ros2 service call /spawn turtlesim/srv/Spawn "{x: 8.0, y: 1.0, theta: 1.0, name: 'turtle2'}"
        cmd = [
            FindExecutable(name = "ros2"), # 不可以有空格
            " service call",
            " /spawn turtlesim/srv/Spawn",
            " \"{x: 8.0, y: 1.0,theta: 1.0, name: 'turtle2'}\""
        ],
        output = "both",
        shell = True
    )
    # 注册事件
    start_event = RegisterEventHandler(
        event_handler=OnProcessStart(
            target_action = turtle,
            on_start = spawn
        )
    )
    exit_event = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action = turtle,
            on_exit = [LogInfo(msg="turtlesim_node退出!")]
        )
    )
    return LaunchDescription([turtle, start_event, exit_event])