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

// 3.自定义节点类；
class MyNode : public rclcpp::Node
{
public:
  MyNode() : Node("my_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
  }
protected:

private:
  
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