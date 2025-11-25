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
    if (habilidad_activa) {
        ctx.agregar_log("Defensa mejorada ya activa para este Soldado");
        return;
    }

    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        bonus_temporal_defensa = 5;
        habilidad_activa = true;
        ctx.agregar_log("Soldado activó defensa mejorada (+5 DEF temporal)");
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
    if (habilidad_activa) {
        ctx.agregar_log("Flecha potenciada ya activa para este Arquero");
        return;
    }

    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        bonus_temporal_ataque = 10;
        habilidad_activa = true;
        ctx.agregar_log("Arquero disparó flecha potenciada (+10 ATK temporal)");
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
    if (habilidad_activa) {
        ctx.agregar_log("Carga devastadora ya activa para este Caballero");
        return;
    }

    if (ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 1))) {
        ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 1));
        bonus_temporal_ataque = 15;
        habilidad_activa = true;
        ctx.agregar_log("Caballero ejecutó carga devastadora (+15 ATK temporal)");
    } else {
        ctx.agregar_log("Sin energía para carga");
    }
}

bool Mago::mover(const Coordenada& destino, Contexto& ctx) {
    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);
    int costo = celda_destino.obtener_terreno()->costo_movimiento(*this);

    if (costo >= 99) {
        return false;
    }

    return true;
}

void Mago::atacar(Unidad& objetivo, Contexto& ctx) {
    int dano = calcular_dano_ataque();
    objetivo.recibir_dano(dano);
}

void Mago::habilidad_especial(Contexto& ctx) {
    if (!ctx.obtener_jugador().puede_pagar(Recursos(0, 0, 2))) {
        ctx.agregar_log("Sin energía para hechizo");
        return;
    }

    ctx.obtener_jugador().consumir_recursos(Recursos(0, 0, 2));

    Mapa& mapa = ctx.obtener_mapa();
    int enemigos_danados = 0;
    int dano_area = 15;

    for (int df = -1; df <= 1; df++) {
        for (int dc = -1; dc <= 1; dc++) {
            Coordenada pos_area(posicion.fila + df, posicion.columna + dc);

            if (mapa.es_valida(pos_area)) {
                Celda& celda = mapa.obtener_celda(pos_area);

                if (celda.tiene_unidad()) {
                    auto unidad = celda.obtener_unidad();

                    if (unidad->obtener_propietario() != propietario) {
                        unidad->recibir_dano(dano_area);
                        enemigos_danados++;

                        ctx.agregar_log("Hechizo de área golpeó a " +
                                       unidad->obtener_codigo() +
                                       " causando " + std::to_string(dano_area) + " de daño");

                        if (!unidad->esta_activa()) {
                            celda.eliminar_unidad();
                            ctx.agregar_puntaje(10);
                            ctx.agregar_log("Unidad enemiga eliminada por hechizo (+10 puntos)");
                        }
                    }
                }
            }
        }
    }

    if (enemigos_danados > 0) {
        ctx.agregar_log("Mago lanzó hechizo de área (" +
                       std::to_string(enemigos_danados) + " enemigos afectados)");
    } else {
        ctx.agregar_log("Mago lanzó hechizo de área (sin enemigos en rango)");
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