#include "VisualizadorGrafo.h"
#include "CompatSFML.h"
#include <cmath>

VisualizadorGrafo::VisualizadorGrafo()
    : totalCiudades(0), totalAristas(0),
      ciudadOrigenIdx(-1), ciudadDestinoIdx(-1),
      longitudRutaAnimada(0), tramoActual(0), progresoTramo(0.0f), animacionActiva(false),
      longitudRutaMaleta(0), tramoMaletaActual(0), progresoMaleta(0.0f), animacionMaletaActiva(false) {
}

void VisualizadorGrafo::limpiar() {
    totalCiudades = 0;
    totalAristas = 0;
    ciudadOrigenIdx = -1;
    ciudadDestinoIdx = -1;
    limpiarResaltados();
}

int VisualizadorGrafo::agregarCiudad(const std::string& codigo, const std::string& nombre, float x, float y) {
    if (totalCiudades >= MAX_CIUDADES) return -1;
    ciudades[totalCiudades].codigo = codigo;
    ciudades[totalCiudades].nombre = nombre;
    ciudades[totalCiudades].x = x;
    ciudades[totalCiudades].y = y;
    if (ciudadOrigenIdx == -1) ciudadOrigenIdx = 0;
    if (ciudadDestinoIdx == -1) ciudadDestinoIdx = 0;
    return totalCiudades++;
}

void VisualizadorGrafo::agregarArista(int origen, int destino, int costo, int tiempo) {
    if (totalAristas >= MAX_ARISTAS) return;
    aristas[totalAristas].origen = origen;
    aristas[totalAristas].destino = destino;
    aristas[totalAristas].costo = costo;
    aristas[totalAristas].tiempo = tiempo;
    aristas[totalAristas].esRutaPasajero = false;
    aristas[totalAristas].esRutaMaleta = false;
    aristas[totalAristas].esAristaMST = false;
    totalAristas++;
}

void VisualizadorGrafo::limpiarResaltados() {
    for (int i = 0; i < totalAristas; ++i) {
        aristas[i].esRutaPasajero = false;
        aristas[i].esRutaMaleta = false;
        aristas[i].esAristaMST = false;
    }
    animacionActiva = false;
    longitudRutaAnimada = 0;
    animacionMaletaActiva = false;
    longitudRutaMaleta = 0;
}

void VisualizadorGrafo::iniciarAnimacionRuta(const int indicesCiudades[], int cantidad, bool esMaleta) {
    if (cantidad < 2) return;

    if (!esMaleta) {
        longitudRutaAnimada = (cantidad > MAX_CIUDADES) ? MAX_CIUDADES : cantidad;
        for (int i = 0; i < longitudRutaAnimada; ++i) {
            rutaAnimada[i] = indicesCiudades[i];
        }
        tramoActual = 0;
        progresoTramo = 0.0f;
        animacionActiva = true;

        for (int i = 0; i < longitudRutaAnimada - 1; ++i) {
            int u = rutaAnimada[i];
            int v = rutaAnimada[i + 1];
            for (int j = 0; j < totalAristas; ++j) {
                if (aristas[j].origen == u && aristas[j].destino == v) {
                    aristas[j].esRutaPasajero = true;
                }
            }
        }
    } else {
        longitudRutaMaleta = (cantidad > MAX_CIUDADES) ? MAX_CIUDADES : cantidad;
        for (int i = 0; i < longitudRutaMaleta; ++i) {
            rutaMaletaAnimada[i] = indicesCiudades[i];
        }
        tramoMaletaActual = 0;
        progresoMaleta = 0.0f;
        animacionMaletaActiva = true;

        for (int i = 0; i < longitudRutaMaleta - 1; ++i) {
            int u = rutaMaletaAnimada[i];
            int v = rutaMaletaAnimada[i + 1];
            for (int j = 0; j < totalAristas; ++j) {
                if (aristas[j].origen == u && aristas[j].destino == v) {
                    aristas[j].esRutaMaleta = true;
                }
            }
        }
    }
}

void VisualizadorGrafo::resaltarAristaMST(int origen, int destino) {
    for (int i = 0; i < totalAristas; ++i) {
        if ((aristas[i].origen == origen && aristas[i].destino == destino) ||
            (aristas[i].origen == destino && aristas[i].destino == origen)) {
            aristas[i].esAristaMST = true;
        }
    }
}

void VisualizadorGrafo::actualizarAnimacion(float deltaTiempo) {
    const float velocidad = 0.75f;

    if (animacionActiva && longitudRutaAnimada >= 2) {
        progresoTramo += deltaTiempo * velocidad;
        if (progresoTramo >= 1.0f) {
            progresoTramo = 0.0f;
            tramoActual++;
            if (tramoActual >= longitudRutaAnimada - 1) {
                tramoActual = 0;
            }
        }
    }

    if (animacionMaletaActiva && longitudRutaMaleta >= 2) {
        progresoMaleta += deltaTiempo * (velocidad * 0.85f);
        if (progresoMaleta >= 1.0f) {
            progresoMaleta = 0.0f;
            tramoMaletaActual++;
            if (tramoMaletaActual >= longitudRutaMaleta - 1) {
                tramoMaletaActual = 0;
            }
        }
    }
}

int VisualizadorGrafo::detectarCiudadClic(float mx, float my) const {
    const float radio = 24.0f;
    for (int i = 0; i < totalCiudades; ++i) {
        float dx = mx - ciudades[i].x;
        float dy = my - ciudades[i].y;
        if (dx * dx + dy * dy <= radio * radio) {
            return i;
        }
    }
    return -1;
}

void VisualizadorGrafo::setCiudadOrigen(int idx) {
    if (idx >= 0 && idx < totalCiudades) ciudadOrigenIdx = idx;
}

