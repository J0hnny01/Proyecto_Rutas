#ifndef VUELO_H
#define VUELO_H

#include <string>

using namespace std;

class Vuelo {
private:
    string codigoVuelo;
    string origenCodigo;
    string destinoCodigo;
    string placaAeronave;
    int costo;
    int tiempoMin;

public:
    Vuelo();
    Vuelo(const string& codigoVuelo,
          const string& origenCodigo,
          const string& destinoCodigo,
          const string& placaAeronave,
          int costo,
          int tiempoMin);

    string obtenerCodigoVuelo() const;
    string obtenerOrigenCodigo() const;
    string obtenerDestinoCodigo() const;
    string obtenerPlacaAeronave() const;
    int obtenerCosto() const;
    int obtenerTiempoMin() const;

    void establecerCodigoVuelo(const string& codigoVuelo);
    void establecerOrigenCodigo(const string& origenCodigo);
    void establecerDestinoCodigo(const string& destinoCodigo);
    void establecerPlacaAeronave(const string& placaAeronave);
    void establecerCosto(int costo);
    void establecerTiempoMin(int tiempoMin);
};

#endif
