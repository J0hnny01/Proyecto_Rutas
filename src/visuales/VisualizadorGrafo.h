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

    struct TrayectoriaAnimada {
        int indices[MAX_CIUDADES];
        int longitud;
        int tramoActual;
        float progreso;
        bool activa;
    };

    CiudadVisual ciudades[MAX_CIUDADES];
    int totalCiudades;

    AristaVisual aristas[MAX_ARISTAS];
    int totalAristas;

    int ciudadOrigenIdx;
    int ciudadDestinoIdx;

    TrayectoriaAnimada trayectoriaVuelo;
    TrayectoriaAnimada trayectoriaMaleta;

    void reiniciarTrayectoria(TrayectoriaAnimada& trayectoria);
    void cargarTrayectoria(TrayectoriaAnimada& trayectoria, const int indicesCiudades[], int cantidad, bool esMaleta);
    void avanzarTrayectoria(TrayectoriaAnimada& trayectoria, float deltaTiempo, float velocidad);
    bool existeArista(int origen, int destino) const;
    bool calcularSegmento(int origen, int destino, sf::Vector2f& p1, sf::Vector2f& p2) const;
    void dibujarAristaIndividual(sf::RenderWindow& ventana, const sf::Font& fuente, const AristaVisual& arista) const;
    void dibujarMarcador(sf::RenderWindow& ventana, const sf::Font& fuente,
                         const TrayectoriaAnimada& trayectoria, sf::Color color,
                         const std::string& etiqueta, float desplazamientoY) const;

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
