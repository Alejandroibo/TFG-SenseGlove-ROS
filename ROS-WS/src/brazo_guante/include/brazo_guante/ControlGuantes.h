#include <brazo_guante/SenseGlove/Connect/SGConnect.hpp>
#include <brazo_guante/SenseGlove/Core/Debugger.hpp>
#include <brazo_guante/SenseGlove/Core/SenseCom.hpp>
#include <brazo_guante/SenseGlove/Core/Library.hpp>

#include <brazo_guante/SenseGlove/Core/HandLayer.hpp>

#include <brazo_guante/SenseGlove/Core/HandPose.hpp>
#include <brazo_guante/SenseGlove/Core/Quat.hpp>
#include <brazo_guante/SenseGlove/Core/Vect3D.hpp>

#include <brazo_guante/SenseGlove/Core/HapticGlove.hpp>
#include <brazo_guante/SenseGlove/Core/Tracking.hpp>
#include <brazo_guante/SenseGlove/Core/StringUtils.hpp>

#include "brazo_guante/DigitoEnum.h"

#include <string>
#include <vector>

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

        static bool comprobarSenseCom();

        static void comprobarGuantes();

        static void vibracionGuante(bool derecha, float amplitud, float duracion, float frecuencia, DIGITO zonaVibracion);

        static void feedbackFuerza(bool derecha, std::vector<float> dedosFF);
};

#endif  //CONTROLGUANTES