#include "VisualizadorArbol.h"
#include "CompatSFML.h"

VisualizadorArbol::VisualizadorArbol()
    : totalNodos(0), raizIdx(-1), esArbolGeneral(false), tituloArbol("Arbol") {
}

void VisualizadorArbol::limpiar(const string& titulo) {
    totalNodos = 0;
    raizIdx = -1;
    esArbolGeneral = false;
    tituloArbol = titulo;
    strPreorden = "";
    strInorden = "";
    strPostorden = "";
}

int VisualizadorArbol::agregarNodo(const string& etiqueta, const string& subtexto,
                                   int izq, int der, bool resaltado) {
    if (totalNodos >= MAX_NODOS) return -1;
    nodos[totalNodos].etiqueta = etiqueta;
    nodos[totalNodos].subtexto = subtexto;
    nodos[totalNodos].izq = izq;
    nodos[totalNodos].der = der;
    nodos[totalNodos].primerHijo = -1;
    nodos[totalNodos].siguienteHermano = -1;
    nodos[totalNodos].resaltado = resaltado;
    return totalNodos++;
}

int VisualizadorArbol::agregarNodoGeneral(const string& etiqueta, const string& subtexto, bool resaltado) {
    esArbolGeneral = true;
    return agregarNodo(etiqueta, subtexto, -1, -1, resaltado);
}

void VisualizadorArbol::enlazarHijo(int padreIdx, int hijoIdx) {
    if (padreIdx < 0 || padreIdx >= totalNodos || hijoIdx < 0 || hijoIdx >= totalNodos) return;
    esArbolGeneral = true;
    if (nodos[padreIdx].primerHijo == -1) {
        nodos[padreIdx].primerHijo = hijoIdx;
        return;
    }
    int actual = nodos[padreIdx].primerHijo;
    while (nodos[actual].siguienteHermano != -1) {
        actual = nodos[actual].siguienteHermano;
    }
    nodos[actual].siguienteHermano = hijoIdx;
}

void VisualizadorArbol::setRaiz(int idx) {
    raizIdx = idx;
}

void VisualizadorArbol::setRecorridos(const string& pre, const string& in, const string& post) {
    strPreorden = pre;
    strInorden = in;
    strPostorden = post;
}

int VisualizadorArbol::contarHojasGeneral(int idx) const {
    if (idx < 0 || idx >= totalNodos) return 0;
    int hijo = nodos[idx].primerHijo;
    if (hijo == -1) return 1;
    int total = 0;
    while (hijo != -1) {
        total += contarHojasGeneral(hijo);
        hijo = nodos[hijo].siguienteHermano;
    }
    return total;
}

int VisualizadorArbol::calcularProfundidadBinaria(int idx) const {
    if (idx < 0 || idx >= totalNodos) return -1;
    int profIzq = calcularProfundidadBinaria(nodos[idx].izq);
    int profDer = calcularProfundidadBinaria(nodos[idx].der);
    int mayor = (profIzq > profDer) ? profIzq : profDer;
    return mayor + 1;
}

int VisualizadorArbol::calcularProfundidadGeneral(int idx) const {
    if (idx < 0 || idx >= totalNodos) return -1;
    int hijo = nodos[idx].primerHijo;
    if (hijo == -1) return 0;
    int maxProf = 0;
    while (hijo != -1) {
        int profHijo = calcularProfundidadGeneral(hijo) + 1;
        if (profHijo > maxProf) {
            maxProf = profHijo;
        }
        hijo = nodos[hijo].siguienteHermano;
    }
    return maxProf;
}

void VisualizadorArbol::dibujarNodoVisual(sf::RenderWindow& ventana, const sf::Font& fuente,
                                          int idx, float x, float y, float radioNodo) const {
    sf::CircleShape circulo(radioNodo);
    circulo.setOrigin(sf::Vector2f(radioNodo, radioNodo));
    circulo.setPosition(sf::Vector2f(x, y));

    if (nodos[idx].resaltado) {
        circulo.setFillColor(sf::Color(35, 115, 75));
        circulo.setOutlineColor(sf::Color(68, 210, 136));
        circulo.setOutlineThickness(2.2f);
    } else {
        circulo.setFillColor(sf::Color(46, 54, 68));
        circulo.setOutlineColor(sf::Color(102, 116, 140));
        circulo.setOutlineThickness(1.5f);
    }
    ventana.draw(circulo);

    unsigned int tamEtiqueta = (radioNodo >= 17.0f) ? 12 : 10;
    unsigned int tamSub = (radioNodo >= 17.0f) ? 11 : 10;
    CompatSFML::dibujarTextoCentrado(ventana, fuente, nodos[idx].etiqueta, x, y, tamEtiqueta, sf::Color(242, 245, 250));
    if (!nodos[idx].subtexto.empty()) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente, nodos[idx].subtexto,
                                         x, y + radioNodo + 8.0f, tamSub, sf::Color(160, 170, 186));
    }
}

