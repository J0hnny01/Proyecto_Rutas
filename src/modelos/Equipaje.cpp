#include "Equipaje.h"

Equipaje::Equipaje()
    : idPasajero(""), cantidadMaletas(0), rutaMasBarata(), costoEnvio(0), tiempoEnvio(0) {
}

Equipaje::Equipaje(const string& idPasajero, int cantidadMaletas, int costoEnvio, int tiempoEnvio)
    : idPasajero(idPasajero), cantidadMaletas(cantidadMaletas), rutaMasBarata(), costoEnvio(costoEnvio), tiempoEnvio(tiempoEnvio) {
}

string Equipaje::obtenerIdPasajero() const {
    return idPasajero;
}

int Equipaje::obtenerCantidadMaletas() const {
    return cantidadMaletas;
}

const ListaEnlazada<string>& Equipaje::obtenerRutaMasBarata() const {
    return rutaMasBarata;
}

int Equipaje::obtenerCostoEnvio() const {
    return costoEnvio;
}

int Equipaje::obtenerTiempoEnvio() const {
    return tiempoEnvio;
}

string Equipaje::obtenerRutaTexto() const {
    string texto = "";
    const ListaEnlazada<string>::NodoLista* actual = rutaMasBarata.obtenerCabeza();
    while (actual != nullptr) {
        if (!texto.empty()) {
            texto += " -> ";
        }
        texto += actual->dato;
        actual = actual->siguiente;
    }
    return texto;
}

void Equipaje::establecerIdPasajero(const string& idPasajero) {
    this->idPasajero = idPasajero;
}

void Equipaje::establecerCantidadMaletas(int cantidadMaletas) {
    this->cantidadMaletas = cantidadMaletas;
}

void Equipaje::establecerRutaMasBarata(const ListaEnlazada<string>& ruta) {
    this->rutaMasBarata = ruta;
}

void Equipaje::establecerCostoEnvio(int costoEnvio) {
    this->costoEnvio = costoEnvio;
}

void Equipaje::establecerTiempoEnvio(int tiempoEnvio) {
    this->tiempoEnvio = tiempoEnvio;
}

void Equipaje::agregarCiudadRuta(const string& codigoCiudad) {
    rutaMasBarata.insertarAlFinal(codigoCiudad);
}

void Equipaje::limpiarRuta() {
    rutaMasBarata.limpiar();
}
