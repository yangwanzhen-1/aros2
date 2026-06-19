""" 
    运行：
    ros2 run pkg06_py02_service demo02_client_py 10 87

    需求：编写客户端，发送两个整型变量作为请求数据，并处理响应结果。
    步骤：
        1.导包；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.创建客户端；
            3-2.等待服务连接；
            3-3.组织请求数据并发送；
        4.创建对象调用其功能，处理响应结果；
        5.释放资源。

"""

# 1.导包；
import rclpy
import sys
from rclpy.node import Node
from base_interfaces_demo.srv import AddInts

# 3.定义节点类；
class MinimalClient(Node):
    def __init__(self):
        super().__init__('minimal_client_py')
        # 3-1.创建客户端；
        self.cli = self.create_client(AddInts,'add_ints')
        # 3-2.等待服务连接；
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('服务器连接中，请稍候...')
        self.req = AddInts.Request()

    # 3-3.组织请求数据并发送；
    def send_request(self):
        if len(sys.argv) == 3:
            self.req.num1 = int(sys.argv[1])
            self.req.num2 = int(sys.argv[2])
        else:
            self.get_logger().info("未提交两个整型数据，使用默认值: 10 和 20")
            self.req.num1 = 10
            self.req.num2 = 20
        self.future = self.cli.call_async(self.req)

def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    minimal_client = MinimalClient()
    minimal_client.send_request()

    # 处理响应
    rclpy.spin_until_future_complete(minimal_client,minimal_client.future)
    try:
        response: AddInts.Response = minimal_client.future.result()
    except Exception as e:
        minimal_client.get_logger().error('服务请求失败：%r'%(e,))
    else:
        minimal_client.get_logger().info('响应结果： %d + %d = %d' % (minimal_client.req.num1, minimal_client.req.num2, response.sum))

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()