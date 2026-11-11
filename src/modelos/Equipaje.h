#ifndef EQUIPAJE_H
#define EQUIPAJE_H

#include <string>

#include "../estructuras/ListaEnlazada.h"

using namespace std;

class Equipaje {
private:
    string idPasajero;
    int cantidadMaletas;
    ListaEnlazada<string> rutaMasBarata;
    int costoEnvio;
    int tiempoEnvio;

public:
    Equipaje();
    Equipaje(const string& idPasajero, int cantidadMaletas, int costoEnvio = 0, int tiempoEnvio = 0);

    string obtenerIdPasajero() const;
    int obtenerCantidadMaletas() const;
    const ListaEnlazada<string>& obtenerRutaMasBarata() const;
    int obtenerCostoEnvio() const;
    int obtenerTiempoEnvio() const;
    string obtenerRutaTexto() const;

    void establecerIdPasajero(const string& idPasajero);
    void establecerCantidadMaletas(int cantidadMaletas);
    void establecerRutaMasBarata(const ListaEnlazada<string>& ruta);
    void establecerCostoEnvio(int costoEnvio);
    void establecerTiempoEnvio(int tiempoEnvio);

    void agregarCiudadRuta(const string& codigoCiudad);
    void limpiarRuta();
};

#endif
