//
// Created by tadeo on 23/11/2025.
//

#include "Edificio.h"
#include "Faccion.h"

void Granja::efecto_turno(Faccion& faccion) {
    faccion.agregar_recursos(Recursos(2, 0, 0));
}

void Granja::reaccion_evento(const Evento& evento) {
}

void Cuartel::efecto_turno(Faccion& faccion) {
    faccion.agregar_recursos(Recursos(0, 1, 0));
}

void Cuartel::reaccion_evento(const Evento& evento) {
}

void Torre::efecto_turno(Faccion& faccion) {
    faccion.agregar_recursos(Recursos(0, 0, 1));
}

void Torre::reaccion_evento(const Evento& evento) {
}

void Forja::efecto_turno(Faccion& faccion) {
    faccion.agregar_recursos(Recursos(0, 2, 0));
}

void Forja::reaccion_evento(const Evento& evento) {
}