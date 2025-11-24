//
// Created by tadeo on 23/11/2025.
//

#include "Evento.h"
#include "Contexto.h"
#include "Unidad.h"
#include <iostream>

void EventoRefuerzo::ejecutar(Contexto& ctx) {
    ctx.agregar_log("Evento de refuerzo activado: " +
                    std::to_string(cantidad) + " " + tipo_unidad +
                    " para " + faccion);

    Mapa& mapa = ctx.obtener_mapa();
    int colocados = 0;

    for (int f = 0; f < mapa.obtener_filas() && colocados < cantidad; f++) {
        for (int c = 0; c < mapa.obtener_columnas() && colocados < cantidad; c++) {
            Coordenada pos(f, c);
            Celda& celda = mapa.obtener_celda(pos);

            if (celda.esta_libre()) {
                std::shared_ptr<Unidad> nueva_unidad;

                if (tipo_unidad == "S") {
                    nueva_unidad = std::make_shared<Soldado>(faccion);
                } else if (tipo_unidad == "A") {
                    nueva_unidad = std::make_shared<Arquero>(faccion);
                }

                if (nueva_unidad) {
                    celda.colocar_unidad(nueva_unidad);
                    colocados++;
                }
            }
        }
    }

    std::cout << "Refuerzos desplegados: " << colocados << " unidades" << std::endl;
}

void EventoClima::ejecutar(Contexto& ctx) {
    ctx.agregar_log("Evento climático: " + efecto);

    if (efecto == "Tormenta") {
        ctx.obtener_jugador().modificar_moral(-10);
        ctx.obtener_sistema().modificar_moral(-10);
        std::cout << "¡Tormenta! Moral reducida para ambos bandos" << std::endl;
    } else if (efecto == "Clima despejado") {
        ctx.obtener_jugador().modificar_moral(5);
        std::cout << "¡Clima favorable! Moral aumentada" << std::endl;
    }
}