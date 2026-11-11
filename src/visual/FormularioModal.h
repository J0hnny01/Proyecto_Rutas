#ifndef FORMULARIO_MODAL_H
#define FORMULARIO_MODAL_H

#include <SFML/Graphics.hpp>
#include <string>
#include "BotonVisual.h"

using namespace std;

enum AccionModal {
    MODAL_NINGUNA = 0,
    MODAL_CLIENTE_BUSCAR_ABB,
    MODAL_CLIENTE_CONSULTAR_AVL,
    MODAL_CLIENTE_RESERVAR_VUELO,
    MODAL_CRUD_CIUDAD_AGREGAR,
    MODAL_CRUD_CIUDAD_EDITAR,
    MODAL_CRUD_CIUDAD_ELIMINAR,
    MODAL_CRUD_RUTA_AGREGAR,
    MODAL_CRUD_RUTA_EDITAR,
    MODAL_CRUD_RUTA_ELIMINAR,
    MODAL_CRUD_AERONAVE_AGREGAR,
    MODAL_CRUD_AERONAVE_EDITAR,
    MODAL_CRUD_AERONAVE_ELIMINAR,
    MODAL_CRUD_TRIPULANTE_AGREGAR,
    MODAL_CRUD_TRIPULANTE_EDITAR,
    MODAL_CRUD_TRIPULANTE_ELIMINAR,
    MODAL_CRUD_VUELO_AGREGAR,
    MODAL_CRUD_VUELO_EDITAR,
    MODAL_CRUD_VUELO_ELIMINAR,
    MODAL_CRUD_PASAJERO_AGREGAR,
    MODAL_CRUD_PASAJERO_EDITAR,
    MODAL_CRUD_PASAJERO_ELIMINAR
};

struct CampoFormulario {
    string etiqueta;
    string valor;
    bool esNumerico;
    bool permitirFlotante;
    int maxLongitud;
};

class FormularioModal {
private:
    static const int MAX_CAMPOS = 6;
    CampoFormulario campos[MAX_CAMPOS];
    int totalCampos;
    int campoActivo;
    bool visible;

    int accionId;
    string titulo;
    string subtitulo;
    string textoConfirmar;
    string mensajeError;

    float modalX;
    float modalY;
    float modalAncho;
    float modalAlto;

    BotonVisual btnConfirmar;
    BotonVisual btnCancelar;

    void recalcularGeometria();
    bool caracterValidoParaCampo(const CampoFormulario& campo, char c) const;
    void calcularRectCampo(int indice, float& x, float& y, float& ancho, float& alto) const;

public:
    FormularioModal();

    void abrir(int idAccion, const string& nuevoTitulo, const string& nuevoSubtitulo,
               const string& textoConfirmar = "Confirmar");
    void agregarCampo(const string& etiqueta, const string& valorInicial = "",
                      bool soloNumeros = false, int maxLong = 40, bool conDecimal = false);
    void cerrar();

    bool estaVisible() const;
    int getAccionId() const;
    int getTotalCampos() const;
    string getValor(int indice) const;
    void setMensajeError(const string& error);

    bool manejarClicIzquierdo(float mx, float my, bool& confirmado);
    void manejarTextoIngresado(unsigned int unicode);
    void manejarTeclaEspecial(bool esEnter, bool esBackspace, bool esTab,
                              bool esEscape, bool esArriba, bool esAbajo,
                              bool& confirmado);

    void renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const;
};

#endif
