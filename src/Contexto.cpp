//
// Created by tadeo on 23/11/2025.
//
#include "Contexto.h"
#include "Unidad.h"
#include "Edificio.h"
#include <iostream>
#include <iomanip>

using namespace std;

Contexto::Contexto()
    : mapa(6, 6),
      jugador("J1", Recursos(18, 12, 7)),
      sistema("J2", Recursos(15, 10, 5)),
      turno_actual(1),
      puntos_accion(2),
      objetivo_dominio(60),
      turno_limite(20),
      puntaje_jugador(0)
{
}

void Contexto::inicializar_escenario() {
    mapa.obtener_celda(Coordenada(0, 0)).colocar_edificio(make_shared<Cuartel>("J1"));
    jugador += Coordenada(0, 0);

    mapa.obtener_celda(Coordenada(1, 1)).colocar_unidad(make_shared<Soldado>("J1"));

    mapa.obtener_celda(Coordenada(1, 3)).colocar_edificio(make_shared<Granja>("J1"));
    jugador += Coordenada(1, 3);

    mapa.obtener_celda(Coordenada(5, 2)).colocar_edificio(make_shared<Torre>("J1"));
    jugador += Coordenada(5, 2);

    mapa.obtener_celda(Coordenada(0, 2)).colocar_unidad(make_shared<Soldado>("J2"));

    mapa.obtener_celda(Coordenada(2, 2)).colocar_unidad(make_shared<Mago>("J2"));

    mapa.obtener_celda(Coordenada(2, 3)).colocar_edificio(make_shared<Granja>("J2"));
    sistema += Coordenada(2, 3);

    mapa.obtener_celda(Coordenada(4, 2)).colocar_unidad(make_shared<Arquero>("J2"));

    mapa.obtener_celda(Coordenada(4, 5)).colocar_edificio(make_shared<Cuartel>("J2"));
    sistema += Coordenada(4, 5);

    mapa.obtener_celda(Coordenada(3, 2)).colocar_edificio(make_shared<Cuartel>("Ne"));

    agregar_log("Escenario inicial cargado");

    agregar_evento(make_shared<EventoRefuerzo>(3, "J2", "S", 1));
    agregar_evento(make_shared<EventoRefuerzo>(5, "J2", "A", 1));
    agregar_evento(make_shared<EventoClima>(4, "Tormenta"));
    agregar_evento(make_shared<EventoClima>(7, "Clima despejado"));

    agregar_log("Eventos programados cargados");
}

void Contexto::procesar_eventos() {
    for (auto& evento : eventos) {
        if (evento->debe_activarse(turno_actual)) {
            evento->ejecutar(*this);
        }
    }
}

void Contexto::mostrar_panel_superior() const {
    cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║ Turno #" << setw(2) << turno_actual
         << " | Puntos de Acción: " << puntos_accion << "/2 | Puntaje: " << puntaje_jugador << "       ║" << endl;
    cout << "║ Objetivo: Alcanzar " << objetivo_dominio
         << "% de dominio antes del turno " << turno_limite << "              ║" << endl;
    cout << "║ Recursos: " << jugador.obtener_recursos() << "           ║" << endl;
    cout << "║ Moral: " << jugador.obtener_moral() << "/100                                          ║" << endl;
    cout << "║ Jugador: Dominio=" << setw(2) << calcular_dominio_jugador()
         << "% | Sistema: Dominio=" << setw(2) << calcular_dominio_sistema() << "%                 ║" << endl;

    if (turno_actual >= 5) {
        cout << "║ Alerta: Guardianes activos en cuadrante noreste               ║" << endl;
    }

    if (mision_activa && !mision_cumplida) {
        cout << "║ Misión: " << descripcion_mision << " ║" << endl;
    } else if (mision_cumplida) {
        cout << "║ Misión: COMPLETADA ✓                                          ║" << endl;
    }

    cout << "╚═══════════════════════════════════════════════════════════╝" << endl;
}