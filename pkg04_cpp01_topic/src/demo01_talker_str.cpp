/*
使用指令
. install/setup.bash
ros2 run pkg04_cpp01_topic demo01_talker_str 

*/

/*

  需求：以某个固定频率发送文本“hello world!”,文本后缀编号，每发布一条，编号+1。
  流程：
    1.包含头文件；
    2.初始化ROS2客户端；
    3.自定义节点类；
      3-1.创建消息发布方；
      3-2.创建定时器；
      3-3.组织并发布消息；
    4.调用spin函数，传入自定义类对象指针；
    5.释放资源；


*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;

// 3.定义节点类；
class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher() : Node("minimal_publisher"), count_(0)
  {
    RCLCPP_INFO(this->get_logger(), "发布节点创建！");
    // 3-1.创建发布方；
    /*
      模版：被发布的消息类型
      参数：
        1.话题名称；
        2.QOS(消息队列长度)；
      返回值：发布对象指针
    */
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    
    // 3-2.创建定时器
    timer_ = this->create_wall_timer(1s, std::bind(&MinimalPublisher::timer_callback, this));

  }
protected:
  void timer_callback()
    {
      // 3-3.组织消息并发布。
      auto message = std_msgs::msg::String();
      if(count_>=100){
        count_ = 0;
      }
      message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "发布的消息：'%s'", message.data.c_str());
      publisher_->publish(message);
    }
private:
  size_t count_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;


};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());

  // 5.释放资源
  rclcpp::shutdown();
  return 0;
}
