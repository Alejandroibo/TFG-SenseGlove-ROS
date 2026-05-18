#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

#include <chrono>
#include <thread>

#include "Aplicacion/Gesto.h"
#include "Aplicacion/ConjuntoGestos.h"
#include "Aplicacion/LectorEscritorGestos.h"
#include "Aplicacion/ControlGuantes.h"

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
    
    Gesto g1("pulgarArriba", false);
    g1.cargarInformacionDedo("/../data/guantes/izquierda/dedos/0/pulgarArriba.txt");
/*
    std::cout<<"Cargamos:"<<std::endl;
    std::cin.get();

    std::cout<<g1.to_string()<<std::endl;

    std::cin.get();
*/
    //2: Activamos reconocimiento

    //ControlGuantes::comprobarLibreria();
   // ControlGuantes::comprobarGuantes();

    Gesto g2("pulgarAbajo", false), g3("pulgarExtendido", false), g4("pulgarMedio", false);
    g2.cargarInformacionDedo("/../data/guantes/izquierda/dedos/0/pulgarBajo.txt");
    g3.cargarInformacionDedo("/../data/guantes/izquierda/dedos/0/pulgarExtendido.txt");
    g4.cargarInformacionDedo("/../data/guantes/izquierda/dedos/0/pulgarMedio.txt");

    ConjuntoGestos conjunto1;
    conjunto1.addGesto(g1);
    conjunto1.addGesto(g2);
    conjunto1.addGesto(g3);
    conjunto1.addGesto(g4);

    Gesto g5("manoContraida", false), g6("manoMitad", false), g7("manoExtendida", false);
    g5.cargarInformacionDedo("/../data/guantes/izquierda/dedos/1/indiceContraido.txt");
    g5.cargarInformacionDedo("/../data/guantes/izquierda/dedos/2/anularContraido.txt");
    g5.cargarInformacionDedo("/../data/guantes/izquierda/dedos/3/corazonContraidoFull.txt");
    g6.cargarInformacionDedo("/../data/guantes/izquierda/dedos/1/indiceMitad.txt");
    g6.cargarInformacionDedo("/../data/guantes/izquierda/dedos/2/anularMitad.txt");
    g6.cargarInformacionDedo("/../data/guantes/izquierda/dedos/3/corazonMitad.txt");
    g7.cargarInformacionDedo("/../data/guantes/izquierda/dedos/1/indiceExtendido.txt");
    g7.cargarInformacionDedo("/../data/guantes/izquierda/dedos/2/anularExtendido.txt");
    g7.cargarInformacionDedo("/../data/guantes/izquierda/dedos/3/corazonExtendido.txt");

    ConjuntoGestos conjunto;
    conjunto.addGesto(g5);
    conjunto.addGesto(g6);
    conjunto.addGesto(g7);

    string respuesta = "";

    cout<<"Inicio de reconocimiento de gestos:"<<endl;
    std::cin.get();
    
    while (respuesta != "n" && respuesta != "N"){

        cout<<"Captamos infor del guante (Enter para continuar):"<<endl;
        //cin.get();

        string gestoInfo = ControlGuantes::captarDatosGuante(false);
        //string gestoInfo2 = "Thumb:	CMC-Abd: 0,	CMC-Tw: -20,	CMC-Flx: -29,	MCP-Flx: 6,	IP-Flx: 13\nIndex:	MCP-Abd: 0,	MCP-Flx: 9,	PIP-Flx: 12,	DIP-Flx: 11\nMiddle:	MCP-Abd: 0,	MCP-Flx: 9,	PIP-Flx: 13,	DIP-Flx: 11\nRing:	MCP-Abd: 0,	MCP-Flx: 0,	PIP-Flx: 0,	DIP-Flx: 0\nPinky:	MCP-Abd: 0,	MCP-Flx: 0,	PIP-Flx: 0,	DIP-Flx: 0";        

        Gesto comprobar("test", false, gestoInfo);

        cout<<comprobar.to_string()<<endl;

        cout<<"Buscamos parecido:"<<endl;
        
        Gesto parecido = conjunto.getGestoProximo(comprobar, 10000);

        cout<<parecido.to_string()<<endl;

        //cout<<"Desea continuar?"<<endl;
        //cin>>respuesta;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));//s to ms
    }

}