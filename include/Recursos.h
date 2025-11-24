//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_RECURSOS_H
#define UNTITLED19_RECURSOS_H
#include <iostream>

class Recursos {
public:
    int comida;
    int metal;
    int energia;

    Recursos(int c = 0, int m = 0, int e = 0)
        : comida(c), metal(m), energia(e) {}

    bool tiene_suficiente(const Recursos& costo) const {
        return comida >= costo.comida &&
               metal >= costo.metal &&
               energia >= costo.energia;
    }

    Recursos& operator+=(const Recursos& otros) {
        comida += otros.comida;
        metal += otros.metal;
        energia += otros.energia;
        return *this;
    }

    Recursos& operator-=(const Recursos& otros) {
        comida -= otros.comida;
        metal -= otros.metal;
        energia -= otros.energia;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Recursos& r) {
        os << "Comida=" << r.comida << " | Metal=" << r.metal
           << " | Energía=" << r.energia;
        return os;
    }
};
#endif //UNTITLED19_RECURSOS_H