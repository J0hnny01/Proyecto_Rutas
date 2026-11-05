#include "VisualizadorArbol.h"
#include "CompatSFML.h"

VisualizadorArbol::VisualizadorArbol() : totalNodos(0), raizIdx(-1), tituloArbol("Visualizador de Arbol") {
}

void VisualizadorArbol::limpiar(const std::string& titulo) {
    totalNodos = 0;
    raizIdx = -1;
    tituloArbol = titulo;
    strPreorden = "";
    strInorden = "";
    strPostorden = "";
}

int VisualizadorArbol::agregarNodo(const std::string& etiqueta, const std::string& subtexto,
                                   int izq, int der, bool resaltado) {
    if (totalNodos >= MAX_NODOS) return -1;
    nodos[totalNodos].etiqueta = etiqueta;
    nodos[totalNodos].subtexto = subtexto;
    nodos[totalNodos].izq = izq;
    nodos[totalNodos].der = der;
    nodos[totalNodos].resaltado = resaltado;
    return totalNodos++;
}

void VisualizadorArbol::setRaiz(int idx) {
    raizIdx = idx;
}

void VisualizadorArbol::setRecorridos(const std::string& pre, const std::string& in, const std::string& post) {
    strPreorden = pre;
    strInorden = in;
    strPostorden = post;
}

void VisualizadorArbol::dibujarNodoRecursivo(sf::RenderWindow& ventana, const sf::Font& fuente,
                                             int idx, float x, float y, float separacionX) const {
    if (idx < 0 || idx >= totalNodos) return;

    const float saltoVertical = 95.0f;
    const float radio = 24.0f;

    if (nodos[idx].izq != -1) {
        float hijoX = x - separacionX;
        float hijoY = y + saltoVertical;
        CompatSFML::dibujarLinea(ventana, sf::Vector2f(x, y), sf::Vector2f(hijoX, hijoY),
                                 sf::Color(110, 140, 185), 2.5f);
        dibujarNodoRecursivo(ventana, fuente, nodos[idx].izq, hijoX, hijoY, separacionX * 0.52f);
    }

    if (nodos[idx].der != -1) {
        float hijoX = x + separacionX;
        float hijoY = y + saltoVertical;
        CompatSFML::dibujarLinea(ventana, sf::Vector2f(x, y), sf::Vector2f(hijoX, hijoY),
                                 sf::Color(110, 140, 185), 2.5f);
        dibujarNodoRecursivo(ventana, fuente, nodos[idx].der, hijoX, hijoY, separacionX * 0.52f);
    }

    sf::CircleShape circulo(radio);
    circulo.setOrigin(sf::Vector2f(radio, radio));
    circulo.setPosition(sf::Vector2f(x, y));

    if (nodos[idx].resaltado) {
        circulo.setFillColor(sf::Color(45, 165, 95));
        circulo.setOutlineColor(sf::Color(150, 255, 180));
        circulo.setOutlineThickness(3.5f);
    } else {
        circulo.setFillColor(sf::Color(50, 80, 130));
        circulo.setOutlineColor(sf::Color(170, 200, 245));
        circulo.setOutlineThickness(2.0f);
    }
    ventana.draw(circulo);

    CompatSFML::dibujarTexto(ventana, fuente, nodos[idx].etiqueta, x - 20.0f, y - 9.0f, 13, sf::Color::White);
    if (!nodos[idx].subtexto.empty()) {
        CompatSFML::dibujarTexto(ventana, fuente, nodos[idx].subtexto, x - 35.0f, y + 27.0f, 12, sf::Color(190, 215, 240));
    }
}

void VisualizadorArbol::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    CompatSFML::dibujarTexto(ventana, fuente, tituloArbol, 385.0f, 78.0f, 17, sf::Color(135, 205, 255));

    if (raizIdx != -1) {
        dibujarNodoRecursivo(ventana, fuente, raizIdx, 765.0f, 155.0f, 170.0f);
    } else {
        CompatSFML::dibujarTexto(ventana, fuente, "Sin datos cargados en el arbol", 660.0f, 250.0f, 16, sf::Color(140, 155, 180));
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Recorridos del Arbol:", 385.0f, 415.0f, 14, sf::Color(250, 215, 90));
    CompatSFML::dibujarTexto(ventana, fuente, "Preorden:  " + strPreorden,  385.0f, 438.0f, 13, sf::Color(215, 225, 240));
    CompatSFML::dibujarTexto(ventana, fuente, "Inorden:   " + strInorden,   385.0f, 460.0f, 13, sf::Color(215, 225, 240));
    CompatSFML::dibujarTexto(ventana, fuente, "Postorden: " + strPostorden, 385.0f, 482.0f, 13, sf::Color(215, 225, 240));
}
