#include <fstream>
#include "Contexto.h"
#include "Unidad.h"
#include "Edificio.h"
#include "Controlador.h"
#include <iostream>
#include <limits>

using namespace std;

ControladorJugador ctrl_jugador;
ControladorSistema ctrl_sistema;

void pausar() {
    cout << "\nPresiona ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void mostrar_menu_principal() {
    cout << "\n============ RIVAL FRONTIERS ============" << endl;
    cout << "1. Mover unidad" << endl;
    cout << "2. Atacar con unidad" << endl;
    cout << "3. Construir edificio" << endl;
    cout << "4. Reclutar unidad" << endl;
    cout << "5. Usar habilidad especial" << endl;
    cout << "6. Ver mapa completo" << endl;
    cout << "7. Ver estadísticas" << endl;
    cout << "8. Finalizar turno" << endl;
    cout << "9. Guardar partida" << endl;
    cout << "10. Cargar partida" << endl;
    cout << "11. Ayuda / Glosario" << endl;
    cout << "12. Salir" << endl;
    cout << "=========================================" << endl;
    cout << "Elige una opción: ";
}

void mostrar_ayuda() {
    cout << "\n=== AYUDA / GLOSARIO ===" << endl;
    cout << "Unidades:" << endl;
    cout << "  J1S / J2S : Soldado jugador / guardián" << endl;
    cout << "  J1A / J2A : Arquero jugador / guardián" << endl;
    cout << "  J1C / J2C : Caballero jugador / guardián" << endl;
    cout << "  J1M / J2M : Mago jugador / guardián" << endl;
    cout << "  J1I / J2I : Ingeniero jugador / guardián" << endl;
    cout << "\nEdificios:" << endl;
    cout << "  J1Cu / J2Cu / NeCu : Cuartel (recluta unidades)" << endl;
    cout << "  J1Gr / J2Gr / NeGr : Granja (produce comida)" << endl;
    cout << "  J1To / J2To / NeTo : Torre (produce energía / visión)" << endl;
    cout << "\nTerrenos:" << endl;
    cout << "  LL : Llanura (costo 1, defensa 0)" << endl;
    cout << "  BO : Bosque (costo 2, defensa +1)" << endl;
    cout << "  MO : Montaña (costo 3, defensa +2)" << endl;
    cout << "  AG : Agua (no accesible, costo 99)" << endl;
    cout << "  PA : Pantano (costo 2, defensa -1)" << endl;
}

void mover_unidad(Contexto& ctx) {
    int f_origen, c_origen, f_destino, c_destino;

    cout << "\nMOVER UNIDAD" << endl;
    cout << "Coordenada origen (fila columna): ";
    cin >> f_origen >> c_origen;
    cout << "Coordenada destino (fila columna): ";
    cin >> f_destino >> c_destino;

    Coordenada origen(f_origen, c_origen);
    Coordenada destino(f_destino, c_destino);

    if (!ctx.obtener_mapa().es_valida(origen) || !ctx.obtener_mapa().es_valida(destino)) {
        cout << "¡Coordenadas inválidas!" << endl;
        return;
    }

    Celda& celda_origen = ctx.obtener_mapa().obtener_celda(origen);

    if (!celda_origen.tiene_unidad()) {
        cout << "¡No hay unidad en la posición de origen!" << endl;
        return;
    }

    auto unidad = celda_origen.obtener_unidad();

    if (unidad->obtener_propietario() != "J1") {
        cout << "¡No puedes mover unidades enemigas!" << endl;
        return;
    }

    if (!unidad->esta_activa()) {
        cout << "¡Esa unidad está inactiva y no puede moverse!" << endl;
        return;
    }

    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);

    if (celda_destino.tiene_unidad()) {
        cout << "¡La casilla destino está ocupada!" << endl;
        return;
    }

    int dif_f = abs(destino.fila - origen.fila);
    int dif_c = abs(destino.columna - origen.columna);

    if (!((dif_f == 1 && dif_c == 0) || (dif_f == 0 && dif_c == 1))) {
        cout << "¡Movimiento inválido! Solo se permite movimiento ortogonal adyacente." << endl;
        return;
    }

    if (!unidad->mover(destino, ctx)) {
        cout << "¡La unidad no puede entrar en ese terreno!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "¡No tienes puntos de acción suficientes!" << endl;
        return;
    }

    celda_destino.colocar_unidad(unidad);
    celda_origen.eliminar_unidad();

    ctx.agregar_log("Unidad " + unidad->obtener_codigo() + " movida de " +
                   to_string(origen.fila) + "," + to_string(origen.columna) +
                   " a " + to_string(destino.fila) + "," + to_string(destino.columna));

    cout << "¡Unidad movida exitosamente!" << endl;
}

