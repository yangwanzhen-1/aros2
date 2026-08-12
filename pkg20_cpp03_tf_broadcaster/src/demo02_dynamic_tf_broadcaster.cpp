/*   
  需求：编写动态坐标变换程序，启动 turtlesim_node 以及 turtle_teleop_key 后，该程序可以发布
       乌龟坐标系到窗口坐标系的坐标变换，并且键盘控制乌龟运动时，乌龟坐标系与窗口坐标系的相对关系
       也会实时更新。
  步骤：
    1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.创建动态坐标变换发布方；
      3-2.创建乌龟位姿订阅方；
      3-3.根据订阅到的乌龟位姿生成坐标帧并广播。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include <turtlesim/msg/pose.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <turtlesim/msg/pose.hpp>

using std::placeholders::_1;
// 3.自定义节点类；
class MinimalDynamicFrameBroadcaster : public rclcpp::Node
{
public:
  MinimalDynamicFrameBroadcaster() : Node("minimal_dynamic_frame_broadcaster")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建动态坐标变化发布方
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    subscription_= this->create_subscription<turtlesim::msg::Pose>(
      "/turtle1/pose", 10, std::bind(&MinimalDynamicFrameBroadcaster::handle_turtle_pose, this, _1));
  }
protected:
  // 3-3.根据订阅到的乌龟位姿生成坐标帧并广播。
  void handle_turtle_pose(const turtlesim::msg::Pose & msg){
    // 组织消息
    geometry_msgs::msg::TransformStamped tfs;
    rclcpp::Time now = this->get_clock()->now();

    tfs.header.stamp = now;
    tfs.header.frame_id = "world";
    tfs.child_frame_id = "turtle1";

    tfs.transform.translation.x = msg.x;
    tfs.transform.translation.y = msg.y;
    tfs.transform.translation.z = 0.0;

    // 从欧拉角转换为四元数
    tf2::Quaternion q;
    q.setRPY(0, 0 ,msg.theta);
    tfs.transform.rotation.x = q.x();
    tfs.transform.rotation.y = q.y();
    tfs.transform.rotation.z = q.z();
    tfs.transform.rotation.w = q.w();
    // 发布消息
    tf_broadcaster_->sendTransform(tfs);
  }
private:
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MinimalDynamicFrameBroadcaster>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}