#ifndef PANEL_ADMIN_H
#define PANEL_ADMIN_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../sistema/SistemaAerolinea.h"
#include "BotonVisual.h"

using namespace std;

enum class ModoPanelAdmin {
    FlotaTripulacion,
    VuelosPasajeros
};

class PanelAdmin {
private:
    static const int FILAS_IZQUIERDA = 9;
    static const int FILAS_DERECHA = 7;

    ModoPanelAdmin modoActual;
    int paginaIzq;
    int paginaDer;

    BotonVisual btnIzqPagAnt;
    BotonVisual btnIzqPagSig;
    BotonVisual btnDerPagAnt;
    BotonVisual btnDerPagSig;

    string acortar(const string& texto, size_t maxChars) const;
    string aMinuscula(const string& texto) const;
    sf::Color colorPorEstado(const string& estado) const;
    void dibujarCaja(sf::RenderWindow& ventana, float x, float y, float w, float h,
                     sf::Color fondo, sf::Color borde) const;
    int totalPaginas(int totalItems, int porPagina) const;
    int clampPagina(int pagina, int totalItems, int porPagina) const;

    ListaEnlazada<Tripulante> recolectarTripulacionAeronave(const SistemaAerolinea& sistema,
                                                             const string& placa) const;
    int contarTripulantesSinPlacaValida(const SistemaAerolinea& sistema) const;
    int contarVuelosPorPlaca(const SistemaAerolinea& sistema, const string& placa) const;
    ListaEnlazada<Pasajero> recolectarPasajerosVuelo(const ListaEnlazada<Pasajero>& cachePasajeros,
                                                     const string& codigoVuelo) const;
    bool buscarAeronavePorPlaca(const SistemaAerolinea& sistema, const string& placa,
                                Aeronave& salida) const;
    string nombreCiudadPorCodigo(const SistemaAerolinea& sistema, const string& codigo) const;
    void contarEstadosFlota(const SistemaAerolinea& sistema, int& activas, int& enVuelo,
                            int& mantenimiento, int& otros) const;

    void dibujarPaginador(sf::RenderWindow& ventana, const sf::Font& fuente,
                          const BotonVisual& btnAnt, const BotonVisual& btnSig,
                          float centroPagX, float yTexto,
                          int paginaMostrada, int totPaginas) const;

    void renderizarFlota(sf::RenderWindow& ventana, const sf::Font& fuente,
                         const SistemaAerolinea& sistema, int idxAeronave) const;
    void renderizarVuelos(sf::RenderWindow& ventana, const sf::Font& fuente,
                          const SistemaAerolinea& sistema, int idxVuelo) const;

public:
    PanelAdmin();

    void setModo(ModoPanelAdmin nuevoModo);
    ModoPanelAdmin getModo() const;
    void resetPaginas();
    void resetPaginaDerecha();
    void asegurarSeleccionIzquierda(int indiceSeleccionado);

    int manejarClic(float mx, float my, const SistemaAerolinea& sistema,
                    int& idxAeronave, int& idxVuelo);
    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente,
                    const SistemaAerolinea& sistema,
                    int idxAeronave, int idxVuelo) const;
};

#endif
