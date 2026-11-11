#ifndef COLA_PRIORIDAD_H
#define COLA_PRIORIDAD_H

struct ElementoPrioridad {
    int vertice;
    int prioridad;
};

class ColaPrioridad {
private:
    struct NodoPrioridad {
        ElementoPrioridad dato;
        NodoPrioridad* siguiente;
        NodoPrioridad(int v, int p) {
            dato.vertice = v;
            dato.prioridad = p;
            siguiente = nullptr;
        }
    };

    NodoPrioridad* frente;
    int cantidad;

    void copiarDesde(const ColaPrioridad& otra);

public:
    ColaPrioridad();
    ColaPrioridad(const ColaPrioridad& otra);
    ColaPrioridad& operator=(const ColaPrioridad& otra);
    ~ColaPrioridad();

    void insertar(int vertice, int prioridad);
    ElementoPrioridad extraerMinimo();
    const ElementoPrioridad& verMinimo() const;
    bool estaVacia() const;
    int tamano() const;
    void limpiar();
};

#endif
