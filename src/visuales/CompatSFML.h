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
                             float x, float y, unsigned int tamano = 16, sf::Color color = sf::Color::White) {
        sf::Text txt = crearTexto(fuente, contenido, tamano, color);
        txt.setPosition(sf::Vector2f(x, y));
        ventana.draw(txt);
    }

    inline void dibujarLinea(sf::RenderWindow& ventana, sf::Vector2f inicio, sf::Vector2f fin,
                             sf::Color color = sf::Color::White, float grosor = 2.5f) {
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

}

#endif
