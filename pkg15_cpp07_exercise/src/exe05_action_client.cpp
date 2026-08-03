/*
   需求：向动作服务端发送目标点数据，并处理服务端的响应数据。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
            3-1.创建动作客户端；
            3-2.发送请求数据，并处理服务端响应；
            3-3.处理目标响应；
            3-4.处理响应的连续反馈；
            3-5.处理最终响应。
       4.调用spin函数，并传入节点对象指针；
       5.释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

// 3.自定义节点类；
class ExeNavActionClient : public rclcpp::Node
{
public:
  ExeNavActionClient(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
    : Node("my_node_cpp", options){
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建动作客户端；
    nav_client = rclcpp_action::create_client<base_interfaces_demo::action::Nav>(this, "nav");


  }
  void send_goal(float x, float y, float theta){
    // 连接动作服务端，如果超时（5s），那么直接退出。
    if (!nav_client->wait_for_action_server(5s))
    {
        RCLCPP_ERROR(this->get_logger(),"服务连接失败!");
        return;
    }
    // 组织请求数据

    auto goal = base_interfaces_demo::action::Nav::Goal();
    goal.goal_x = x;
    goal.goal_y = y;
    goal.goal_theta = theta;
    auto send_goal_options = rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions();
    send_goal_options.goal_response_callback = std::bind(&ExeNavActionClient::goal_response_callback, this, _1);
    send_goal_options.feedback_callback = std::bind(&ExeNavActionClient::feedback_callback, this, _1, _2);
    send_goal_options.result_callback = std::bind(&ExeNavActionClient::result_callback, this, _1);
    RCLCPP_INFO(this->get_logger(), "Sending goal"); 
    nav_client->async_send_goal(goal, send_goal_options);

  }

protected:
  using ClientGoalHandle = rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Nav>;
  void goal_response_callback(const ClientGoalHandle::SharedPtr & future)
  {
    auto goal_handle = future.get();
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝");
    } else {
      RCLCPP_INFO(this->get_logger(), "目标请求被接收!");
    }
  }
  // 3-4.处理响应的连续反馈；
  void feedback_callback(
    ClientGoalHandle::SharedPtr,
    const std::shared_ptr<const base_interfaces_demo::action::Nav::Feedback> feedback)
  {
    RCLCPP_INFO(this->get_logger(),"距离目标点还有 %.2f 米。",feedback->distance);
  }
  // 3-5.处理最终响应。
  void result_callback(const ClientGoalHandle::WrappedResult & result)
  {
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        RCLCPP_INFO(this->get_logger(), "乌龟最终坐标:(%.2f,%.2f),航向:%.2f",
                    result.result->turtle_x, result.result->turtle_y, result.result->turtle_theta);
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "任务被中止");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "任务被取消");
        return;
      default:
        RCLCPP_ERROR(this->get_logger(), "未知异常");
        return;
    }
  }




private:
  rclcpp_action::Client<base_interfaces_demo::action::Nav>::SharedPtr nav_client;

};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  auto client = std::make_shared<ExeNavActionClient>();
  if (argc != 5)
  {
    RCLCPP_INFO(client->get_logger(),"请传入目标的位姿参数:(x,y,theta)");
    return 1;    
  }
  
  // 发送目标点
  client->send_goal(atof(argv[1]), atof(argv[2]), atof(argv[3]));

  rclcpp::spin(client);

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}