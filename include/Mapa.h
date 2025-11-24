//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_MAPA_H
#define UNTITLED19_MAPA_H
#include "Celda.h"
#include "Coordenada.h"
#include <vector>

class Mapa {
    int filas;
    int columnas;
    std::vector<std::vector<Celda>> celdas;

public:
    Mapa(int f, int c);

    int obtener_filas() const { return filas; }
    int obtener_columnas() const { return columnas; }

    bool es_valida(const Coordenada& coord) const {
        return coord.fila >= 0 && coord.fila < filas &&
               coord.columna >= 0 && coord.columna < columnas;
    }

    Celda& obtener_celda(const Coordenada& coord) {
        return celdas[coord.fila][coord.columna];
    }

    const Celda& obtener_celda(const Coordenada& coord) const {
        return celdas[coord.fila][coord.columna];
    }

    void mostrar() const;

    friend std::ostream& operator<<(std::ostream& os, const Mapa& mapa) {
        mapa.mostrar();
        return os;
    }
};
#endif //UNTITLED19_MAPA_H