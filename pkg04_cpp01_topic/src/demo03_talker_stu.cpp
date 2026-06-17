/*
  需求：节点功能描述
  流程：
    1. 包含头文件；
    2. 初始化 ROS 2 客户端；
    3. 自定义节点类；

    4. 调用 spin 函数，传入自定义类对象指针；
    5. 释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using namespace std::chrono_literals;
using base_interfaces_demo::msg::Student;
// 3.自定义节点类；
class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher() : Node("student_publisher")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建发布方；
    /*
      模版：被发布的消息类型
      参数：
        1.话题名称；
        2.QOS(消息队列长度)；
      返回值：发布对象指针
    */
    publisher_ = this->create_publisher<Student>("topic_stu", 10);
    
    // 3-2.创建定时器
    timer_ = this->create_wall_timer(1s, std::bind(&MinimalPublisher::timer_callback, this));


  }

protected:
  void timer_callback()
    {
      // 3-3.组织消息并发布。
      auto stu = Student();
      if(count_>=100){
        count_ = 0;
      }
      stu.name = "张三";
      stu.age = count_++;
      stu.height = 1.65;

      RCLCPP_INFO(this->get_logger(), "学生信息:name=%s,age=%d,height=%.2f", stu.name.c_str(), stu.age, stu.height);
      publisher_->publish(stu);
    }
private:
  size_t count_;
  rclcpp::Publisher<Student>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MinimalPublisher>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}