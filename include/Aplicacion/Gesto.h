#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream> 

#include "Aplicacion/DigitoEnum.h"

#ifndef GESTO_H

#define GESTO_H

using namespace std;

class Gesto{
    private:
        string nombreGesto;
        vector<pair<bool, vector<int>>> informacionGesto;
        int prioridad;

        string obtenerPathLocal();
        bool casiIgual(int A, int B, int precision);

    public:
        Gesto();
        Gesto(string infoGesto);

        //Generacion de gesto
        void cargarInformacionDedo(vector<int> info);
        void cargarInformacionDedo(string ruta);

        //Comprobacion de gesto
        bool comprobarGesto(vector<vector<int>> informacion, int precision);
        bool comprobarDigito(vector<int> infoDigito, DIGITO digito, int precision);

        //
        void setInfoGesto(vector<pair<bool, vector<int>>> informacion);
        vector<pair<bool, vector<int>>> getInfoGesto();
        string to_string();

};

#endif //GESTO_H