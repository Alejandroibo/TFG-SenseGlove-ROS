#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class CaptadorGuante : public rclcpp::Node
{
public:
    CaptadorGuante() : Node("captador_guante")
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>("guante_estado", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&CaptadorGuante::publicar_datos, this));
    }

private:
    void publicar_datos()
    {
        auto msg = std_msgs::msg::String();
        msg.data = "InfoTest";
        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CaptadorGuante>());
  rclcpp::shutdown();
  return 0;
}
