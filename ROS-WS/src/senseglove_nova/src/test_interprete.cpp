
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "senseglove_nova/ConjuntoGestos.h"
#include "senseglove_nova/Gesto.h"

class TestInterprete : public rclcpp::Node {
    public:
        TestInterprete() : Node("interprete_guante") {

            cargarListaGestos();
            crearMapaAcciones();

            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "guante_estado", 10, std::bind(&TestInterprete::obtenerGesto, this, std::placeholders::_1));    

            publisher_ = this->create_publisher<std_msgs::msg::String>("test_interprete", 10);
        }

    private:
        void cargarListaGestos(){
            ////// AÑADIR AQUI TUS GESTOS /////
            //conjunto.addGesto("test", true, "test")
            Gesto g1("pulgarArriba", false), g2("pulgarBajo", false), g3("pulgarExtendido", false), g4("pulgarMedio", false);
            g1.cargarInformacionDedo("/src/senseglove_nova/data/izquierda/dedos/1/pulgarArriba.txt");
            g2.cargarInformacionDedo("/src/senseglove_nova/data/izquierda/dedos/1/pulgarBajo.txt");
            g3.cargarInformacionDedo("/src/senseglove_nova/data/izquierda/dedos/1/pulgarExtendido.txt");
            g4.cargarInformacionDedo("/src/senseglove_nova/data/izquierda/dedos/1/pulgarMedio.txt");

            conjunto.addGesto(g1);
            conjunto.addGesto(g2);
            conjunto.addGesto(g3);
            conjunto.addGesto(g4);
        }

        void crearMapaAcciones(){
            traduccion = {
                {"InfoTest", "InfoResultadoTest"},
                {"NULL", "No hay gesto"},
                {"pulgarArriba", "Gesto: Pulgar Arriba"},
                {"pulgarBajo", "Gesto: Pulgar Abajo"},
                {"pulgarExtendido", "Gesto: Pulgar Extendido"},
                {"pulgarMedio", "Gesto: Pulgar Medio"},
            };
        }

        void obtenerGesto(std_msgs::msg::String::SharedPtr msg) {
            RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

            std::string gestoInfo = msg->data.c_str();

            Gesto gestoGuante("GestoGuante", false, gestoInfo);

            //RCLCPP_INFO(this->get_logger(), "test: '%s'", gestoGuante.to_string().substr(15,28));
            //std::cout<<gestoGuante.to_string()<<std::endl;

            Gesto resultado = conjunto.getGestoProximo(gestoGuante, 10000);

            RCLCPP_INFO(this->get_logger(), "Resultado: '%s'", resultado.getNombreGesto());

            std::string res = traduccion[resultado.getNombreGesto()];

            publicarGesto(res);
        }

        void publicarGesto(std::string nombreGesto){
            auto msg = std_msgs::msg::String();
            msg.data = nombreGesto;
            publisher_->publish(msg);        
        }

        ConjuntoGestos conjunto;
        std::map<std::string, std::string> traduccion;


        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

//En el main habra que hacer todo el paripe de cargar los gestos que el usuario quiera + generar el diccionario de traducciones
//El nodo este tendra que crear su propio publish para mandar la informacion al turtlesim

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TestInterprete>());
  rclcpp::shutdown();
  return 0;
}
