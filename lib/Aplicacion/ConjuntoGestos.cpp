#include "Aplicacion/ConjuntoGestos.h"
#include "Aplicacion/Gesto.h"
#include <cmath>

ConjuntoGestos::ConjuntoGestos(){

}

void ConjuntoGestos::addGesto(Gesto gesto){
    gestos.push_back(gesto);
}

Gesto ConjuntoGestos::getGesto(int indice){
    return gestos[indice];
}

Gesto ConjuntoGestos::getGestoProximo(Gesto gestoActual, int precision){
    int mejorDistancia = INT_MAX;
    Gesto mejorGesto;

    for (int i = 0; i < gestos.size(); i++){
        //cout <<"i: " << i<<endl;
        Gesto gestoComprobar = gestos[i];
        int distanciaTotal = 0;
        int dedosValidos = 0;

        for (int j = 0; j < gestoComprobar.getInfoGesto().size(); j++){
            //cout <<"j: " << j<<endl;
            Dedo dedoComprobar = gestoComprobar.getDedo(j);
            Dedo dedoActual = gestoActual.getDedo(j);
            if (dedoComprobar.activo){
                distanciaTotal += Gesto::distanciaDedos(dedoComprobar, dedoActual);
                //cout <<"distanciaTotal: " << distanciaTotal<<endl;
                dedosValidos++;
            }
        }

        if (dedosValidos > 0){
            int distanciaMedia = distanciaTotal / dedosValidos;
            //cout <<"distanciaMedia: " << distanciaMedia<<endl;

            if (distanciaMedia < mejorDistancia){
                mejorDistancia = distanciaMedia;
                mejorGesto = gestoComprobar;
            }
            //cout <<"mejorDistancia: " << mejorDistancia<<endl;
        }

        if (mejorDistancia > precision){
            Gesto null;
            mejorGesto = null;
        }

    }

    return mejorGesto;

}