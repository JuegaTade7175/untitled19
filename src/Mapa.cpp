//
// Created by tadeo on 23/11/2025.
//

#include "Mapa.h"
#include "Terreno.h"
#include <iomanip>
#include <memory>

using namespace std;

Mapa::Mapa(int f, int c) : filas(f), columnas(c) {
    for (int i = 0; i < filas; i++) {
        vector<Celda> fila_celdas;
        for (int j = 0; j < columnas; j++) {
            shared_ptr<Terreno> terreno;

            if ((i == 1 && j == 0) || (i == 3 && j == 5)) {
                terreno = make_shared<Pantano>();
            } else if ((i == 0 && j == 1) || (i == 2 && j == 4) || (i == 4 && j == 4)) {
                terreno = make_shared<Bosque>();
            } else if (i == 2 && j == 0) {
                terreno = make_shared<Montana>();
            } else if ((i == 0 && j == 3) || (i == 2 && j == 5) || (i == 5 && j == 0)) {
                terreno = make_shared<Agua>();
            } else {
                terreno = make_shared<Llanura>();
            }

            fila_celdas.push_back(Celda(terreno));
        }
        celdas.push_back(fila_celdas);
    }
}

void Mapa::mostrar() const {
    cout << "\n=== MAPA DEL JUEGO ===" << endl;
    cout << "   ";
    for (int j = 0; j < columnas; j++) {
        cout << setw(3) << j << "        ";
    }
    cout << endl;

    for (int i = 0; i < filas; i++) {
        cout << setw(2) << i << " ";
        for (int j = 0; j < columnas; j++) {
            cout << "[" << setw(8) << left << celdas[i][j].obtener_representacion() << "]";
        }
        cout << endl;
    }
    cout << endl;
}