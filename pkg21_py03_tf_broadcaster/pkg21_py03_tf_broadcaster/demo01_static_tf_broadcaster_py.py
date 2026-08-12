"""  
    需求：编写静态坐标变换程序，执行时传入两个坐标系的相对位姿关系以及父子级坐标系id，
         程序运行发布静态坐标变换。
         ros2 run pkg21_py03_tf_broadcaster demo01_static_tf_broadcaster_py 0.4 0 0.2 0 0 0 base_link laser
         ros2 run pkg21_py03_tf_broadcaster demo01_static_tf_broadcaster_py -0.5 0 0.4 0 0 0 base_link camera
    步骤：
        1.导包；
        2.判断终端传入的参数是否合法；
        3.初始化 ROS 客户端；
        4.定义节点类；
            4-1.创建静态坐标变换发布方；
            4-2.组织并发布消息。
        5.调用 spin 函数，并传入对象；
        6.释放资源。 

"""
# 1.导包；
import sys
from geometry_msgs.msg import TransformStamped
import rclpy
from rclpy.node import Node
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster
import tf_transformations
from rclpy.logging import get_logger

# 4.定义节点类；
class MinimalStaticFrameBroadcasterPy(Node):

    def __init__(self, transformation):
        super().__init__('minimal_static_frame_broadcaster_py')
        self.get_logger().info("minimal_static_frame_broadcaster_py 创建")
        # 4-1.创建静态坐标变换发布方；
        self._tf_publisher = StaticTransformBroadcaster(self)
        self.make_transforms(transformation)

    # 4-2.组织并发布消息。
    def make_transforms(self, transformation):
        # 组织消息
        static_transformStamped = TransformStamped()
        static_transformStamped.header.stamp = self.get_clock().now().to_msg()
        static_transformStamped.header.frame_id = transformation[7]     #设置父坐标系
        static_transformStamped.child_frame_id = transformation[8]      #设置子坐标系
        # 设置平移
        static_transformStamped.transform.translation.x = float(transformation[1])
        static_transformStamped.transform.translation.y = float(transformation[2])
        static_transformStamped.transform.translation.z = float(transformation[3])
        # 设置四元数
        # 将欧拉角转换为四元数
        quat = tf_transformations.quaternion_from_euler(
            float(transformation[4]), float(transformation[5]), float(transformation[6]))
        static_transformStamped.transform.rotation.x = quat[0]
        static_transformStamped.transform.rotation.y = quat[1]
        static_transformStamped.transform.rotation.z = quat[2]
        static_transformStamped.transform.rotation.w = quat[3]
        # 发布消息
        self._tf_publisher.sendTransform(static_transformStamped)


def main():
    # 2.判断终端传入的参数是否合法；
    if len(sys.argv) < 9:
        get_logger("rclpy").error('运行程序时请按照：x y z roll pitch yaw frame_id child_frame_id 的格式传入参数')
        sys.exit(0)

    # 3.初始化 ROS 客户端；
    rclpy.init()
    # 5.调用 spin 函数，并传入对象；
    node = MinimalStaticFrameBroadcasterPy(sys.argv)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    # 6.释放资源。 
    rclpy.shutdown()