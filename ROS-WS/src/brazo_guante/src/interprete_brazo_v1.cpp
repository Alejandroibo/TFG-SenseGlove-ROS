
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include "std_msgs/msg/string.hpp"

//#include "brazo_guante/qos.h"

#include "brazo_guante/ConjuntoGestos.h"
//#include "turtlesim_guante/Gesto.h"

class InterpreteBrazoRobot : public rclcpp::Node {
    public:
        InterpreteBrazoRobot() : Node("interprete_guante") {

            cargarListaGestos();
            crearMapaAcciones();

            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "guante_estado", 10, std::bind(&InterpreteBrazoRobot::obtenerGesto, this, std::placeholders::_1));    

            publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("/servo_node/delta_twist_cmds", 10);
        }

    private:
        void cargarListaGestos(){
            ////// AÑADIR AQUI TUS GESTOS /////
            //Pulgar
            Gesto g_P_1("pulgarArriba", false), g_P_2("pulgarBajo", false), g_P_3("pulgarExtendido", false), g_P_4("pulgarMedio", false);
            g_P_1.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarArriba.txt");
            g_P_2.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarBajo.txt");
            g_P_3.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarExtendido.txt");
            g_P_4.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/0/pulgarMedio.txt");

            conjuntoPulgar.addGesto(g_P_1);
            conjuntoPulgar.addGesto(g_P_2);
            conjuntoPulgar.addGesto(g_P_3);
            conjuntoPulgar.addGesto(g_P_4);

            //Indice + Anular
            Gesto g_IA_1("IndiceAnularExtendido", false), g_IA_2("IndiceAnularMitad", false), g_IA_3("IndiceAnularContraido", false);
            g_IA_1.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/1/indiceExtendido.txt");
            g_IA_1.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/2/anularExtendido.txt");
            g_IA_2.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/1/indiceMita.txt");
            g_IA_2.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/2/anularMitad.txt");
            g_IA_3.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/1/indiceContraido.txt");
            g_IA_3.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/2/anularContraido.txt");

            conjuntoIndiceAnular.addGesto(g_IA_1);
            conjuntoIndiceAnular.addGesto(g_IA_2);
            conjuntoIndiceAnular.addGesto(g_IA_3);

            //Corazon
            Gesto g_C_1("corazonExtendido", false), g_C_2("corazonMitad", false), g_C_3("corazonContraido", false);
            g_C_1.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/3/corazonContraido.txt");
            g_C_2.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/3/corazonExtendido.txt");
            g_C_3.cargarInformacionDedo("/src/brazo_guante/data/izquierda/dedos/3/corazonMitad.txt");

            conjuntoCorazon.addGesto(g_C_1);
            conjuntoCorazon.addGesto(g_C_2);    
            conjuntoCorazon.addGesto(g_C_3);
        }

        void crearMapaAcciones(){
            traduccionPulgar = {
                {"NULL", 0},
                {"pulgarArriba", 1},
                {"pulgarBajo", 2},
                {"pulgarMedio", 3},      
                {"pulgarExtendido", 4},          
            };

            traduccionIndiceAnular ={
                {"NULL", 0},
                {"IndiceAnularMitad", 1},
                {"IndiceAnularExtendido", 2},
                {"IndiceAnularContraido", 3},       
            };

            traduccionCorazon = {
                {"NULL", 0},
                {"corazonMitad", 1},
                {"corazonExtendido", 2},
                {"corazonContraido", 3},                
            };
        }

        void obtenerGesto(std_msgs::msg::String::SharedPtr msg) {
            RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

            std::string gestoInfo = msg->data.c_str();

            Gesto gestoGuante("GestoGuante", false, gestoInfo);

            Gesto resultadoPulgar = conjuntoPulgar.getGestoProximo(gestoGuante, 10000);
            Gesto resultadoIndiceAnular = conjuntoIndiceAnular.getGestoProximo(gestoGuante, 10000);
            Gesto resultadoCorazon = conjuntoCorazon.getGestoProximo(gestoGuante, 10000);

            RCLCPP_INFO(this->get_logger(), "Gesto Pulgar: '%s'", resultadoPulgar.getNombreGesto());
            RCLCPP_INFO(this->get_logger(), "Gesto Ind + An: '%s'", resultadoIndiceAnular.getNombreGesto());
            RCLCPP_INFO(this->get_logger(), "Gesto Corazon: '%s'", resultadoCorazon.getNombreGesto());

            double linear_X = 0.0;
            double linear_Y = 0.0;
            double linear_Z = 0.0;

            double angular_X = 0.0;
            double angular_Y = 0.0;
            double angular_Z = 0.0;

            int valor_pinza = 0;

            //std::string resPulgar = resultadoPulgar.getNombreGesto();
            int resPulgar = traduccionPulgar[resultadoPulgar.getNombreGesto()];
            int resIndAnl = traduccionIndiceAnular[resultadoIndiceAnular.getNombreGesto()];
            int resCorazon = traduccionCorazon[resultadoCorazon.getNombreGesto()];

            if (resIndAnl == 2){    //Extendido
                switch (resPulgar){
                case 1:
                    if (resCorazon == 3) angular_X = -1.0;
                    else linear_X = -1.0;
                    break;
                case 2:
                    if (resCorazon == 3) angular_X = 1.0;
                    else linear_X = 1.0;
                    break;
                case 3:
                    if (resCorazon == 3) angular_Z = -1.0;
                    else linear_Z = -1.0;
                    break;
                case 4:
                    if (resCorazon == 3) angular_Z = 1.0;
                    else linear_Z = 1.0;
                    break;
                default:
                    break;
                };
            } else if (resIndAnl == 1) {
                switch (resPulgar){
                case 1:
                    if (resCorazon == 3) angular_Y = -1.0;
                    else linear_Y = 1.0;
                    break;
                case 2:
                    if (resCorazon == 3) angular_Y = -1.0;
                    else linear_Y = -1.0;
                    break;
                default:
                    break;
                };              
            } else if(resIndAnl == 3){
                switch (resPulgar){
                case 1:
                    valor_pinza = maxPinza;
                    break;
                case 3:
                    valor_pinza = minPinza;
                    break;
                default:
                    break;
                };                        
            }
            
            if ((linear_X != 0.0 || linear_Y != 0.0 || linear_Z != 0.0) || 
                (angular_X != 0.0 || angular_Y != 0.0 || angular_Z != 0.0) ||
                (valor_pinza != 0)) 
                publicarMovimiento(linear_X, linear_Y, linear_Z, angular_X, angular_Y, angular_Z);
        }

        void publicarMovimiento(double linear_X, double linear_Y, double linear_Z, 
            double angular_X, double angular_Y, double angular_Z){

            geometry_msgs::msg::TwistStamped mensaje;
            std::string header = "base_link";
            mensaje.header.frame_id = header;
            mensaje.twist.linear.x = linear_X * scale_linear_x;
            mensaje.twist.linear.y = linear_Y * scale_linear_y;
            mensaje.twist.linear.z = linear_Z * scale_linear_z;

            mensaje.twist.angular.x = angular_X * scale_angular_x;
            mensaje.twist.angular.y = angular_Y * scale_angular_y;
            mensaje.twist.angular.z = angular_Z * scale_angular_z;

            publisher_->publish(mensaje);
        }

        ConjuntoGestos conjuntoPulgar;
        ConjuntoGestos conjuntoIndiceAnular;
        ConjuntoGestos conjuntoCorazon;

        std::map<std::string, int> traduccionPulgar;
        std::map<std::string, int> traduccionIndiceAnular;
        std::map<std::string, int> traduccionCorazon;

        double scale_linear_x = 0.1;
        double scale_linear_y = 0.1;
        double scale_linear_z = 0.1;

        double scale_angular_x = 0.1;
        double scale_angular_y = 0.1;
        double scale_angular_z = 0.1;

        int maxPinza = 770;
        int minPinza = 330;

        rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
        
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InterpreteBrazoRobot>());
  rclcpp::shutdown();
  return 0;
}
