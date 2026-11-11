#include "VisualizadorGrafo.h"
#include "CompatSFML.h"
#include <cmath>

VisualizadorGrafo::VisualizadorGrafo()
    : totalCiudades(0), totalAristas(0), ciudadOrigenIdx(-1), ciudadDestinoIdx(-1) {
    reiniciarTrayectoria(trayectoriaVuelo);
    reiniciarTrayectoria(trayectoriaMaleta);
}

void VisualizadorGrafo::reiniciarTrayectoria(TrayectoriaAnimada& trayectoria) {
    trayectoria.longitud = 0;
    trayectoria.tramoActual = 0;
    trayectoria.progreso = 0.0f;
    trayectoria.activa = false;
}

void VisualizadorGrafo::limpiar() {
    totalCiudades = 0;
    totalAristas = 0;
    ciudadOrigenIdx = -1;
    ciudadDestinoIdx = -1;
    limpiarResaltados();
}

int VisualizadorGrafo::agregarCiudad(const string& codigo, const string& nombre, float x, float y) {
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
    reiniciarTrayectoria(trayectoriaVuelo);
    reiniciarTrayectoria(trayectoriaMaleta);
}

void VisualizadorGrafo::cargarTrayectoria(TrayectoriaAnimada& trayectoria, const int indicesCiudades[], int cantidad, bool esMaleta) {
    trayectoria.longitud = (cantidad > MAX_CIUDADES) ? MAX_CIUDADES : cantidad;
    for (int i = 0; i < trayectoria.longitud; ++i) {
        trayectoria.indices[i] = indicesCiudades[i];
    }
    trayectoria.tramoActual = 0;
    trayectoria.progreso = 0.0f;
    trayectoria.activa = true;

    for (int i = 0; i < trayectoria.longitud - 1; ++i) {
        resaltarAristaRuta(trayectoria.indices[i], trayectoria.indices[i + 1], esMaleta);
    }
}

void VisualizadorGrafo::resaltarAristaRuta(int origen, int destino, bool esMaleta) {
    for (int j = 0; j < totalAristas; ++j) {
        if (aristas[j].origen == origen && aristas[j].destino == destino) {
            if (esMaleta) {
                aristas[j].esRutaMaleta = true;
            } else {
                aristas[j].esRutaPasajero = true;
            }
        }
    }
}

