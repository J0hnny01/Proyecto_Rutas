#ifndef APLICACION_VISUAL_H
#define APLICACION_VISUAL_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../sistema/SistemaAerolinea.h"
#include "BotonVisual.h"
#include "VisualizadorArbol.h"
#include "VisualizadorGrafo.h"

using namespace std;

enum class ModuloActivo {
    Cliente,
    Administrativo
};

enum class VistaLienzo {
    MapaGrafo,
    ArbolEstructura
};

class AplicacionVisual {
private:
    sf::RenderWindow ventana;
    sf::Font fuente;
    sf::Clock relojDelta;

    ModuloActivo moduloActual;
    VistaLienzo vistaActual;

    SistemaAerolinea sistema;
    VisualizadorGrafo grafoVisual;
    VisualizadorArbol arbolVisual;

    bool criterioBellmanCosto;
    int ultimaAccionCliente;
    int indicePasajeroActivo;

    BotonVisual btnTabCliente;
    BotonVisual btnTabAdmin;

    BotonVisual btnOrigenAnt;
    BotonVisual btnOrigenSig;
    BotonVisual btnDestinoAnt;
    BotonVisual btnDestinoSig;

    static const int TOTAL_BOTONES_ACCION = 6;
    BotonVisual botonesCliente[TOTAL_BOTONES_ACCION];
    BotonVisual botonesAdmin[TOTAL_BOTONES_ACCION];

    static const int MAX_LINEAS_INFO = 4;
    string tituloResultado;
    string lineasResultado[MAX_LINEAS_INFO];

    void inicializarInterfaz();
    void sincronizarMapaVisual();
    void procesarEventos();
    void manejarClicIzquierdo(float mx, float my);
    void manejarClicDerecho(float mx, float my);

    bool validarOrigenDestino(string& codOrigen, string& codDestino, bool exigirDistintos);
    void animarIndices(const ListaEnlazada<int>& indicesLista, bool esMaleta);
    Pasajero obtenerPasajeroActivo(bool& hayPasajeros) const;
    string truncarTexto(const string& texto, int maxChars) const;

    void mostrarRutaMasRapida();
    void mostrarRutaMasBarata();
    void mostrarExplorarDestinos();
    void mostrarBusquedaCiudadABB();
    void mostrarVuelosPasajeroAVL();
    void mostrarMapaRutas();
    void mostrarRutaEquipajeAdmin();

    void ejecutarAccionCliente(int indiceAccion);
    void ejecutarAccionAdmin(int indiceAccion);
    void limpiarLineasResultado(const string& nuevoTitulo);

    void renderizar();

public:
    AplicacionVisual();
    void ejecutar();
};

#endif
