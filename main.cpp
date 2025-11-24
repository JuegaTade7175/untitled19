#include <fstream>
#include "Contexto.h"
#include "Unidad.h"
#include "Edificio.h"
#include "Controlador.h"
#include <iostream>
#include <limits>
#include <memory>

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
    cout << "7. Ver estadisticas" << endl;
    cout << "8. Finalizar turno" << endl;
    cout << "9. Guardar partida" << endl;
    cout << "10. Cargar partida" << endl;
    cout << "11. Ayuda / Glosario" << endl;
    cout << "12. Desbloquear mejoras" << endl;
    cout << "13. Construccion rapida (Ingeniero)" << endl;
    cout << "14. Salir" << endl;
    cout << "=========================================" << endl;
    cout << "Elige una opcion: ";
}

void mostrar_ayuda() {
    cout << "\n=== AYUDA / GLOSARIO ===" << endl;
    cout << "Unidades:" << endl;
    cout << "  J1S / J2S : Soldado jugador / guardian" << endl;
    cout << "  J1A / J2A : Arquero jugador / guardian" << endl;
    cout << "  J1C / J2C : Caballero jugador / guardian" << endl;
    cout << "  J1M / J2M : Mago jugador / guardian" << endl;
    cout << "  J1I / J2I : Ingeniero jugador / guardian" << endl;
    cout << "\nEdificios:" << endl;
    cout << "  J1Cu / J2Cu / NeCu : Cuartel (recluta unidades)" << endl;
    cout << "  J1Gr / J2Gr / NeGr : Granja (produce comida)" << endl;
    cout << "  J1To / J2To / NeTo : Torre (produce energia / vision)" << endl;
    cout << "  J1Fo / J2Fo / NeFo : Forja (produce metal)" << endl;
    cout << "\nTerrenos:" << endl;
    cout << "  LL : Llanura (costo 1, defensa 0)" << endl;
    cout << "  BO : Bosque (costo 2, defensa +1)" << endl;
    cout << "  MO : Montana (costo 3, defensa +2)" << endl;
    cout << "  AG : Agua (no accesible, costo 99)" << endl;
    cout << "  PA : Pantano (costo 2, defensa -1)" << endl;
    cout << "\nHabilidades especiales:" << endl;
    cout << "  Soldado: Defensa mejorada (+5 DEF, costo: 1 energia)" << endl;
    cout << "  Arquero: Flecha potenciada (+10 ATK, costo: 1 energia)" << endl;
    cout << "  Caballero: Carga devastadora (+15 ATK, costo: 1 energia)" << endl;
    cout << "  Mago: Hechizo de area (dano multiple, costo: 2 energia)" << endl;
    cout << "  Ingeniero: Construccion rapida (costo: 1 comida, 1 energia)" << endl;
    cout << "\nMejoras desbloqueables:" << endl;
    cout << "  Mejora de ataque: +5 ATK a todas las unidades (Costo: 5 metal, 3 energia)" << endl;
    cout << "  Mejora de defensa: +3 DEF a todas las unidades (Costo: 4 metal, 2 energia)" << endl;
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
        cout << "Coordenadas invalidas!" << endl;
        return;
    }

    Celda& celda_origen = ctx.obtener_mapa().obtener_celda(origen);

    if (!celda_origen.tiene_unidad()) {
        cout << "No hay unidad en la posicion de origen!" << endl;
        return;
    }

    auto unidad = celda_origen.obtener_unidad();

    if (unidad->obtener_propietario() != "J1") {
        cout << "No puedes mover unidades enemigas!" << endl;
        return;
    }

    if (!unidad->esta_activa()) {
        cout << "Esa unidad esta inactiva y no puede moverse!" << endl;
        return;
    }

    Celda& celda_destino = ctx.obtener_mapa().obtener_celda(destino);

    if (celda_destino.tiene_unidad()) {
        cout << "La casilla destino esta ocupada!" << endl;
        return;
    }

    int dif_f = abs(destino.fila - origen.fila);
    int dif_c = abs(destino.columna - origen.columna);

    if (!((dif_f == 1 && dif_c == 0) || (dif_f == 0 && dif_c == 1))) {
        cout << "Movimiento invalido! Solo se permite movimiento ortogonal adyacente." << endl;
        return;
    }

    if (!unidad->mover(destino, ctx)) {
        cout << "La unidad no puede entrar en ese terreno!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "No tienes puntos de accion suficientes!" << endl;
        return;
    }

    unidad->establecer_posicion(destino);
    celda_destino.colocar_unidad(unidad);
    celda_origen.eliminar_unidad();

    ctx.agregar_log("Unidad " + unidad->obtener_codigo() + " movida de " +
                   to_string(origen.fila) + "," + to_string(origen.columna) +
                   " a " + to_string(destino.fila) + "," + to_string(destino.columna));

    cout << "Unidad movida exitosamente!" << endl;
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
        cout << "Coordenadas invalidas!" << endl;
        return;
    }

    Celda& celda_atacante = ctx.obtener_mapa().obtener_celda(pos_atacante);

    if (!celda_atacante.tiene_unidad()) {
        cout << "No hay unidad en la posicion del atacante!" << endl;
        return;
    }

    auto atacante = celda_atacante.obtener_unidad();

    if (atacante->obtener_propietario() != "J1") {
        cout << "No puedes atacar con unidades enemigas!" << endl;
        return;
    }

    int dif_f = abs(pos_objetivo.fila - pos_atacante.fila);
    int dif_c = abs(pos_objetivo.columna - pos_atacante.columna);
    int rango = atacante->obtener_rango_ataque();
    int distancia = dif_f + dif_c;

    if (distancia > rango) {
        cout << "El objetivo esta fuera del rango de ataque (rango: " << rango << ")!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "No tienes puntos de accion suficientes!" << endl;
        return;
    }

    Celda& celda_objetivo = ctx.obtener_mapa().obtener_celda(pos_objetivo);

    if (celda_objetivo.tiene_unidad()) {
        auto objetivo = celda_objetivo.obtener_unidad();

        if (objetivo->obtener_propietario() == "J1") {
            cout << "No puedes atacar tus propias unidades!" << endl;
            ctx.reiniciar_puntos_accion();
            ctx.consumir_punto_accion();
            return;
        }

        int dano_base = atacante->calcular_dano_ataque();

        // Aplicar mejoras si están desbloqueadas
        if (ctx.tiene_mejora_ataque()) {
            dano_base += 5;
        }

        int bono_def = celda_objetivo.obtener_terreno()->bono_defensa(*objetivo);
        int def_objetivo = objetivo->calcular_defensa();

        if (ctx.tiene_mejora_defensa()) {
            def_objetivo += 3;
        }

        int moral = ctx.obtener_jugador().obtener_moral();
        double factor_moral = 0.5 + (moral / 200.0);

        int dano_con_moral = dano_base * factor_moral;
        int dano_final = max(1, dano_con_moral - bono_def);

        objetivo->recibir_dano(dano_final);

        ctx.agregar_log("Unidad " + atacante->obtener_codigo() + " ataco a " +
                       objetivo->obtener_codigo() + " causando " + to_string(dano_final) +
                       " de dano (Base: " + to_string(dano_base) +
                       " | Moral: x" + to_string(factor_moral) +
                       " | Def terreno: -" + to_string(bono_def) + ")");

        if (!objetivo->esta_activa()) {
            celda_objetivo.eliminar_unidad();
            ctx.agregar_puntaje(10);
            ctx.agregar_log("Unidad enemiga eliminada! +10 puntos");
            cout << "Enemigo eliminado! +10 puntos" << endl;
        } else {
            cout << "Dano causado: " << dano_final << " | Vida restante: " << objetivo->obtener_vida() << endl;
        }

    } else if (celda_objetivo.tiene_edificio()) {
        auto edificio = celda_objetivo.obtener_edificio();

        if (edificio->obtener_propietario() == "J1") {
            cout << "No puedes atacar tus propios edificios!" << endl;
            ctx.reiniciar_puntos_accion();
            ctx.consumir_punto_accion();
            return;
        }

        celda_objetivo.eliminar_edificio();
        ctx.agregar_puntaje(25);
        ctx.agregar_log("Edificio enemigo destruido! +25 puntos");
        cout << "Edificio enemigo destruido! +25 puntos" << endl;

    } else {
        cout << "No hay objetivo valido en esa casilla!" << endl;
        ctx.reiniciar_puntos_accion();
        ctx.consumir_punto_accion();
    }
}

