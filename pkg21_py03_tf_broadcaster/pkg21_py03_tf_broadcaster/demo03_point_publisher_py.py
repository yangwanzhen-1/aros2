"""  

    需求：发布雷达坐标系中某个坐标点相对于雷达（laser）坐标系的位姿。
    步骤：
        1.导包；
        2.初始化 ROS 客户端；
        3.定义节点类；
            3-1.创建坐标点发布方；
            3-2.创建定时器；
            3-3.组织并发布坐标点消息。
        4.调用 spin 函数，并传入对象；
        5.释放资源。
"""

# 1.导包；
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PointStamped

# 3.定义节点类；
class MinimalPointPublisher(Node):
    def __init__(self):
        super().__init__('minimal_point_publisher_py')
        # 3-1.创建坐标点发布方；
        self.pub = self.create_publisher(PointStamped, 'point', 10)
        # 3-2.创建定时器；
        self.timer = self.create_timer(1.0, self.on_timer)
        self.x = 0.1
    def on_timer(self):
        # 3-3.组织并发布坐标点消息。        
        ps = PointStamped()
        ps.header.stamp = self.get_clock().now().to_msg()
        ps.header.frame_id = 'laser'
        self.x += 0.02

        if self.x > 2: 
            self.x = 0

        ps.point.x = self.x
        ps.point.y = 0.0
        ps.point.z = 0.2
        self.pub.publish(ps)

def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    rclpy.spin(MinimalPointPublisher())

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()