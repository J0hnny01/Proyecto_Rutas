#ifndef COMPAT_SFML_H
#define COMPAT_SFML_H

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

namespace CompatSFML {

    inline sf::VideoMode crearVideoMode(unsigned int ancho, unsigned int alto) {
    #if SFML_VERSION_MAJOR >= 3
        return sf::VideoMode({ancho, alto});
    #else
        return sf::VideoMode(ancho, alto);
    #endif
    }

    inline bool cargarFuente(sf::Font& fuente, const std::string& ruta) {
    #if SFML_VERSION_MAJOR >= 3
        return fuente.openFromFile(ruta);
    #else
        return fuente.loadFromFile(ruta);
    #endif
    }

    inline sf::Text crearTexto(const sf::Font& fuente, const std::string& contenido, unsigned int tamano, sf::Color color = sf::Color::White) {
    #if SFML_VERSION_MAJOR >= 3
        sf::Text txt(fuente, contenido, tamano);
    #else
        sf::Text txt(contenido, fuente, tamano);
    #endif
        txt.setFillColor(color);
        return txt;
    }

    inline void dibujarTexto(sf::RenderWindow& ventana, const sf::Font& fuente, const std::string& contenido,
                             float x, float y, unsigned int tamano = 14, sf::Color color = sf::Color::White) {
        sf::Text txt = crearTexto(fuente, contenido, tamano, color);
        txt.setPosition(sf::Vector2f(x, y));
        ventana.draw(txt);
    }

    inline sf::FloatRect obtenerLimitesTexto(const sf::Text& txt) {
        return txt.getLocalBounds();
    }

    inline void dibujarTextoCentrado(sf::RenderWindow& ventana, const sf::Font& fuente, const std::string& contenido,
                                     float cx, float cy, unsigned int tamano = 14, sf::Color color = sf::Color::White) {
        sf::Text txt = crearTexto(fuente, contenido, tamano, color);
        sf::FloatRect bounds = obtenerLimitesTexto(txt);
    #if SFML_VERSION_MAJOR >= 3
        txt.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f,
                                   bounds.position.y + bounds.size.y / 2.0f));
    #else
        txt.setOrigin(sf::Vector2f(bounds.left + bounds.width / 2.0f,
                                   bounds.top + bounds.height / 2.0f));
    #endif
        txt.setPosition(sf::Vector2f(cx, cy));
        ventana.draw(txt);
    }

    inline void dibujarLinea(sf::RenderWindow& ventana, sf::Vector2f inicio, sf::Vector2f fin,
                             sf::Color color = sf::Color::White, float grosor = 1.5f) {
        sf::Vector2f delta = fin - inicio;
        float longitud = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (longitud <= 0.001f) return;

        float anguloGrados = std::atan2(delta.y, delta.x) * 180.0f / 3.14159265f;

        sf::RectangleShape linea(sf::Vector2f(longitud, grosor));
        linea.setOrigin(sf::Vector2f(0.0f, grosor / 2.0f));
        linea.setPosition(inicio);
        linea.setFillColor(color);

    #if SFML_VERSION_MAJOR >= 3
        linea.setRotation(sf::degrees(anguloGrados));
    #else
        linea.setRotation(anguloGrados);
    #endif
        ventana.draw(linea);
    }

    inline void dibujarFlecha(sf::RenderWindow& ventana, sf::Vector2f inicio, sf::Vector2f fin,
                              sf::Color color, float grosor, float recorteDestino = 0.0f) {
        sf::Vector2f delta = fin - inicio;
        float longitud = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (longitud <= recorteDestino + 6.0f) return;

        sf::Vector2f dir(delta.x / longitud, delta.y / longitud);
        sf::Vector2f perp(-dir.y, dir.x);
        sf::Vector2f punta = fin - dir * recorteDestino;
        float largoPunta = 8.0f;
        float anchoPunta = 4.2f;
        sf::Vector2f basePunta = punta - dir * largoPunta;

        dibujarLinea(ventana, inicio, basePunta, color, grosor);

        sf::ConvexShape triangulo;
        triangulo.setPointCount(3);
        triangulo.setPoint(0, punta);
        triangulo.setPoint(1, basePunta + perp * anchoPunta);
        triangulo.setPoint(2, basePunta - perp * anchoPunta);
        triangulo.setFillColor(color);
        ventana.draw(triangulo);
    }

}

#endif
