#include "Pila.h"

#include <stdexcept>

void Pila::copiarDesde(const Pila& otra) {
    NodoPila* actual = otra.tope;
    NodoPila* ultimo = nullptr;
    while (actual != nullptr) {
        NodoPila* nuevo = new NodoPila;
        nuevo->dato = actual->dato;
        nuevo->siguiente = nullptr;
        if (tope == nullptr) {
            tope = nuevo;
        } else {
            ultimo->siguiente = nuevo;
        }
        ultimo = nuevo;
        cantidad++;
        actual = actual->siguiente;
    }
}

Pila::Pila() : tope(nullptr), cantidad(0) {
}

Pila::Pila(const Pila& otra) : tope(nullptr), cantidad(0) {
    copiarDesde(otra);
}

Pila& Pila::operator=(const Pila& otra) {
    if (this != &otra) {
        limpiar();
        copiarDesde(otra);
    }
    return *this;
}

Pila::~Pila() {
    limpiar();
}

void Pila::apilar(int dato) {
    NodoPila* nuevo = new NodoPila;
    nuevo->dato = dato;
    nuevo->siguiente = tope;
    tope = nuevo;
    cantidad++;
}

bool Pila::desapilar() {
    if (tope == nullptr) {
        return false;
    }
    NodoPila* borrar = tope;
    tope = tope->siguiente;
    delete borrar;
    cantidad--;
    return true;
}

void Pila::limpiar() {
    while (tope != nullptr) {
        desapilar();
    }
}

int Pila::verTope() const {
    if (tope == nullptr) {
        throw std::out_of_range("Pila vacia");
    }
    return tope->dato;
}

int Pila::tamano() const {
    return cantidad;
}

bool Pila::estaVacia() const {
    return cantidad == 0;
}
