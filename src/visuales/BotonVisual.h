#ifndef BOTON_VISUAL_H
#define BOTON_VISUAL_H

#include <SFML/Graphics.hpp>
#include <string>

class BotonVisual {
private:
    sf::RectangleShape caja;
    std::string etiqueta;
    sf::Color colorBase;
    sf::Color colorActivo;
    bool activo;

public:
    BotonVisual();
    void configurar(float x, float y, float ancho, float alto, const std::string& texto,
                    sf::Color colorFondo = sf::Color(55, 65, 90));
    void setActivo(bool estado);
    bool estaActivo() const;
    void setTexto(const std::string& nuevoTexto);
    bool contienePunto(float mx, float my) const;
    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
