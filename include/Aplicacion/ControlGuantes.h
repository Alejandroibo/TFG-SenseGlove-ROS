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