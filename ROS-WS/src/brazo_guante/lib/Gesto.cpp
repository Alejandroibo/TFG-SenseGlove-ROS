#include "brazo_guante/Gesto.h"
#include "brazo_guante/LectorEscritorGestos.h"

#include <cmath>

Gesto::Gesto(){
    for (int i = 0; i < 5; i++){
        Dedo dedo;
        if (i == 0) dedo.articulaciones = vector<int>(5, 0);
        else dedo.articulaciones = vector<int>(4, 0);
        informacionGesto.push_back(dedo);
    }

    derecha = false;
}

Gesto::Gesto(string _nombre, bool _derecha){

    for (int i = 0; i < 5; i++){
        Dedo dedo;
        if (i == 0) dedo.articulaciones = vector<int>(5, 0);
        else dedo.articulaciones = vector<int>(4, 0);
        informacionGesto.push_back(dedo);
    }

    nombreGesto = _nombre;
    derecha = _derecha;
}

Gesto::Gesto(string _nombre, bool _derecha, string infoGesto){
    for (int i = 0; i < 5; i++){
        //cout<<"i: "<<i<<endl;
        Dedo dedo;
        size_t fin;
        string dato;

        int dataMax = i == 0 ? 5 : 4; 
        //cout<<"dataMax: "<<dataMax<<endl;

        for (int j = 0; j < dataMax; j++){
            bool terminar = false;
            //cout<<"j: "<<j<<endl;
            fin = infoGesto.find_first_of(" ");
            infoGesto = infoGesto.substr(fin+1);
            //cout<<"infoGesto: "<<infoGesto<<endl;

            if (i < 4 && j < dataMax-1){
                if (j == dataMax-1) fin = infoGesto.find_first_of("\n");
                else fin = infoGesto.find_first_of(",");
            } else {
                fin = infoGesto.size();
                terminar = true;
            }

            dato = infoGesto.substr(0,fin);
            //cout<<"dato: "<<dato<<endl;
            dedo.activo = true;
            dedo.articulaciones.push_back(stoi(dato));
            if (!terminar) infoGesto = infoGesto.substr(fin+1);
            //cout<<"infoGesto: "<<infoGesto<<endl;
        }

        informacionGesto.push_back(dedo);
        //cout<<"Pusheamos dedo"<<endl;
    }    

    nombreGesto = _nombre;
    derecha = _derecha;
}

bool Gesto::casiIgual(int objetivo, int aprox, int precision){
    return (abs(objetivo-aprox) <= precision);
}

int Gesto::distancia(int A, int B){
    return abs(A-B);
}

int Gesto::distanciaDedos(Dedo A, Dedo B){
    int distancia = 0;
    for (int i = 0; i < A.articulaciones.size(); i++){
        distancia += abs(A.articulaciones[i]-B.articulaciones[i]);
        //cout<<"Distancia: "<< A.articulaciones[i] << " - " << B.articulaciones[i] << " += " << distancia <<endl;  
    }

    return distancia;
}

void Gesto::cargarInformacionDedo(string ruta){

    pair<DIGITO, vector<int>> datosDedo = LectorEscritorGestos::cargarDatosDedo(ruta);

    informacionGesto[datosDedo.first].activo = true;
    informacionGesto[datosDedo.first].articulaciones = datosDedo.second;

    //std::cout<<" THIS "<<informacionGesto[datosDedo.first].second[0]<<std::endl;

}

bool Gesto::comprobarDigito(vector<int> infoDigito, DIGITO digito, int precision){
    vector<int> gestoDigito = informacionGesto[digito].articulaciones;

    bool esGestoDigito = true;

    for (int i = 0; i < infoDigito.size() && esGestoDigito; i++){
        esGestoDigito = casiIgual(gestoDigito[i], infoDigito[i], precision);
    }

    return esGestoDigito;
}

bool Gesto::comprobarGesto(vector<Dedo> informacion, int precision){

    bool esGesto = true;

    for (int i = 0; i < informacion.size() && esGesto; i++){
        if (informacionGesto[i].activo){
            esGesto = comprobarDigito(informacion[i].articulaciones, static_cast<DIGITO>(i), precision);
        }
    }

    return esGesto;

}

Dedo Gesto::getDedo(int indice){
    return informacionGesto[indice];
}

vector<Dedo> Gesto::getInfoGesto(){
    return informacionGesto;
}

void Gesto::setInfoGesto(vector<Dedo> info){
    informacionGesto = info;
}

string Gesto::getNombreGesto(){
    return nombreGesto;
}

string Gesto::to_string(){

    string resultado = "";
    resultado += nombreGesto + "\n";
    resultado += derecha ? "1\n" : "0\n";

    for (int i = 0; i < informacionGesto.size(); i++){
        if (informacionGesto[i].activo == true){
            resultado += std::to_string(i) + ": ";
            for (int j = 0; j < informacionGesto[i].articulaciones.size(); j++){
                resultado += std::to_string(informacionGesto[i].articulaciones[j]) + " ";
            }
            resultado += "\n";
        }
    }

    return resultado;
}
