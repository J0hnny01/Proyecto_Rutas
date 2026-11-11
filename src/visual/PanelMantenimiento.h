#ifndef PANEL_MANTENIMIENTO_H
#define PANEL_MANTENIMIENTO_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../sistema/SistemaAerolinea.h"
#include "BotonVisual.h"

using namespace std;

class PanelMantenimiento {
private:
    static const int FILAS_POR_PAGINA = 10;

    int entidadActiva;
    int indiceSeleccionado;
    int paginaActual;

    BotonVisual btnPagAnt;
    BotonVisual btnPagSig;

    string acortar(const string& texto, size_t maxChars) const;
    int totalRegistrosPorEntidad(const SistemaAerolinea& sistema, int entidad,
                                 const ListaEnlazada<Pasajero>& cachePasajeros) const;
    string estadoAeronavePorPlaca(const SistemaAerolinea& sistema, const string& placa) const;
    string resumenDisponibles(const SistemaAerolinea& sistema, int entidad) const;
    void dibujarEncabezados(sf::RenderWindow& ventana, const sf::Font& fuente) const;
    void dibujarFila(sf::RenderWindow& ventana, const sf::Font& fuente,
                     const SistemaAerolinea& sistema, int indiceGlobal,
                     float yFila, bool seleccionada,
                     const ListaEnlazada<Pasajero>& cachePasajeros) const;

public:
    PanelMantenimiento();

    void setEntidad(int nuevaEntidad, const SistemaAerolinea& sistema);
    void ajustarSeleccion(int nuevoIndice, const SistemaAerolinea& sistema);

    int getEntidad() const;
    int getIndiceSeleccionado() const;
    int getPaginaActual() const;
    int getTotalPaginas(const SistemaAerolinea& sistema) const;
    int getTotalRegistros(const SistemaAerolinea& sistema) const;

    string getNombreSingular(int entidad) const;
    string getNombrePlural(int entidad) const;
    string getArchivoDatos(int entidad) const;
    string getDescripcionMemoria(int entidad) const;
    string getResumenCiudadesDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const;
    string getResumenPlacasDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const;
    string getResumenVuelosDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const;

    bool manejarClic(float mx, float my, const SistemaAerolinea& sistema, bool& cambioSeleccion);
    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente,
                    const SistemaAerolinea& sistema) const;
};

#endif
