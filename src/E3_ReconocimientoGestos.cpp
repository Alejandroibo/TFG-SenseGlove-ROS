#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

#include "Aplicacion/Gesto.h"
#include "Aplicacion/ConjuntoGestos.h"
#include "Aplicacion/LectorEscritorGestos.h"

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

int32_t main(){

    //Esta aplicacion es de test para probar las clases realmente
    
    //1: Generamos gestos para testear
    
    Gesto g1;
    g1.cargarInformacionDedo("/../data/guantes/izquierda/dedos/1/pulgarArriba.txt");

    std::cout<<"Cargamos:"<<std::endl;
    std::cin.get();

    std::cout<<g1.to_string()<<std::endl;

    std::cin.get();

    //2: Activamos reconocimiento

}