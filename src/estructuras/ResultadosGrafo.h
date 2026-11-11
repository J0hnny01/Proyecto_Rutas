#ifndef RESULTADOS_GRAFO_H
#define RESULTADOS_GRAFO_H

#include "ListaEnlazada.h"

using namespace std;

enum CriterioRuta {
    CRITERIO_COSTO,
    CRITERIO_TIEMPO
};

bool esMejorCamino(int costoNuevo, int tiempoNuevo, int costoActual, int tiempoActual, CriterioRuta criterio);

struct ResultadoRuta {
    bool encontrada;
    int costoTotal;
    int duracionTotal;
    int cantidadEscalas;
    ListaEnlazada<int> caminoIndices;

    ResultadoRuta();
};

struct ResultadoBellmanFord {
private:
    void copiarDesde(const ResultadoBellmanFord& otro);

public:
    int* costos;
    int* tiempos;
    int* previo;
    bool* alcanzable;
    int cantidadVertices;
    bool hayCicloNegativo;

    ResultadoBellmanFord();
    ResultadoBellmanFord(const ResultadoBellmanFord& otro);
    ResultadoBellmanFord& operator=(const ResultadoBellmanFord& otro);
    ~ResultadoBellmanFord();

    void inicializar(int cantidad);
    void liberar();
};

struct AristaMST {
    int origenIdx;
    int destinoIdx;
    int peso;

    AristaMST();
    AristaMST(int origenIdx, int destinoIdx, int peso);
};

struct ResultadoMST {
    ListaEnlazada<AristaMST> aristas;
    int pesoTotal;
    bool esConexo;

    ResultadoMST();
};

#endif
