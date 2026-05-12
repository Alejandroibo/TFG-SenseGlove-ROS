// Aplicacacion test para comprobar datos por pantalla
//

#define String std::string

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

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

using namespace SGCore;
using namespace SGCore::Kinematics;

/// <summary> Test HandPoses </summary>
/// <param name="rightHand"></param>
static void TestHandTracking(bool rightHand)
{
    if (!HandLayer::DeviceConnected(rightHand))
        return;

    std::string hand = rightHand ? "right hand" : "left hand";
    SGCore::EHapticGloveCalibrationState calState = HandLayer::GetCalibrationState(rightHand);
    SGCore:HandPose handPose;
    if (calState == EHapticGloveCalibrationState::Unknown) {
        std::cout << ("THe Calibration State of the " + hand + " is not known. So we cannot retrieve a proper HandPose...") << std::endl;
    }
    if (calState == EHapticGloveCalibrationState::MoveFingers)//improvised calibration
    {
        int32_t timeout = 20;             //every 20ms, since the Nova doesn't update more than 60Hz anyway.
        int32_t sanityTimeout = 20 * 1000;//10 s timeout.
        int32_t sanityTimer = 0;

        int32_t moveTimer = 0;
        int32_t moveTimeout = 1 * 1000;//s after you start moving
        std::cout << ("Calibration on the " + hand + " is yet to be completed. If it's a Nova Glove, calibration will automatically happen as you call a GetHandPose() / GetSensorData() function.") << std::endl;
        std::cout << ("Let's try it now: Move each of the sensors on your " + hand + " within the next " + std::to_string(sanityTimeout / 1000.0f) + " seconds. Pay special attention to your thumb sensor(s)") << std::endl;
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));//s to ms
            HandLayer::GetHandPose(rightHand, handPose);//normally, you'd grab this mutliple times.
            sanityTimer += timeout;
            calState = HandLayer::GetCalibrationState(rightHand);
            if (calState == EHapticGloveCalibrationState::AllSensorsMoved) {
                moveTimer += timeout;
            }
        } while (sanityTimer < sanityTimeout && moveTimer < moveTimeout);

        if (calState == EHapticGloveCalibrationState::MoveFingers)//you still haven't moved enough yet. That was the longer timeout.
        {
            std::cout << ("You haven't moved all sensors yet, so some of the angles might a bit off. But we can grab a HandPose either way. Press a key to continue...") << std::endl;
            system("pause");
        } else {
            std::cout << ("You've moved enough of your fingers! Press a key to grab a proper pose... ") << std::endl;
            system("pause");
        }
    } else {
        std::cout << ("Press a key to grab a HandPose for the " + hand) << std::endl;
        system("pause");
    }
    //Actually grab a HandPose
    if (HandLayer::GetHandPose(rightHand, handPose)) {
        std::cout << ("Grabbed a HandPose for the " + hand + ":") << std::endl;
        std::cout << (handPose.ToString()) << std::endl;
    } else {
        std::cout << ("We couldn't grab a " + hand + " pose. That can happen because sensor data was corrupted, or because the glove is (no longer) connected. Try again later..") << std::endl;
    }
}

