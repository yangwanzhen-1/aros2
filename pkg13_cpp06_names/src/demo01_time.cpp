

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;

// 3.自定义节点类；
class TimeNode : public rclcpp::Node
{
public:
  TimeNode() : Node("time_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "节点创建！");
    // DemoRate();
    // DemoTime();
    // DemoDuration();
    DemoOpt();
  }


protected:

private:
  void DemoOpt()
  {
    rclcpp::Time t1(1,500000000);
    rclcpp::Time t2(10,0);

    rclcpp::Duration du1(3,0);
    rclcpp::Duration du2(5,0);

    // 比较
    RCLCPP_INFO(this->get_logger(),"t1 >= t2 ? %d",t1 >= t2);
    RCLCPP_INFO(this->get_logger(),"t1 < t2 ? %d",t1 < t2);
    // 数学运算
    rclcpp::Time t3 = t2 + du1;
    rclcpp::Time t4 = t1 - du1;
    rclcpp::Duration du3 = t2 - t1;

    RCLCPP_INFO(this->get_logger(), "t3 = %.2f",t3.seconds());  
    RCLCPP_INFO(this->get_logger(), "t4 = %.2f",t4.seconds()); 
    RCLCPP_INFO(this->get_logger(), "du3 = %.2f",du3.seconds()); 

    RCLCPP_INFO(this->get_logger(),"--------------------------------------");
    // 比较
    RCLCPP_INFO(this->get_logger(),"du1 >= du2 ? %d", du1 >= du2);
    RCLCPP_INFO(this->get_logger(),"du1 < du2 ? %d", du1 < du2);
    // 数学运算
    rclcpp::Duration du4 = du1 * 3.0;
    rclcpp::Duration du5 = du1 + du2;
    rclcpp::Duration du6 = du1 - du2;

    RCLCPP_INFO(this->get_logger(), "du4 = %.2f",du4.seconds()); 
    RCLCPP_INFO(this->get_logger(), "du5 = %.2f",du5.seconds()); 
    RCLCPP_INFO(this->get_logger(), "du6 = %.2f",du6.seconds());
    
  }

  // 演示 Duration 的使用
  void DemoDuration()
  {
    // 1.创建 Duration 对象；
    rclcpp::Duration du1(1s);
    rclcpp::Duration du2(2,500000000);
    // 2.调用函数
    RCLCPP_INFO(this->get_logger(),"du1, s = %.2f, ns = %ld", du1.seconds(),du1.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"du2, s = %.2f, ns = %ld", du2.seconds(),du2.nanoseconds());
  }

  // 演示 rate 的使用
  void DemoRate(){
    // 1,创建 rate 对象
    rclcpp::Rate rate1(1000ms); // 创建 Rate 对象方式1
    rclcpp::Rate rate2(1.0); // 创建 Rate 对象方式2
    int count = 0;
    // 通过循环调用 rate 的 sleep 函数
    while (rclcpp::ok())
    {
      RCLCPP_INFO(this->get_logger(),"--hello rate---");
      count ++;
      if(count > 4){
        break;
      }
      // rate1.sleep();
      rate2.sleep();
    }
    

  }

  // 演示 Time 对象
  void DemoTime(){
    // 1.创建 Time 对象
    rclcpp::Time t1(10500000000L);
    rclcpp::Time t2(2,1000000000L);
    // rclcpp::Time right_now = this->get_clock()->now();
    rclcpp::Time right_now = this->now();

    // 2.调用 Time 对象的函数
    RCLCPP_INFO(this->get_logger(),"s = %.2f, ns = %ld",t1.seconds(),t1.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"s = %.2f, ns = %ld",t2.seconds(),t2.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"s = %.2f, ns = %ld",right_now.seconds(),right_now.nanoseconds());

  }
  
};

int main(int argc, char *argv[])
{
  // 2.初始化 ROS 2 客户端；
  rclcpp::init(argc, argv);

  // 4.调用 spin 函数，传入自定义类对象指针；
  rclcpp::spin(std::make_shared<TimeNode>());

  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}