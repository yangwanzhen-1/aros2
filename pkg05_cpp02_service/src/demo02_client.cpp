/*  
  使用：
  ros2 run pkg05_cpp02_service demo02_client
  ros2 run pkg05_cpp02_service demo02_client 3 5

  需求：编写客户端，发送两个整型变量作为请求数据，并处理响应结果。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建客户端；
      3-2.等待服务连接；
      3-3.组织请求数据并发送；
    4.创建对象指针调用其功能,并处理响应；
    5.释放资源。。
*/

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
using base_interfaces_demo::srv::AddInts;
using namespace std::chrono_literals;

class MinimalClient: public rclcpp::Node{
public:
  MinimalClient():Node("minimal_client"){
    RCLCPP_INFO(this->get_logger(),"[MinimalClient] 节点初始化");
    // 3-1.创建客户端；
    client_ = this->create_client<AddInts>("add_ints");
    RCLCPP_INFO(this->get_logger(),"客户端创建，等待连接服务端！");
  }

  // 3-2.等待服务连接；
  bool connect_server(){
    // 在指定超时时间内连接服务器，如果连接上了，那么返回true,否则返回false.
    while (!client_->wait_for_service(1s))  //循环以1s超时时间连接服务器，直到连接服务器才退出循环
    {
      // 对 ctrl+c 这个操作作出特殊处理
      // 1.怎么判断 ctrl+c按下？ 2.如何处理？
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强制退出！");
        return false;
      }
      RCLCPP_INFO(this->get_logger(), "服务连接中，清稍候。。。。");
    }
    return true;
  }

  rclcpp::Client<AddInts>::FutureAndRequestId send_request(int32_t num1, int32_t num2){
    /*
    rclcpp::Client<base_interfaces_demo::srv::AddInts>::FutureAndRequestId 
    async_send_request(std::shared_ptr<base_interfaces_demo::srv::AddInts_Request> request)
    */
    auto request = std::make_shared<AddInts::Request>();
    request->num1 = num1;
    request->num2 = num2;

    return client_->async_send_request(request);
  }



protected:

private:
  rclcpp::Client<AddInts>::SharedPtr client_;

};

int main(int argc, char const *argv[])
{
  int32_t num1 = 10;
  int32_t num2 = 20;
  if(argc != 3){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"未提交两个整型数据，使用默认值: %d 和 %d", num1, num2);
  } else {
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);
  }
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc,argv);

  // 4.调用spin函数，并传入节点对象指针；
  auto client = std::make_shared<MinimalClient>();
  bool flag = client->connect_server();
  if (!flag)
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接失败！");
    return 0;
  }

  auto response = client->send_request(num1,num2);
  // 处理响应
  if (rclcpp::spin_until_future_complete(client,response) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(client->get_logger(),"请求正常处理");
    RCLCPP_INFO(client->get_logger(),"响应结果:%d!", response.get()->sum);

  } else {
    RCLCPP_INFO(client->get_logger(),"请求异常");
  }

  // 5.释放资源。
  rclcpp::shutdown();
  return 0;
}
