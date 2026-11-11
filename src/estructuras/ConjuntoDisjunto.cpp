#include "ConjuntoDisjunto.h"

ConjuntoDisjunto::ConjuntoDisjunto() : representante(nullptr), tamano(0) {
}

ConjuntoDisjunto::ConjuntoDisjunto(int n) : representante(nullptr), tamano(0) {
    reiniciar(n);
}

ConjuntoDisjunto::ConjuntoDisjunto(const ConjuntoDisjunto& otra) : representante(nullptr), tamano(0) {
    copiarDesde(otra);
}

ConjuntoDisjunto& ConjuntoDisjunto::operator=(const ConjuntoDisjunto& otra) {
    if (this == &otra) {
        return *this;
    }
    delete[] representante;
    representante = nullptr;
    tamano = 0;
    copiarDesde(otra);
    return *this;
}

ConjuntoDisjunto::~ConjuntoDisjunto() {
    delete[] representante;
}

void ConjuntoDisjunto::copiarDesde(const ConjuntoDisjunto& otra) {
    tamano = otra.tamano;
    if (tamano <= 0) {
        representante = nullptr;
        tamano = 0;
        return;
    }
    representante = new int[tamano];
    for (int i = 0; i < tamano; i++) {
        representante[i] = otra.representante[i];
    }
}

void ConjuntoDisjunto::reiniciar(int n) {
    delete[] representante;
    representante = nullptr;
    tamano = 0;
    if (n <= 0) {
        return;
    }
    tamano = n;
    representante = new int[tamano];
    for (int i = 0; i < tamano; i++) {
        representante[i] = i;
    }
}

int ConjuntoDisjunto::encontrar(int x) const {
    if (x < 0 || x >= tamano) {
        return -1;
    }
    while (representante[x] != x) {
        x = representante[x];
    }
    return x;
}

bool ConjuntoDisjunto::unir(int a, int b) {
    int ra = encontrar(a);
    int rb = encontrar(b);
    if (ra == -1 || rb == -1 || ra == rb) {
        return false;
    }
    representante[ra] = rb;
    return true;
}

bool ConjuntoDisjunto::mismoConjunto(int a, int b) const {
    int ra = encontrar(a);
    int rb = encontrar(b);
    if (ra == -1 || rb == -1) {
        return false;
    }
    return ra == rb;
}

int ConjuntoDisjunto::obtenerTamano() const {
    return tamano;
}
