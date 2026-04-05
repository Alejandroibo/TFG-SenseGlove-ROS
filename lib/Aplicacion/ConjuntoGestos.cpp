#include "ConjuntoGestos.h";
#include "Gesto.h";
#include <cmath>;

ConjuntoGestos::ConjuntoGestos(){

}

void ConjuntoGestos::addGesto(Gesto gesto){
    gestos.push_back(gesto);
}

Gesto ConjuntoGestos::getGesto(int indice){
    return gestos[indice];
}

Gesto ConjuntoGestos::getGestoProximo(Gesto gestoActual, int precision){
    return Gesto();
}