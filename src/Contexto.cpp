//
// Created by tadeo on 23/11/2025.
//

#include "Contexto.h"
#include "Unidad.h"
#include "Edificio.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

Contexto::Contexto(int filas, int columnas)
    : mapa(filas, columnas),
      jugador("J1", Recursos(18, 12, 7)),
      sistema("J2", Recursos(15, 10, 5)),
      turno_actual(1),
      puntos_accion(2),
      puntos_accion_maximos(2),
      objetivo_dominio(60),
      turno_limite(20),
      puntaje_jugador(0),
      mision_activa(true),
      mision_cumplida(false),
      descripcion_mision("Capturar el cuartel neutral en (3,2) antes del turno 8"),
      objetivo_mision(3, 2),
      mejora_ataque_desbloqueada(false),
      mejora_defensa_desbloqueada(false),
      clima_actual("Despejado")
{
    jugador.establecer_moral(100);
    sistema.establecer_moral(100);
}

void Contexto::inicializar_escenario() {

    mapa.obtener_celda(Coordenada(0, 0)).colocar_edificio(make_shared<Cuartel>("J1"));
    jugador += Coordenada(0, 0);

    auto soldado_j1 = make_shared<Soldado>("J1");
    soldado_j1->establecer_posicion(Coordenada(1, 1));
    mapa.obtener_celda(Coordenada(1, 1)).colocar_unidad(soldado_j1);

    mapa.obtener_celda(Coordenada(1, 3)).colocar_edificio(make_shared<Granja>("J1"));
    jugador += Coordenada(1, 3);

    mapa.obtener_celda(Coordenada(5, 2)).colocar_edificio(make_shared<Torre>("J1"));
    jugador += Coordenada(5, 2);

    auto soldado_j2 = make_shared<Soldado>("J2");
    soldado_j2->establecer_posicion(Coordenada(0, 2));
    mapa.obtener_celda(Coordenada(0, 2)).colocar_unidad(soldado_j2);

    auto mago_j2 = make_shared<Mago>("J2");
    mago_j2->establecer_posicion(Coordenada(2, 2));
    mapa.obtener_celda(Coordenada(2, 2)).colocar_unidad(mago_j2);

    mapa.obtener_celda(Coordenada(2, 3)).colocar_edificio(make_shared<Granja>("J2"));
    sistema += Coordenada(2, 3);

    auto arquero_j2 = make_shared<Arquero>("J2");
    arquero_j2->establecer_posicion(Coordenada(4, 2));
    mapa.obtener_celda(Coordenada(4, 2)).colocar_unidad(arquero_j2);

    mapa.obtener_celda(Coordenada(4, 5)).colocar_edificio(make_shared<Cuartel>("J2"));
    sistema += Coordenada(4, 5);

    mapa.obtener_celda(Coordenada(3, 2)).colocar_edificio(make_shared<Cuartel>("Ne"));

    agregar_log("Escenario inicial configurado");

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

void Contexto::agregar_evento(std::shared_ptr<Evento> ev) {
    eventos.push_back(ev);
}

void Contexto::incrementar_turno() {
    turno_actual++;

    if (bitacora.size() > 50) {
        bitacora.erase(bitacora.begin(), bitacora.begin() + 25);
    }
}

bool Contexto::consumir_punto_accion(int cantidad) {
    if (puntos_accion >= cantidad) {
        puntos_accion -= cantidad;
        return true;
    }
    return false;
}

void Contexto::agregar_log(const string& msg) {
    bitacora.push_back("[T" + to_string(turno_actual) + "] " + msg);
}

void Contexto::mostrar_bitacora() const {
    cout << "\n═══════════════════════════════════════════════════════\n";
    cout << " BITÁCORA DEL TURNO " << turno_actual << "\n";
    cout << "═══════════════════════════════════════════════════════\n";

    int mostrados = 0;
    for (auto it = bitacora.rbegin(); it != bitacora.rend() && mostrados < 10; ++it, ++mostrados) {
        cout << *it << "\n";
    }
}

void Contexto::limpiar_bitacora() {
}

void Contexto::guardar_bitacora_archivo(const string& archivo) {
    ofstream file(archivo, ios::app);
    file << "\n=== TURNO " << turno_actual << " ===\n";
    for (const auto& msg : bitacora) file << msg << "\n";
}

void Contexto::verificar_captura_neutrales() {
    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {

            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_edificio() && celda.tiene_unidad()) {

                auto edificio = celda.obtener_edificio();
                auto unidad = celda.obtener_unidad();

                if (unidad->obtener_propietario() == "J1" &&
                    edificio->obtener_propietario() != "J1") {

                    string antiguo = edificio->obtener_propietario();
                    edificio->cambiar_propietario("J1");
                    jugador += pos;

                    agregar_log("Edificio " + edificio->obtener_tipo() +
                                " capturado en (" + to_string(f) + "," + to_string(c) + ")");

                    agregar_puntaje(15);
                }
            }
        }
    }
}

