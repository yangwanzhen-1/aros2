/*  
  需求：编写动作服务端实习，可以提取客户端请求提交的整型数据，并累加从1到该数据之间的所有整数以求和，
       每累加一次都计算当前运算进度并连续反馈回客户端，最后，在将求和结果返回给客户端。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建动作服务端；
      3-2.处理请求数据；
      3-3.处理取消任务请求；
      3-4.生成连续反馈。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

using base_interfaces_demo::action::Progress;
using ClientGoalHandle = rclcpp_action::ClientGoalHandle<Progress>;
using std::placeholders::_1;
using std::placeholders::_2;

// 3.自定义节点类；
class MinimalActionClient : public rclcpp::Node
{
public:
  explicit MinimalActionClient(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions())
  : Node("minimal_action_client", node_options)
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // 3-1.创建动作客户端
    this->client_ptr_ = rclcpp_action::create_client<Progress>(this, "get_sum");
  }

  // 3-2.发送请求；
  void send_goal(int64_t num)
  {
    if(!this->client_ptr_){
      RCLCPP_ERROR(this->get_logger(), "动作客户端未被初始化。");
    }

    if(!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))){
      RCLCPP_ERROR(this->get_logger(), "服务连接失败！");
      return;
    }


    
    
    auto goal_msg = Progress::Goal();
    goal_msg.num = num;
    RCLCPP_INFO(this->get_logger(), "发送请求数据！数值：%ld", goal_msg.num);

    auto send_goal_options = rclcpp_action::Client<Progress>::SendGoalOptions();
    send_goal_options.goal_response_callback = std::bind(&MinimalActionClient::goal_response_callback, this, _1);
    send_goal_options.feedback_callback = std::bind(&MinimalActionClient::feedback_callback, this, _1, _2);
    send_goal_options.result_callback = std::bind(&MinimalActionClient::result_callback, this, _1);
    auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);

  }


protected:
  // 3-3.处理目标发送后的反馈；
  void goal_response_callback(const ClientGoalHandle::SharedPtr & future)
  {
    auto goal_handle = future.get();
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(), "目标请求被服务器拒绝！");
    } else {
      RCLCPP_INFO(this->get_logger(), "目标被接收，等待结果中");
    }
  }
  
  // 3-4.处理连续反馈；
  void feedback_callback(ClientGoalHandle::SharedPtr,
    const std::shared_ptr<const Progress::Feedback> feedback)
  {
    int32_t progress = (int32_t)(feedback->progress * 100);
    RCLCPP_INFO(this->get_logger(), "当前进度: %d%%", progress);
  }

  // 3-5.处理最终响应。
  void result_callback(const ClientGoalHandle::WrappedResult & result)
  {
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        // RCLCPP_INFO(this->get_logger(), "服务器已成功执行目标 Server successfully executed goal");
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "任务被中止 Goal was aborted");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "任务被取消 Goal was canceled");
        return;
      default:
        RCLCPP_ERROR(this->get_logger(), "未知异常 Unknown result code");
        return;
    }
    RCLCPP_INFO(this->get_logger(), "任务执行完毕，最终结果: %ld", result.result->sum);
    
    // note: 这里可以不要，我只是想让程序执行完毕后自动结束
    rclcpp::shutdown();
  }

private:
  rclcpp_action::Client<Progress>::SharedPtr client_ptr_;
  
};

int main(int argc, char *argv[])
{
  int32_t num1 = 10;
  if(argc != 2){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"未提交计算值，使用默认值计算 %d", num1);
  } else {
    num1 = atoi(argv[1]);
  }

  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  auto action_client = std::make_shared<MinimalActionClient>();
  action_client->send_goal(num1);
  rclcpp::spin(action_client);

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}