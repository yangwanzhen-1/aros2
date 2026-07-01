

# 1.导包；
import rclpy
from rclpy.node import Node
from rclpy.timer import Rate
from rclpy.time import Time
from rclpy.duration import Duration

import threading

# 3.定义节点类；
class MyNode(Node):
    def __init__(self):
        super().__init__('time_node_py')
        # self.demo_rate()
        # self.demo_time()
        # self.demo_duration()
        self.demo_opt()

    def demo_opt(self):
        t1 = Time(seconds=20)
        t2 = Time(seconds=15)
        du1 = Duration(seconds=7)
        du2 = Duration(seconds=13)
        # 比较
        self.get_logger().info("t1 >= t2 ? %d" % (t1 >= t2))    # 1
        self.get_logger().info("t1 < t2 ? %d" % (t1 < t2))      # 0
        # 数学运算
        t3 = t1 + du1
        t4 = t1 - t2    
        t5 = t1 - du1        
        self.get_logger().info("t3 = %d" % t3.nanoseconds)
        self.get_logger().info("t4 = %d" % t4.nanoseconds)
        self.get_logger().info("t5 = %d" % t5.nanoseconds)

        # 比较
        self.get_logger().info("-" * 80)
        self.get_logger().info("du1 >= du2 ? %d" % (du1 >= du2))
        self.get_logger().info("du1 < du2 ? %d" % (du1 < du2))


    def demo_duration(self):
        # 1.创建duration对象
        du1 = Duration(seconds = 2,nanoseconds = 500000000)
        # 2.调用函数
        self.get_logger().info("ns = %d" % du1.nanoseconds)

    def demo_time(self):
        # 1. 创建 time 对象
        t1 = Time(seconds=10,nanoseconds=500000000)
        right_now = self.get_clock().now()

        # 2. 调用 time 函数
        self.get_logger().info("s = %.2f, ns = %d" % (right_now.seconds_nanoseconds()[0], right_now.seconds_nanoseconds()[1]))
        self.get_logger().info("s = %.2f, ns = %d" % (t1.seconds_nanoseconds()[0], t1.seconds_nanoseconds()[1]))
        self.get_logger().info("ns = %d" % right_now.nanoseconds)
        self.get_logger().info("ns = %d" % t1.nanoseconds)
        
    def demo_rate(self):
        # 1.创建 rate 对象
        self.rate = self.create_rate(1.0)
        # # 2.调用 sleep 函数 ---导致程序阻塞
        # while rclpy.ok():
        #     self.get_logger().info("+++++++++++++++++++++++")
        #     # self.rate.sleep()

        # 创建子线程实现运行评率控制
        thread = threading.Thread(target=self.do_some)
        thread.start()

    def do_some(self):
        while rclpy.ok():
            self.get_logger().info("+++++++++++++++++++++++")
            self.rate.sleep()

def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    rclpy.spin(MyNode())

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()