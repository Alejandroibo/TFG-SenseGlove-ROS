#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include "senseglove_nova/Gesto.h"

#ifndef CONJUNTOGESTOS_H

#define CONJUNTOGESTOS_H

using namespace std;

class ConjuntoGestos{
    private:
        vector<Gesto> gestos;
        //map<string, Gesto> gestos;

    public:
        ConjuntoGestos();

        void addGesto(Gesto gesto);
        void addGesto(string nombre, bool derecha, string infoGesto);
        Gesto getGesto(int indice);

        //Metodos para determinar parecido

        Gesto getGestoProximo(Gesto gestoActual, int precision);

};

#endif //CONJUNTOGESTOS_H