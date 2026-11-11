#ifndef VISUALIZADOR_ARBOL_H
#define VISUALIZADOR_ARBOL_H

#include <SFML/Graphics.hpp>
#include <string>

struct NodoArbolVisual {
    std::string etiqueta;
    std::string subtexto;
    int izq;
    int der;
    int primerHijo;
    int siguienteHermano;
    bool resaltado;
};

class VisualizadorArbol {
private:
    static const int MAX_NODOS = 63;
    NodoArbolVisual nodos[MAX_NODOS];
    int totalNodos;
    int raizIdx;
    bool esArbolGeneral;

    std::string tituloArbol;
    std::string strPreorden;
    std::string strInorden;
    std::string strPostorden;

    int contarHojasGeneral(int idx) const;
    void dibujarBinario(sf::RenderWindow& ventana, const sf::Font& fuente,
                        int idx, float x, float y, float separacionX) const;
    void dibujarGeneral(sf::RenderWindow& ventana, const sf::Font& fuente,
                        int idx, float xMin, float xMax, float y) const;
    void dibujarNodoVisual(sf::RenderWindow& ventana, const sf::Font& fuente,
                           int idx, float x, float y) const;

public:
    VisualizadorArbol();

    void limpiar(const std::string& titulo = "Arbol");
    int agregarNodo(const std::string& etiqueta, const std::string& subtexto = "",
                    int izq = -1, int der = -1, bool resaltado = false);
    int agregarNodoGeneral(const std::string& etiqueta, const std::string& subtexto = "",
                           bool resaltado = false);
    void enlazarHijo(int padreIdx, int hijoIdx);
    void setRaiz(int idx);
    void setRecorridos(const std::string& pre, const std::string& in, const std::string& post);

    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
