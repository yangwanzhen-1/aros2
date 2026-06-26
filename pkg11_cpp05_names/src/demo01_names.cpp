/*
  需求：
  流程：
    1. 包含头文件；
    2. 初始化 ROS 2 客户端；
    3. 自定义节点类；

    4. 调用 spin 函数，传入自定义类对象指针；
    5. 释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// 3.自定义节点类；
class MyNode : public rclcpp::Node
{
public:
  MyNode() : Node("zhen_keng","zuoxie")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 全局话题：和命名空间，节点名称无关系
    publisher_ = this->create_publisher<std_msgs::msg::String>("/shi",10);
    RCLCPP_INFO(this->get_logger(), "ywz1, publisher topic: %s", publisher_->get_topic_name());
    // 相对话题：
    publisher_2_ = this->create_publisher<std_msgs::msg::String>("kaihui",10);
    RCLCPP_INFO(this->get_logger(), "ywz2, publisher topic: %s", publisher_2_->get_topic_name());

    publisher_3_ = this->create_publisher<std_msgs::msg::String>("~/vip",10);
    RCLCPP_INFO(this->get_logger(), "ywz3, publisher topic: %s", publisher_3_->get_topic_name());


  }
protected:

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_2_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_3_;
  
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MyNode>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}