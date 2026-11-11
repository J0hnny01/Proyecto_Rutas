#include "BotonVisual.h"
#include "CompatSFML.h"

BotonVisual::BotonVisual() : activo(false), textoCentrado(false), tamanoTexto(14) {
    colorBase = sf::Color(35, 39, 47);
    colorActivo = sf::Color(52, 114, 206);
}

void BotonVisual::configurar(float x, float y, float ancho, float alto, const std::string& texto,
                             bool centrarTexto, unsigned int tamanoLetra) {
    etiqueta = texto;
    activo = false;
    textoCentrado = centrarTexto;
    tamanoTexto = tamanoLetra;

    caja.setPosition(sf::Vector2f(x, y));
    caja.setSize(sf::Vector2f(ancho, alto));
    caja.setFillColor(colorBase);
    caja.setOutlineColor(sf::Color(52, 58, 70));
    caja.setOutlineThickness(1.0f);
}

void BotonVisual::setColores(sf::Color base, sf::Color seleccionado) {
    colorBase = base;
    colorActivo = seleccionado;
    setActivo(activo);
}

void BotonVisual::setActivo(bool estado) {
    activo = estado;
    caja.setFillColor(activo ? colorActivo : colorBase);
    caja.setOutlineColor(activo ? sf::Color(86, 148, 240) : sf::Color(52, 58, 70));
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
    sf::Color colorTexto = activo ? sf::Color(248, 250, 255) : sf::Color(218, 223, 232);

    if (textoCentrado) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente, etiqueta,
                                         pos.x + tam.x / 2.0f, pos.y + tam.y / 2.0f,
                                         tamanoTexto, colorTexto);
    } else {
        CompatSFML::dibujarTexto(ventana, fuente, etiqueta,
                                 pos.x + 14.0f, pos.y + (tam.y - static_cast<float>(tamanoTexto)) / 2.0f - 2.0f,
                                 tamanoTexto, colorTexto);
    }
}
