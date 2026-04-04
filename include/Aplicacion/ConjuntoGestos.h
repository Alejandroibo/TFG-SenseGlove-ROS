#include "Gesto.h"

#include <string>
#include <cstdlib>
#include <vector>

#ifndef CONJUNTOGESTOS_H

#define CONJUNTOGESTOS_H

using namespace std;

class ConjuntoGestos{
    private:
        vector<Gesto> gestos;

    public:
        ConjuntoGestos();

        void addGesto(Gesto gesto);
        Gesto getGesto(int indice);

        //Metodos para determinar parecido

        Gesto getGestoProximo(string infoGesto);

};

#endif //CONJUNTOGESTOS_H