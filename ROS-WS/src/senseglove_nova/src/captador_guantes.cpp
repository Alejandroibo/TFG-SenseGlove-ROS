#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "senseglove_nova/LectorEscritorGestos.h"
#include "senseglove_nova/ControlGuantes.h"

using namespace std::chrono_literals;

class CaptadorGuante : public rclcpp::Node
{
public:
    CaptadorGuante() : Node("captador_guante")
    {
        ControlGuantes::comprobarLibreria();
        ControlGuantes::comprobarGuantes();

        publisher_ = this->create_publisher<std_msgs::msg::String>("guante_estado", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&CaptadorGuante::publicar_datos, this));
    }

private:
    void publicar_datos()
    {
        string gestoInfo = ControlGuantes::captarDatosGuante(false);
        auto msg = std_msgs::msg::String();
        //msg.data = "InfoTest";
        //msg.data = "Thumb:	CMC-Abd: 0,	CMC-Tw: -20,	CMC-Flx: -29,	MCP-Flx: 6,	IP-Flx: 13\nIndex:	MCP-Abd: 0,	MCP-Flx: 9,	PIP-Flx: 12,	DIP-Flx: 11\nMiddle:	MCP-Abd: 0,	MCP-Flx: 9,	PIP-Flx: 13,	DIP-Flx: 11\nRing:	MCP-Abd: 0,	MCP-Flx: 0,	PIP-Flx: 0,	DIP-Flx: 0\nPinky:	MCP-Abd: 0,	MCP-Flx: 0,	PIP-Flx: 0,	DIP-Flx: 0";
        msg.data = gestoInfo;
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
