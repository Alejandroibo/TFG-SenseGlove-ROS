
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twiststamped.hpp>
#include "std_msgs/msg/string.hpp"

#include "brazo_guante/qos.h"

#include "brazo_guante/ConjuntoGestos.h"
//#include "turtlesim_guante/Gesto.h"

class InterpreteBrazoRobot : public rclcpp::Node {
    public:
        InterpreteBrazoRobot() : Node("interprete_guante") {

            cargarListaGestos();
            crearMapaAcciones();

            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "guante_estado", 10, std::bind(&InterpreteBrazoRobot::obtenerGesto, this, std::placeholders::_1));    

            publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("/servo_node/delta_twist_cmds", turtlesim::topic_qos());
        }

    private:
        void cargarListaGestos(){
            ////// AÑADIR AQUI TUS GESTOS /////
            Gesto g1("pulgarArriba", false), g2("pulgarBajo", false), g3("pulgarExtendido", false), g4("pulgarMedio", false);
            g1.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarArriba.txt");
            g2.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarBajo.txt");
            g3.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarExtendido.txt");
            g4.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarMedio.txt");

            conjuntoPulgar.addGesto(g1);
            conjuntoPulgar.addGesto(g2);
            conjuntoPulgar.addGesto(g3);
            conjuntoPulgar.addGesto(g4);

            Gesto g5("dedosContraidos", false), g6("dedosExtendidos", false);
            g5.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/3/corazonContraidoFull.txt");
            g6.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/3/corazonExtendido.txt");

            conjuntoDedos.addGesto(g5);
            conjuntoDedos.addGesto(g6);    
        }

        void crearMapaAcciones(){
            traduccionPulgar = {
                {"NULL", 0},
                {"pulgarArriba", 1},
                {"pulgarBajo", 2},
                {"pulgarExtendido", 3},
                {"pulgarMedio", 4},                
            };
            traduccionDedos = {
                {"NULL", 0},
                {"dedosNeutral", 0},
                {"dedosContraidos", 1},
                {"dedosExtendidos", 2},                
            };
        }

        void obtenerGesto(std_msgs::msg::String::SharedPtr msg) {
            RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

            std::string gestoInfo = msg->data.c_str();

            Gesto gestoGuante("GestoGuante", false, gestoInfo);

            Gesto resultadoPulgar = conjuntoPulgar.getGestoProximo(gestoGuante, 10000);
            Gesto resultadoDedos = conjuntoDedos.getGestoProximo(gestoGuante, 10000);

            RCLCPP_INFO(this->get_logger(), "Gesto Pulgar: '%s'", resultadoPulgar.getNombreGesto());
            RCLCPP_INFO(this->get_logger(), "Gesto Dedos: '%s'", resultadoDedos.getNombreGesto());

            double linear_X = 0.0;
            double linear_Y = 0.0;
            double linear_Z = 0.0

            //std::string resPulgar = resultadoPulgar.getNombreGesto();
            int resPulgar = traduccionPulgar[resultadoPulgar.getNombreGesto()];
            int resDedos = traduccionDedos[resultadoDedos.getNombreGesto()];

            if (resDedos == 2){
                switch (resPulgar){
                case 1:
                    linear_X = -1.0;
                    break;
                case 2:
                    linear_X = 1.0;
                    break;
                case 3:
                    linear_Z = -1.0;
                    break;
                case 4:
                    linear_Z = 1.0;
                    break;
                default:
                    break;
                };
            } else if (resDedos == 1 {
                switch (resPulgar){
                case 1:
                    linear_Y = 1.0;
                    break;
                case 2:
                    linear_Y = -1.0;
                    break;
                default:
                    break;
                };              
            })
            
            if (linear_X != 0.0 || linear_Y != 0.0 || linear_Z != 0.0) publicarMovimiento(linear_X, linear_Y, linear_Z);
        }

        void publicarMovimiento(double X, double Y, double Z){
            geometry_msgs::msg::TwistStamped mensaje;
            std::string header = "base_link";
            mensaje.header.frame_id = header;
            mensaje.twist.linear.x = X * scale_linear_x;
            mensaje.twist.linear.y = Y * scale_linear_y;
            mensaje.twist.linear.z = Z * scale_linear_z;

            publisher_->publish(mensaje);
        }

        ConjuntoGestos conjuntoPulgar;
        ConjuntoGestos conjuntoDedos;

        std::map<std::string, int> traduccionPulgar;
        std::map<std::string, int> traduccionDedos;

        double scale_linear_x = 0.1;
        double scale_linear_y = 0.1;
        double scale_linear_z = 0.1;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InterpreteBrazoRobot>());
  rclcpp::shutdown();
  return 0;
}
