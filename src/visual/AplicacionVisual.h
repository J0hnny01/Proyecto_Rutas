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
    int ultimaAccionAdmin;
    int indicePasajeroActivo;
    int indiceAeronaveActiva;
    int indiceVueloActivo;
    int modoArbolVista;
    bool ultimoMSTEsPrim;
    ArbolMST ultimoArbolMST;

    BotonVisual btnTabCliente;
    BotonVisual btnTabAdmin;

    BotonVisual btnOrigenAnt;
    BotonVisual btnOrigenSig;
    BotonVisual btnDestinoAnt;
    BotonVisual btnDestinoSig;

    static const int TOTAL_BOTONES_ACCION = 6;
    BotonVisual botonesCliente[TOTAL_BOTONES_ACCION];
    BotonVisual botonesAdmin[TOTAL_BOTONES_ACCION];

    BotonVisual btnAdminEstadoAeronave;
    BotonVisual btnAdminAjustarRuta;
    BotonVisual btnAdminToggleVueloPasajero;

    static const int MAX_LINEAS_INFO = 4;
    string tituloResultado;
    string lineasResultado[MAX_LINEAS_INFO];

    void inicializarInterfaz();
    void sincronizarMapaVisual();
    void procesarEventos();
    void manejarClicIzquierdo(float mx, float my);
    void manejarClicDerecho(float mx, float my);
    bool manejarClicEdicionRapidaAdmin(float mx, float my);

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
    int volcarABBRec(const NodoABB* nodo, const string& codigoResaltado);
    int volcarAVLRec(const NodoAVL* nodo, const string& idResaltado);
    void volcarMSTRec(const NodoMST* nodo, int padreVisualIdx);

    void mostrarArbolABBVisual(const string& codigoResaltado);
    void mostrarArbolAVLVisual(const string& idResaltado);
    void mostrarArbolMSTVisual(const string& titulo);

    void mostrarRutaEquipajeAdmin();
    void mostrarAeronavesYCrewAdmin();
    void mostrarVuelosYPasajerosAdmin();
    void mostrarMSTAdmin(bool usarPrim);
    void mostrarSiguienteArbolAdmin();
    void rotarEstadoAeronaveRapida();
    void ajustarTarifaRutaRapida(int deltaCosto);
    void alternarVueloPasajeroRapido();

    void ejecutarAccionCliente(int indiceAccion);
    void ejecutarAccionAdmin(int indiceAccion);
    void limpiarLineasResultado(const string& nuevoTitulo);

    void renderizar();

public:
    AplicacionVisual();
    void ejecutar();
};

#endif
