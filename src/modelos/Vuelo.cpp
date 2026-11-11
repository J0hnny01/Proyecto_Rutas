#include "Vuelo.h"

Vuelo::Vuelo()
    : codigoVuelo(""), origenCodigo(""), destinoCodigo(""), placaAeronave(""), costo(0), tiempoMin(0) {
}

Vuelo::Vuelo(const string& codigoVuelo,
             const string& origenCodigo,
             const string& destinoCodigo,
             const string& placaAeronave,
             int costo,
             int tiempoMin)
    : codigoVuelo(codigoVuelo),
      origenCodigo(origenCodigo),
      destinoCodigo(destinoCodigo),
      placaAeronave(placaAeronave),
      costo(costo),
      tiempoMin(tiempoMin) {
}

string Vuelo::obtenerCodigoVuelo() const {
    return codigoVuelo;
}

string Vuelo::obtenerOrigenCodigo() const {
    return origenCodigo;
}

string Vuelo::obtenerDestinoCodigo() const {
    return destinoCodigo;
}

string Vuelo::obtenerPlacaAeronave() const {
    return placaAeronave;
}

int Vuelo::obtenerCosto() const {
    return costo;
}

int Vuelo::obtenerTiempoMin() const {
    return tiempoMin;
}

void Vuelo::establecerCodigoVuelo(const string& codigoVuelo) {
    this->codigoVuelo = codigoVuelo;
}

void Vuelo::establecerOrigenCodigo(const string& origenCodigo) {
    this->origenCodigo = origenCodigo;
}

void Vuelo::establecerDestinoCodigo(const string& destinoCodigo) {
    this->destinoCodigo = destinoCodigo;
}

void Vuelo::establecerPlacaAeronave(const string& placaAeronave) {
    this->placaAeronave = placaAeronave;
}

void Vuelo::establecerCosto(int costo) {
    this->costo = costo;
}

void Vuelo::establecerTiempoMin(int tiempoMin) {
    this->tiempoMin = tiempoMin;
}
