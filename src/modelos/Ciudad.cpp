#include "Ciudad.h"

Ciudad::Ciudad() : codigo(""), nombre(""), x(0.0f), y(0.0f) {
}

Ciudad::Ciudad(const string& codigo, const string& nombre, float x, float y)
    : codigo(codigo), nombre(nombre), x(x), y(y) {
}

string Ciudad::obtenerCodigo() const {
    return codigo;
}

string Ciudad::obtenerNombre() const {
    return nombre;
}

float Ciudad::obtenerX() const {
    return x;
}

float Ciudad::obtenerY() const {
    return y;
}

void Ciudad::establecerCodigo(const string& codigo) {
    this->codigo = codigo;
}

void Ciudad::establecerNombre(const string& nombre) {
    this->nombre = nombre;
}

void Ciudad::establecerPosicion(float x, float y) {
    this->x = x;
    this->y = y;
}

void Ciudad::establecerX(float x) {
    this->x = x;
}

void Ciudad::establecerY(float y) {
    this->y = y;
}
