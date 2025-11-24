//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_CELDA_H
#define UNTITLED19_CELDA_H
#include "Terreno.h"
#include "Unidad.h"
#include "Edificio.h"
#include <memory>

class Celda {
    std::shared_ptr<Terreno> terreno;
    std::shared_ptr<Unidad> unidad;
    std::shared_ptr<Edificio> edificio;

public:
    Celda(std::shared_ptr<Terreno> terr)
        : terreno(terr), unidad(nullptr), edificio(nullptr) {}

    std::shared_ptr<Terreno> obtener_terreno() const { return terreno; }

    void colocar_unidad(std::shared_ptr<Unidad> u) { unidad = u; }
    std::shared_ptr<Unidad> obtener_unidad() const { return unidad; }
    void eliminar_unidad() { unidad = nullptr; }
    bool tiene_unidad() const { return unidad != nullptr; }

    void colocar_edificio(std::shared_ptr<Edificio> e) { edificio = e; }
    std::shared_ptr<Edificio> obtener_edificio() const { return edificio; }
    void eliminar_edificio() { edificio = nullptr; }
    bool tiene_edificio() const { return edificio != nullptr; }

    bool esta_libre() const { return !tiene_unidad() && !tiene_edificio(); }

    std::string obtener_representacion() const {
        std::string repr = "";

        if (tiene_unidad()) {
            repr = unidad->obtener_codigo();
        } else if (tiene_edificio()) {
            repr = edificio->obtener_codigo();
        } else {
            repr = "..";
        }

        repr += "/" + terreno->obtener_codigo();
        return repr;
    }
};
#endif //UNTITLED19_CELDA_H