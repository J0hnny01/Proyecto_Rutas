#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <string>

#include "GrafoVuelos.h"
#include "ListaEnlazada.h"
#include "ResultadosGrafo.h"

using namespace std;

class BellmanFord {
public:
    static ResultadoBellmanFord calcularDesdeOrigen(const GrafoVuelos& grafo,
                                                    int indiceOrigen,
                                                    CriterioRuta criterio);
    static ResultadoBellmanFord calcularDesdeOrigen(const GrafoVuelos& grafo,
                                                    const string& codigoOrigen,
                                                    CriterioRuta criterio);
    static ListaEnlazada<int> reconstruirCamino(const ResultadoBellmanFord& resultado,
                                                int indiceDestino);
};

#endif
