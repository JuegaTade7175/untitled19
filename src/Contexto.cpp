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

    file << "Turno: " << turno_actual << "\n";
    file << "Puntaje: " << puntaje_jugador << "\n";

    file << "Recursos: "
         << jugador.obtener_recursos().comida << " "
         << jugador.obtener_recursos().metal << " "
         << jugador.obtener_recursos().energia << "\n";

    file << "Moral: " << jugador.obtener_moral() << "\n";
    file << "Misión Cumplida: " << mision_cumplida << "\n";

    return true;
}

bool Contexto::cargar_partida(const string& archivo) {

    ifstream file(archivo);
    if (!file.is_open()) return false;

    string etiqueta;
    int comida, metal, energia, moral;

    file >> etiqueta >> turno_actual;
    file >> etiqueta >> puntaje_jugador;

    file >> etiqueta >> comida >> metal >> energia;

    file >> etiqueta >> moral;
    file >> etiqueta >> mision_cumplida;

    jugador.establecer_recursos(Recursos(comida, metal, energia));
    jugador.establecer_moral(moral);

    agregar_log("Partida cargada desde " + archivo);

    return true;
}

void Contexto::generar_reporte_final(const string& archivo) {

    int unidades_j1 = 0, unidades_j2 = 0;
    int edificios_j1 = 0, edificios_j2 = 0;

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_unidad()) {
                if (celda.obtener_unidad()->obtener_propietario() == "J1") unidades_j1++;
                else if (celda.obtener_unidad()->obtener_propietario() == "J2") unidades_j2++;
            }

            if (celda.tiene_edificio()) {
                if (celda.obtener_edificio()->obtener_propietario() == "J1") edificios_j1++;
                else if (celda.obtener_edificio()->obtener_propietario() == "J2") edificios_j2++;
            }
        }
    }

    ofstream out(archivo);

    out << "═══════════════════════════════════════════════════════════════\n";
    out << "           RIVAL FRONTIERS - REPORTE FINAL DE PARTIDA         \n";
    out << "═══════════════════════════════════════════════════════════════\n\n";

    bool victoria = verificar_victoria();
    out << "RESULTADO: " << (victoria ? "✓ VICTORIA" : "✗ DERROTA") << "\n\n";

    out << "--- INFORMACIÓN DE PARTIDA ---\n";
    out << "Turno final: " << turno_actual << "/" << turno_limite << "\n";
    out << "Misión: " << (mision_cumplida ? "✓ Completada" : "✗ No completada") << "\n";
    out << "Clima final: " << clima_actual << "\n\n";

    out << "--- CONTROL TERRITORIAL ---\n";
    out << "Dominio Jugador (J1): " << calcular_dominio_jugador() << "%\n";
    out << "Dominio Sistema (J2): " << calcular_dominio_sistema() << "%\n";
    out << "Territorios neutrales: " << calcular_neutrales() << "%\n\n";

    out << "--- RECURSOS Y MORAL ---\n";
    out << "Recursos finales J1: " << jugador.obtener_recursos() << "\n";
    out << "Moral final J1: " << jugador.obtener_moral() << "/100\n";
    out << "Factor de moral: x" << fixed << setprecision(2)
        << jugador.obtener_factor_moral() << "\n\n";

    out << "--- FUERZAS MILITARES ---\n";
    out << "Unidades J1: " << unidades_j1 << "\n";
    out << "Unidades J2: " << unidades_j2 << "\n";
    out << "Edificios J1: " << edificios_j1 << "\n";
    out << "Edificios J2: " << edificios_j2 << "\n\n";

    out << "--- PUNTAJE ---\n";
    out << "Puntaje total: " << puntaje_jugador << " puntos\n\n";

    out << "--- MEJORAS DESBLOQUEADAS ---\n";
    out << "Mejora de ataque: " << (mejora_ataque_desbloqueada ? "✓ Sí" : "✗ No") << "\n";
    out << "Mejora de defensa: " << (mejora_defensa_desbloqueada ? "✓ Sí" : "✗ No") << "\n\n";

    out << "--- ANÁLISIS FINAL ---\n";
    if (victoria) {
        if (mision_cumplida) {
            out << "Victoria por misión completada.\n";
        } else if (calcular_dominio_jugador() >= objetivo_dominio) {
            out << "Victoria por dominio territorial (" << calcular_dominio_jugador() << "% >= " << objetivo_dominio << "%).\n";
        }
    } else {
        if (turno_actual >= turno_limite) {
            out << "Derrota por límite de turnos alcanzado sin cumplir objetivos.\n";
        } else if (jugador.obtener_recursos().comida < -10) {
            out << "Derrota por colapso económico (comida: " << jugador.obtener_recursos().comida << ").\n";
        } else if (calcular_dominio_sistema() >= 70) {
            out << "Derrota por superioridad territorial enemiga (" << calcular_dominio_sistema() << "%).\n";
        }
    }

    out << "\n═══════════════════════════════════════════════════════════════\n";
    out << "               ¡Gracias por jugar RIVAL FRONTIERS!            \n";
    out << "═══════════════════════════════════════════════════════════════\n";

    out.close();
}