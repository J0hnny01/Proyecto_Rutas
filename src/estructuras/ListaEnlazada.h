#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <stdexcept>

using namespace std;

template <typename T>
class ListaEnlazada {
public:
    struct NodoLista {
        T dato;
        NodoLista* siguiente;
    };

private:
    NodoLista* cabeza;
    NodoLista* cola;
    int cantidad;

    void copiarDesde(const ListaEnlazada<T>& otra);
    NodoLista* obtenerNodo(int indice) const;

public:
    ListaEnlazada();
    ListaEnlazada(const ListaEnlazada<T>& otra);
    ListaEnlazada<T>& operator=(const ListaEnlazada<T>& otra);
    ~ListaEnlazada();

    void insertarAlInicio(const T& dato);
    void insertarAlFinal(const T& dato);
    bool eliminarEn(int indice);
    bool eliminarPrimero();
    void limpiar();

    T& obtener(int indice);
    const T& obtener(int indice) const;
    NodoLista* obtenerCabeza();
    const NodoLista* obtenerCabeza() const;

    int tamano() const;
    bool estaVacia() const;
};

template <typename T>
void ListaEnlazada<T>::copiarDesde(const ListaEnlazada<T>& otra) {
    NodoLista* actual = otra.cabeza;
    while (actual != nullptr) {
        insertarAlFinal(actual->dato);
        actual = actual->siguiente;
    }
}

template <typename T>
typename ListaEnlazada<T>::NodoLista* ListaEnlazada<T>::obtenerNodo(int indice) const {
    if (indice < 0 || indice >= cantidad) {
        throw out_of_range("Indice fuera de rango");
    }
    NodoLista* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual;
}

template <typename T>
ListaEnlazada<T>::ListaEnlazada() : cabeza(nullptr), cola(nullptr), cantidad(0) {
}

template <typename T>
ListaEnlazada<T>::ListaEnlazada(const ListaEnlazada<T>& otra)
    : cabeza(nullptr), cola(nullptr), cantidad(0) {
    copiarDesde(otra);
}

template <typename T>
ListaEnlazada<T>& ListaEnlazada<T>::operator=(const ListaEnlazada<T>& otra) {
    if (this != &otra) {
        limpiar();
        copiarDesde(otra);
    }
    return *this;
}

template <typename T>
ListaEnlazada<T>::~ListaEnlazada() {
    limpiar();
}

template <typename T>
void ListaEnlazada<T>::insertarAlInicio(const T& dato) {
    NodoLista* nuevo = new NodoLista;
    nuevo->dato = dato;
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    if (cola == nullptr) {
        cola = nuevo;
    }
    cantidad++;
}

template <typename T>
void ListaEnlazada<T>::insertarAlFinal(const T& dato) {
    NodoLista* nuevo = new NodoLista;
    nuevo->dato = dato;
    nuevo->siguiente = nullptr;
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        cola = nuevo;
    }
    cantidad++;
}

template <typename T>
bool ListaEnlazada<T>::eliminarEn(int indice) {
    if (indice < 0 || indice >= cantidad) {
        return false;
    }
    if (indice == 0) {
        return eliminarPrimero();
    }
    NodoLista* anterior = obtenerNodo(indice - 1);
    NodoLista* borrar = anterior->siguiente;
    anterior->siguiente = borrar->siguiente;
    if (borrar == cola) {
        cola = anterior;
    }
    delete borrar;
    cantidad--;
    return true;
}

template <typename T>
bool ListaEnlazada<T>::eliminarPrimero() {
    if (cabeza == nullptr) {
        return false;
    }
    NodoLista* borrar = cabeza;
    cabeza = cabeza->siguiente;
    if (cabeza == nullptr) {
        cola = nullptr;
    }
    delete borrar;
    cantidad--;
    return true;
}

template <typename T>
void ListaEnlazada<T>::limpiar() {
    NodoLista* actual = cabeza;
    while (actual != nullptr) {
        NodoLista* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
    cola = nullptr;
    cantidad = 0;
}

template <typename T>
T& ListaEnlazada<T>::obtener(int indice) {
    return obtenerNodo(indice)->dato;
}

template <typename T>
const T& ListaEnlazada<T>::obtener(int indice) const {
    return obtenerNodo(indice)->dato;
}

template <typename T>
typename ListaEnlazada<T>::NodoLista* ListaEnlazada<T>::obtenerCabeza() {
    return cabeza;
}

template <typename T>
const typename ListaEnlazada<T>::NodoLista* ListaEnlazada<T>::obtenerCabeza() const {
    return cabeza;
}

template <typename T>
int ListaEnlazada<T>::tamano() const {
    return cantidad;
}

template <typename T>
bool ListaEnlazada<T>::estaVacia() const {
    return cantidad == 0;
}

#endif
