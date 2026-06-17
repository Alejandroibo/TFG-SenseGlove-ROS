// Aplicacacion test para comprobar datos por pantalla
//

#define String std::string

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

#include "Aplicacion/LectorEscritorGestos.h"
#include "Aplicacion/ControlGuantes.h"
#include "Aplicacion/Gesto.h"

#include <SenseGlove/Connect/SGConnect.hpp>
#include <SenseGlove/Core/Debugger.hpp>
#include <SenseGlove/Core/SenseCom.hpp>
#include <SenseGlove/Core/Library.hpp>

#include <SenseGlove/Core/HandLayer.hpp>

#include <SenseGlove/Core/HandPose.hpp>
#include <SenseGlove/Core/Quat.hpp>
#include <SenseGlove/Core/Vect3D.hpp>

#include <SenseGlove/Core/HapticGlove.hpp>
#include <SenseGlove/Core/Tracking.hpp>
#include <SenseGlove/Core/StringUtils.hpp>

using namespace std;
using namespace SGCore;
using namespace SGCore::Kinematics;

int32_t main()
{

    /*
    std::cout << ("Cargando Archivo de prueba.") << std::endl;
    std::cout << ("=========================================================================") << std::endl;  
    std::cout << CargarGesto("/../data/guantes/izquierda/dedos/1/Test2.txt", "1") << std::endl;  
*/

    //SGCore::Diagnostics::Debugger::SetDebugLevel(SGCore::Diagnostics::EDebugLevel::All);
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Checking the Library

    ControlGuantes::comprobarLibreria();

    // Displaying Library information - Useful to know when asking for any kind of support

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Ensuring connectivity

    // Connecting to SenseGlove devices is done in a separate "Connection Process" - contained in the SGConnect library.
    // We can test if this Connection Process is running on this PC. Usually, it runs inside SenseCom.
    // It's good practice to start this process if your user has not sone so yet.
 
    ControlGuantes::comprobarSenseCom();

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Checking for Connections

    ControlGuantes::comprobarGuantes();


    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Finger Tracking
    {
        std::cout << ("Vamos a probar el tracking de los dedos") << std::endl;

        //TestHandTracking(true);
        //TestHandTracking(false);

        ControlGuantes::captarDatosGuante(true);
        ControlGuantes::captarDatosGuante(false);

        std::cin.ignore();

        std::cout << ("-------------------------------------------------------------------------") << std::endl;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Guardado de Gestos
    // Se selecionara una mano a guardar el gesto.
    // Despues se pedira que dedo se quiere guardar la posicion
    // Finalmente se guardaran los datos en la carpeta correspondiente en data
    {

        int32_t gloveAmount = HandLayer::GlovesConnected(); //GlovesConnected gives you the amount of gloves connected to your system.
        bool salir = false;
        bool atras = false;
        bool g_derecho = false;

        std::string respuesta;

        std::cout << ("Vamos a proceder a la lectura y guardado de gestos.") << std::endl;
        std::cout << ("El proceso se puede hacer individualmente por cada dedo, o hacer una lectura completa de la mano.") << std::endl;
        std::cout << ("(Presione enter para continuar)") << std::endl;
        std::cin.get();
        
        do {
            salir = false;
            //Comprobamos cuantos guantes hay conectados
            if (gloveAmount == 1){
                if (!atras){

                    std::cout<< ("Hay 1 guante conectado al ordenador.") <<std::endl;

                    if(HandLayer::DeviceConnected(true)){
                        std::cout<< ("El guante conectado es el derecho. Los gestos se guardaran para la mano derecha.") <<std::endl;
                        g_derecho = true;
                    } else if (HandLayer::DeviceConnected(false)){
                        std::cout<< ("El guante conectado es el izquierdo. Los gestos se guardaran para la mano izquierda.") <<std::endl;
                        g_derecho = false;                    
                    }
                } else salir = true;
            } else if (gloveAmount == 2){
                std::cout<< ("Hay 2 guantes conectados al ordenador.") <<std::endl;
                if(HandLayer::DeviceConnected(true)){
                    std::cout<< ("Hay un guante derecho conectado.") <<std::endl;
                }
                if (HandLayer::DeviceConnected(false)){
                    std::cout<< ("El guante conectado es el izquierdo. Los gestos se guardaran para la mano izquierda.") <<std::endl;
                }

                do {
                    std::cout << ("Por favor, elija una mano a capturar gestos:") << std::endl;
                    std::cout << ("---- 1: Izquierda") << std::endl;
                    std::cout << ("---- 2: Derecha") << std::endl;
                    std::cout << ("---- 3: Salir") << std::endl;   
                    std::cin >> respuesta;         
                } while (respuesta != "1" && respuesta != "2" && respuesta != "3");

                if (respuesta == "1"){
                    std::cout<< ("Los gestos se guardaran para la mano izquierda.") <<std::endl;
                    g_derecho = false;                    
                } else if (respuesta == "2"){
                    std::cout<< ("Los gestos se guardaran para la mano derecha.") <<std::endl;
                    g_derecho = true;                    
                } else salir = true;

            }
            if (!salir){
                do {
                    atras = false;
                    do {
                        std::cout << ("Por favor, elija una opcion:") << std::endl;
                        std::cout << ("---- 1: Guardar gesto") << std::endl;
                        std::cout << ("---- 2: Leer gesto") << std::endl;
                        std::cout << ("---- 3: Atras") << std::endl;
                        std::cin >> respuesta;
                    } while (respuesta != "1" && respuesta != "2" && respuesta != "3");

                    if (respuesta == "1"){
                        do{
                            std::cout << ("Por favor, elija un digito:") << std::endl;
                            std::cout << ("---- 0: Pulgar") << std::endl;
                            std::cout << ("---- 1: Indice") << std::endl;
                            std::cout << ("---- 2: Anular") << std::endl;          
                            std::cout << ("---- 3: Corazon/Menique") << std::endl;
                            std::cout << ("---- 4: Atras") << std::endl;       
                            std::cin >> respuesta;       
                        } while (respuesta != "0" && respuesta != "1" && respuesta != "2" && respuesta != "3" && respuesta != "4");
                        
                        if (respuesta != "4"){
                            String datosGesto;
                            String aceptar;    

                            std::cout<<("Presione una tecla mientras matiene el gesto a guardar con el dedo")  << std::endl;
                                
                            std::cin.get();
                            std::cin.ignore();

                            datosGesto = ControlGuantes::captarDatosGuante(g_derecho);
                            //std::cout<<datosGesto<<std::endl;

                            std::cout<<("Desea guardar esos datos? y/n")  << std::endl;
                            std::cin>>aceptar;

                            if (aceptar == "y" || aceptar == "Y") {
                                LectorEscritorGestos::guardarDatosDedo(datosGesto,g_derecho,respuesta);
                                std::cin.ignore();
                            }
                        }

                    } else if (respuesta == "2"){
                        std::cout << ("TO-DO") << std::endl;
                    } else{
                        atras = true;
                    }
                } while (!atras);
            }

        } while (!salir);

        std::cout << ("-------------------------------------------------------------------------") << std::endl;
    }

    std::cout << "=========================================================================" << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    system("pause");
}
