
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

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
        }

        void crearMapaAcciones(){
            traduccion = {{"InfoTest", "InfoResultadoTest"}};
        }

        void obtenerGesto(std_msgs::msg::String::SharedPtr msg) {
            RCLCPP_INFO(this->get_logger(), "Obtenemos: '%s'", msg->data.c_str());

            RCLCPP_INFO(this->get_logger(), "Decimos que estamos haciendo el paripe por ahora");

            std::string res = traduccion["InfoTest"];

            publicarGesto(res);
        }

        void publicarGesto(std::string nombreGesto){
            auto msg = std_msgs::msg::String();
            msg.data = nombreGesto;
            publisher_->publish(msg);        
        }

        //ConjuntoGestos conjunto;
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
