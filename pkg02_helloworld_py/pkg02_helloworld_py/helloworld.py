import rclpy
from rclpy.node import Node

# # 方式一：不推荐
# def main():
#     # 初始化 ROS2 
#     rclpy.init() 
#     # 创建节点 
#     node = rclpy.create_node("helloworld_py_node") 
#     # 输出文本 
#     node.get_logger().info("杨碗振 hello world!") 
#     # 释放资源 
#     rclpy.shutdown()


# 方式二：推荐
# 自定义类：
class MyNode(Node):
    def __init__(self):
        super().__init__("node_name_py")
        self.get_logger().info("hello world! (python继承方式)")

def main():
    # 初始化 ROS2 
    rclpy.init()   
    node = MyNode()
    rclpy.shutdown()
    # 。。。。。。

if __name__ == '__main__':
    main()


