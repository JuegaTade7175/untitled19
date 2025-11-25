//
// Created by tadeo on 23/11/2025.
//

#include "Controlador.h"
#include "Contexto.h"
#include "Mapa.h"
#include "Celda.h"
#include "Unidad.h"
#include "Evento.h"
#include "Edificio.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

void ControladorJugador::resolver_fase(Contexto& ctx) {
    ctx.agregar_log("Fase del jugador iniciada - " + to_string(ctx.obtener_puntos_accion()) + " puntos de acción disponibles");
}

void ControladorJugador::aplicar_rutina(const Evento& evento, Contexto& ctx) {
    if (evento.obtener_tipo() == "Clima") {
        cout << "\n[ALERTA] Evento climático afecta tus unidades" << endl;
    } else if (evento.obtener_tipo() == "Refuerzo") {
        cout << "\n[ALERTA] El sistema ha recibido refuerzos" << endl;
    }
}

void ControladorSistema::ejecutar_patrullaje(Contexto& ctx) {
    Mapa& mapa = ctx.obtener_mapa();
    vector<pair<Coordenada, Coordenada>> movimientos;

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_unidad()) {
                auto unidad = celda.obtener_unidad();
                if (unidad->obtener_propietario() == "J2") {
                    Coordenada nueva_pos = pos;

                    if (f < mapa.obtener_filas() - 1) {
                        nueva_pos.fila = f + 1;
                    }
                    else if (c < mapa.obtener_columnas() - 1) {
                        nueva_pos.columna = c + 1;
                    }

                    if (mapa.es_valida(nueva_pos) &&
                        !mapa.obtener_celda(nueva_pos).tiene_unidad()) {
                        movimientos.push_back({pos, nueva_pos});
                    }
                }
            }
        }
    }

    for (auto& mov : movimientos) {
        Celda& origen = mapa.obtener_celda(mov.first);
        Celda& destino = mapa.obtener_celda(mov.second);

        auto unidad = origen.obtener_unidad();
        destino.colocar_unidad(unidad);
        origen.eliminar_unidad();

        ctx.agregar_log("Sistema: " + unidad->obtener_codigo() +
                       " patrulló hacia (" + to_string(mov.second.fila) +
                       "," + to_string(mov.second.columna) + ")");
    }
}

void ControladorSistema::ejecutar_ofensiva(Contexto& ctx) {
    Mapa& mapa = ctx.obtener_mapa();

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_unidad() && celda.obtener_unidad()->obtener_propietario() == "J2") {
                vector<Coordenada> adyacentes = {
                    {f-1, c}, {f+1, c}, {f, c-1}, {f, c+1}
                };

                for (auto& adj : adyacentes) {
                    if (mapa.es_valida(adj)) {
                        Celda& celda_adj = mapa.obtener_celda(adj);
                        if (celda_adj.tiene_unidad() &&
                            celda_adj.obtener_unidad()->obtener_propietario() == "J1") {

                            auto atacante = celda.obtener_unidad();
                            auto objetivo = celda_adj.obtener_unidad();

                            int dano = atacante->calcular_dano_ataque();
                            objetivo->recibir_dano(dano);

                            ctx.agregar_log("Sistema: " + atacante->obtener_codigo() +
                                          " atacó a " + objetivo->obtener_codigo() +
                                          " causando " + to_string(dano) + " de daño");

                            if (!objetivo->esta_activa()) {
                                celda_adj.eliminar_unidad();
                                ctx.agregar_log("Sistema: Unidad enemiga eliminada");
                            }

                            return;
                        }
                    }
                }
            }
        }
    }
}

void ControladorSistema::ejecutar_defensa(Contexto& ctx) {
    Mapa& mapa = ctx.obtener_mapa();

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_edificio() &&
                celda.obtener_edificio()->obtener_propietario() == "J2") {
                ctx.agregar_log("Sistema: Defendiendo posición estratégica en (" +
                               to_string(f) + "," + to_string(c) + ")");
            }
        }
    }
}

void ControladorSistema::generar_guardianes_automaticos(Contexto& ctx) {
    if (ctx.obtener_turno() % 3 != 0) {
        return;
    }

    Mapa& mapa = ctx.obtener_mapa();
    int generados = 0;
    const int MAX_GENERACIONES = 1;

    for (int f = 0; f < mapa.obtener_filas() && generados < MAX_GENERACIONES; f++) {
        for (int c = 0; c < mapa.obtener_columnas() && generados < MAX_GENERACIONES; c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_edificio() &&
                celda.obtener_edificio()->obtener_tipo() == "Cu" &&
                celda.obtener_edificio()->obtener_propietario() == "J2" &&
                !celda.tiene_unidad()) {

                auto nuevo_guardian = make_shared<Soldado>("J2");
                nuevo_guardian->establecer_posicion(pos);
                celda.colocar_unidad(nuevo_guardian);
                generados++;

                ctx.agregar_log("Sistema: Guardián generado automáticamente en cuartel (" +
                               to_string(f) + "," + to_string(c) + ")");
            }
        }
    }

    if (generados > 0) {
        cout << "[SISTEMA] " << generados << " guardianes generados automáticamente" << endl;
    }
}

void ControladorSistema::resolver_fase(Contexto& ctx) {
    cout << "\n=== FASE DEL SISTEMA ===" << endl;
    ctx.agregar_log("Fase del sistema iniciada");

    generar_guardianes_automaticos(ctx);

    ejecutar_ofensiva(ctx);
    ejecutar_patrullaje(ctx);
    ejecutar_defensa(ctx);

    cout << "Fase del sistema completada." << endl;
}

void ControladorSistema::aplicar_rutina(const Evento& evento, Contexto& ctx) {
    if (evento.obtener_tipo() == "Refuerzo") {
        ctx.agregar_log("Sistema: Refuerzos recibidos, fortaleciendo posiciones");
    } else if (evento.obtener_tipo() == "Clima") {
        ctx.agregar_log("Sistema: Adaptando estrategia al clima");
    }
}

void ControladorNeutral::resolver_fase(Contexto& ctx) {
}

void ControladorNeutral::aplicar_rutina(const Evento& evento, Contexto& ctx) {
    ctx.agregar_log("Neutral: Estructura neutral afectada por evento");
}