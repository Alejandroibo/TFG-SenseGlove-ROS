#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream> 

#ifndef GESTO_H

#define GESTO_H

using namespace std;

enum DIGITO{
    PULGAR,
    INDICE,
    ANULAR,
    CORAZON
};

class Gesto{
    private:
        vector<pair<bool, vector<int>>> informacionGesto;
        int prioridad;

        string obtenerPathLocal();
        bool casiIgual(int A, int B, int precision);

    public:
        Gesto();
        Gesto(string path);

        //Generacion de gesto
        void cargarInformacionDigito(string path, DIGITO digito);

        //Comprobacion de gesto
        bool comprobarGesto(vector<vector<int>> informacion, int precision);
        bool comprobarDigito(vector<int> infoDigito, DIGITO digito, int precision);

        //
        void setInfoGesto(vector<pair<bool, vector<int>>> informacion);
        vector<pair<bool, vector<int>>> getInfoGesto();
        string to_string();

};

#endif //GESTO_H