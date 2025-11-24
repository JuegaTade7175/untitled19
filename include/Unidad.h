//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_UNIDAD_H
#define UNTITLED19_UNIDAD_H
#include "Coordenada.h"
#include <string>
#include <iostream>

class Contexto;

class Unidad {
protected:
    std::string tipo;
    std::string propietario;
    int vida;
    int ataque;
    int defensa;
    bool activa;

public:
    Unidad(const std::string& t, const std::string& prop, int v, int atq, int def)
        : tipo(t), propietario(prop), vida(v), ataque(atq), defensa(def), activa(true) {}

    virtual ~Unidad() {}

    virtual bool mover(const Coordenada& destino, Contexto& ctx) = 0;
    virtual void atacar(Unidad& objetivo, Contexto& ctx) = 0;
    virtual void habilidad_especial(Contexto& ctx) = 0;

    std::string obtener_codigo() const { return propietario + tipo; }
    std::string obtener_propietario() const { return propietario; }
    std::string obtener_tipo() const { return tipo; }
    int obtener_vida() const { return vida; }
    bool esta_activa() const { return activa; }

    void recibir_dano(int dano) {
        vida -= dano;
        if (vida <= 0) {
            vida = 0;
            activa = false;
        }
    }

    void desactivar() { activa = false; }
    void activar() { activa = true; }

    int calcular_dano_ataque() const { return ataque; }
    int calcular_defensa() const { return defensa; }

    friend std::ostream& operator<<(std::ostream& os, const Unidad& u) {
        os << u.obtener_codigo() << "(V:" << u.vida << ")";
        return os;
    }
};

class Soldado : public Unidad {
public:
    Soldado(const std::string& prop)
        : Unidad("S", prop, 100, 20, 10) {}

    bool mover(const Coordenada& destino, Contexto& ctx) override;
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

class Arquero : public Unidad {
public:
    Arquero(const std::string& prop)
        : Unidad("A", prop, 80, 25, 5) {}

    bool mover(const Coordenada& destino, Contexto& ctx) override;
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

class Caballero : public Unidad {
public:
    Caballero(const std::string& prop)
        : Unidad("C", prop, 120, 30, 15) {}

    bool mover(const Coordenada& destino, Contexto& ctx) override;
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

class Mago : public Unidad {
public:
    Mago(const std::string& prop)
        : Unidad("M", prop, 70, 35, 5) {}

    bool mover(const Coordenada& destino, Contexto& ctx) override;
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};

class Ingeniero : public Unidad {
public:
    Ingeniero(const std::string& prop)
        : Unidad("I", prop, 60, 10, 8) {}

    bool mover(const Coordenada& destino, Contexto& ctx) override;
    void atacar(Unidad& objetivo, Contexto& ctx) override;
    void habilidad_especial(Contexto& ctx) override;
};
#endif //UNTITLED19_UNIDAD_H