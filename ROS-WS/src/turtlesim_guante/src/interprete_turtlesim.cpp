
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include "std_msgs/msg/string.hpp"

#include "turtlesim_guante/qos.h"

#include "turtlesim_guante/ConjuntoGestos.h"
//#include "turtlesim_guante/Gesto.h"

class InterpreteTurtlesim : public rclcpp::Node {
    public:
        InterpreteTurtlesim() : Node("interprete_guante") {

            cargarListaGestos();
            crearMapaAcciones();

            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "guante_estado", 10, std::bind(&InterpreteTurtlesim::obtenerGesto, this, std::placeholders::_1));    

            publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", turtlesim::topic_qos());
        }

    private:
        void cargarListaGestos(){
            ////// AÑADIR AQUI TUS GESTOS /////
            Gesto g1("pulgarArriba", false), g2("pulgarBajo", false), g3("pulgarExtendido", false), g4("pulgarMedio", false);
            g1.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/0/pulgarArriba.txt");
            g2.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/0/pulgarBajo.txt");
            g3.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/0/pulgarExtendido.txt");
            g4.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/0/pulgarMedio.txt");

            conjuntoPulgar.addGesto(g1);
            conjuntoPulgar.addGesto(g2);
            conjuntoPulgar.addGesto(g3);
            conjuntoPulgar.addGesto(g4);

            Gesto g5("dedosContraidos", false), g6("dedosExtendidos", false);
            g5.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/3/corazonContraidoFull.txt");
            g6.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/3/corazonExtendido.txt");

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
                {"corazonContraido", 5},
                {"corazonExtendido", 6},                
            };
            traduccionDedos = {
                {"NULL", 0},
                {"dedosContraidos", 0},
                {"dedosExtendidos", 1},                
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

            double linear = 0.0;
            double angular = 0.0;
            //std::string resPulgar = resultadoPulgar.getNombreGesto();
            int resPulgar = traduccionPulgar[resultadoPulgar.getNombreGesto()];
            int resDedos = traduccionDedos[resultadoDedos.getNombreGesto()];

            if (resDedos == 1){
                switch (resPulgar){
                case 1:
                    linear = 1.0;
                    break;
                case 2:
                    linear = -1.0;
                    break;
                case 3:
                    angular = -1.0;
                    break;
                case 4:
                    angular = 1.0;
                    break;
                default:
                    break;
                };
            }
            
            if (linear != 0.0 || angular != 0.0) publicarMovimiento(linear, angular);
        }

        void publicarMovimiento(double linear, double angular){
            geometry_msgs::msg::Twist twist;
            twist.angular.z = scale_angular * angular;
            twist.linear.x = scale_linear * linear;

            publisher_->publish(twist);        
        }

        ConjuntoGestos conjuntoPulgar;
        ConjuntoGestos conjuntoDedos;

        std::map<std::string, int> traduccionPulgar;
        std::map<std::string, int> traduccionDedos;

        double scale_linear = 2.0;
        double scale_angular = 2.0;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

//En el main habra que hacer todo el paripe de cargar los gestos que el usuario quiera + generar el diccionario de traducciones
//El nodo este tendra que crear su propio publish para mandar la informacion al turtlesim

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InterpreteTurtlesim>());
  rclcpp::shutdown();
  return 0;
}