void construir_edificio(Contexto& ctx) {
    int fila, columna, tipo;

    cout << "\nCONSTRUIR EDIFICIO" << endl;
    cout << "Tipo de edificio:" << endl;
    cout << "1. Granja (Costo: Metal=1, Energia=1)" << endl;
    cout << "2. Cuartel (Costo: Metal=2, Energia=1)" << endl;
    cout << "3. Torre (Costo: Metal=2, Energia=1)" << endl;
    cout << "4. Forja (Costo: Metal=3, Energia=2)" << endl;
    cout << "Elige tipo: ";
    cin >> tipo;

    cout << "Coordenada (fila columna): ";
    cin >> fila >> columna;

    Coordenada pos(fila, columna);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "Coordenada invalida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    if (celda.tiene_unidad()) {
        cout << "Hay una unidad en esa casilla! Debes desocuparla primero." << endl;
        return;
    }

    if (celda.tiene_edificio()) {
        cout << "Ya hay un edificio en esa casilla!" << endl;
        return;
    }

    if (celda.obtener_terreno()->obtener_codigo() == "AG") {
        cout << "No se puede construir en agua!" << endl;
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
            cout << "Tipo invalido!" << endl;
            return;
    }

    if (!ctx.obtener_jugador().puede_pagar(costo)) {
        cout << "No tienes recursos suficientes!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "No tienes puntos de accion suficientes!" << endl;
        return;
    }

    ctx.obtener_jugador().consumir_recursos(costo);
    celda.colocar_edificio(edificio);
    ctx.obtener_jugador() += pos;

    ctx.agregar_log("Edificio " + edificio->obtener_codigo() + " construido en " +
                   to_string(fila) + "," + to_string(columna));

    cout << "Edificio construido exitosamente!" << endl;
}

