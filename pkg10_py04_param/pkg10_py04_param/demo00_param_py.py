"""  

    需求：节点功能描述
    步骤：
        1.导包；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.参数对象创建
            3-2.参数对象解析（获取键、值、将值转换成字符串....）
        4.调用spin函数，并传入节点对象；
        5.释放资源。
"""

# 1.导包；
import rclpy
from rclpy.node import Node

# 3.定义节点类；
class MyParam(Node):
    def __init__(self):
        super().__init__('my_param_node_py')
        self.get_logger().info("产生API使用（python）")
        # 3-1.参数对象创建
        p1 = rclpy.Parameter("car_name", value="Horse")
        p2 = rclpy.Parameter("length", value=0.5)
        p3 = rclpy.Parameter("wheels", value=4)

        # 3-2.参数对象解析（获取键、值、将值转换成字符串....）
        # 获取参数值
        self.get_logger().info("car_name = %s" % p1.value)
        self.get_logger().info("length = %s" % p2.value)
        self.get_logger().info("carwheels_name = %s" % p3.value)

        # 获取参数键
        self.get_logger().info("p1 name = %s" % p1.name)

def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    rclpy.spin(MyParam())

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()