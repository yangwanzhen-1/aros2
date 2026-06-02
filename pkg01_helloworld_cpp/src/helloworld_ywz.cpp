#include <rclcpp/rclcpp.hpp>
#include <iostream>
#include <memory>

/* 
// 方式1：不推荐的
int main(int argc, char * argv[])
{
  rclcpp::init(argc,argv);

  auto node = rclcpp::Node::make_shared("helloworld_node_ywz");
  float test_zz = 1;
  RCLCPP_INFO(node->get_logger(),"杨碗振----%f",test_zz);
  
  
  rclcpp::shutdown();
  return 0;
}
*/

// 方式2：推荐
// 自定义类继承 NOde
class MyNode: public rclcpp::Node{
public:
  MyNode():Node("helloworld_node_ywz_cpp"){
    RCLCPP_INFO(this->get_logger(), "helloworld_node_ywz");

  }

};

int main(int argc, char * argv[])
{
  // 初始化
  rclcpp::init(argc, argv);
  // 实例化自定义
  auto node = std::make_shared<MyNode>();

  // 资源释放
  rclcpp::shutdown();
  return 0;
}





