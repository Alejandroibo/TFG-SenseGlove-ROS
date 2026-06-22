#include "brazo_guante/LectorEscritorGestos.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

using namespace std;

void LectorEscritorGestos::comprobarPathLocal(){
    filesystem::path pa = filesystem::current_path();
    cout << "Ruta actual: " << pa.string() <<std::endl;
}

void LectorEscritorGestos::guardarDatosDedo(string info, bool derecho, string dedo){

    std::string hand = derecho ? "derecha" : "izquierda";

    //Comprobamos camino y creamos ruta
    //////////////////////////////////////

    std::string rutaArchivoLocal = ("\\..\\data\\guantes\\"+ hand +"\\dedos\\"+dedo);

    std::filesystem::path pa = std::filesystem::current_path();
 
    std::cout << "Ruta actual: " << pa <<std::endl<<std::endl;

    std::filesystem::path rutaArchivo = pa.string() + rutaArchivoLocal;

    std::cout << "Ruta archivo: " << rutaArchivo <<std::endl<<std::endl;

    if (std::filesystem::exists(rutaArchivo)){
        std::cout << "El archivo o carpeta existe\n";


        //Obtenemos datos del gesto
        //////////////////////////////////////

        std::string datoProcesado = info;

        int dedoInt = stoi(dedo);

        //Cogemos la linea que nos interesa
        for (int i = 1; i < dedoInt; i++){
            size_t finlinea = datoProcesado.find_first_of('\n');
            datoProcesado = datoProcesado.substr(finlinea+1);
        }

        size_t finlinea = datoProcesado.find_first_of('\n');
        datoProcesado = datoProcesado.substr(0,finlinea);

        datoProcesado = dedo + "\n" + datoProcesado;

        //Generamos fichero
        //////////////////////////////////////

        std::string nombreArchivo;
        
        std::cout<<("Introduzca nombre del gesto:")  << std::endl<<std::endl;
        std::cin >> nombreArchivo;
        std::ofstream ArchivoGesto (rutaArchivo.string() + "\\" +nombreArchivo+".txt");

        std::cout << "La ruta del archivo: " << (rutaArchivo.string() + "\\" +nombreArchivo+".txt") + " ahora contiene:" <<std::endl<<std::endl;
        
        ArchivoGesto << datoProcesado;

        std::cout << datoProcesado <<std::endl<<std::endl;

        ArchivoGesto.close();

        std::cout << "Operacion completada con exito\n";

    }else
        //Reemplazar por creacion del directorio? O una afirmacion del usuario
        std::cout << "El archivo o carpeta no existe\n";

}

pair<DIGITO, vector<int>> LectorEscritorGestos::cargarDatosDedo(string ruta){

    pair<DIGITO, vector<int>> datosDedo;

    filesystem::path pa = filesystem::current_path();
    filesystem::path rutaArchivo = pa.string() + ruta;
    cout << "Ruta actual: " << rutaArchivo <<std::endl;

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