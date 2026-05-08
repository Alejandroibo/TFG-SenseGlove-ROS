
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class InterpreteGuanteTurtlesim : public rclcpp::Node
{
public:
  InterpreteGuanteTurtlesim() : Node("interprete_guante_turtlesim") {
    auto topic_callback =
      [this](std_msgs::msg::String::UniquePtr msg) -> void {
        RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());
      };
    subscription_ =
      this->create_subscription<std_msgs::msg::String>("guante_estado", 10, topic_callback);  //Topic_callback deberia de ser bindeado a otra funcion
    
    
    publisher_ = this->create_publisher<std_msgs::msg::String>("turtle1/cmd_vel", 10);
  
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

//En el main habra que hacer todo el paripe de cargar los gestos que el usuario quiera + generar el diccionario de traducciones
//El nodo este tendra que crear su propio publish para mandar la informacion al turtlesim

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InterpreteGuanteTurtlesim>());
  rclcpp::shutdown();
  return 0;
}
