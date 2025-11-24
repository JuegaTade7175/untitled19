//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_FACCION_H
#define UNTITLED19_FACCION_H
#ifndef FACCION_H
#define FACCION_H
#include "Recursos.h"
#include "Coordenada.h"
#include <string>
#include <vector>

class Faccion {
protected:
    std::string id;
    Recursos recursos;
    std::vector<Coordenada> territorios;
    int moral;

public:
    Faccion(const std::string& identificador, const Recursos& rec_iniciales)
        : id(identificador), recursos(rec_iniciales), moral(100) {}

    virtual ~Faccion() {}

    std::string obtener_id() const { return id; }
    Recursos& obtener_recursos() { return recursos; }
    const Recursos& obtener_recursos() const { return recursos; }
    void establecer_recursos(const Recursos& r) {
        recursos = r;
    }
    int obtener_moral() const { return moral; }
    double obtener_factor_moral() const {
        return 1.0 + (moral - 50) / 100.0;
    }

    void agregar_recursos(const Recursos& rec) {
        recursos += rec;
    }

    void consumir_recursos(const Recursos& costo) {
        recursos -= costo;
    }

    bool puede_pagar(const Recursos& costo) const {
        return recursos.tiene_suficiente(costo);
    }

    void modificar_moral(int delta) {
        moral += delta;
        if (moral < 0) moral = 0;
        if (moral > 100) moral = 100;
    }

    void establecer_moral(int valor) {
        moral = valor;
        if (moral < 0) moral = 0;
        if (moral > 100) moral = 100;
    }

    void agregar_territorio(const Coordenada& coord) {
        territorios.push_back(coord);
    }

    int contar_territorios() const {
        return territorios.size();
    }

    Faccion& operator+=(const Coordenada& coord) {
        agregar_territorio(coord);
        return *this;
    }

    bool operator>(const Faccion& otra) const {
        return contar_territorios() > otra.contar_territorios();
    }
};
#endif
#endif //UNTITLED19_FACCION_H