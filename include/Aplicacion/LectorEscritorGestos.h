#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Gesto.h"

#ifndef LECTORESCRITORGESTOS_H

#define LECTORESCRITORGESTOS_H

using namespace std;

class LectorEscritorGestos{
    private:


    public:
        void guardarGestoBasico(bool derecha, string dedo);
        void guardarGestoCompleto(bool derecha);
        Gesto cargarGesto(string path);

};

#endif //LECTORESCRITORGESTOS_H