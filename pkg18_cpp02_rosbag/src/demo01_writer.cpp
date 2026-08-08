/* 
  需求：录制 turtle_teleop_key 节点发布的速度指令。
  步骤：
    1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.创建写出对象指针；
      3-2.设置写出的目标文件；
      3-3.写出消息。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。

 */

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/writer.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

// 3.自定义节点类；
class SimpleBagRecorder : public rclcpp::Node
{
public:
  SimpleBagRecorder() : Node("simple_bag_recorder")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建写出对象指针；创建录制对象
    writer_ = std::make_unique<rosbag2_cpp::Writer>();
    // 3-2.设置写出的目标文件；设置磁盘文件
    writer_->open("my_bag");  // 相对路径，是工作空间的直接子级
    
    subscription_ = create_subscription<geometry_msgs::msg::Twist>(
      "/turtle1/cmd_vel", 10, std::bind(&SimpleBagRecorder::topic_callback, this, _1));


  }
protected:
  void topic_callback(std::shared_ptr<rclcpp::SerializedMessage> msg) const{
    rclcpp::Time time_stamp = this->now();
    // 3-3,写出消息
    // write(std::shared_ptr<const rclcpp::SerializedMessage> message, // 被写出的消除
    //   const std::string &topic_name, //话题名称
    //   const std::string &type_name, //消息类型
    //   const rclcpp::Time &time)  // 时间戳
    RCLCPP_INFO(this->get_logger(), "数据写出........");

    writer_->write(msg, "/turtle1/cmd_vel", "geometry_msgs/msg/Twist", time_stamp);

  }
private:
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
  std::unique_ptr<rosbag2_cpp::Writer> writer_;
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<SimpleBagRecorder>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}