// Aplicacacion test para comprobar datos por pantalla
//

#include <iostream>
#include <thread>
#include <vector>

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
static void TestHandTracking(bool rightHand, int iteraciones, bool extra = false)
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
    for (int i = 0; i < iteraciones; i++){

        if (HandLayer::GetHandPose(rightHand, handPose)) {
            std::cout << ("/////////////// Grabbed a HandPose for the " + hand + ":") << std::endl;
            std::cout <<(handPose.ToString()) << std::endl << std::endl;

            //std::cout <<(handPose.Serialize()) << std::endl << std::endl;

            if (extra){
                std::cout<<"TESTING DE DATOS DIRECTOS, EN VEZ DE TOSTRING LMAO"<<std::endl;
                std::cout<<"PRIMER TEST:"<<std::endl;
                for (int i = 0; i < handPose.GetHandAngles().size(); i++){
                    for (int j = 0; j < handPose.GetHandAngles()[i].size(); j++ ){
                        std::cout<<handPose.GetHandAngles()[i][j].ToString() <<" ";
                    }
                    std::cout<<"\n/////////////\n";
                }
                std::cout<<"\n";
                std::cout<<"SEGUNDO TEST:"<<std::endl;
                for (int i = 0; i < handPose.GetJointPositions().size(); i++){
                    for (int j = 0; j < handPose.GetJointPositions()[i].size(); j++ ){
                        std::cout<<handPose.GetJointPositions()[i][j].ToString() <<" ";
                    }
                    std::cout<<"\n/////////////\n";
                }
                std::cout<<"\n";
                std::cout<<"TERCER TEST:"<<std::endl;
                for (int i = 0; i < handPose.GetJointRotations().size(); i++){
                    for (int j = 0; j < handPose.GetJointRotations()[i].size(); j++ ){
                        std::cout<<handPose.GetJointRotations()[i][j].ToString() <<" ";
                    }
                    std::cout<<"\n/////////////\n";
                }
                std::cout<<"\n";
                std::cout<<"CUARTO TEST:"<<std::endl;
                for (int i = 0; i < handPose.GetNormalizedFlexion().size(); i++){
                    std::cout<<handPose.GetNormalizedFlexion()[i] <<" ";
                }
            }

        } else {
            std::cout << ("We couldn't grab a " + hand + " pose. That can happen because sensor data was corrupted, or because the glove is (no longer) connected. Try again later..") << std::endl;
        }

        std::cin.get();
    }
}


int32_t main()
{
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
        std::string iters_s;
        int iters = 0;
        std::cout << ("Vamos a obtener y mostrar los datos del guante. Presione enter para continuar. ") << std::endl;
        //std::cout << ("Alternativamente introduzca el numero de iteraciones (Tiempo a mostrar los datos, defecto: 5)") << std::endl;
        //std::cin >> iters;

        std::cin.get();

        TestHandTracking(true, 5);
        TestHandTracking(false, 5);
/*
        std::cin.get();

        TestHandTracking(true, 1, true);
        TestHandTracking(false, 1, true);
*/
        std::cout << ("-------------------------------------------------------------------------") << std::endl;
    }


    std::cout << "=========================================================================" << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file