void atacar_con_unidad(Contexto& ctx) {
    int f_atacante, c_atacante, f_objetivo, c_objetivo;

    cout << "\nATACAR CON UNIDAD" << endl;
    cout << "Coordenada de tu unidad (fila columna): ";
    cin >> f_atacante >> c_atacante;
    cout << "Coordenada del objetivo (fila columna): ";
    cin >> f_objetivo >> c_objetivo;

    Coordenada pos_atacante(f_atacante, c_atacante);
    Coordenada pos_objetivo(f_objetivo, c_objetivo);

    if (!ctx.obtener_mapa().es_valida(pos_atacante) ||
        !ctx.obtener_mapa().es_valida(pos_objetivo)) {
        cout << "¡Coordenadas inválidas!" << endl;
        return;
    }

    int dif_f = abs(pos_objetivo.fila - pos_atacante.fila);
    int dif_c = abs(pos_objetivo.columna - pos_atacante.columna);

    if (!((dif_f == 1 && dif_c == 0) || (dif_f == 0 && dif_c == 1))) {
        cout << "¡El objetivo debe estar en una casilla adyacente ortogonal!" << endl;
        return;
    }

    Celda& celda_atacante = ctx.obtener_mapa().obtener_celda(pos_atacante);
    Celda& celda_objetivo = ctx.obtener_mapa().obtener_celda(pos_objetivo);

    if (!celda_atacante.tiene_unidad()) {
        cout << "¡No hay unidad en la posición del atacante!" << endl;
        return;
    }

    auto atacante = celda_atacante.obtener_unidad();

    if (atacante->obtener_propietario() != "J1") {
        cout << "¡No puedes atacar con unidades enemigas!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "¡No tienes puntos de acción suficientes!" << endl;
        return;
    }

    if (celda_objetivo.tiene_unidad()) {
        auto objetivo = celda_objetivo.obtener_unidad();

        if (objetivo->obtener_propietario() == "J1") {
            cout << "¡No puedes atacar tus propias unidades!" << endl;
            ctx.reiniciar_puntos_accion();
            ctx.consumir_punto_accion(); // Devolver el PA
            return;
        }

        int dano_base = atacante->calcular_dano_ataque();
        int bono_def = celda_objetivo.obtener_terreno()->bono_defensa(*objetivo);

        int moral = ctx.obtener_jugador().obtener_moral();
        double factor_moral = 0.5 + (moral / 200.0); // Rango: 0.5 a 1.0

        int dano_con_moral = dano_base * factor_moral;
        int dano_final = max(1, dano_con_moral - bono_def);

        objetivo->recibir_dano(dano_final);

        ctx.agregar_log("Unidad " + atacante->obtener_codigo() + " atacó a " +
                       objetivo->obtener_codigo() + " causando " + to_string(dano_final) +
                       " de daño (Base: " + to_string(dano_base) +
                       " | Moral: x" + to_string(factor_moral) +
                       " | Def terreno: -" + to_string(bono_def) + ")");

        if (!objetivo->esta_activa()) {
            celda_objetivo.eliminar_unidad();
            ctx.agregar_puntaje(10);
            ctx.agregar_log("¡Unidad enemiga eliminada! +10 puntos");
            cout << "¡Enemigo eliminado! +10 puntos" << endl;
        } else {
            cout << "Daño causado: " << dano_final << " | Vida restante: " << objetivo->obtener_vida() << endl;
        }

    } else if (celda_objetivo.tiene_edificio()) {
        auto edificio = celda_objetivo.obtener_edificio();

        if (edificio->obtener_propietario() == "J1") {
            cout << "¡No puedes atacar tus propios edificios!" << endl;
            ctx.reiniciar_puntos_accion();
            ctx.consumir_punto_accion();
            return;
        }

        // Edificios se destruyen directamente
        celda_objetivo.eliminar_edificio();
        ctx.agregar_puntaje(25);
        ctx.agregar_log("¡Edificio enemigo destruido! +25 puntos");
        cout << "¡Edificio enemigo destruido! +25 puntos" << endl;

    } else {
        cout << "¡No hay objetivo válido en esa casilla!" << endl;
        ctx.reiniciar_puntos_accion();
        ctx.consumir_punto_accion();
    }
}

