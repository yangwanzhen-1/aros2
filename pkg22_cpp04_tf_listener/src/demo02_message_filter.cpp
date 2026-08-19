/*  
  需求：将雷达感知到的障碍物的坐标点数据（相对于 laser 坐标系），
       转换成相对于底盘坐标系（base_link）的坐标点。

      广播laser->base_link的坐标系相对关系，然后发布point->laser的坐标系，
      求解point->base_link的坐标系。

      ros2 run pkg20_cpp03_tf_broadcaster demo01_static_tf_broadcaster 0.4 0 0.2 0 0 0 base_link laser
      ros2 run pkg20_cpp03_tf_broadcaster demo03_point_publisher
      ros2 run pkg22_cpp04_tf_listener demo02_message_filter

  步骤：
    1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.创建tf缓存对象指针；
      3-2.创建tf监听器；
      3-3.创建坐标点订阅方并订阅指定话题；
      3-4.创建消息过滤器过滤被处理的数据；
      3-5.为消息过滤器注册转换坐标点数据的回调函数。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/create_timer_ros.h"
#include "tf2_ros/message_filter.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include <message_filters/subscriber.h>
#include <geometry_msgs/msg/point_stamped.hpp>

// 3.自定义节点类；
class MessageFilterPointListener : public rclcpp::Node
{
public:
  MessageFilterPointListener() : Node("message_filter_point_listener")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    target_frame_ = "base_link";
    typedef std::chrono::duration<int> seconds_type;
    seconds_type buffer_timeout(1);
    // 3-1.创建tf缓存对象指针
    tf2_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
        this->get_node_base_interface(),
        this->get_node_timers_interface());
    tf2_buffer_->setCreateTimerInterface(timer_interface);
    // 3-2.创建tf监听器；
    tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
    // 3-3.创建坐标点订阅方并订阅指定话题；
    point_sub_.subscribe(this, "point");
    // 3-4.创建消息过滤器过滤被处理的数据；
    tf2_filter_ = std::make_shared<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(
      point_sub_,*tf2_buffer_,target_frame_, 10, this->get_node_logging_interface(),
      this->get_node_clock_interface(), buffer_timeout);
    // 3-5.为消息过滤器注册转换坐标点数据的回调函数。
    tf2_filter_->registerCallback(&MessageFilterPointListener::msgCallback, this);

  }
protected:
  void msgCallback(const geometry_msgs::msg::PointStamped::SharedPtr point_ptr){
    // 实现坐标点变换
    geometry_msgs::msg::PointStamped point_out;
    try{
      tf2_buffer_->transform(*point_ptr, point_out, target_frame_);
      RCLCPP_INFO(
        this->get_logger(), "坐标点相对于base_link的坐标:(%.2f,%.2f,%.2f)",
        point_out.point.x,
        point_out.point.y,
        point_out.point.z);

    }catch (tf2::TransformException & ex){
        RCLCPP_WARN(
          // Print exception which was caught
          this->get_logger(), "Failure %s\n", ex.what());
    }
  }

private:
  std::string target_frame_;
  std::shared_ptr<tf2_ros::Buffer> tf2_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
  message_filters::Subscriber<geometry_msgs::msg::PointStamped> point_sub_;
  std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>> tf2_filter_;
  
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<MessageFilterPointListener>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}