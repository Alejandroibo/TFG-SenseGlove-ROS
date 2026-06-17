
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "senseglove_nova/ControlGuantes.h"
#include "senseglove_nova/DigitoEnum.h"

class TestFeedback : public rclcpp::Node {
    public:
        TestFeedback(): Node("test_feedback"){

            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "test_interprete", 10, std::bind(&TestFeedback::aplicarFeedback, this, std::placeholders::_1));    

        }

private:
    void aplicarFeedback(std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

        std::string info = msg->data.c_str();

        if (info == "Gesto: Pulgar Extendido"){
            ControlGuantes::vibracionGuante(false, amplitud, duracion, frecuencia, DIGITO::MANO);
        }

        if (info == "Gesto: Pulgar Abajo"){
            ControlGuantes::vibracionGuante(false, amplitud, duracion, frecuencia, DIGITO::PULGAR);
            ControlGuantes::vibracionGuante(false, amplitud, duracion, frecuencia, DIGITO::INDICE);
        }

        if (info == "Gesto: Pulgar Medio"){
            std::vector<float> fingerFfb = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };//there's 5 fingers.

            ControlGuantes::feedbackFuerza(false, fingerFfb);
        }

        if (info == "Gesto: Pulgar Arriba"){
            std::vector<float> fingerFfb = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };//there's 5 fingers.

            ControlGuantes::feedbackFuerza(false, fingerFfb);
        }

    }

    float amplitud = 1.0f; //100% intensity
    float duracion = 0.200f;//200 ms
    float frecuencia = 80.0f;//80 Hz

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TestFeedback>());
  rclcpp::shutdown();
  return 0;
}
