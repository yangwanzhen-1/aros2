/*
   需求：处理请求发送的目标点，控制乌龟向该目标点运动，并连续反馈乌龟与目标点之间的剩余距离。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
            3-1.创建原生乌龟位姿订阅方，回调函数中获取乌龟位姿；
            3-2.创建原生乌龟速度发布方；
            3-3.创建动作服务端；
            3-4.解析动作客户端发送的请求；
            3-5.处理动作客户端发送的取消请求；
            3-6.创建新线程处理请求；
            3-7.新线程产生连续反馈并响应最终结果。
       4.调用spin函数，并传入节点对象指针；
       5.释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::placeholders;
using ServerGoalHandle = rclcpp_action::ServerGoalHandle<base_interfaces_demo::action::Nav>;

// 3.自定义节点类；
class ExeNavActionServer : public rclcpp::Node
{
public:
  ExeNavActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions()) : 
    Node("exe_nav_action_server", options){

    RCLCPP_INFO(this->get_logger(), "节点创建！ExeNavActionServers");
    // 3-1.创建原生乌龟位姿订阅方，回调函数中获取乌龟位姿；
    pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&ExeNavActionServer::poseCallBack, this, _1));
    // 3-2.创建原生乌龟速度发布方；
    smd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    // 3-3.创建动作服务端；   
    nav_action_server_ = rclcpp_action::create_server<base_interfaces_demo::action::Nav>(
      this,
      "nav",
      std::bind(&ExeNavActionServer::goal_callback, this, _1, _2),
      std::bind(&ExeNavActionServer::cancel_callback, this, _1),
      std::bind(&ExeNavActionServer::accepted_callback, this, _1));
  }
protected:
  void poseCallBack(const turtlesim::msg::Pose::SharedPtr pose)
  {
    // RCLCPP_INFO(this->get_logger(), "Received message. turtle1_pose_(%f, %f, %f)",turtle1_pose_->x, turtle1_pose_->y, turtle1_pose_->theta);
    turtle1_pose_ = pose;
  }


  // 3-4.解析动作客户端发送的请求；
  rclcpp_action::GoalResponse goal_callback(const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const base_interfaces_demo::action::Nav::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(), "Received goal request");
    (void)uuid;
    if (false) {
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  
  // 3-5.处理动作客户端发送的取消请求；
  rclcpp_action::CancelResponse cancel_callback(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Received cancel request");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }
  
  // 3-5.处理动作客户端发送的取消请求；
  void accepted_callback(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    std::thread(std::bind(&ExeNavActionServer::execute, this, _1), goal_handle).detach();
  }
  
  // 3-7.新线程产生连续反馈并响应最终结果。
  void execute(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Executing goal");
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<base_interfaces_demo::action::Nav::Feedback>();
    auto result = std::make_shared<base_interfaces_demo::action::Nav::Result>();
  
    rclcpp::Rate loop_rate(1);
    for (size_t i = 0; i < 10; i++) {
      if (goal_handle->is_canceling()) {
        goal_handle->canceled(result);
        RCLCPP_ERROR(this->get_logger(), "Goal Canceled");
        return;
      }
  
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "Sent feedback");
      loop_rate.sleep();
    }
  
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "Successfully executed goal");
  }

private:
  turtlesim::msg::Pose::SharedPtr turtle1_pose_ = nullptr;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr smd_vel_pub_;
  rclcpp_action::Server<base_interfaces_demo::action::Nav>::SharedPtr nav_action_server_;
  
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<ExeNavActionServer>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}