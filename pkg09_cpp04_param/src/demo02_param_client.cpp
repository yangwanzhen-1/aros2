/*
    需求：编写参数客户端，获取或修改服务端参数。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.查询参数；
            3-2.修改参数；
        4.创建节点对象指针，调用参数操作函数；
        5.释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;

// 3.自定义节点类；
class MinimalParamClient : public rclcpp::Node
{
public:
  MinimalParamClient() : Node("paramDemoClient_node")
  {
    RCLCPP_INFO(this->get_logger(), "paramDemoClient_node 节点创建！");
    // 创建参数客户端对象，
    // 参数1：当前对象依赖的节点
    // 参数2：参数服务端节点名称
    paramClient = std::make_shared<rclcpp::SyncParametersClient>(this, "minimal_param_server");
    /*
      问题：服务通信不是通过服务话题关联吗？为什么参数客户端是通过参数服务端的节点名称关联？
      答：
        1.参数服务端启动后，底层封装了多个服务通信的服务端。
        2.每个服务端的话题，都是采用 /服务节点名称/xxxxxx;
        3.参数客户端创建后，也会封装多个服务通信的客户端；
        4.这些客户端与服务端相呼应，也要使用相同的话题，因此客户端在创建时需要使用服务端节点名称。
    
    */
  }

  bool connect_server(){
    // 等待服务连接
    while (!paramClient->wait_for_service(1s))
    {
      if(!rclcpp::ok()){
        return false;
      }
      RCLCPP_INFO(this->get_logger(),"服务未连接");
    }
    return true;
  }

  // 3-1.查询参数
  void get_param(){
    RCLCPP_INFO(this->get_logger(),"-----------参数客户端查询参数-----------");
    // 获得某个参数
    double height = paramClient->get_parameter<double>("height");
    RCLCPP_INFO(this->get_logger(),"height = %.2f", height);
    // 判断是否包含某个参数
    RCLCPP_INFO(this->get_logger(),"car_type 存在吗？%d", paramClient->has_parameter("car_type"));
    // 获得多个参数
    auto params = paramClient->get_parameters({"car_type","height","wheels"});
    for (auto &param : params)
    {
      RCLCPP_INFO(this->get_logger(),"%s = %s", param.get_name().c_str(),param.value_to_string().c_str());
      
    }
    
  }

  // 3-2.修改参数；
  void update_param(){
    RCLCPP_INFO(this->get_logger(),"-----------参数客户端修改参数-----------");
    paramClient->set_parameters({rclcpp::Parameter("car_type","Mouse"),
      rclcpp::Parameter("height",2.0),
      //这是服务端不存在的参数，只有服务端设置了rclcpp::NodeOptions().allow_undeclared_parameters(true)时，
      // 这个参数才会被成功设置。
      rclcpp::Parameter("width",0.15),
      rclcpp::Parameter("wheels",6)});
  }

protected:

private:
  rclcpp::SyncParametersClient::SharedPtr paramClient;
  
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  auto paramClient = std::make_shared<MinimalParamClient>();
  bool falg = paramClient->connect_server();
  if(!falg){
    return 0;
  }
  paramClient->get_param();
  paramClient->update_param();
  paramClient->get_param();


  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}