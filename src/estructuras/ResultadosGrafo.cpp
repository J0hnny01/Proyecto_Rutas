#include "ResultadosGrafo.h"

ResultadoRuta::ResultadoRuta()
    : encontrada(false), costoTotal(0), duracionTotal(0), cantidadEscalas(0), caminoIndices() {
}

void ResultadoBellmanFord::copiarDesde(const ResultadoBellmanFord& otro) {
    if (otro.cantidadVertices <= 0) {
        return;
    }
    inicializar(otro.cantidadVertices);
    hayCicloNegativo = otro.hayCicloNegativo;
    for (int i = 0; i < cantidadVertices; i++) {
        costos[i] = otro.costos[i];
        tiempos[i] = otro.tiempos[i];
        previo[i] = otro.previo[i];
        alcanzable[i] = otro.alcanzable[i];
    }
}

ResultadoBellmanFord::ResultadoBellmanFord()
    : costos(nullptr),
      tiempos(nullptr),
      previo(nullptr),
      alcanzable(nullptr),
      cantidadVertices(0),
      hayCicloNegativo(false) {
}

ResultadoBellmanFord::ResultadoBellmanFord(const ResultadoBellmanFord& otro)
    : costos(nullptr),
      tiempos(nullptr),
      previo(nullptr),
      alcanzable(nullptr),
      cantidadVertices(0),
      hayCicloNegativo(false) {
    copiarDesde(otro);
}

ResultadoBellmanFord& ResultadoBellmanFord::operator=(const ResultadoBellmanFord& otro) {
    if (this != &otro) {
        liberar();
        copiarDesde(otro);
    }
    return *this;
}

ResultadoBellmanFord::~ResultadoBellmanFord() {
    liberar();
}

void ResultadoBellmanFord::inicializar(int cantidad) {
    liberar();
    if (cantidad <= 0) {
        return;
    }
    cantidadVertices = cantidad;
    hayCicloNegativo = false;
    costos = new int[cantidad];
    tiempos = new int[cantidad];
    previo = new int[cantidad];
    alcanzable = new bool[cantidad];
    for (int i = 0; i < cantidad; i++) {
        costos[i] = 0;
        tiempos[i] = 0;
        previo[i] = -1;
        alcanzable[i] = false;
    }
}

void ResultadoBellmanFord::liberar() {
    delete[] costos;
    delete[] tiempos;
    delete[] previo;
    delete[] alcanzable;
    costos = nullptr;
    tiempos = nullptr;
    previo = nullptr;
    alcanzable = nullptr;
    cantidadVertices = 0;
    hayCicloNegativo = false;
}

AristaMST::AristaMST() : origenIdx(0), destinoIdx(0), peso(0) {
}

AristaMST::AristaMST(int origenIdx, int destinoIdx, int peso)
    : origenIdx(origenIdx), destinoIdx(destinoIdx), peso(peso) {
}

ResultadoMST::ResultadoMST() : aristas(), pesoTotal(0), esConexo(false) {
}
