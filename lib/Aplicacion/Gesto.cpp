#include "Gesto.h";
#include <cmath>;

Gesto::Gesto(){

}

bool Gesto::casiIgual(int objetivo, int aprox, int precision){
    return (abs(objetivo-aprox) <= precision);
}

void Gesto::cargarInformacionDigito(string path, DIGITO digito){

    string datos;

    ifstream MyReadFile(path);

    //Hay que ver si el archivo de datos ya estara pre-procesado o no

}

bool Gesto::comprobarDigito(vector<int> infoDigito, DIGITO digito, int precision){
    vector<int> gestoDigito = informacionGesto[digito].second;

    bool esGestoDigito = true;

    for (int i = 0; i < infoDigito.size() && esGestoDigito; i++){
        esGestoDigito = casiIgual(gestoDigito[i], infoDigito[i], precision);
    }

    return esGestoDigito;
}

bool Gesto::comprobarGesto(vector<vector<int>> informacion, int precision){

    bool esGesto = true;

    for (int i = 0; i < informacion.size(); i++){
        if (informacionGesto[i].first){
            esGesto = comprobarDigito(informacion[i], static_cast<DIGITO>(i), precision);
        }
    }

}

vector<pair<bool, vector<int>>> Gesto::getInfoGesto(){
    return informacionGesto;
}

void Gesto::setInfoGesto(vector<pair<bool, vector<int>>> info){
    informacionGesto = info;
}