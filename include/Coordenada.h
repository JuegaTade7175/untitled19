//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_COORDENADA_H
#define UNTITLED19_COORDENADA_H
#include <iostream>

class Coordenada {
public:
    int fila;
    int columna;

    Coordenada(int f = 0, int c = 0) : fila(f), columna(c) {}

    bool operator==(const Coordenada& otra) const {
        return fila == otra.fila && columna == otra.columna;
    }

    bool operator!=(const Coordenada& otra) const {
        return !(*this == otra);
    }

    friend std::ostream& operator<<(std::ostream& os, const Coordenada& coord) {
        os << "(" << coord.fila << "," << coord.columna << ")";
        return os;
    }
};
#endif //UNTITLED19_COORDENADA_H