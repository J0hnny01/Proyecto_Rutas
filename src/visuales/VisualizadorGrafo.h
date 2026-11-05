#ifndef VISUALIZADOR_GRAFO_H
#define VISUALIZADOR_GRAFO_H

#include <SFML/Graphics.hpp>
#include <string>

struct CiudadVisual {
    std::string codigo;
    std::string nombre;
    float x;
    float y;
};

struct AristaVisual {
    int origen;
    int destino;
    int costo;
    int tiempo;
    bool esRutaPasajero;
    bool esRutaMaleta;
    bool esAristaMST;
};

class VisualizadorGrafo {
private:
    static const int MAX_CIUDADES = 30;
    static const int MAX_ARISTAS = 120;

    CiudadVisual ciudades[MAX_CIUDADES];
    int totalCiudades;

    AristaVisual aristas[MAX_ARISTAS];
    int totalAristas;

    int ciudadOrigenIdx;
    int ciudadDestinoIdx;

    int rutaAnimada[MAX_CIUDADES];
    int longitudRutaAnimada;
    int tramoActual;
    float progresoTramo;
    bool animacionActiva;

    int rutaMaletaAnimada[MAX_CIUDADES];
    int longitudRutaMaleta;
    int tramoMaletaActual;
    float progresoMaleta;
    bool animacionMaletaActiva;

public:
    VisualizadorGrafo();

    void limpiar();
    int agregarCiudad(const std::string& codigo, const std::string& nombre, float x, float y);
    void agregarArista(int origen, int destino, int costo, int tiempo);

    void limpiarResaltados();
    void iniciarAnimacionRuta(const int indicesCiudades[], int cantidad, bool esMaleta = false);
    void resaltarAristaMST(int origen, int destino);
    void actualizarAnimacion(float deltaTiempo);

    int detectarCiudadClic(float mx, float my) const;
    void setCiudadOrigen(int idx);
    void setCiudadDestino(int idx);
    int getCiudadOrigen() const;
    int getCiudadDestino() const;
    int getTotalCiudades() const;
    const CiudadVisual& getCiudad(int idx) const;

    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