static std::string GuardarGestoBasico(bool derecha, String dedo){

    if (!HandLayer::DeviceConnected(derecha))
        return "ERROR: Guante a capturar gestos no conectado";

    std::string hand = derecha ? "derecha" : "izquierda";
    SGCore:HandPose handPose;

    //Comprobamos camino y creamos ruta
    //////////////////////////////////////

    String rutaArchivoLocal = ("\\..\\data\\guantes\\"+ hand +"\\dedos\\"+dedo);

    std::filesystem::path pa = std::filesystem::current_path();
 
    std::cout << "The current path: " << pa;

    std::filesystem::path rutaArchivo = pa.string() + rutaArchivoLocal;

    std::cout << "The current path: " << rutaArchivo;

    if (std::filesystem::exists(rutaArchivo))
        std::cout << "File or directory exists\n";
    else
        //Reemplazar por creacion del directorio? O una afirmacion del usuario
        return "File or directory does not exist";

    //Obtenemos datos del gesto
    //////////////////////////////////////

    String datosGesto;
    String respuesta;    

    do{

        std::cout<<("Presione una tecla mientras matiene el gesto a guardar con el dedo")  << std::endl;
        
        std::cin.get();
        std::cin.ignore();

        if (HandLayer::GetHandPose(derecha, handPose)) {
            std::cout << ("Grabbed a HandPose for the " + hand + ":") << std::endl;
            std::cout << (handPose.ToString()) << std::endl;
        } else {
            return "We couldn't grab a " + hand + " pose. That can happen because sensor data was corrupted, or because the glove is (no longer) connected. Try again later..";
            
        }    

        std::cout<<("Desea guardar esos datos? y/n")  << std::endl;
        std::cin>>respuesta;

    } while (respuesta != "y" && respuesta != "Y");

    datosGesto = handPose.ToString();
    String datoProcesado = datosGesto;

    int dedoInt = stoi(dedo);

    //Cogemos la linea que nos interesa
    for (int i = 0; i < dedoInt; i++){
        size_t finlinea = datoProcesado.find_first_of('\n');
        datoProcesado = datoProcesado.substr(finlinea+1);
    }

    size_t finlinea = datoProcesado.find_first_of('\n');
    datoProcesado = datoProcesado.substr(0,finlinea);

    datosGesto = dedo + "\n" + datoProcesado;

    //Generamos fichero
    //////////////////////////////////////

    String nombreArchivo;
    
    std::cout<<("Introduzca nombre del gesto:")  << std::endl;
    std::cin >> nombreArchivo;
    std::ofstream ArchivoGesto (rutaArchivo.string() + "\\" +nombreArchivo+".txt");

    std::cout << "The current path " << (rutaArchivo.string() + "\\" +nombreArchivo+".txt") + " decomposes into:\n";

    ArchivoGesto << datosGesto;

    ArchivoGesto.close();

    return "Operacion completada con exito";

}

static std::string CargarGesto(String path, String dedo){

    std::filesystem::path pa = std::filesystem::current_path();
 
    std::cout << "The current path: " << pa;

    std::filesystem::path rutaArchivo = pa.string() + path;

    std::cout << "The current path: " << rutaArchivo;

    if (std::filesystem::exists(rutaArchivo))
        std::cout << "File or directory exists\n";
    else
        //Reemplazar por creacion del directorio? O una afirmacion del usuario
        return "File or directory does not exist";

    //Abrimos archivo
    //////////////////////////////

    std::ifstream ArchivoGesto(rutaArchivo);

    //Procesamos informacion:
    //////////////////////////////

    int linea = std::stoi(dedo);
    char datos[100];
    char dato;
    String numero;
    String resultado ="";

    while (!ArchivoGesto.eof()){
        //std::cout<<"/////"<<std::endl;

        ArchivoGesto.get(datos, 100, ' ');
        //std::cout<<datos<<std::endl;

        ArchivoGesto.get(dato);
        //std::cout<<dato<<std::endl;

        ArchivoGesto.get(datos, 10, ',');
        //std::cout<<datos<<std::endl;

        String numero(datos);
        resultado += (numero + " ");
    }

    ArchivoGesto.close();

    return resultado;

}

