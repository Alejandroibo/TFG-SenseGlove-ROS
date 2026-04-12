#include "Aplicacion/Gesto.h"
#include "Aplicacion/LectorEscritorGestos.h"

#include <cmath>

Gesto::Gesto(){

    for (int i = 0; i < 5; i++){
        pair<bool, vector<int>> par;
        par.first = false;
        informacionGesto.push_back(par);
    }

}

bool Gesto::casiIgual(int objetivo, int aprox, int precision){
    return (abs(objetivo-aprox) <= precision);
}

void Gesto::cargarInformacionDedo(string ruta){

    pair<DIGITO, vector<int>> datosDedo = LectorEscritorGestos::cargarDatosDedo(ruta);

    informacionGesto[datosDedo.first].first = true;
    informacionGesto[datosDedo.first].second = datosDedo.second;

    //std::cout<<" THIS "<<informacionGesto[datosDedo.first].second[0]<<std::endl;

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

    for (int i = 0; i < informacion.size() && esGesto; i++){
        if (informacionGesto[i].first){
            esGesto = comprobarDigito(informacion[i], static_cast<DIGITO>(i), precision);
        }
    }

    return esGesto;

}

vector<pair<bool, vector<int>>> Gesto::getInfoGesto(){
    return informacionGesto;
}

void Gesto::setInfoGesto(vector<pair<bool, vector<int>>> info){
    informacionGesto = info;
}


string Gesto::to_string(){

    string resultado = "";

    for (int i = 0; i < informacionGesto.size(); i++){
        if (informacionGesto[i].first == true){
            resultado += std::to_string(i) + ": ";
            for (int j = 0; j < informacionGesto[i].second.size(); j++){
                resultado += std::to_string(informacionGesto[i].second[j]) + " ";
            }
            resultado += "\n";
        }
    }

    return resultado;
}
