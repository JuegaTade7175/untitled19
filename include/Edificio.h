//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_EDIFICIO_H
#define UNTITLED19_EDIFICIO_H
#include "Recursos.h"
#include <string>

class Faccion;
class Evento;

class Edificio {
protected:
    std::string tipo;
    std::string propietario;

public:
    Edificio(const std::string& t, const std::string& prop)
        : tipo(t), propietario(prop) {}

    virtual ~Edificio() {}

    virtual void efecto_turno(Faccion& faccion) = 0;
    virtual void reaccion_evento(const Evento& evento) = 0;

    std::string obtener_codigo() const { return propietario + tipo; }
    std::string obtener_propietario() const { return propietario; }
    std::string obtener_tipo() const { return tipo; }

    void cambiar_propietario(const std::string& nuevo_prop) {
        propietario = nuevo_prop;
    }
};

class Granja : public Edificio {
public:
    Granja(const std::string& prop) : Edificio("Gr", prop) {}

    void efecto_turno(Faccion& faccion) override;
    void reaccion_evento(const Evento& evento) override;
};

class Cuartel : public Edificio {
public:
    Cuartel(const std::string& prop) : Edificio("Cu", prop) {}

    void efecto_turno(Faccion& faccion) override;
    void reaccion_evento(const Evento& evento) override;
};

class Torre : public Edificio {
public:
    Torre(const std::string& prop) : Edificio("To", prop) {}

    void efecto_turno(Faccion& faccion) override;
    void reaccion_evento(const Evento& evento) override;
};

class Forja : public Edificio {
public:
    Forja(const std::string& prop) : Edificio("Fo", prop) {}

    void efecto_turno(Faccion& faccion) override;
    void reaccion_evento(const Evento& evento) override;
};
#endif //UNTITLED19_EDIFICIO_H