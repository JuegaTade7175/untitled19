//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_CONTEXTO_H
#define UNTITLED19_CONTEXTO_H
#include "Mapa.h"
#include "Faccion.h"
#include "Evento.h"
#include <vector>
#include <string>
#include <memory>

class Contexto {
    Mapa mapa;
    Faccion jugador;
    Faccion sistema;
    int turno_actual;
    int puntos_accion;
    std::vector<std::shared_ptr<Evento>> eventos;
    std::vector<std::string> bitacora;

    int objetivo_dominio;
    int turno_limite;
    int puntaje_jugador;

    bool mision_activa = true;
    bool mision_cumplida = false;
    std::string descripcion_mision = "Capturar el cuartel neutral en (3,2) antes del turno 8.";
    bool mejora_ataque_desbloqueada = false;
public:
    void desbloquear_mejora_ataque() { mejora_ataque_desbloqueada = true; }
    bool tiene_mejora_ataque() const { return mejora_ataque_desbloqueada; }
    Contexto();

    Mapa& obtener_mapa() { return mapa; }
    Faccion& obtener_jugador() { return jugador; }
    Faccion& obtener_sistema() { return sistema; }

    int obtener_turno() const { return turno_actual; }
    int obtener_puntos_accion() const { return puntos_accion; }
    int obtener_puntaje() const { return puntaje_jugador; }

    bool es_mision_activa() const { return mision_activa; }
    bool es_mision_cumplida() const { return mision_cumplida; }
    const std::string& obtener_descripcion_mision() const { return descripcion_mision; }

    void marcar_mision_cumplida() { mision_cumplida = true; }
    void agregar_puntaje(int puntos) { puntaje_jugador += puntos; }

    void incrementar_turno() { turno_actual++; }
    void reiniciar_puntos_accion() { puntos_accion = 2; }
    bool consumir_punto_accion() {
        if (puntos_accion > 0) {
            puntos_accion--;
            return true;
        }
        return false;
    }

    void agregar_log(const std::string& mensaje) {
        bitacora.push_back(mensaje);
    }

    const std::vector<std::string>& obtener_logs() const {
        return bitacora;
    }

    void mostrar_bitacora() const {
        std::cout << "\n=== BITÁCORA DEL TURNO ===" << std::endl;
        for (const auto& msg : bitacora) {
            std::cout << "  - " << msg << std::endl;
        }
    }

    void limpiar_bitacora() {
        bitacora.clear();
    }

    void agregar_evento(std::shared_ptr<Evento> evento) {
        eventos.push_back(evento);
    }

    void procesar_eventos();

    void verificar_captura_neutrales() {
        for (int f = 0; f < mapa.obtener_filas(); f++) {
            for (int c = 0; c < mapa.obtener_columnas(); c++) {
                Coordenada pos(f, c);
                Celda& celda = mapa.obtener_celda(pos);

                if (celda.tiene_edificio() && celda.tiene_unidad()) {
                    auto edificio = celda.obtener_edificio();
                    auto unidad = celda.obtener_unidad();

                    if (unidad->obtener_propietario() == "J1" &&
                        edificio->obtener_propietario() != "J1") {

                        std::string antiguo = edificio->obtener_propietario();
                        edificio->cambiar_propietario("J1");

                        jugador += pos;

                        agregar_log("¡Edificio capturado en (" + std::to_string(f) +
                                   "," + std::to_string(c) + ")! " + antiguo + " → J1");
                        agregar_puntaje(15);
                        }
                }
            }
        }
    }

    void verificar_mision() {
        if (!mision_activa || mision_cumplida) return;

        Celda& celda_objetivo = mapa.obtener_celda(Coordenada(3, 2));

        if (celda_objetivo.tiene_edificio() &&
            celda_objetivo.obtener_edificio()->obtener_propietario() == "J1") {
            mision_cumplida = true;
            agregar_puntaje(50);
            agregar_log("¡MISIÓN COMPLETADA! Cuartel capturado. +50 puntos");
            std::cout << "\n*** ¡MISIÓN COMPLETADA! +50 puntos ***\n" << std::endl;
        }

        if (turno_actual >= 8 && !mision_cumplida) {
            agregar_log("MISIÓN FALLIDA - Se acabó el tiempo");
            std::cout << "\n*** MISIÓN FALLIDA - Tiempo agotado ***\n" << std::endl;
        }
    }

    bool verificar_victoria() const {
        int total_celdas = mapa.obtener_filas() * mapa.obtener_columnas();
        int territorios_jugador = jugador.contar_territorios();
        int porcentaje = (territorios_jugador * 100) / total_celdas;

        return porcentaje >= objetivo_dominio || mision_cumplida;
    }

    bool verificar_derrota() const {
        if (turno_actual >= turno_limite) return true;
        if (jugador.obtener_recursos().comida < -5) return true;
        return false;
    }

    int calcular_dominio_jugador() const {
        int total = mapa.obtener_filas() * mapa.obtener_columnas();
        return (jugador.contar_territorios() * 100) / total;
    }

    int calcular_dominio_sistema() const {
        int total = mapa.obtener_filas() * mapa.obtener_columnas();
        return (sistema.contar_territorios() * 100) / total;
    }

    void mostrar_panel_superior() const;
    void inicializar_escenario();
};
#endif //UNTITLED19_CONTEXTO_H