#include <rclcpp/rclcpp.hpp>
#include <iostream>
// // 方式1 不推荐
// int main(int argc, char * argv[])
// {
//   // 初始化 ROS2
//   rclcpp::init(argc,argv);

//   // 创建节点
//   auto node = rclcpp::Node::make_shared("helloworld_node");
  
  
//   // 输出文本
//   RCLCPP_INFO(node->get_logger(),"hello world");

//   // 释放资源
//   rclcpp::shutdown();
//   return 0;
// }

// // 方式二 推荐



#include "rclcpp/rclcpp.hpp"
class MyNode: public rclcpp::Node{
public:
  MyNode():Node("node_name"){
    RCLCPP_INFO(this->get_logger(), "hello,world!(继承的方式)");
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MyNode>();

  // .....

  
  rclcpp::shutdown();
  return 0;
}
