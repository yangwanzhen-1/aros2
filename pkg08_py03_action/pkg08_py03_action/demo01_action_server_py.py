"""  
    需求：编写动作服务端实习，可以提取客户端请求提交的整型数据，并累加从1到该数据之间的所有整数以求和，
       每累加一次都计算当前运算进度并连续反馈回客户端，最后，在将求和结果返回给客户端。
    步骤：
        1.导包；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.创建动作服务端；
            3-2.生成连续反馈；
            3-3.生成最终响应。
        4.调用spin函数，并传入节点对象；
        5.释放资源。
"""

# 1.导包；
import time
import rclpy
from rclpy.node import Node
from rclpy.action import ActionServer
from rclpy.action.server import ServerGoalHandle

from base_interfaces_demo.action import Progress

# 3.定义节点类；
class ProgressActionServer(Node):
    def __init__(self):
        super().__init__('progress_action_server')
        # 3-1.创建动作服务端;
        self._action_server = ActionServer(
            self,
            Progress,
            'get_sum',
            self.execute_callback)
        self.get_logger().info('动作服务已经启动！')

    # 3-4.生成连续反馈与最终响应（回调函数）；
    def execute_callback(self, goal_handle: ServerGoalHandle):
        self.get_logger().info('开始执行任务....')
        # 3-2.生成连续反馈；
        feedback_msg = Progress.Feedback()
        
        sum = 0
        for i in range(1, goal_handle.request.num + 1):
            sum += i
            feedback_msg.progress = i / goal_handle.request.num
            self.get_logger().info('连续反馈: %.2f' % feedback_msg.progress)
            goal_handle.publish_feedback(feedback_msg)
            time.sleep(0.1)
        
        # 3-3.生成最终响应。
        goal_handle.succeed()
        result = Progress.Result()
        result.sum = sum
        self.get_logger().info('任务完成！')

        return result        



def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    rclpy.spin(ProgressActionServer())

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()