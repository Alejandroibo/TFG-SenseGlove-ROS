#include "senseglove_nova/ControlGuantes.h"
#include <cmath>
#include <thread>

void ControlGuantes::comprobarLibreria(){
    std::cout << ("Testing " + SGCore::Library::Version() + ", compiled for " + SGCore::Library::BackendVersion());
    if (SGCore::Library::GetBackendType() == SGCore::EBackendType::SharedMemory)// By default, your library will be compiled to use Shared Memory via the SGConnect library
    {
        std::cout << (" using " + SGCore::Library::SGConnectVersion());//If you replace SGConnect.dll, this will give you its current version number.
    }
    std::cout << std::endl;
    std::cout << ("The source code for this program is located in the SGCoreCs/test/ folder.") << std::endl;
    std::cout << ("=========================================================================") << std::endl;
}

bool ControlGuantes::comprobarSenseCom(){
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

    return connectionsActive;
}

void ControlGuantes::comprobarGuantes(){

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

string ControlGuantes::captarDatosGuante(bool derecha){
    std::string infoGuante = "NULL";

    if (!HandLayer::DeviceConnected(derecha))
        return infoGuante;

    std::string hand = derecha ? "right hand" : "left hand";
    SGCore::EHapticGloveCalibrationState calState = HandLayer::GetCalibrationState(derecha);
    SGCore:HandPose handPose;

    //Actually grab a HandPose
    if (HandLayer::GetHandPose(derecha, handPose)) {
        std::cout << ("Grabbed a HandPose for the " + hand + ":") << std::endl;
        std::cout << (handPose.ToString()) << std::endl;
        infoGuante = handPose.ToString();
    } else {
        std::cout << ("We couldn't grab a " + hand + " pose. That can happen because sensor data was corrupted, or because the glove is (no longer) connected. Try again later..") << std::endl;
    }

    return infoGuante;
}

void ControlGuantes::vibracionGuante(bool derecha, float amplitud, float duracion, 
    float frecuencia, DIGITO zonaVibracion){

    if (!HandLayer::DeviceConnected(derecha)) return;

    SGCore::EHapticLocation localizacion;

    switch (zonaVibracion){
        case DIGITO::PULGAR:
            localizacion = EHapticLocation::ThumbTip;
            break;
        case DIGITO::INDICE:
            localizacion = EHapticLocation::IndexTip;
            break;
        default:
            localizacion = EHapticLocation::WholeHand;
            break;
    }

    std::string hand = derecha ? "right hand" : "left hand";
    if (HandLayer::SupportsCustomWaveform(derecha, localizacion)) {
        SGCore::CustomWaveform waveform{amplitud, duracion, frecuencia};
        HandLayer::SendCustomWaveform(derecha, waveform, localizacion);
    } else {
        std::cout << ("The " + hand + " does not support Custom Waveforms (at " + HapticGlove::ToString(localizacion) + "), so we're sending a vibration to the Index Finger instead") << std::endl;
        //whole hand and / or custom waveforms not supported. So we're pulsing the index finger instead.
        HandLayer::QueueCommand_VibroLevel(derecha, 1, 1.0f, true);
        std::this_thread::sleep_for(std::chrono::milliseconds( (int32_t)(duracion * 1000) )); //s to ms
        HandLayer::QueueCommand_VibroLevel(derecha, 1, 0.0f, true);//turn it back off
    }
}

void ControlGuantes::feedbackFuerza(bool derecha, std::vector<float> dedosFF){
    HandLayer::QueueCommand_ForceFeedbackLevels(derecha, dedosFF, true);
    
}
