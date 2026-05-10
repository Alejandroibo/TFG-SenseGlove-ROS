#include <senseglove_nova/SenseGlove/Connect/SGConnect.hpp>
#include <senseglove_nova/SenseGlove/Core/Debugger.hpp>
#include <senseglove_nova/SenseGlove/Core/SenseCom.hpp>
#include <senseglove_nova/SenseGlove/Core/Library.hpp>

#include <senseglove_nova/SenseGlove/Core/HandLayer.hpp>

#include <senseglove_nova/SenseGlove/Core/HandPose.hpp>
#include <senseglove_nova/SenseGlove/Core/Quat.hpp>
#include <senseglove_nova/SenseGlove/Core/Vect3D.hpp>

#include <senseglove_nova/SenseGlove/Core/HapticGlove.hpp>
#include <senseglove_nova/SenseGlove/Core/Tracking.hpp>
#include <senseglove_nova/SenseGlove/Core/StringUtils.hpp>

#include <string>

#ifndef CONTROLGUANTES_H

#define CONTROLGUANTES_H

using namespace std;
using namespace SGCore;
using namespace SGCore::Kinematics;

class ControlGuantes{
    private:

    public:
        static string captarDatosGuante(bool derecho);

        static void comprobarLibreria();

        static void comprobarGuantes();
};

#endif  //CONTROLGUANTES