void reclutar_unidad(Contexto& ctx) {
    int fila, columna, tipo;

    cout << "\nRECLUTAR UNIDAD" << endl;
    cout << "Tipo de unidad:" << endl;
    cout << "1. Soldado (Costo: Comida=1, Metal=1)" << endl;
    cout << "2. Arquero (Costo: Comida=1, Metal=1)" << endl;
    cout << "3. Caballero (Costo: Comida=2, Metal=2)" << endl;
    cout << "4. Mago (Costo: Comida=2, Metal=1, Energia=1)" << endl;
    cout << "5. Ingeniero (Costo: Comida=1, Metal=2)" << endl;
    cout << "Elige tipo: ";
    cin >> tipo;

    cout << "Coordenada del cuartel (fila columna): ";
    cin >> fila >> columna;

    Coordenada pos(fila, columna);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "Coordenada invalida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    if (!celda.tiene_edificio() || celda.obtener_edificio()->obtener_tipo() != "Cu") {
        cout << "Debe haber un cuartel en esa posicion!" << endl;
        return;
    }

    if (celda.obtener_edificio()->obtener_propietario() != "J1") {
        cout << "Ese cuartel no es tuyo!" << endl;
        return;
    }

    if (celda.tiene_unidad()) {
        cout << "La casilla ya tiene una unidad!" << endl;
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
        case 4:
            costo = Recursos(2, 1, 1);
            unidad = make_shared<Mago>("J1");
            break;
        case 5:
            costo = Recursos(1, 2, 0);
            unidad = make_shared<Ingeniero>("J1");
            break;
        default:
            cout << "Tipo invalido!" << endl;
            return;
    }

    if (!ctx.obtener_jugador().puede_pagar(costo)) {
        cout << "No tienes recursos suficientes!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "No tienes puntos de accion suficientes!" << endl;
        return;
    }

    ctx.obtener_jugador().consumir_recursos(costo);
    unidad->establecer_posicion(pos);
    celda.colocar_unidad(unidad);

    ctx.agregar_log("Unidad " + unidad->obtener_codigo() + " reclutada en " +
                   to_string(fila) + "," + to_string(columna));

    cout << "Unidad reclutada exitosamente!" << endl;
}

