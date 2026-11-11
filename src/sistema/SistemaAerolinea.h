#ifndef SISTEMA_AEROLINEA_H
#define SISTEMA_AEROLINEA_H

#include <string>

#include "../arboles/ArbolABB.h"
#include "../arboles/ArbolAVL.h"
#include "../estructuras/BellmanFord.h"
#include "../estructuras/Dijkstra.h"
#include "../estructuras/GrafoVuelos.h"
#include "../estructuras/ListaEnlazada.h"
#include "../estructuras/ResultadosGrafo.h"
#include "../modelos/Aeronave.h"
#include "../modelos/Ciudad.h"
#include "../modelos/Equipaje.h"
#include "../modelos/Pasajero.h"
#include "../modelos/Ruta.h"
#include "../modelos/Tripulante.h"
#include "../modelos/Vuelo.h"
#include "GestorArchivos.h"

using namespace std;

class SistemaAerolinea {
private:
    GrafoVuelos grafo;
    ArbolABB arbolCiudades;
    ArbolAVL arbolPasajeros;
    ListaEnlazada<Ciudad> ciudades;
    ListaEnlazada<Ruta> rutas;
    ListaEnlazada<Aeronave> aeronaves;
    ListaEnlazada<Tripulante> tripulantes;
    ListaEnlazada<Vuelo> vuelos;
    string directorioDatos;

    string resolverRutaArchivo(const string& nombreArchivo) const;
    void recolectarPasajerosRec(const NodoAVL* nodo, ListaEnlazada<Pasajero>& destino) const;

public:
    SistemaAerolinea(const string& carpetaDatos = "data");

    bool cargarDatos();
    bool guardarDatos() const;
    void reconstruirGrafo();

    ResultadoRuta calcularRutaMasRapida(const string& codigoOrigen, const string& codigoDestino) const;
    ResultadoRuta calcularRutaMasBarata(const string& codigoOrigen, const string& codigoDestino) const;
    Equipaje calcularRutaEquipaje(const string& idPasajero, int cantidadMaletas,
                                  const string& codigoOrigen, const string& codigoDestino) const;
    ResultadoBellmanFord explorarDestinos(const string& codigoOrigen, CriterioRuta criterio) const;

    bool sonRutasDistintas(const ResultadoRuta& rutaA, const ResultadoRuta& rutaB) const;
    string formatearCaminoIndices(const ListaEnlazada<int>& indices) const;
    string obtenerVuelosParaCamino(const ListaEnlazada<int>& indices) const;
    ListaEnlazada<Vuelo> buscarVuelosDirectos(const string& codigoOrigen, const string& codigoDestino) const;

    const NodoABB* buscarCiudadABB(const string& nombre) const;
    const NodoAVL* buscarPasajeroAVL(const string& id) const;
    bool asignarVueloAPasajero(const string& idPasajero, const string& codigoVuelo);
    ListaEnlazada<Pasajero> obtenerListaPasajeros() const;
    int contarRutasDesdeCiudad(const string& codigoCiudad) const;

    const GrafoVuelos& obtenerGrafo() const;
    GrafoVuelos& obtenerGrafoMutable();
    const ArbolABB& obtenerArbolCiudades() const;
    ArbolABB& obtenerArbolCiudadesMutable();
    const ArbolAVL& obtenerArbolPasajeros() const;
    ArbolAVL& obtenerArbolPasajerosMutable();

    const ListaEnlazada<Ciudad>& obtenerCiudades() const;
    ListaEnlazada<Ciudad>& obtenerCiudadesMutable();
    const ListaEnlazada<Ruta>& obtenerRutas() const;
    ListaEnlazada<Ruta>& obtenerRutasMutable();
    const ListaEnlazada<Aeronave>& obtenerAeronaves() const;
    ListaEnlazada<Aeronave>& obtenerAeronavesMutable();
    const ListaEnlazada<Tripulante>& obtenerTripulantes() const;
    ListaEnlazada<Tripulante>& obtenerTripulantesMutable();
    const ListaEnlazada<Vuelo>& obtenerVuelos() const;
    ListaEnlazada<Vuelo>& obtenerVuelosMutable();
    const string& obtenerDirectorioDatos() const;
};

#endif
