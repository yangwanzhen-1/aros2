"""  

    需求：节点功能描述
    步骤：
        1.导包；
        2.初始化 ROS2 客户端；
        3.定义节点类；

        4.调用spin函数，并传入节点对象；
        5.释放资源。
"""

# 1.导包；
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# 3.定义节点类；
class MyNode(Node):
    def __init__(self):
        super().__init__('zhenkeng_py',namespace="zuoxie_py")
        # 全局话题
        self.pub1 = self.create_publisher(String, "/shi",10)
        self.get_logger().info("pub1 的话题名字是：%s" % self.pub1.topic_name)

        # 相对话题
        self.pub2 = self.create_publisher(String, "shi",10)
        self.get_logger().info("pub1 的话题名字是：%s" % self.pub2.topic_name)

        # 私有话题
        self.pub2 = self.create_publisher(String, "~/shi",10)
        self.get_logger().info("pub1 的话题名字是：%s" % self.pub2.topic_name)

def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    rclpy.spin(MyNode())

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()