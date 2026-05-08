
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TestDatosGuante : public rclcpp::Node
{
public:
  TestDatosGuante()
  : Node("test_datos_guantes")
  {
    auto topic_callback =
      [this](std_msgs::msg::String::UniquePtr msg) -> void {
        RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());
      };
      
    //subscription_ = this->create_subscription<std_msgs::msg::String>("guante_estado", 10, topic_callback);
    subscription_ = this->create_subscription<std_msgs::msg::String>("test_interprete", 10, topic_callback);
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TestDatosGuante>());
  rclcpp::shutdown();
  return 0;
}