void construir_edificio(Contexto& ctx) {
    int fila, columna, tipo;

    cout << "\nCONSTRUIR EDIFICIO" << endl;
    cout << "Tipo de edificio:" << endl;
    cout << "1. Granja (Costo: Metal=1, Energía=1)" << endl;
    cout << "2. Cuartel (Costo: Metal=2, Energía=1)" << endl;
    cout << "3. Torre (Costo: Metal=2, Energía=1)" << endl;
    cout << "4. Forja (Costo: Metal=3, Energía=2)" << endl;
    cout << "Elige tipo: ";
    cin >> tipo;

    cout << "Coordenada (fila columna): ";
    cin >> fila >> columna;

    Coordenada pos(fila, columna);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "¡Coordenada inválida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    // VALIDAR QUE ESTÉ LIBRE (ni unidad ni edificio)
    if (celda.tiene_unidad()) {
        cout << "¡Hay una unidad en esa casilla! Debes desocuparla primero." << endl;
        return;
    }

    if (celda.tiene_edificio()) {
        cout << "¡Ya hay un edificio en esa casilla!" << endl;
        return;
    }

    // VALIDAR TERRENO (no construir en agua)
    if (celda.obtener_terreno()->obtener_codigo() == "AG") {
        cout << "¡No se puede construir en agua!" << endl;
        return;
    }

    Recursos costo;
    shared_ptr<Edificio> edificio;

    switch(tipo) {
        case 1:
            costo = Recursos(0, 1, 1);
            edificio = make_shared<Granja>("J1");
            break;
        case 2:
            costo = Recursos(0, 2, 1);
            edificio = make_shared<Cuartel>("J1");
            break;
        case 3:
            costo = Recursos(0, 2, 1);
            edificio = make_shared<Torre>("J1");
            break;
        case 4:
            costo = Recursos(0, 3, 2);
            edificio = make_shared<Forja>("J1");
            break;
        default:
            cout << "¡Tipo inválido!" << endl;
            return;
    }

    if (!ctx.obtener_jugador().puede_pagar(costo)) {
        cout << "¡No tienes recursos suficientes!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "¡No tienes puntos de acción suficientes!" << endl;
        return;
    }

    ctx.obtener_jugador().consumir_recursos(costo);
    celda.colocar_edificio(edificio);
    ctx.obtener_jugador() += pos;

    ctx.agregar_log("Edificio " + edificio->obtener_codigo() + " construido en " +
                   to_string(fila) + "," + to_string(columna));

    cout << "¡Edificio construido exitosamente!" << endl;
}

void reclutar_unidad(Contexto& ctx) {
    int fila, columna, tipo;

    cout << "\nRECLUTAR UNIDAD" << endl;
    cout << "Tipo de unidad:" << endl;
    cout << "1. Soldado (Costo: Comida=1, Metal=1)" << endl;
    cout << "2. Arquero (Costo: Comida=1, Metal=1)" << endl;
    cout << "3. Caballero (Costo: Comida=2, Metal=2)" << endl;
    cout << "Elige tipo: ";
    cin >> tipo;

    cout << "Coordenada del cuartel (fila columna): ";
    cin >> fila >> columna;

    Coordenada pos(fila, columna);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "¡Coordenada inválida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    if (!celda.tiene_edificio() || celda.obtener_edificio()->obtener_tipo() != "Cu") {
        cout << "¡Debe haber un cuartel en esa posición!" << endl;
        return;
    }

    // VALIDAR QUE SEA TUYO
    if (celda.obtener_edificio()->obtener_propietario() != "J1") {
        cout << "¡Ese cuartel no es tuyo!" << endl;
        return;
    }

    if (celda.tiene_unidad()) {
        cout << "¡La casilla ya tiene una unidad!" << endl;
        return;
    }

    Recursos costo;
    shared_ptr<Unidad> unidad;

    switch(tipo) {
        case 1:
            costo = Recursos(1, 1, 0);
            unidad = make_shared<Soldado>("J1");
            break;
        case 2:
            costo = Recursos(1, 1, 0);
            unidad = make_shared<Arquero>("J1");
            break;
        case 3:
            costo = Recursos(2, 2, 0);
            unidad = make_shared<Caballero>("J1");
            break;
        default:
            cout << "¡Tipo inválido!" << endl;
            return;
    }

    if (!ctx.obtener_jugador().puede_pagar(costo)) {
        cout << "¡No tienes recursos suficientes!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "¡No tienes puntos de acción suficientes!" << endl;
        return;
    }

    ctx.obtener_jugador().consumir_recursos(costo);
    celda.colocar_unidad(unidad);

    ctx.agregar_log("Unidad " + unidad->obtener_codigo() + " reclutada en " +
                   to_string(fila) + "," + to_string(columna));

    cout << "¡Unidad reclutada exitosamente!" << endl;
}

