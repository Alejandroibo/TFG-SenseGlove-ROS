#include "brazo_guante/LectorEscritorGestos.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

using namespace std;

void LectorEscritorGestos::comprobarPathLocal(){
    filesystem::path pa = filesystem::current_path();
    cout << "The current path: " << pa.string() <<std::endl;
}

void LectorEscritorGestos::guardarDatosDedo(bool derecha, string dedo){

}

pair<DIGITO, vector<int>> LectorEscritorGestos::cargarDatosDedo(string ruta){

    pair<DIGITO, vector<int>> datosDedo;

    filesystem::path pa = filesystem::current_path();
    filesystem::path rutaArchivo = pa.string() + ruta;
    cout << "The current path: " << rutaArchivo <<std::endl;

    if (!filesystem::exists(rutaArchivo)){

        cout << "La ruta especificada no es correcta"<<std::endl;
        return datosDedo;
    }

    //Abrimos archivo
    //////////////////////////////

    ifstream ArchivoGesto(rutaArchivo);

    //Procesamos informacion:
    //////////////////////////////

    char datos[100];
    char dato;
    string numero;
    string resultado ="";

    ArchivoGesto.get(dato);

    datosDedo.first = static_cast<DIGITO>(int(dato)-48);

    int i = 0;

    while (!ArchivoGesto.eof()){
        //cout<<"/////"<<endl;

        //Saltamos hasta el espacio
        ArchivoGesto.get(datos, 100, ' ');

        //cout<<datos<<endl;

        //Saltamos espacio
        ArchivoGesto.get(dato);
        //cout<<dato<<endl;

        //Obtenemos numero
        ArchivoGesto.get(datos, 10, ',');
        //cout<<datos<<endl;

        //Guardamos numero
        datosDedo.second.push_back(stoi(datos));
        //std::cout<<datosDedo.second[i]<< " ";
        i++;

    }

    std::cout<<std::endl;

    ArchivoGesto.close();

    return datosDedo;
}