#ifndef PILA_H
#define PILA_H

class Pila {
private:
    struct NodoPila {
        int dato;
        NodoPila* siguiente;
    };

    NodoPila* cima;
    int cantidad;

    void copiarDesde(const Pila& otra);

public:
    Pila();
    Pila(const Pila& otra);
    Pila& operator=(const Pila& otra);
    ~Pila();

    void apilar(int dato);
    bool desapilar();
    void limpiar();

    int verTope() const;
    int tamano() const;
    bool estaVacia() const;
};

#endif
