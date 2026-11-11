#ifndef APLICACION_VISUAL_H
#define APLICACION_VISUAL_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../sistema/SistemaAerolinea.h"
#include "BotonVisual.h"
#include "FormularioModal.h"
#include "PanelAdmin.h"
#include "PanelMantenimiento.h"
#include "VisualizadorArbol.h"
#include "VisualizadorGrafo.h"

using namespace std;

enum class ModuloActivo {
    Cliente,
    Administrativo,
    Mantenimientos
};

enum class VistaLienzo {
    MapaGrafo,
    ArbolEstructura,
    PanelMantenimiento,
    PanelAdmin
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
    FormularioModal modal;
    PanelAdmin panelAdmin;
    PanelMantenimiento panelMantenimiento;

    BotonVisual btnTabCliente;
    BotonVisual btnTabAdmin;
    BotonVisual btnTabMantenimiento;

    BotonVisual btnOrigenAnt;
    BotonVisual btnOrigenSig;
    BotonVisual btnDestinoAnt;
    BotonVisual btnDestinoSig;

    static const int TOTAL_BOTONES_ACCION = 6;
    BotonVisual botonesCliente[TOTAL_BOTONES_ACCION];
    BotonVisual botonesAdmin[TOTAL_BOTONES_ACCION];

    BotonVisual btnClienteReservarVuelo;
    BotonVisual btnClienteSiguientePasajero;

    BotonVisual botonesMantenimientoEntidad[TOTAL_BOTONES_ACCION];
    BotonVisual btnMaintAgregar;
    BotonVisual btnMaintEditar;
    BotonVisual btnMaintEliminar;
    BotonVisual btnMaintVerEstructura;
    BotonVisual btnMaintRecargar;

    BotonVisual btnAdminIrMantenimientos;

    static const int MAX_LINEAS_INFO = 4;
    string tituloResultado;
    string lineasResultado[MAX_LINEAS_INFO];

    void inicializarInterfaz();
    void sincronizarMapaVisual();
    void cambiarModulo(ModuloActivo nuevoModulo);
    void actualizarTextosMantenimiento();
    void actualizarDetalleRegistroMantenimiento();
    void finalizarCrudMantenimiento(int idxSeleccionado, const string& accionMensaje);
    void mostrarEstructuraDesdeMantenimiento();
    void recargarDatosDesdeMantenimiento();
    void procesarEventos();
    void manejarClicIzquierdo(float mx, float my);
    void manejarClicDerecho(float mx, float my);
    bool manejarClicPanelMantenimiento(float mx, float my);
    bool manejarClicPanelAdmin(float mx, float my);
    bool manejarClicClienteExtra(float mx, float my);

    bool validarOrigenDestino(string& codOrigen, string& codDestino, bool exigirDistintos);
    void animarIndices(const ListaEnlazada<int>& indicesLista, bool esMaleta);
    Pasajero obtenerPasajeroActivo(bool& hayPasajeros) const;
    string truncarTexto(const string& texto, int maxChars) const;
    string limpiarCampoTexto(const string& texto) const;
    bool igualSinCaso(const string& a, const string& b) const;
    bool parsearEntero(const string& texto, int& salida) const;
    bool parsearFlotante(const string& texto, float& salida) const;
    void calcularPosicionCiudadNueva(float& x, float& y) const;
    void seleccionarPasajeroActivoPorId(const string& idPasajero);
    void refrescarTrasCambioCiudadesORutas();

    void mostrarRutaMasRapida();
    void mostrarRutaMasBarata();
    void mostrarExplorarDestinos();
    void mostrarBusquedaCiudadABB();
    void mostrarVuelosPasajeroAVL();
    void mostrarPasajeroAVLEncontrando(const NodoAVL* nodo, const string& tituloInfo);
    void mostrarSiguientePasajeroCliente();
    void mostrarMapaRutas();

    void abrirModalClienteBuscarABB();
    void abrirModalClienteConsultarAVL();
    void abrirModalClienteReservarVuelo();
    void abrirModalCrud(int tipoOperacion);
    void abrirModalCrudCiudad(int tipoOperacion);
    void abrirModalCrudRuta(int tipoOperacion);
    void abrirModalCrudAeronave(int tipoOperacion);
    void abrirModalCrudTripulante(int tipoOperacion);
    void abrirModalCrudVuelo(int tipoOperacion);
    void abrirModalCrudPasajero(int tipoOperacion);
    void confirmarModalActivo();
    void procesarModalCliente(int accion);
    void procesarModalCiudad(int accion);
    void procesarModalRuta(int accion);
    void procesarModalAeronave(int accion);
    void procesarModalTripulante(int accion);
    void procesarModalVuelo(int accion);
    void procesarModalPasajero(int accion);
    int volcarABBRec(const NodoABB* nodo, const string& codigoResaltado);
    int volcarAVLRec(const NodoAVL* nodo, const string& idResaltado);
    void volcarMSTRec(const NodoMST* nodo, int padreVisualIdx);

    void mostrarArbolABBVisual(const string& codigoResaltado);
    void mostrarArbolAVLVisual(const string& idResaltado);
    void mostrarArbolMSTVisual(const string& titulo);

    void actualizarResumenInferiorAdmin();
    void mostrarRutaEquipajeAdmin();
    void mostrarAeronavesYCrewAdmin();
    void mostrarVuelosYPasajerosAdmin();
    void mostrarMSTAdmin(bool usarPrim);
    void mostrarSiguienteArbolAdmin();

    void ejecutarAccionCliente(int indiceAccion);
    void ejecutarAccionAdmin(int indiceAccion);
    void limpiarLineasResultado(const string& nuevoTitulo);

    void renderizar();

public:
    AplicacionVisual();
    void ejecutar();
};

#endif