void VisualizadorArbol::dibujarBinario(sf::RenderWindow& ventana, const sf::Font& fuente,
                                       int idx, float x, float y, float separacionX,
                                       float saltoVertical, float radioNodo) const {
    if (idx < 0 || idx >= totalNodos) return;

    if (nodos[idx].izq != -1) {
        float hijoX = x - separacionX;
        float hijoY = y + saltoVertical;
        CompatSFML::dibujarLinea(ventana, sf::Vector2f(x, y), sf::Vector2f(hijoX, hijoY),
                                 sf::Color(76, 86, 104), 1.8f);
        dibujarBinario(ventana, fuente, nodos[idx].izq, hijoX, hijoY,
                       separacionX * 0.52f, saltoVertical, radioNodo);
    }

    if (nodos[idx].der != -1) {
        float hijoX = x + separacionX;
        float hijoY = y + saltoVertical;
        CompatSFML::dibujarLinea(ventana, sf::Vector2f(x, y), sf::Vector2f(hijoX, hijoY),
                                 sf::Color(76, 86, 104), 1.8f);
        dibujarBinario(ventana, fuente, nodos[idx].der, hijoX, hijoY,
                       separacionX * 0.52f, saltoVertical, radioNodo);
    }

    dibujarNodoVisual(ventana, fuente, idx, x, y, radioNodo);
}

void VisualizadorArbol::dibujarGeneral(sf::RenderWindow& ventana, const sf::Font& fuente,
                                       int idx, float xMin, float xMax, float y,
                                       float saltoVertical, float radioNodo) const {
    if (idx < 0 || idx >= totalNodos) return;

    float x = (xMin + xMax) * 0.5f;
    int totalHojas = contarHojasGeneral(idx);
    if (totalHojas < 1) totalHojas = 1;

    float cursorX = xMin;
    float anchoTotal = xMax - xMin;
    int hijo = nodos[idx].primerHijo;

    while (hijo != -1) {
        int hojasHijo = contarHojasGeneral(hijo);
        if (hojasHijo < 1) hojasHijo = 1;
        float anchoHijo = anchoTotal * (static_cast<float>(hojasHijo) / static_cast<float>(totalHojas));
        float hijoCentroX = cursorX + anchoHijo * 0.5f;
        float hijoY = y + saltoVertical;

        CompatSFML::dibujarLinea(ventana, sf::Vector2f(x, y), sf::Vector2f(hijoCentroX, hijoY),
                                 sf::Color(76, 86, 104), 1.8f);
        dibujarGeneral(ventana, fuente, hijo, cursorX, cursorX + anchoHijo, hijoY,
                       saltoVertical, radioNodo);

        cursorX += anchoHijo;
        hijo = nodos[hijo].siguienteHermano;
    }

    dibujarNodoVisual(ventana, fuente, idx, x, y, radioNodo);
}

void VisualizadorArbol::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    CompatSFML::dibujarTexto(ventana, fuente, tituloArbol, 300.0f, 68.0f, 15, sf::Color(220, 226, 236));

    if (raizIdx != -1) {
        int profundidad = esArbolGeneral
            ? calcularProfundidadGeneral(raizIdx)
            : calcularProfundidadBinaria(raizIdx);

        float yRaiz = 116.0f;
        float saltoVertical = 76.0f;
        float radioNodo = 20.0f;

        if (profundidad >= 5) {
            yRaiz = 110.0f;
            radioNodo = (profundidad >= 7) ? 14.0f : ((profundidad == 6) ? 16.0f : 18.0f);
            float yMaxCentro = 492.0f - (radioNodo + 14.0f);
            saltoVertical = (yMaxCentro - yRaiz) / static_cast<float>(profundidad);
            if (saltoVertical < 2.0f * radioNodo + 8.0f) {
                radioNodo = (saltoVertical - 10.0f) * 0.5f;
                if (radioNodo < 10.0f) radioNodo = 10.0f;
            }
        }

        if (esArbolGeneral) {
            dibujarGeneral(ventana, fuente, raizIdx, 310.0f, 1166.0f, yRaiz, saltoVertical, radioNodo);
        } else {
            dibujarBinario(ventana, fuente, raizIdx, 738.0f, yRaiz, 200.0f, saltoVertical, radioNodo);
        }
    } else {
        CompatSFML::dibujarTextoCentrado(ventana, fuente, "Sin datos cargados en el arbol",
                                         738.0f, 300.0f, 15, sf::Color(125, 134, 150));
    }

    sf::RectangleShape separadorRecorridos(sf::Vector2f(884.0f, 1.0f));
    separadorRecorridos.setPosition(sf::Vector2f(296.0f, 502.0f));
    separadorRecorridos.setFillColor(sf::Color(44, 49, 58));
    ventana.draw(separadorRecorridos);

    CompatSFML::dibujarTexto(ventana, fuente, "Preorden:  " + strPreorden,  300.0f, 514.0f, 12, sf::Color(178, 186, 200));
    CompatSFML::dibujarTexto(ventana, fuente, "Inorden:   " + strInorden,   300.0f, 536.0f, 12, sf::Color(178, 186, 200));
    CompatSFML::dibujarTexto(ventana, fuente, "Postorden: " + strPostorden, 300.0f, 558.0f, 12, sf::Color(178, 186, 200));
}
