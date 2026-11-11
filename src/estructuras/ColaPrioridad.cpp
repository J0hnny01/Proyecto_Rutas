#include "ColaPrioridad.h"

ColaPrioridad::ColaPrioridad() : frente(nullptr), cantidad(0) {
}

ColaPrioridad::ColaPrioridad(const ColaPrioridad& otra) : frente(nullptr), cantidad(0) {
    copiarDesde(otra);
}

ColaPrioridad& ColaPrioridad::operator=(const ColaPrioridad& otra) {
    if (this == &otra) {
        return *this;
    }
    limpiar();
    copiarDesde(otra);
    return *this;
}

ColaPrioridad::~ColaPrioridad() {
    limpiar();
}

void ColaPrioridad::copiarDesde(const ColaPrioridad& otra) {
    NodoPrioridad* actual = otra.frente;
    NodoPrioridad* ultimo = nullptr;
    while (actual != nullptr) {
        NodoPrioridad* nuevo = new NodoPrioridad(actual->dato.vertice, actual->dato.prioridad);
        if (frente == nullptr) {
            frente = nuevo;
        } else {
            ultimo->siguiente = nuevo;
        }
        ultimo = nuevo;
        cantidad++;
        actual = actual->siguiente;
    }
}

void ColaPrioridad::insertar(int vertice, int prioridad) {
    NodoPrioridad* nuevo = new NodoPrioridad(vertice, prioridad);
    if (frente == nullptr || prioridad < frente->dato.prioridad) {
        nuevo->siguiente = frente;
        frente = nuevo;
        cantidad++;
        return;
    }
    NodoPrioridad* actual = frente;
    while (actual->siguiente != nullptr && actual->siguiente->dato.prioridad <= prioridad) {
        actual = actual->siguiente;
    }
    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    cantidad++;
}

ElementoPrioridad ColaPrioridad::extraerMinimo() {
    NodoPrioridad* borrar = frente;
    ElementoPrioridad minimo = borrar->dato;
    frente = frente->siguiente;
    delete borrar;
    cantidad--;
    return minimo;
}

const ElementoPrioridad& ColaPrioridad::verMinimo() const {
    return frente->dato;
}

bool ColaPrioridad::estaVacia() const {
    return frente == nullptr;
}

int ColaPrioridad::tamano() const {
    return cantidad;
}

void ColaPrioridad::limpiar() {
    while (frente != nullptr) {
        NodoPrioridad* borrar = frente;
        frente = frente->siguiente;
        delete borrar;
    }
    cantidad = 0;
}
