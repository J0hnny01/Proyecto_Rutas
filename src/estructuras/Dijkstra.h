#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>

#include "../modelos/Equipaje.h"
#include "GrafoVuelos.h"
#include "ResultadosGrafo.h"

using namespace std;

class Dijkstra {
public:
    static ResultadoRuta calcularRutaOptima(const GrafoVuelos& grafo,
                                            int indiceOrigen,
                                            int indiceDestino,
                                            CriterioRuta criterio);
    static ResultadoRuta calcularRutaOptima(const GrafoVuelos& grafo,
                                            const string& codigoOrigen,
                                            const string& codigoDestino,
                                            CriterioRuta criterio);
    static ResultadoRuta calcularRutaMasBarata(const GrafoVuelos& grafo,
                                               const string& codigoOrigen,
                                               const string& codigoDestino);
    static ResultadoRuta calcularRutaMasRapida(const GrafoVuelos& grafo,
                                               const string& codigoOrigen,
                                               const string& codigoDestino);
    static Equipaje calcularRutaEquipaje(const GrafoVuelos& grafo,
                                         const string& idPasajero,
                                         int cantidadMaletas,
                                         const string& codigoOrigen,
                                         const string& codigoDestino);
};

#endif
