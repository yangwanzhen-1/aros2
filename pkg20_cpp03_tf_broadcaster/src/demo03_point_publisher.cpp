/*  
    需求：发布雷达坐标系中某个坐标点相对于雷达（laser）坐标系的位姿。
    步骤：
        1.包含头文件；
        2.初始化 ROS 客户端；
        3.定义节点类；
            3-1.创建坐标点发布方；
            3-2.创建定时器；
            3-3.组织并发布坐标点消息。
        4.调用 spin 函数，并传入对象指针；
        5.释放资源。


*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using namespace std::chrono_literals;

// 3.自定义节点类；
class MinimalPointPublisher : public rclcpp::Node
{
public:
  MinimalPointPublisher() : Node("minimal_point_publisher"),x(0.1)
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建坐标点发布方；
    point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", 10);
    // 3-2.创建定时器；
    timer_ = this->create_wall_timer(0.1s, std::bind(&MinimalPointPublisher::on_timer, this));

  }
protected:
  void on_timer(){
    // 3-3.组织并发布坐标点消息。
    geometry_msgs::msg::PointStamped point;
    point.header.frame_id = "laser";
    point.header.stamp = this->now();
    x += 0.004;
    if(x > 2) x = 0;
    point.point.x = x;
    point.point.y = 0.0;
    point.point.z = 0.1;
    point_pub_->publish(point);

  }
private:
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  double x;
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MinimalPointPublisher>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}