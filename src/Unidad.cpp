//
// Created by tadeo on 23/11/2025.
//
#include "Unidad.h"
#include "Contexto.h"
#include "Mapa.h"
#include "Celda.h"

bool Soldado::mover(const Coordenada& destino, Contexto& ctx) {
    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);
    int costo = celda_destino.obtener_terreno()->costo_movimiento(*this);

    if (costo >= 99) {
        return false;
    }

    return true;
}

void Soldado::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Soldado::habilidad_especial(Contexto& ctx) {
    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        defensa += 5;
        ctx.agregar_log("Soldado activó defensa mejorada (+5 DEF)");
    } else {
        ctx.agregar_log("Sin energía para activar habilidad");
    }
}

bool Arquero::mover(const Coordenada& destino, Contexto& ctx) {
    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);
    int costo = celda_destino.obtener_terreno()->costo_movimiento(*this);

    if (costo >= 99) {
        return false;
    }

    return true;
}

void Arquero::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Arquero::habilidad_especial(Contexto& ctx) {
    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        ataque += 10; 
        ctx.agregar_log("Arquero disparó flecha potenciada (+10 ATK)");
    } else {
        ctx.agregar_log("Sin energía para habilidad especial");
    }
}

bool Caballero::mover(const Coordenada& destino, Contexto& ctx) {
    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);
    int costo = celda_destino.obtener_terreno()->costo_movimiento(*this);

    if (costo >= 3) {
        return false;
    }

    return true;
}

void Caballero::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Caballero::habilidad_especial(Contexto& ctx) {
    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        ataque += 15;
        ctx.agregar_log("Caballero ejecutó carga devastadora (+15 ATK)");
    } else {
        ctx.agregar_log("Sin energía para carga");
    }
}

bool Mago::mover(const Coordenada& destino, Contexto& ctx) {
    return true;
}

void Mago::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Mago::habilidad_especial(Contexto& ctx) {
    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 2))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 2));
        ctx.agregar_log("Mago lanzó hechizo de área (daña múltiples enemigos)");
    } else {
        ctx.agregar_log("Sin energía para hechizo");
    }
}

bool Ingeniero::mover(const Coordenada& destino, Contexto& ctx) {
    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);
    int costo = celda_destino.obtener_terreno()->costo_movimiento(*this);

    if (costo >= 99) {
        return false;
    }

    return true;
}

void Ingeniero::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Ingeniero::habilidad_especial(Contexto& ctx) {
    if (ctx.obtener_jugador().puede_pagar(Recursos(1, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(1, 0, 1));
        ctx.agregar_log("Ingeniero construyó estructura rápida (usa siguiente turno para colocarla)");
    } else {
        ctx.agregar_log("Sin recursos para construcción rápida");
    }
}