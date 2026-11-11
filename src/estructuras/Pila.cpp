#include "Pila.h"

#include <stdexcept>

using namespace std;

void Pila::copiarDesde(const Pila& otra) {
    NodoPila* actual = otra.cima;
    NodoPila* ultimo = nullptr;
    while (actual != nullptr) {
        NodoPila* nuevo = new NodoPila;
        nuevo->dato = actual->dato;
        nuevo->siguiente = nullptr;
        if (cima == nullptr) {
            cima = nuevo;
        } else {
            ultimo->siguiente = nuevo;
        }
        ultimo = nuevo;
        cantidad++;
        actual = actual->siguiente;
    }
}

Pila::Pila() : cima(nullptr), cantidad(0) {
}

Pila::Pila(const Pila& otra) : cima(nullptr), cantidad(0) {
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
    nuevo->siguiente = cima;
    cima = nuevo;
    cantidad++;
}

bool Pila::desapilar() {
    if (cima == nullptr) {
        return false;
    }
    NodoPila* borrar = cima;
    cima = cima->siguiente;
    delete borrar;
    cantidad--;
    return true;
}

void Pila::limpiar() {
    while (cima != nullptr) {
        desapilar();
    }
}

int Pila::verTope() const {
    if (cima == nullptr) {
        throw out_of_range("Pila vacia");
    }
    return cima->dato;
}

int Pila::tamano() const {
    return cantidad;
}

bool Pila::estaVacia() const {
    return cantidad == 0;
}
