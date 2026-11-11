#ifndef CIUDAD_H
#define CIUDAD_H

#include <string>

using namespace std;

class Ciudad {
private:
    string codigo;
    string nombre;
    float x;
    float y;

public:
    Ciudad();
    Ciudad(const string& codigo, const string& nombre, float x, float y);

    string obtenerCodigo() const;
    string obtenerNombre() const;
    float obtenerX() const;
    float obtenerY() const;

    void establecerCodigo(const string& codigo);
    void establecerNombre(const string& nombre);
    void establecerPosicion(float x, float y);
    void establecerX(float x);
    void establecerY(float y);
};

#endif
