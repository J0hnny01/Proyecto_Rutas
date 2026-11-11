#ifndef GRAFO_VUELOS_H
#define GRAFO_VUELOS_H

#include <string>

#include "../modelos/Ciudad.h"

using namespace std;

struct NodoArista {
    int destino;
    int costo;
    int tiempoMin;
    NodoArista* siguiente;
};

struct VerticeCiudad {
    Ciudad ciudad;
    NodoArista* adyacentes;
};

class GrafoVuelos {
private:
    VerticeCiudad* vertices;
    int numVertices;
    int capacidad;

    void redimensionar();
    void liberarAdyacentes(int indiceVertice);
    void liberarTodo();
    void copiarDesde(const GrafoVuelos& otro);

public:
    GrafoVuelos();
    GrafoVuelos(const GrafoVuelos& otro);
    GrafoVuelos& operator=(const GrafoVuelos& otro);
    ~GrafoVuelos();

    bool agregarCiudad(const Ciudad& ciudad);
    bool eliminarCiudad(const string& codigo);
    int buscarIndicePorCodigo(const string& codigo) const;
    int buscarIndicePorNombre(const string& nombre) const;
    bool existeCiudad(const string& codigo) const;

    bool agregarRutaDirigida(const string& origenCodigo, const string& destinoCodigo, int costo, int tiempoMin);
    bool eliminarRuta(const string& origenCodigo, const string& destinoCodigo);
    bool modificarRuta(const string& origenCodigo, const string& destinoCodigo, int nuevoCosto, int nuevoTiempoMin);
    bool existeRuta(const string& origenCodigo, const string& destinoCodigo) const;

    int obtenerCantidadVertices() const;
    const Ciudad& obtenerCiudad(int indice) const;
    const NodoArista* obtenerAdyacentes(int indice) const;
    void limpiar();
};

#endif