int32_t main()
{

    std::cout << ("Cargando Archivo de prueba.") << std::endl;
    std::cout << ("=========================================================================") << std::endl;  
    std::cout << CargarGesto("/../data/guantes/izquierda/dedos/1/Test2.txt", "1") << std::endl;  

    //SGCore::Diagnostics::Debugger::SetDebugLevel(SGCore::Diagnostics::EDebugLevel::All);
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Checking the Library

    // Displaying Library information - Useful to know when asking for any kind of support
    std::cout << ("Testing " + SGCore::Library::Version() + ", compiled for " + SGCore::Library::BackendVersion());
    if (SGCore::Library::GetBackendType() == SGCore::EBackendType::SharedMemory)// By default, your library will be compiled to use Shared Memory via the SGConnect library
    {
        std::cout << (" using " + SGCore::Library::SGConnectVersion());//If you replace SGConnect.dll, this will give you its current version number.
    }
    std::cout << std::endl;
    std::cout << ("The source code for this program is located in the SGCoreCs/test/ folder.") << std::endl;
    std::cout << ("=========================================================================") << std::endl;


    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Ensuring connectivity

    // Connecting to SenseGlove devices is done in a separate "Connection Process" - contained in the SGConnect library.
    // We can test if this Connection Process is running on this PC. Usually, it runs inside SenseCom.
    // It's good practice to start this process if your user has not sone so yet.
    {
        bool connectionsActive = SGCore::SenseCom::ScanningActive();//returns true if SenseCom (or another program) has started the SenseGlove Communications Process.
        if (!connectionsActive)                                    // If this process is not running yet, we can "Force-Start" SenseCom. Provided it has run on this PC at least once.
        {
            std::cout << ("SenseCom is not yet running. Without it, we cannot connect to SenseGlove devices.") << std::endl;
            bool startedSenseCom = SGCore::SenseCom::StartupSenseCom();//Returns true if the process was started.
            if (startedSenseCom) {
                std::cout << ("Successfully started SenseCom. It will take a few seconds to connect...") << std::endl;
                connectionsActive = SGCore::SenseCom::ScanningActive();//this will return false immedeately after you called StartupSenseCom(). Because the program has yet to initialize.
                                                                      // Even if SenseCom started and the connections process is active, there's no guarantee that the user has turned their device(s) on. More on that later.
            } else                                                    // If StartupSenseCom() returns false, you've either never run SenseCom, or it is already running. But at that point, the ScanningActive() should have returned true.
            {
                std::cout << ("Could not Start the SenseCom process.") << std::endl;
            }
            std::cout << ("-------------------------------------------------------------------------") << std::endl;
        }
    }


    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Checking for Connections
    {
        int32_t gloveAmount = HandLayer::GlovesConnected();//GlovesConnected gives you the amount of gloves connected to your system.
        while (gloveAmount == 0)                      //For this exercise, I'll keep trying to connect to a glove.
        {
            std::cout << ("Failed to Detect a Haptic Glove on your System. Please ensure your device is paired or connected via USB.") << std::endl;
            std::cout << ("Press Return to try again...") << std::endl;
            system("pause");
            gloveAmount = HandLayer::GlovesConnected();
        }

        // When we get here, we've got one glove connected. Yay.

        if (gloveAmount == 1) {
            std::cout << ("There is 1 Haptic Glove connected to your system.") << std::endl;
            bool rightHand = HandLayer::GetFirstGloveHandedness();
            std::cout << "It is a ";
            std::cout << (rightHand ? "Right" : "Left");
            std::cout << " handed glove of type " + SGDevice::ToString( HandLayer::GetDeviceType(rightHand) ) << std::endl;
        } else {
            std::cout << ("There are " + std::to_string( gloveAmount ) + " Haptic Gloves connected to your system.") << std::endl;

            if (HandLayer::DeviceConnected(true)) {
                std::cout << ("The right hand is a " + SGDevice::ToString( HandLayer::GetDeviceType(true) )) << std::endl;
            } else {
                std::cout << ("There is no right hand connected.") << std::endl;
            }

            if (HandLayer::DeviceConnected(false)) {
                std::cout << ("The left hand is a " + SGDevice::ToString( HandLayer::GetDeviceType(false) )) << std::endl;
            } else {
                std::cout << ("There is no left hand connected.") << std::endl;
            }
        }
        std::cout << ("-------------------------------------------------------------------------") << std::endl;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Finger Tracking
    {
        std::cout << ("Vamos a probar el tracking de los dedos") << std::endl;

        TestHandTracking(true);
        TestHandTracking(false);

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
                            std::cout << ("Elija digito:") << std::endl;
                            std::cout << ("---- 0: Pulgar") << std::endl;
                            std::cout << ("---- 1: Indice") << std::endl;
                            std::cout << ("---- 2: Anular") << std::endl;          
                            std::cout << ("---- 3: Corazon") << std::endl;
                            std::cout << ("---- 4: Mano") << std::endl;
                            std::cout << ("---- 5: Atras") << std::endl;       
                            std::cin >> respuesta;       
                        } while (respuesta != "0" && respuesta != "1" && respuesta != "2" && respuesta != "3" && respuesta != "4" && respuesta != "5");
                        
                        if (respuesta != "5"){
                            std::cout<< GuardarGestoBasico(g_derecho,respuesta) << std::endl;
                            std::cin.ignore();
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