void usar_habilidad(Contexto& ctx) {
    int f, c;

    cout << "\nUSAR HABILIDAD ESPECIAL" << endl;
    cout << "Coordenada de tu unidad (fila columna): ";
    cin >> f >> c;

    Coordenada pos(f, c);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "¡Coordenada inválida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    if (!celda.tiene_unidad()) {
        cout << "¡No hay unidad en esa posición!" << endl;
        return;
    }

    auto unidad = celda.obtener_unidad();

    if (unidad->obtener_propietario() != "J1") {
        cout << "¡No es tu unidad!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "¡No tienes puntos de acción suficientes!" << endl;
        return;
    }

    unidad->habilidad_especial(ctx);
    cout << "¡Habilidad especial activada!" << endl;
}

void producir_recursos(Contexto& ctx) {
    Mapa& mapa = ctx.obtener_mapa();

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_edificio()) {
                auto edificio = celda.obtener_edificio();

                if (edificio->obtener_propietario() == "J1") {
                    edificio->efecto_turno(ctx.obtener_jugador());
                } else if (edificio->obtener_propietario() == "J2") {
                    edificio->efecto_turno(ctx.obtener_sistema());
                }
            }
        }
    }

    ctx.agregar_log("Recursos producidos este turno");
}

void mantenimiento_unidades(Contexto& ctx) {
    Mapa& mapa = ctx.obtener_mapa();
    int unidades_jugador = 0;

    for (int f = 0; f < mapa.obtener_filas(); f++) {
        for (int c = 0; c < mapa.obtener_columnas(); c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.tiene_unidad()) {
                auto u = celda.obtener_unidad();
                if (u->obtener_propietario() == "J1") {
                    unidades_jugador++;
                }
            }
        }
    }

    if (unidades_jugador == 0) {
        return;
    }

    Recursos costo_mantenimiento(unidades_jugador, 0, 0);
    ctx.obtener_jugador().consumir_recursos(costo_mantenimiento);
    ctx.agregar_log("Mantenimiento: " + to_string(unidades_jugador) +
                    " unidades consumieron " + to_string(unidades_jugador) + " de comida");

    if (ctx.obtener_jugador().obtener_recursos().comida < 0) {
        ctx.obtener_jugador().modificar_moral(-10);
        ctx.agregar_log("Comida insuficiente: moral reducida (-10). Algunas unidades quedan inactivas.");

        int desactivadas = 0;
        for (int f = 0; f < mapa.obtener_filas() && desactivadas < 2; f++) {
            for (int c = 0; c < mapa.obtener_columnas() && desactivadas < 2; c++) {
                Coordenada pos(f, c);
                Celda& celda = mapa.obtener_celda(pos);
                if (celda.tiene_unidad()) {
                    auto u = celda.obtener_unidad();
                    if (u->obtener_propietario() == "J1" && u->esta_activa()) {
                        u->desactivar();
                        desactivadas++;
                    }
                }
            }
        }
    }
}

