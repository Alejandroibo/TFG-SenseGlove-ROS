#include "Aplicacion/ControlGuantes.h"
#include <cmath>

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
    if (!HandLayer::DeviceConnected(derecha))
        return "";

    std::string hand = derecha ? "right hand" : "left hand";
    SGCore::EHapticGloveCalibrationState calState = HandLayer::GetCalibrationState(derecha);
    SGCore:HandPose handPose;
/*
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
*/
    //Actually grab a HandPose
    if (HandLayer::GetHandPose(derecha, handPose)) {
        std::cout << ("Grabbed a HandPose for the " + hand + ":") << std::endl;
        std::cout << (handPose.ToString()) << std::endl;
    } else {
        std::cout << ("We couldn't grab a " + hand + " pose. That can happen because sensor data was corrupted, or because the glove is (no longer) connected. Try again later..") << std::endl;
    }

    return handPose.ToString();
}