#include "BotonVisual.h"
#include "CompatSFML.h"

BotonVisual::BotonVisual() : activo(false) {
    colorBase = sf::Color(55, 65, 90);
    colorActivo = sf::Color(85, 130, 195);
}

void BotonVisual::configurar(float x, float y, float ancho, float alto, const std::string& texto, sf::Color colorFondo) {
    colorBase = colorFondo;
    colorActivo = sf::Color(85, 135, 200);
    etiqueta = texto;
    activo = false;

    caja.setPosition(sf::Vector2f(x, y));
    caja.setSize(sf::Vector2f(ancho, alto));
    caja.setFillColor(colorBase);
    caja.setOutlineColor(sf::Color(120, 140, 175));
    caja.setOutlineThickness(1.5f);
}

void BotonVisual::setActivo(bool estado) {
    activo = estado;
    caja.setFillColor(activo ? colorActivo : colorBase);
    caja.setOutlineColor(activo ? sf::Color(220, 235, 255) : sf::Color(120, 140, 175));
}

bool BotonVisual::estaActivo() const {
    return activo;
}

void BotonVisual::setTexto(const std::string& nuevoTexto) {
    etiqueta = nuevoTexto;
}

bool BotonVisual::contienePunto(float mx, float my) const {
    sf::Vector2f pos = caja.getPosition();
    sf::Vector2f tam = caja.getSize();
    return (mx >= pos.x && mx <= pos.x + tam.x && my >= pos.y && my <= pos.y + tam.y);
}

void BotonVisual::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    ventana.draw(caja);
    sf::Vector2f pos = caja.getPosition();
    sf::Vector2f tam = caja.getSize();
    CompatSFML::dibujarTexto(ventana, fuente, etiqueta, pos.x + 10.0f, pos.y + (tam.y / 2.0f) - 10.0f, 15, sf::Color::White);
}
