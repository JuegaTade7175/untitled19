//
// Created by tadeo on 23/11/2025.
//

#ifndef UNTITLED19_TERRENO_H
#define UNTITLED19_TERRENO_H
#include <string>

class Unidad;

class Terreno {
protected:
    std::string codigo;

public:
    Terreno(const std::string& cod) : codigo(cod) {}
    virtual ~Terreno() {}

    virtual int costo_movimiento(const Unidad& unidad) const = 0;
    virtual int bono_defensa(const Unidad& unidad) const = 0;

    std::string obtener_codigo() const { return codigo; }
};

class Llanura : public Terreno {
public:
    Llanura() : Terreno("LL") {}
    int costo_movimiento(const Unidad& unidad) const override { return 1; }
    int bono_defensa(const Unidad& unidad) const override { return 0; }
};

class Bosque : public Terreno {
public:
    Bosque() : Terreno("BO") {}
    int costo_movimiento(const Unidad& unidad) const override { return 2; }
    int bono_defensa(const Unidad& unidad) const override { return 1; }
};

class Montana : public Terreno {
public:
    Montana() : Terreno("MO") {}
    int costo_movimiento(const Unidad& unidad) const override { return 3; }
    int bono_defensa(const Unidad& unidad) const override { return 2; }
};

class Agua : public Terreno {
public:
    Agua() : Terreno("AG") {}
    int costo_movimiento(const Unidad& unidad) const override { return 99; }
    int bono_defensa(const Unidad& unidad) const override { return 0; }
};

class Pantano : public Terreno {
public:
    Pantano() : Terreno("PA") {}
    int costo_movimiento(const Unidad& unidad) const override { return 2; }
    int bono_defensa(const Unidad& unidad) const override { return -1; }
};
#endif //UNTITLED19_TERRENO_H