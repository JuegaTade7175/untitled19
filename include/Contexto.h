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
#include <fstream>

class Contexto {
    Mapa mapa;
    Faccion jugador;
    Faccion sistema;

    int turno_actual;
    int puntos_accion;
    int puntos_accion_maximos;

    std::vector<std::shared_ptr<Evento>> eventos;
    std::vector<std::string> bitacora;

    int objetivo_dominio;
    int turno_limite;
    int puntaje_jugador;

    bool mision_activa;
    bool mision_cumplida;
    std::string descripcion_mision;
    Coordenada objetivo_mision;

    bool mejora_ataque_desbloqueada;
    bool mejora_defensa_desbloqueada;

    std::string clima_actual;

public:
    Contexto(int filas = 12, int columnas = 12);

    Mapa& obtener_mapa() { return mapa; }
    const Mapa& obtener_mapa() const { return mapa; }

    Faccion& obtener_jugador() { return jugador; }
    const Faccion& obtener_jugador() const { return jugador; }

    Faccion& obtener_sistema() { return sistema; }
    const Faccion& obtener_sistema() const { return sistema; }

    int obtener_turno() const { return turno_actual; }
    int obtener_puntos_accion() const { return puntos_accion; }
    int obtener_puntaje() const { return puntaje_jugador; }
    std::string obtener_clima() const { return clima_actual; }

    bool es_mision_activa() const { return mision_activa; }
    bool es_mision_cumplida() const { return mision_cumplida; }
    const std::string& obtener_descripcion_mision() const { return descripcion_mision; }
    Coordenada obtener_objetivo_mision() const { return objetivo_mision; }

    void marcar_mision_cumplida();
    void verificar_mision();
    void agregar_puntaje(int p) { puntaje_jugador += p; }

    void incrementar_turno();
    void reiniciar_puntos_accion() { puntos_accion = puntos_accion_maximos; }
    bool consumir_punto_accion(int cantidad = 1);

    void agregar_log(const std::string& mensaje);
    const std::vector<std::string>& obtener_logs() const { return bitacora; }
    void mostrar_bitacora() const;
    void limpiar_bitacora();
    void guardar_bitacora_archivo(const std::string& archivo);

    void agregar_evento(std::shared_ptr<Evento> evento);
    void procesar_eventos();
    void establecer_clima(const std::string& c) { clima_actual = c; }

    void verificar_captura_neutrales();

    bool verificar_victoria() const;
    bool verificar_derrota() const;

    int calcular_dominio_jugador() const;
    int calcular_dominio_sistema() const;
    int calcular_neutrales() const;

    void mostrar_panel_superior() const;
    void inicializar_escenario();

    bool guardar_partida(const std::string& archivo);
    bool cargar_partida(const std::string& archivo);
    void generar_reporte_final(const std::string& archivo);

    void desbloquear_mejora_ataque() { mejora_ataque_desbloqueada = true; }
    void desbloquear_mejora_defensa() { mejora_defensa_desbloqueada = true; }

    bool tiene_mejora_ataque() const { return mejora_ataque_desbloqueada; }
    bool tiene_mejora_defensa() const { return mejora_defensa_desbloqueada; }
};
#endif //UNTITLED19_CONTEXTO_H