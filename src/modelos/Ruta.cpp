#include "Ruta.h"

Ruta::Ruta() : origenCodigo(""), destinoCodigo(""), costo(0), tiempoMin(0), activa(true) {
}

Ruta::Ruta(const string& origenCodigo, const string& destinoCodigo, int costo, int tiempoMin, bool activa)
    : origenCodigo(origenCodigo), destinoCodigo(destinoCodigo), costo(costo), tiempoMin(tiempoMin), activa(activa) {
}

string Ruta::obtenerOrigenCodigo() const {
    return origenCodigo;
}

string Ruta::obtenerDestinoCodigo() const {
    return destinoCodigo;
}

int Ruta::obtenerCosto() const {
    return costo;
}

int Ruta::obtenerTiempoMin() const {
    return tiempoMin;
}

bool Ruta::estaActiva() const {
    return activa;
}

void Ruta::establecerOrigenCodigo(const string& origenCodigo) {
    this->origenCodigo = origenCodigo;
}

void Ruta::establecerDestinoCodigo(const string& destinoCodigo) {
    this->destinoCodigo = destinoCodigo;
}

void Ruta::establecerCosto(int costo) {
    this->costo = costo;
}

void Ruta::establecerTiempoMin(int tiempoMin) {
    this->tiempoMin = tiempoMin;
}

void Ruta::establecerActiva(bool activa) {
    this->activa = activa;
}
