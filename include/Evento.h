//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_EVENTO_H
#define UNTITLED19_EVENTO_H
#include <string>

class Contexto;

class Evento {
protected:
    std::string tipo;
    int turno_activacion;

public:
    Evento(const std::string& t, int turno)
        : tipo(t), turno_activacion(turno) {}

    virtual ~Evento() {}

    virtual void ejecutar(Contexto& ctx) = 0;

    std::string obtener_tipo() const { return tipo; }
    int obtener_turno() const { return turno_activacion; }

    bool debe_activarse(int turno_actual) const {
        return turno_actual == turno_activacion;
    }
};

class EventoRefuerzo : public Evento {
    std::string faccion;
    std::string tipo_unidad;
    int cantidad;

public:
    EventoRefuerzo(int turno, const std::string& fac, const std::string& tipo, int cant)
        : Evento("Refuerzo", turno), faccion(fac), tipo_unidad(tipo), cantidad(cant) {}

    void ejecutar(Contexto& ctx) override;
};

class EventoClima : public Evento {
    std::string efecto;

public:
    EventoClima(int turno, const std::string& ef)
        : Evento("Clima", turno), efecto(ef) {}

    void ejecutar(Contexto& ctx) override;
};
#endif //UNTITLED19_EVENTO_H