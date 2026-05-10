
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
#include "turtlesim_guante/Gesto.h"

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
            //conjunto.addGesto("test", true, "test")
            Gesto g1("pulgarArriba", false), g2("pulgarBajo", false), g3("pulgarExtendido", false), g4("pulgarMedio", false);
            g1.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/1/pulgarArriba.txt");
            g2.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/1/pulgarBajo.txt");
            g3.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/1/pulgarExtendido.txt");
            g4.cargarInformacionDedo("/src/turtlesim_guante/data/izquierda/dedos/1/pulgarMedio.txt");

            conjunto.addGesto(g1);
            conjunto.addGesto(g2);
            conjunto.addGesto(g3);
            conjunto.addGesto(g4);
        }

        void crearMapaAcciones(){
            traduccion = {
                {"NULL", 0},
                {"pulgarArriba", 1},
                {"pulgarBajo", 2},
                {"pulgarExtendido", 3},
                {"pulgarMedio", 4},
            };
        }

        void obtenerGesto(std_msgs::msg::String::SharedPtr msg) {
            RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

            std::string gestoInfo = msg->data.c_str();

            Gesto gestoGuante("GestoGuante", false, gestoInfo);

            Gesto resultado = conjunto.getGestoProximo(gestoGuante, 10000);

            RCLCPP_INFO(this->get_logger(), "Resultado: '%s'", resultado.getNombreGesto());

            double linear = 0.0;
            double angular = 0.0;
            //std::string res = resultado.getNombreGesto();
            int res = traduccion[resultado.getNombreGesto()];

            switch (res){
              case 1:
                linear = 1.0;
                break;
              case 2:
                linear = -1.0;
                break;
              case 3:
                angular = 1.0;
                break;
              case 4:
                angular = -1.0;
                break;
              default:
                break;
            };
            
            if (linear != 0.0 || angular != 0.0) publicarMovimiento(linear, angular);
        }

        void publicarMovimiento(double linear, double angular){
            geometry_msgs::msg::Twist twist;
            twist.angular.z = scale_angular * angular;
            twist.linear.x = scale_linear * linear;

            publisher_->publish(twist);        
        }

        ConjuntoGestos conjunto;

        std::map<std::string, int> traduccion;

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
