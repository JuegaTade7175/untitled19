//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_CONTROLADOR_H
#define UNTITLED19_CONTROLADOR_H
#include <string>

class Contexto;
class Evento;

class Controlador {
protected:
    std::string tipo;

public:
    Controlador(const std::string& t) : tipo(t) {}
    virtual ~Controlador() {}

    virtual void resolver_fase(Contexto& ctx) = 0;

    virtual void aplicar_rutina(const Evento& evento, Contexto& ctx) = 0;

    std::string obtener_tipo() const { return tipo; }
};

class ControladorJugador : public Controlador {
public:
    ControladorJugador() : Controlador("Jugador") {}

    void resolver_fase(Contexto& ctx) override;
    void aplicar_rutina(const Evento& evento, Contexto& ctx) override;
};

class ControladorSistema : public Controlador {
    void ejecutar_patrullaje(Contexto& ctx);
    void ejecutar_ofensiva(Contexto& ctx);
    void ejecutar_defensa(Contexto& ctx);

public:
    ControladorSistema() : Controlador("Sistema") {}

    void resolver_fase(Contexto& ctx) override;
    void aplicar_rutina(const Evento& evento, Contexto& ctx) override;
};

class ControladorNeutral : public Controlador {
public:
    ControladorNeutral() : Controlador("Neutral") {}

    void resolver_fase(Contexto& ctx) override;
    void aplicar_rutina(const Evento& evento, Contexto& ctx) override;
};
#endif //UNTITLED19_CONTROLADOR_H