#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

#include "turtlesim_guante/Gesto.h"

#ifndef LECTORESCRITORGESTOS_H

#define LECTORESCRITORGESTOS_H

using namespace std;

class LectorEscritorGestos{
    private:


    public:

        //Test
        static void comprobarPathLocal();

        //Guardar
        static void guardarDatosDedo(bool derecha, string dedo);
        static void guardarGestoCompleto(bool derecha, Gesto gesto);

        //Cargar
        static pair<DIGITO, vector<int>> cargarDatosDedo(string path);
        static Gesto cargarGestoCompleto(string path);

};

#endif //LECTORESCRITORGESTOS_H