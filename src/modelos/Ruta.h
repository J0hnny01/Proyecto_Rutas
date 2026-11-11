#ifndef RUTA_H
#define RUTA_H

#include <string>

using namespace std;

class Ruta {
private:
    string origenCodigo;
    string destinoCodigo;
    int costo;
    int tiempoMin;
    bool activa;

public:
    Ruta();
    Ruta(const string& origenCodigo, const string& destinoCodigo, int costo, int tiempoMin, bool activa = true);

    string obtenerOrigenCodigo() const;
    string obtenerDestinoCodigo() const;
    int obtenerCosto() const;
    int obtenerTiempoMin() const;
    bool estaActiva() const;

    void establecerOrigenCodigo(const string& origenCodigo);
    void establecerDestinoCodigo(const string& destinoCodigo);
    void establecerCosto(int costo);
    void establecerTiempoMin(int tiempoMin);
    void establecerActiva(bool activa);
};

#endif