int main() {
    Contexto ctx;
    ctx.inicializar_escenario();

    cout << "\n¡Bienvenido a RIVAL FRONTIERS!" << endl;
    pausar();

    bool jugando = true;

    while (jugando) {
        // USAR CONTROLADOR JUGADOR
        ctrl_jugador.resolver_fase(ctx);

        ctx.mostrar_panel_superior();
        ctx.obtener_mapa().mostrar();

        mostrar_menu_principal();

        int opcion;
        cin >> opcion;

        switch(opcion) {
            case 1:
                mover_unidad(ctx);
                break;
            case 2:
                atacar_con_unidad(ctx);
                break;
            case 3:
                construir_edificio(ctx);
                break;
            case 4:
                reclutar_unidad(ctx);
                break;
            case 5:
                usar_habilidad(ctx);
                break;
            case 6:
                ctx.obtener_mapa().mostrar();
                break;
            case 7:
                cout << "\n=== ESTADÍSTICAS ===" << endl;
                cout << "Turno: " << ctx.obtener_turno() << endl;
                cout << "Puntaje: " << ctx.obtener_puntaje() << endl;
                cout << "Dominio Jugador: " << ctx.calcular_dominio_jugador() << "%" << endl;
                cout << "Dominio Sistema: " << ctx.calcular_dominio_sistema() << "%" << endl;
                cout << "Moral: " << ctx.obtener_jugador().obtener_moral() << endl;
                break;
            case 8:
                // Finalizar turno
                ctx.mostrar_bitacora();

                // GUARDAR LOG A ARCHIVO
            {
                ofstream log_file("partida_completa.log", ios::app);
                log_file << "=== TURNO " << ctx.obtener_turno() << " ===\n";
                auto logs = ctx.obtener_logs();
                for (const auto& msg : logs) {
                    log_file << msg << "\n";
                }
                log_file.close();
            }

                mantenimiento_unidades(ctx);
                producir_recursos(ctx);

                // USAR CONTROLADOR SISTEMA
                ctrl_sistema.resolver_fase(ctx);

                ctx.incrementar_turno();
                ctx.reiniciar_puntos_accion();

                // VERIFICAR MISIÓN
                ctx.verificar_mision();

                ctx.limpiar_bitacora();

                if (ctx.verificar_victoria()) {
                    cout << "\n¡¡¡VICTORIA!!! Has alcanzado el dominio objetivo." << endl;
                    cout << "Puntaje final: " << ctx.obtener_puntaje() << endl;
                    jugando = false;
                }

                if (ctx.verificar_derrota()) {
                    cout << "\n¡DERROTA! El sistema te ha superado." << endl;
                    jugando = false;
                }
                break;
            case 9: // Guardar
            {
                ofstream archivo("partida.txt");
                archivo << "Turno: " << ctx.obtener_turno() << "\n";
                archivo << "Recursos: " << ctx.obtener_jugador().obtener_recursos() << "\n";
                archivo << "Puntaje: " << ctx.obtener_puntaje() << "\n";
                archivo.close();
                cout << "¡Partida guardada!" << endl;
            }
                break;
            case 10: // Cargar
            {
                ifstream archivo("partida.txt");
                if (archivo.is_open()) {
                    // Implementación básica
                    cout << "Funcionalidad de carga en desarrollo" << endl;
                    archivo.close();
                } else {
                    cout << "No hay partida guardada" << endl;
                }
            }
                break;
            case 11:
                mostrar_ayuda();
                break;
            case 12:
                cout << "¡Gracias por jugar!" << endl;
                jugando = false;
                break;
            default:
                cout << "Opción inválida." << endl;
        }

        if (jugando && opcion >= 1 && opcion <= 5) {
            pausar();
        }
    }

    if (!jugando) {
        ofstream reporte("reporte_final.txt");
        reporte << "======================== FIN DE PARTIDA ========================\n";
        reporte << "RESULTADO: " << (ctx.verificar_victoria() ? "VICTORIA" : "DERROTA") << "\n";
        reporte << "Turno final: " << ctx.obtener_turno() << "\n";
        reporte << "Dominio alcanzado: " << ctx.calcular_dominio_jugador() << "%\n";
        reporte << "Recursos finales: " << ctx.obtener_jugador().obtener_recursos() << "\n";
        reporte << "Puntaje: " << ctx.obtener_puntaje() << "\n";
        reporte << "Misión: " << (ctx.es_mision_cumplida() ? "COMPLETADA ✓" : "NO COMPLETADA") << "\n";
        reporte << "==============================================================\n";
        reporte.close();
        cout << "\nReporte final guardado en 'reporte_final.txt'" << endl;
    }

    return 0;
}
