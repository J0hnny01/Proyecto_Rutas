#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

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

#endif
