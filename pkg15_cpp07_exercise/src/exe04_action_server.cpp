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
    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
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
    RCLCPP_INFO(this->get_logger(), "请求坐标:(%.2f,%.2f),航向:%.2f",goal->goal_x, goal->goal_y, goal->goal_theta);
    (void)uuid;
    if (goal->goal_x < 0 || goal->goal_x > 11.1 || goal->goal_y < 0 || goal->goal_y > 11.1) {
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  
  // 3-5.处理动作客户端发送的取消请求；
  rclcpp_action::CancelResponse cancel_callback(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "任取取消！");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }
  
  // 3-6.创建新线程处理请求；
  void accepted_callback(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    std::thread(std::bind(&ExeNavActionServer::execute, this, _1), goal_handle).detach();
  }
  
  // 3-7.新线程产生连续反馈并响应最终结果。
  void execute(const std::shared_ptr<ServerGoalHandle> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "开始执行任务......");
    // 解析目标值
    // const auto goal = goal_handle->get_goal();
    float goal_x = goal_handle->get_goal()->goal_x;
    float goal_y = goal_handle->get_goal()->goal_y;
    // 创建连续反馈对象指针；
    auto feedback = std::make_shared<base_interfaces_demo::action::Nav::Feedback>();
    // 创建最终结果对象指针；
    auto result = std::make_shared<base_interfaces_demo::action::Nav::Result>();
  
    rclcpp::Rate loop_rate(1);
    while (true) {
      // 任务执行中，关于客户端发送取消请求的处理；
      if (goal_handle->is_canceling()) {
        goal_handle->canceled(result);
        RCLCPP_ERROR(this->get_logger(), "Goal Canceled");
        return;
      }
  
      // 解析原生乌龟位姿数据；
      float turtle1_x = turtle1_pose_->x;
      float turtle1_y = turtle1_pose_->y;
      float turtle1_theta = turtle1_pose_->theta;
      // 计算原生乌龟与目标乌龟的x向以及y向距离；
      float x_distance = goal_x - turtle1_x;
      float y_distance = goal_y - turtle1_y;

      // 计算剩余距离
      float distance = std::hypot(x_distance, y_distance);

      // 计算目标方向
      float target_theta = std::atan2(y_distance, x_distance);
      float angle_error = target_theta - turtle1_theta;
      // 将角度误差归一化到 [-pi, pi]
      angle_error = std::atan2(std::sin(angle_error),std::cos(angle_error));

      // 计算速度
      geometry_msgs::msg::Twist twist;
      if(std::abs(angle_error) > 0.1){
        twist.linear.x = 0.0;
        twist.angular.z = 0.5 * angle_error;
      } else
      {
        twist.linear.x = 0.5 * distance;
        twist.angular.z = 0.5 * angle_error;
      }
      

      cmd_vel_pub_->publish(twist);


      // 当两龟距离小于0.15米时，将当前乌龟位姿设置进result并退出循环
      if(distance < 0.15){
        //将当前乌龟坐标赋值给 result
        result->turtle_x = turtle1_x;
        result->turtle_y = turtle1_y;
        result->turtle_theta = turtle1_theta;
        break;
      }
      feedback->distance = distance;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(),"Sent feedback, distance:%f",distance);
      loop_rate.sleep();
    }
    if(rclcpp::ok()){
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "任务结束!");
    }
  }

private:
  turtlesim::msg::Pose::SharedPtr turtle1_pose_ = nullptr;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
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