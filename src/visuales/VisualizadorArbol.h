#ifndef VISUALIZADOR_ARBOL_H
#define VISUALIZADOR_ARBOL_H

#include <SFML/Graphics.hpp>
#include <string>

struct NodoArbolVisual {
    std::string etiqueta;
    std::string subtexto;
    int izq;
    int der;
    bool resaltado;
};

class VisualizadorArbol {
private:
    static const int MAX_NODOS = 63;
    NodoArbolVisual nodos[MAX_NODOS];
    int totalNodos;
    int raizIdx;

    std::string tituloArbol;
    std::string strPreorden;
    std::string strInorden;
    std::string strPostorden;

    void dibujarNodoRecursivo(sf::RenderWindow& ventana, const sf::Font& fuente,
                              int idx, float x, float y, float separacionX) const;

public:
    VisualizadorArbol();

    void limpiar(const std::string& titulo = "Arbol Binario");
    int agregarNodo(const std::string& etiqueta, const std::string& subtexto = "",
                    int izq = -1, int der = -1, bool resaltado = false);
    void setRaiz(int idx);
    void setRecorridos(const std::string& pre, const std::string& in, const std::string& post);

    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