void usar_habilidad(Contexto& ctx) {
    int f, c;

    cout << "\nUSAR HABILIDAD ESPECIAL" << endl;
    cout << "Coordenada de tu unidad (fila columna): ";
    cin >> f >> c;

    Coordenada pos(f, c);

    if (!ctx.obtener_mapa().es_valida(pos)) {
        cout << "Coordenada invalida!" << endl;
        return;
    }

    Celda& celda = ctx.obtener_mapa().obtener_celda(pos);

    if (!celda.tiene_unidad()) {
        cout << "No hay unidad en esa posicion!" << endl;
        return;
    }

    auto unidad = celda.obtener_unidad();

    if (unidad->obtener_propietario() != "J1") {
        cout << "No es tu unidad!" << endl;
        return;
    }

    if (!ctx.consumir_punto_accion()) {
        cout << "No tienes puntos de accion suficientes!" << endl;
        return;
    }

    unidad->habilidad_especial(ctx);
    cout << "Habilidad especial activada!" << endl;
}

void desbloquear_mejoras(Contexto& ctx) {
    cout << "\n=== DESBLOQUEAR MEJORAS ===" << endl;
    cout << "1. Mejora de ataque (+5 ATK) - Costo: Metal=5, Energia=3" << endl;
    cout << "2. Mejora de defensa (+3 DEF) - Costo: Metal=4, Energia=2" << endl;
    cout << "Elige mejora: ";

    int opcion;
    cin >> opcion;

    if (opcion == 1) {
        if (ctx.tiene_mejora_ataque()) {
            cout << "Ya tienes esta mejora desbloqueada!" << endl;
            return;
        }

        Recursos costo(0, 5, 3);
        if (!ctx.obtener_jugador().puede_pagar(costo)) {
            cout << "No tienes recursos suficientes!" << endl;
            return;
        }

        if (!ctx.consumir_punto_accion()) {
            cout << "No tienes puntos de accion suficientes!" << endl;
            return;
        }

        ctx.obtener_jugador().consumir_recursos(costo);
        ctx.desbloquear_mejora_ataque();
        ctx.agregar_log("Mejora de ataque desbloqueada! Todas las unidades ganan +5 ATK");
        cout << "Mejora de ataque desbloqueada!" << endl;

    } else if (opcion == 2) {
        if (ctx.tiene_mejora_defensa()) {
            cout << "Ya tienes esta mejora desbloqueada!" << endl;
            return;
        }

        Recursos costo(0, 4, 2);
        if (!ctx.obtener_jugador().puede_pagar(costo)) {
            cout << "No tienes recursos suficientes!" << endl;
            return;
        }

        if (!ctx.consumir_punto_accion()) {
            cout << "No tienes puntos de accion suficientes!" << endl;
            return;
        }

        ctx.obtener_jugador().consumir_recursos(costo);
        ctx.desbloquear_mejora_defensa();
        ctx.agregar_log("Mejora de defensa desbloqueada! Todas las unidades ganan +3 DEF");
        cout << "Mejora de defensa desbloqueada!" << endl;

    } else {
        cout << "Opcion invalida!" << endl;
    }
}

