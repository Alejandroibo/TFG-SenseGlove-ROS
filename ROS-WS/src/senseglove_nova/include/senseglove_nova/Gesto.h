#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream> 

#include "senseglove_nova/DigitoEnum.h"

#ifndef GESTO_H

#define GESTO_H

using namespace std;

struct Dedo{
    bool activo = false;
    vector<int> articulaciones;
};

class Gesto{
    private:
        string nombreGesto;
        bool derecha; 
        vector<Dedo> informacionGesto;
        
        int prioridad;

        string obtenerPathLocal();
        static bool casiIgual(int A, int B, int precision);
        static int distancia(int A, int B);

    public:
        Gesto();
        Gesto(string nombre, bool derecha);
        Gesto(string nombre, bool derecha, string infoGesto);

        //Generacion de gesto
        void cargarInformacionDedo(vector<int> info);
        void cargarInformacionDedo(string ruta);

        void cargarInformacionGesto(vector<Dedo> info);
        void cargarInformacionGesto(string ruta);

        //Comprobacion de gesto
        static int distanciaDedos (Dedo A, Dedo B);
        bool comprobarGesto(vector<Dedo> informacion, int precision);
        bool comprobarDigito(vector<int> infoDigito, DIGITO digito, int precision);

        //
        Dedo getDedo(int indice);
        void setInfoGesto(vector<Dedo> informacion);
        vector<Dedo> getInfoGesto();
        void setDerecha(bool derecha);
        string getNombreGesto();
        string to_string();

};

#endif //GESTO_H