void VisualizadorGrafo::iniciarAnimacionRuta(const int indicesCiudades[], int cantidad, bool esMaleta) {
    if (cantidad < 2) return;
    if (esMaleta) {
        cargarTrayectoria(trayectoriaMaleta, indicesCiudades, cantidad, true);
    } else {
        cargarTrayectoria(trayectoriaVuelo, indicesCiudades, cantidad, false);
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

void VisualizadorGrafo::avanzarTrayectoria(TrayectoriaAnimada& trayectoria, float deltaTiempo, float velocidad) {
    if (!trayectoria.activa || trayectoria.longitud < 2) return;
    trayectoria.progreso += deltaTiempo * velocidad;
    if (trayectoria.progreso >= 1.0f) {
        trayectoria.progreso = 0.0f;
        trayectoria.tramoActual++;
        if (trayectoria.tramoActual >= trayectoria.longitud - 1) {
            trayectoria.tramoActual = 0;
        }
    }
}

void VisualizadorGrafo::actualizarAnimacion(float deltaTiempo) {
    avanzarTrayectoria(trayectoriaVuelo, deltaTiempo, 0.75f);
    avanzarTrayectoria(trayectoriaMaleta, deltaTiempo, 0.64f);
}

int VisualizadorGrafo::detectarCiudadClic(float mx, float my) const {
    const float radio = 22.0f;
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

bool VisualizadorGrafo::existeArista(int origen, int destino) const {
    for (int i = 0; i < totalAristas; ++i) {
        if (aristas[i].origen == origen && aristas[i].destino == destino) return true;
    }
    return false;
}

bool VisualizadorGrafo::calcularSegmento(int origen, int destino, sf::Vector2f& p1, sf::Vector2f& p2) const {
    p1 = sf::Vector2f(ciudades[origen].x, ciudades[origen].y);
    p2 = sf::Vector2f(ciudades[destino].x, ciudades[destino].y);
    sf::Vector2f delta = p2 - p1;
    float longitud = sqrt(delta.x * delta.x + delta.y * delta.y);
    if (longitud <= 0.001f) return false;

    if (existeArista(destino, origen)) {
        sf::Vector2f perp(-delta.y / longitud, delta.x / longitud);
        p1 += perp * 7.5f;
        p2 += perp * 7.5f;
    }
    return true;
}

void VisualizadorGrafo::dibujarAristaIndividual(sf::RenderWindow& ventana, const sf::Font& fuente, const AristaVisual& arista) const {
    sf::Vector2f p1;
    sf::Vector2f p2;
    if (!calcularSegmento(arista.origen, arista.destino, p1, p2)) return;

    sf::Color colorLinea(72, 80, 96);
    float grosor = 1.6f;

    if (arista.esAristaMST) {
        colorLinea = sf::Color(232, 194, 68);
        grosor = 2.8f;
    }
    if (arista.esRutaMaleta) {
        colorLinea = sf::Color(238, 148, 56);
        grosor = 2.8f;
    }
    if (arista.esRutaPasajero) {
        colorLinea = sf::Color(46, 184, 114);
        grosor = 3.0f;
    }

    CompatSFML::dibujarFlecha(ventana, p1, p2, colorLinea, grosor, 22.0f);

    float mx = (p1.x + p2.x) * 0.5f;
    float my = (p1.y + p2.y) * 0.5f;
    string etiqueta = "$" + to_string(arista.costo) + " - " + to_string(arista.tiempo) + "m";
    float anchoPill = static_cast<float>(etiqueta.size()) * 6.5f + 12.0f;

    sf::RectangleShape fondoEtiqueta(sf::Vector2f(anchoPill, 16.0f));
    fondoEtiqueta.setOrigin(sf::Vector2f(anchoPill * 0.5f, 8.0f));
    fondoEtiqueta.setPosition(sf::Vector2f(mx, my));
    fondoEtiqueta.setFillColor(sf::Color(23, 26, 31, 230));
    ventana.draw(fondoEtiqueta);

    CompatSFML::dibujarTextoCentrado(ventana, fuente, etiqueta, mx, my, 11, sf::Color(186, 194, 208));
}

void VisualizadorGrafo::dibujarMarcador(sf::RenderWindow& ventana, const sf::Font& fuente,
                                        const TrayectoriaAnimada& trayectoria, sf::Color color,
                                        const string& etiqueta, float desplazamientoY) const {
    if (!trayectoria.activa || trayectoria.longitud < 2) return;
    int u = trayectoria.indices[trayectoria.tramoActual];
    int v = trayectoria.indices[trayectoria.tramoActual + 1];
    sf::Vector2f p1;
    sf::Vector2f p2;
    if (!calcularSegmento(u, v, p1, p2)) return;
    float x = p1.x + (p2.x - p1.x) * trayectoria.progreso;
    float y = p1.y + (p2.y - p1.y) * trayectoria.progreso + desplazamientoY;

    sf::CircleShape punto(6.5f);
    punto.setOrigin(sf::Vector2f(6.5f, 6.5f));
    punto.setPosition(sf::Vector2f(x, y));
    punto.setFillColor(color);
    punto.setOutlineColor(sf::Color(245, 248, 252));
    punto.setOutlineThickness(1.5f);
    ventana.draw(punto);

    CompatSFML::dibujarTextoCentrado(ventana, fuente, etiqueta, x, y - 14.0f, 11, color);
}

void VisualizadorGrafo::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    const float leyendaY = 68.0f;
    const float posLeyendaX[4] = {890.0f, 960.0f, 1040.0f, 1114.0f};
    const sf::Color coloresLeyenda[4] = {
        sf::Color(46, 184, 114),
        sf::Color(220, 82, 82),
        sf::Color(238, 148, 56),
        sf::Color(232, 194, 68)
    };
    const char* textosLeyenda[4] = {"Vuelo", "Destino", "Maleta", "MST"};

    for (int i = 0; i < 4; ++i) {
        float x = posLeyendaX[i];
        sf::CircleShape punto(4.0f);
        punto.setOrigin(sf::Vector2f(4.0f, 4.0f));
        punto.setPosition(sf::Vector2f(x, leyendaY));
        punto.setFillColor(coloresLeyenda[i]);
        ventana.draw(punto);
        CompatSFML::dibujarTexto(ventana, fuente, textosLeyenda[i], x + 8.0f, leyendaY - 7.0f, 11, sf::Color(145, 153, 168));
    }

    if (totalCiudades == 0) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente, "Mapa vacio: carga ciudades y rutas para visualizar el grafo",
                                         738.0f, 320.0f, 15, sf::Color(125, 134, 150));
        return;
    }

    for (int i = 0; i < totalAristas; ++i) {
        if (!aristas[i].esRutaPasajero && !aristas[i].esRutaMaleta && !aristas[i].esAristaMST) {
            dibujarAristaIndividual(ventana, fuente, aristas[i]);
        }
    }

    for (int i = 0; i < totalAristas; ++i) {
        if (aristas[i].esRutaPasajero || aristas[i].esRutaMaleta || aristas[i].esAristaMST) {
            dibujarAristaIndividual(ventana, fuente, aristas[i]);
        }
    }

    dibujarMarcador(ventana, fuente, trayectoriaMaleta, sf::Color(238, 148, 56), "Maleta", 8.0f);
    dibujarMarcador(ventana, fuente, trayectoriaVuelo, sf::Color(46, 184, 114), "Vuelo", 0.0f);

    const float radio = 20.0f;
    for (int i = 0; i < totalCiudades; ++i) {
        sf::CircleShape nodo(radio);
        nodo.setOrigin(sf::Vector2f(radio, radio));
        nodo.setPosition(sf::Vector2f(ciudades[i].x, ciudades[i].y));

        if (i == ciudadOrigenIdx) {
            nodo.setFillColor(sf::Color(35, 115, 75));
            nodo.setOutlineColor(sf::Color(68, 210, 136));
            nodo.setOutlineThickness(2.2f);
        } else if (i == ciudadDestinoIdx) {
            nodo.setFillColor(sf::Color(142, 48, 52));
            nodo.setOutlineColor(sf::Color(235, 102, 102));
            nodo.setOutlineThickness(2.2f);
        } else {
            nodo.setFillColor(sf::Color(46, 54, 68));
            nodo.setOutlineColor(sf::Color(102, 116, 140));
            nodo.setOutlineThickness(1.5f);
        }

        ventana.draw(nodo);

        CompatSFML::dibujarTextoCentrado(ventana, fuente, ciudades[i].codigo,
                                         ciudades[i].x, ciudades[i].y, 13, sf::Color(242, 245, 250));
        CompatSFML::dibujarTextoCentrado(ventana, fuente, ciudades[i].nombre,
                                         ciudades[i].x, ciudades[i].y + 30.0f, 12, sf::Color(175, 184, 198));
    }
}
