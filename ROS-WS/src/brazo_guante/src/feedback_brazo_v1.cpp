
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

#include "brazo_guante/ControlGuantes.h"
#include "brazo_guante/DigitoEnum.h"

class FeedbackBrazoRobot : public rclcpp::Node {
    public:
        FeedbackBrazoRobot(): Node("feedback_brazo"){

            subscription_ = this->create_subscription<std_msgs::msg::Bool>(
                "grip_detected", 10, std::bind(&FeedbackBrazoRobot::aplicarFeedback, this, std::placeholders::_1));    

        }

private:
    void aplicarFeedback(std_msgs::msg::Bool::SharedPtr msg) {
        //RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

        //bool info = msg->data;

        if (msg->data == true){
            std::vector<float> fingerFfb = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };//there's 5 fingers.

            ControlGuantes::vibracionGuante(false, amplitud, duracion, frecuencia, DIGITO::MANO);
            ControlGuantes::feedbackFuerza(false, fingerFfb);
        }

        else if (msg->data == false){
            std::vector<float> fingerFfb = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };//there's 5 fingers.

            ControlGuantes::feedbackFuerza(false, fingerFfb);
        }

    }

    float amplitud = 1.0f; //100% intensity
    float duracion = 0.200f;//200 ms
    float frecuencia = 80.0f;//80 Hz

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FeedbackBrazoRobot>());
  rclcpp::shutdown();
  return 0;
}