void VisualizadorGrafo::setCiudadDestino(int idx) {
    if (idx >= 0 && idx < totalCiudades) ciudadDestinoIdx = idx;
}

int VisualizadorGrafo::getCiudadOrigen() const { return ciudadOrigenIdx; }
int VisualizadorGrafo::getCiudadDestino() const { return ciudadDestinoIdx; }
int VisualizadorGrafo::getTotalCiudades() const { return totalCiudades; }
const CiudadVisual& VisualizadorGrafo::getCiudad(int idx) const { return ciudades[idx]; }

void VisualizadorGrafo::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    if (totalCiudades == 0) {
        CompatSFML::dibujarTexto(ventana, fuente, "Area de Visualizacion del Grafo de Vuelos", 600.0f, 270.0f, 16, sf::Color(140, 155, 180));
        return;
    }

    for (int i = 0; i < totalAristas; ++i) {
        const CiudadVisual& cOrig = ciudades[aristas[i].origen];
        const CiudadVisual& cDest = ciudades[aristas[i].destino];

        sf::Vector2f p1(cOrig.x, cOrig.y);
        sf::Vector2f p2(cDest.x, cDest.y);

        sf::Color colorLinea(90, 105, 130);
        float grosor = 2.0f;

        if (aristas[i].esAristaMST) {
            colorLinea = sf::Color(250, 210, 60);
            grosor = 4.5f;
        }
        if (aristas[i].esRutaMaleta) {
            colorLinea = sf::Color(255, 155, 45);
            grosor = 4.0f;
        }
        if (aristas[i].esRutaPasajero) {
            colorLinea = sf::Color(55, 225, 115);
            grosor = 4.5f;
        }

        CompatSFML::dibujarLinea(ventana, p1, p2, colorLinea, grosor);

        float mx = (p1.x + p2.x) / 2.0f;
        float my = (p1.y + p2.y) / 2.0f;
        std::string etiqueta = "$" + std::to_string(aristas[i].costo) + " | " + std::to_string(aristas[i].tiempo) + "m";
        CompatSFML::dibujarTexto(ventana, fuente, etiqueta, mx - 25.0f, my - 10.0f, 12, sf::Color(210, 220, 235));
    }

    if (animacionMaletaActiva && longitudRutaMaleta >= 2) {
        int u = rutaMaletaAnimada[tramoMaletaActual];
        int v = rutaMaletaAnimada[tramoMaletaActual + 1];
        float x = ciudades[u].x + (ciudades[v].x - ciudades[u].x) * progresoMaleta;
        float y = ciudades[u].y + (ciudades[v].y - ciudades[u].y) * progresoMaleta;

        sf::RectangleShape iconoMaleta(sf::Vector2f(14.0f, 12.0f));
        iconoMaleta.setOrigin(sf::Vector2f(7.0f, 6.0f));
        iconoMaleta.setPosition(sf::Vector2f(x, y + 8.0f));
        iconoMaleta.setFillColor(sf::Color(255, 155, 45));
        iconoMaleta.setOutlineColor(sf::Color::White);
        iconoMaleta.setOutlineThickness(1.5f);
        ventana.draw(iconoMaleta);
        CompatSFML::dibujarTexto(ventana, fuente, "Maleta", x - 18.0f, y + 16.0f, 11, sf::Color(255, 185, 90));
    }

    if (animacionActiva && longitudRutaAnimada >= 2) {
        int u = rutaAnimada[tramoActual];
        int v = rutaAnimada[tramoActual + 1];
        float x = ciudades[u].x + (ciudades[v].x - ciudades[u].x) * progresoTramo;
        float y = ciudades[u].y + (ciudades[v].y - ciudades[u].y) * progresoTramo;

        sf::CircleShape iconoAvion(8.0f);
        iconoAvion.setOrigin(sf::Vector2f(8.0f, 8.0f));
        iconoAvion.setPosition(sf::Vector2f(x, y));
        iconoAvion.setFillColor(sf::Color(55, 240, 120));
        iconoAvion.setOutlineColor(sf::Color::White);
        iconoAvion.setOutlineThickness(2.0f);
        ventana.draw(iconoAvion);
        CompatSFML::dibujarTexto(ventana, fuente, "Vuelo", x - 16.0f, y - 22.0f, 12, sf::Color(120, 255, 160));
    }

    const float radio = 22.0f;
    for (int i = 0; i < totalCiudades; ++i) {
        sf::CircleShape nodo(radio);
        nodo.setOrigin(sf::Vector2f(radio, radio));
        nodo.setPosition(sf::Vector2f(ciudades[i].x, ciudades[i].y));

        if (i == ciudadOrigenIdx) {
            nodo.setFillColor(sf::Color(35, 135, 80));
            nodo.setOutlineColor(sf::Color(100, 255, 150));
            nodo.setOutlineThickness(3.0f);
        } else if (i == ciudadDestinoIdx) {
            nodo.setFillColor(sf::Color(165, 55, 55));
            nodo.setOutlineColor(sf::Color(255, 130, 130));
            nodo.setOutlineThickness(3.0f);
        } else {
            nodo.setFillColor(sf::Color(45, 75, 125));
            nodo.setOutlineColor(sf::Color(150, 185, 235));
            nodo.setOutlineThickness(2.0f);
        }

        ventana.draw(nodo);

        CompatSFML::dibujarTexto(ventana, fuente, ciudades[i].codigo,
                                 ciudades[i].x - 15.0f, ciudades[i].y - 9.0f, 14, sf::Color::White);

        CompatSFML::dibujarTexto(ventana, fuente, ciudades[i].nombre,
                                 ciudades[i].x - 28.0f, ciudades[i].y + 25.0f, 13, sf::Color(200, 215, 240));
    }
}