void construccion_rapida_ingeniero(Contexto& ctx) {
    int f, c;

    cout << "\n=== CONSTRUCCION RAPIDA (INGENIERO) ===" << endl;
    cout << "Paso 1: Selecciona tu ingeniero" << endl;
    cout << "Coordenada del ingeniero (fila columna): ";
    cin >> f >> c;

    Coordenada pos_ingeniero(f, c);

    if (!ctx.obtener_mapa().es_valida(pos_ingeniero)) {
        cout << "Coordenada invalida!" << endl;
        return;
    }

    Celda& celda_ingeniero = ctx.obtener_mapa().obtener_celda(pos_ingeniero);

    if (!celda_ingeniero.tiene_unidad()) {
        cout << "No hay unidad en esa posicion!" << endl;
        return;
    }

    auto unidad = celda_ingeniero.obtener_unidad();

    if (unidad->obtener_propietario() != "J1") {
        cout << "No es tu unidad!" << endl;
        return;
    }

    if (unidad->obtener_tipo() != "I") {
        cout << "Esta unidad no es un ingeniero!" << endl;
        return;
    }

    // Cast a Ingeniero para acceder a métodos específicos
    auto ingeniero = dynamic_pointer_cast<Ingeniero>(unidad);

    if (!ingeniero) {
        cout << "Error al obtener el ingeniero!" << endl;
        return;
    }

    if (ingeniero->tiene_construccion_pendiente()) {
        cout << "\nEste ingeniero ya tiene una construccion pendiente: "
             << ingeniero->obtener_tipo_construccion() << endl;
        cout << "Paso 2: Selecciona donde colocarla" << endl;
        cout << "Coordenada de construccion (fila columna): ";
        cin >> f >> c;

        Coordenada pos_construccion(f, c);

        if (!ctx.obtener_mapa().es_valida(pos_construccion)) {
            cout << "Coordenada invalida!" << endl;
            return;
        }

        Celda& celda_construccion = ctx.obtener_mapa().obtener_celda(pos_construccion);

        if (celda_construccion.tiene_edificio()) {
            cout << "Ya hay un edificio en esa posicion!" << endl;
            return;
        }

        if (celda_construccion.tiene_unidad()) {
            cout << "Hay una unidad ocupando esa posicion!" << endl;
            return;
        }

        if (celda_construccion.obtener_terreno()->obtener_codigo() == "AG") {
            cout << "No se puede construir en agua!" << endl;
            return;
        }

        if (!ctx.consumir_punto_accion()) {
            cout << "No tienes puntos de accion suficientes!" << endl;
            return;
        }

        // Crear el edificio según el tipo pendiente
        shared_ptr<Edificio> edificio;
        string tipo = ingeniero->obtener_tipo_construccion();

        if (tipo == "Gr") {
            edificio = make_shared<Granja>("J1");
        } else if (tipo == "To") {
            edificio = make_shared<Torre>("J1");
        } else if (tipo == "Cu") {
            edificio = make_shared<Cuartel>("J1");
        } else if (tipo == "Fo") {
            edificio = make_shared<Forja>("J1");
        }

        if (edificio) {
            celda_construccion.colocar_edificio(edificio);
            ctx.obtener_jugador() += pos_construccion;
            ingeniero->completar_construccion();

            ctx.agregar_log("Ingeniero completo construccion rapida de " +
                          edificio->obtener_codigo() + " en (" +
                          to_string(f) + "," + to_string(c) + ")");
            cout << "Construccion rapida completada!" << endl;
        }

    } else {
        cout << "\nEste ingeniero no tiene construccion pendiente." << endl;
        cout << "Selecciona el tipo de edificio a preparar:" << endl;
        cout << "1. Granja" << endl;
        cout << "2. Torre" << endl;
        cout << "3. Cuartel" << endl;
        cout << "4. Forja" << endl;
        cout << "Elige tipo: ";

        int tipo_edificio;
        cin >> tipo_edificio;

        string tipo_codigo;
        switch(tipo_edificio) {
            case 1: tipo_codigo = "Gr"; break;
            case 2: tipo_codigo = "To"; break;
            case 3: tipo_codigo = "Cu"; break;
            case 4: tipo_codigo = "Fo"; break;
            default:
                cout << "Tipo invalido!" << endl;
                return;
        }

        Recursos costo(1, 0, 1);
        if (!ctx.obtener_jugador().puede_pagar(costo)) {
            cout << "No tienes recursos suficientes! (Costo: 1 comida, 1 energia)" << endl;
            return;
        }

        if (!ctx.consumir_punto_accion()) {
            cout << "No tienes puntos de accion suficientes!" << endl;
            return;
        }

        ctx.obtener_jugador().consumir_recursos(costo);
        ingeniero->establecer_construccion(tipo_codigo);

        ctx.agregar_log("Ingeniero preparo construccion rapida de tipo " + tipo_codigo);
        cout << "Construccion preparada! En el proximo turno podras colocarla." << endl;
    }
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
void reactivar_unidades(Contexto& ctx) {
Mapa& mapa = ctx.obtener_mapa();
for (int f = 0; f < mapa.obtener_filas(); f++) {
    for (int c = 0; c < mapa.obtener_columnas(); c++) {
        Coordenada pos(f, c);
        Celda& celda = mapa.obtener_celda(pos);

        if (celda.tiene_unidad()) {
            auto u = celda.obtener_unidad();
            if (u->obtener_propietario() == "J1" && !u->esta_activa() && u->obtener_vida() > 0) {
                u->activar();
            }
        }
    }
}
}
void procesar_eventos_turno(Contexto& ctx) {
// Procesar eventos programados
ctx.procesar_eventos();
// Aplicar rutinas de controladores si hay eventos activos
// (esto simula la reacción de los controladores a los eventos)
}
int main() {
Contexto ctx;
ctx.inicializar_escenario();
cout << "\nBienvenido a RIVAL FRONTIERS!" << endl;
pausar();

bool jugando = true;

while (jugando) {
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
            cout << "\n=== ESTADISTICAS ===" << endl;
            cout << "Turno: " << ctx.obtener_turno() << endl;
            cout << "Puntaje: " << ctx.obtener_puntaje() << endl;
            cout << "Dominio Jugador: " << ctx.calcular_dominio_jugador() << "%" << endl;
            cout << "Dominio Sistema: " << ctx.calcular_dominio_sistema() << "%" << endl;
            cout << "Moral: " << ctx.obtener_jugador().obtener_moral() << endl;
            cout << "Clima: " << ctx.obtener_clima() << endl;
            cout << "Recursos: " << ctx.obtener_jugador().obtener_recursos() << endl;
            cout << "Territorios controlados: " << ctx.obtener_jugador().contar_territorios() << endl;
            cout << "Mejoras desbloqueadas:" << endl;
            cout << "  - Ataque: " << (ctx.tiene_mejora_ataque() ? "SI" : "NO") << endl;
            cout << "  - Defensa: " << (ctx.tiene_mejora_defensa() ? "SI" : "NO") << endl;
            break;
        case 8:
            ctx.mostrar_bitacora();

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
            ctx.verificar_captura_neutrales();
            procesar_eventos_turno(ctx);

            ctrl_sistema.resolver_fase(ctx);

            ctx.incrementar_turno();
            ctx.reiniciar_puntos_accion();
            reactivar_unidades(ctx);

            ctx.verificar_mision();

            ctx.limpiar_bitacora();

            if (ctx.verificar_victoria()) {
                cout << "\nVICTORIA! Has alcanzado el dominio objetivo." << endl;
                cout << "Puntaje final: " << ctx.obtener_puntaje() << endl;
                jugando = false;
            }

            if (ctx.verificar_derrota()) {
                cout << "\nDERROTA! El sistema te ha superado." << endl;
                jugando = false;
            }
            break;
        case 9:
            if (ctx.guardar_partida("partida.txt")) {
                cout << "Partida guardada!" << endl;
            } else {
                cout << "Error al guardar la partida" << endl;
            }
            break;
        case 10:
            if (ctx.cargar_partida("partida.txt")) {
                cout << "Partida cargada!" << endl;
            } else {
                cout << "No hay partida guardada o error al cargar" << endl;
            }
            break;
        case 11:
            mostrar_ayuda();
            break;
        case 12:
            desbloquear_mejoras(ctx);
            break;
        case 13:
            construccion_rapida_ingeniero(ctx);
            break;
        case 14:
            cout << "Gracias por jugar!" << endl;
            jugando = false;
            break;
        default:
            cout << "Opcion invalida." << endl;
    }

    if (jugando && opcion >= 1 && opcion <= 5) {
        pausar();
    }

    if (jugando && (opcion == 12 || opcion == 13)) {
        pausar();
    }
}

if (!jugando) {
    ctx.generar_reporte_final("reporte_final.txt");
    cout << "\nReporte final guardado en 'reporte_final.txt'" << endl;
}

return 0;
}