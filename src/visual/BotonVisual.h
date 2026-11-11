#ifndef BOTON_VISUAL_H
#define BOTON_VISUAL_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class BotonVisual {
private:
    sf::RectangleShape caja;
    string etiqueta;
    sf::Color colorBase;
    sf::Color colorActivo;
    bool activo;
    bool textoCentrado;
    unsigned int tamanoTexto;

public:
    BotonVisual();
    void configurar(float x, float y, float ancho, float alto, const string& texto,
                    bool centrarTexto = false, unsigned int tamanoLetra = 14);
    void setColores(sf::Color base, sf::Color seleccionado);
    void setActivo(bool estado);
    bool estaActivo() const;
    void setTexto(const string& nuevoTexto);
    bool contienePunto(float mx, float my) const;
    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