void Contexto::verificar_mision() {

    if (!mision_activa || mision_cumplida) return;

    Celda& celda = mapa.obtener_celda(objetivo_mision);

    if (celda.tiene_edificio() &&
        celda.obtener_edificio()->obtener_propietario() == "J1") {

        mision_cumplida = true;
        agregar_puntaje(50);
        agregar_log("Misión completada: cuartel neutral capturado (+50)");
        return;
    }

    if (turno_actual >= 8 && !mision_cumplida) {
        agregar_log("Misión fallida: límite de turno excedido");
    }
}

void Contexto::marcar_mision_cumplida() {
    mision_cumplida = true;
    agregar_puntaje(50);
}

bool Contexto::verificar_victoria() const {

    if (calcular_dominio_jugador() >= objetivo_dominio) return true;
    if (mision_cumplida) return true;

    return false;
}

bool Contexto::verificar_derrota() const {

    if (turno_actual >= turno_limite) return true;
    if (jugador.obtener_recursos().comida < -10) return true;
    if (calcular_dominio_sistema() >= 70) return true;

    return false;
}

int Contexto::calcular_dominio_jugador() const {
    int total = mapa.obtener_filas() * mapa.obtener_columnas();
    return (jugador.contar_territorios() * 100) / total;
}

int Contexto::calcular_dominio_sistema() const {
    int total = mapa.obtener_filas() * mapa.obtener_columnas();
    return (sistema.contar_territorios() * 100) / total;
}

int Contexto::calcular_neutrales() const {
    return 100 - calcular_dominio_jugador() - calcular_dominio_sistema();
}

void Contexto::mostrar_panel_superior() const {

    cout << "\n════════════════════════════════════════════════════════════\n";
    cout << " Turno " << turno_actual
         << " | PA: " << puntos_accion << "/" << puntos_accion_maximos
         << " | Puntaje: " << puntaje_jugador
         << " | Clima: " << clima_actual << "\n";

    cout << " Objetivo: " << objetivo_dominio
         << "% dominio antes del turno " << turno_limite << "\n";

    cout << " Recursos: " << jugador.obtener_recursos() << "\n";
    cout << " Moral: " << jugador.obtener_moral()
         << " (x" << fixed << setprecision(2)
         << jugador.obtener_factor_moral() << ")\n";

    cout << " Dominio: J1=" << calcular_dominio_jugador()
         << "% | J2=" << calcular_dominio_sistema()
         << "% | Neutral=" << calcular_neutrales() << "%\n";

    if (mision_activa) {
        if (mision_cumplida)
            cout << " Misión: COMPLETADA\n";
        else
            cout << " Misión: " << descripcion_mision << "\n";
    }

    cout << "════════════════════════════════════════════════════════════\n";
}

bool Contexto::guardar_partida(const string& archivo) {

    ofstream file(archivo);
    if (!file.is_open()) return false;

    file << turno_actual << "\n";
    file << puntaje_jugador << "\n";

    file << jugador.obtener_recursos().comida << " "
         << jugador.obtener_recursos().metal << " "
         << jugador.obtener_recursos().energia << "\n";

    file << jugador.obtener_moral() << "\n";
    file << mision_cumplida << "\n";

    return true;
}

bool Contexto::cargar_partida(const string& archivo) {

    ifstream file(archivo);
    if (!file.is_open()) return false;

    int comida, metal, energia, moral;

    file >> turno_actual >> puntaje_jugador;
    file >> comida >> metal >> energia >> moral;
    file >> mision_cumplida;

    jugador.obtener_recursos() = Recursos(comida, metal, energia);
    jugador.establecer_moral(moral);

    agregar_log("Partida cargada desde " + archivo);

    return true;
}

void Contexto::generar_reporte_final(const string& archivo) {

    ofstream out(archivo);

    out << "============================================================\n";
    out << " RIVAL FRONTIERS - REPORTE FINAL\n";
    out << "============================================================\n\n";

    out << "Resultado: " << (verificar_victoria() ? "Victoria" : "Derrota") << "\n";
    out << "Turno final: " << turno_actual << "/" << turno_limite << "\n";
    out << "Dominio jugador: " << calcular_dominio_jugador() << "%\n";
    out << "Dominio sistema: " << calcular_dominio_sistema() << "%\n";
    out << "Neutrales: " << calcular_neutrales() << "%\n\n";

    out << "Recursos finales: " << jugador.obtener_recursos() << "\n";
    out << "Moral final: " << jugador.obtener_moral() << "\n";
    out << "Puntaje total: " << puntaje_jugador << "\n\n";

    out << "Misión: " << (mision_cumplida ? "Completada" : "No completada") << "\n";
    out << "Clima final: " << clima_actual << "\n\n";

    out << "============================================================\n";
    out << " HISTORIAL DE ACCIONES\n";
    out << "============================================================\n";

    for (const auto& log : bitacora) {
        out << log << "\n";
    }
}
