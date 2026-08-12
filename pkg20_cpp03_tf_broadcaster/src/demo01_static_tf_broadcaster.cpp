/*  
  需求：编写静态坐标变换程序，执行时传入两个坐标系的相对位姿关系以及父子级坐标系id，
       程序运行发布静态坐标变换。
       ros2 run pkg20_cpp03_tf_broadcaster demo01_static_tf_broadcaster 0.4 0 0.2 0 0 0 base_link laser
       ros2 run pkg20_cpp03_tf_broadcaster demo01_static_tf_broadcaster -0.5 0 0.4 0 0 0 base_link camera
  步骤：
    1.包含头文件；
    2.判断终端传入的参数是否合法；
    3.初始化 ROS 客户端；
    4.定义节点类；
      4-1.创建静态坐标变换发布方；
      4-2.组织并发布消息。
    5.调用 spin 函数，并传入对象指针；
    6.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/static_transform_broadcaster.h>

// 3.自定义节点类；
class MinimalStaticFrameBroadcaster : public rclcpp::Node
{
public:
  MinimalStaticFrameBroadcaster(char * transformation[]) : Node("minimal_static_frame_broadcaster")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 4-1.创建静态坐标变换发布方
    tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    this->make_transforms(transformation);


  }
protected:
    // 4-2.组织并发布消息。
  void make_transforms(char * transformation[]){
    // 组织消息
    geometry_msgs::msg::TransformStamped t;
    rclcpp::Time now = this->get_clock()->now();
    t.header.stamp = now;     // 时间戳
    t.header.frame_id = transformation[7];  // 父坐标系
    t.child_frame_id = transformation[8];   // 子坐标系
    // 设置偏移量
    t.transform.translation.x = atof(transformation[1]);
    t.transform.translation.y = atof(transformation[2]);
    t.transform.translation.z = atof(transformation[3]);
    // 设置四元数
    // 将欧拉角转换为四元数
    tf2::Quaternion q;
    q.setRPY(
      atof(transformation[4]),
      atof(transformation[5]),
      atof(transformation[6]));
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();
    // 发布消息
    tf_publisher_->sendTransform(t);
  }
private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher_;
  
};

int main(int argc, char *argv[])
{
  // 2.判断终端传入的参数是否合法；
  if (argc != 9) {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "运行程序时请按照：x y z roll pitch yaw frame_id child_frame_id 的格式传入参数");
    return 1;
  }

  // 3.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 5.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MinimalStaticFrameBroadcaster>(argv));

  // 6.释放资源；
  rclcpp::shutdown();
  return 0;
}