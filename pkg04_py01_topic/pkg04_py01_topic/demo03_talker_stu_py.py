"""  
    使用：
    . install/setup.bash
    ros2 run pkg04_py01_topic demo03_talker_stu_py 

    需求：以某个固定频率发送文本“hello world!”，文本后缀编号，每发送一条消息，编号递增1。
    步骤：
        1.导包；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.创建发布方；
            3-2.创建定时器；
            3-3.组织消息并发布。
        4.调用spin函数，并传入节点对象；
        5.释放资源。
"""

# 1.导包；
import rclpy
from rclpy.node import Node
from base_interfaces_demo.msg import Student

# 3.定义节点类；
class MinimalPublisher(Node):
    def __init__(self):
        super().__init__('minimal_publisher_py')
        # 3-1.创建发布方；
        self.publisher_ = self.create_publisher(Student, 'topic_stu', 10)
        # 3-2.创建定时器
        timer_period = 1.0
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    # 3-3.组织消息并发布。
    def timer_callback(self):
        stu = Student()
        stu.name = "李四"
        stu.age = self.i
        stu.height = 1.70
        self.publisher_.publish(stu)
        self.get_logger().info('发布的学生消息(py): name=%s,age=%d,height=%.2f' % (stu.name, stu.age, stu.height))
        self.i += 1


def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    minimal_publisher = MinimalPublisher()
    rclpy.spin(minimal_publisher)

    # 5.释放资源。
    rclpy.shutdown()

if __name__ == '__main__':
    main()
