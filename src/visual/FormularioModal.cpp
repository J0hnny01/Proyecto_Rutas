#include "FormularioModal.h"
#include "CompatSFML.h"

FormularioModal::FormularioModal()
    : totalCampos(0),
      campoActivo(0),
      visible(false),
      accionId(MODAL_NINGUNA),
      textoConfirmar("Confirmar"),
      modalX(457.0f),
      modalY(160.0f),
      modalAncho(560.0f),
      modalAlto(260.0f) {
    btnConfirmar.setColores(sf::Color(42, 118, 80), sf::Color(42, 118, 80));
    btnCancelar.setColores(sf::Color(54, 60, 72), sf::Color(54, 60, 72));
}

void FormularioModal::recalcularGeometria() {
    float altoCampos = static_cast<float>(totalCampos) * 48.0f;
    modalAlto = 152.0f + altoCampos;
    modalAncho = 560.0f;
    modalX = 457.0f;
    modalY = (720.0f - modalAlto) * 0.5f;
    if (modalY < 60.0f) {
        modalY = 60.0f;
    }

    float yBotones = modalY + modalAlto - 48.0f;
    btnConfirmar.configurar(modalX + modalAncho - 244.0f, yBotones, 108.0f, 32.0f,
                            textoConfirmar, true, 13);
    btnConfirmar.setColores(sf::Color(42, 118, 80), sf::Color(42, 118, 80));
    btnConfirmar.setActivo(true);

    btnCancelar.configurar(modalX + modalAncho - 126.0f, yBotones, 102.0f, 32.0f,
                           "Cancelar", true, 13);
    btnCancelar.setColores(sf::Color(54, 60, 72), sf::Color(54, 60, 72));
}

void FormularioModal::abrir(int idAccion, const string& nuevoTitulo,
                            const string& nuevoSubtitulo, const string& botonConfirmarTexto) {
    accionId = idAccion;
    titulo = nuevoTitulo;
    subtitulo = nuevoSubtitulo;
    textoConfirmar = botonConfirmarTexto.empty() ? "Confirmar" : botonConfirmarTexto;
    mensajeError = "";
    totalCampos = 0;
    campoActivo = 0;
    visible = true;
    recalcularGeometria();
}

void FormularioModal::agregarCampo(const string& etiqueta, const string& valorInicial,
                                   bool soloNumeros, int maxLong, bool conDecimal) {
    if (totalCampos >= MAX_CAMPOS) {
        return;
    }
    campos[totalCampos].etiqueta = etiqueta;
    campos[totalCampos].valor = valorInicial;
    campos[totalCampos].esNumerico = soloNumeros;
    campos[totalCampos].permitirFlotante = conDecimal;
    campos[totalCampos].maxLongitud = (maxLong > 0) ? maxLong : 40;
    totalCampos++;
    recalcularGeometria();
}

void FormularioModal::cerrar() {
    visible = false;
    accionId = MODAL_NINGUNA;
    totalCampos = 0;
    campoActivo = 0;
    mensajeError = "";
}

bool FormularioModal::estaVisible() const {
    return visible;
}

int FormularioModal::getAccionId() const {
    return accionId;
}

int FormularioModal::getTotalCampos() const {
    return totalCampos;
}

string FormularioModal::getValor(int indice) const {
    if (indice < 0 || indice >= totalCampos) {
        return "";
    }
    return campos[indice].valor;
}

void FormularioModal::setMensajeError(const string& error) {
    mensajeError = error;
}

bool FormularioModal::caracterValidoParaCampo(const CampoFormulario& campo, char c) const {
    if (c == ';') {
        return false;
    }
    if (!campo.esNumerico) {
        return true;
    }
    if (c >= '0' && c <= '9') {
        return true;
    }
    if (campo.permitirFlotante && c == '.') {
        for (size_t i = 0; i < campo.valor.size(); ++i) {
            if (campo.valor[i] == '.') {
                return false;
            }
        }
        return true;
    }
    return false;
}

void FormularioModal::calcularRectCampo(int indice, float& x, float& y, float& ancho, float& alto) const {
    x = modalX + 24.0f;
    y = modalY + 86.0f + static_cast<float>(indice) * 48.0f;
    ancho = modalAncho - 48.0f;
    alto = 28.0f;
}

bool FormularioModal::manejarClicIzquierdo(float mx, float my, bool& confirmado) {
    confirmado = false;
    if (!visible) {
        return false;
    }

    if (btnConfirmar.contienePunto(mx, my)) {
        confirmado = true;
        return true;
    }
    if (btnCancelar.contienePunto(mx, my)) {
        cerrar();
        return true;
    }

    for (int i = 0; i < totalCampos; ++i) {
        float x = 0.0f;
        float y = 0.0f;
        float ancho = 0.0f;
        float alto = 0.0f;
        calcularRectCampo(i, x, y, ancho, alto);
        if (mx >= x && mx <= x + ancho && my >= y && my <= y + alto) {
            campoActivo = i;
            if (mx >= x + ancho - 24.0f) {
                campos[i].valor = "";
                mensajeError = "";
            }
            return true;
        }
    }

    return true;
}

void FormularioModal::manejarTextoIngresado(unsigned int unicode) {
    if (!visible || totalCampos <= 0 || campoActivo < 0 || campoActivo >= totalCampos) {
        return;
    }
    if (unicode < 32 || unicode >= 127) {
        return;
    }
    char c = static_cast<char>(unicode);
    CampoFormulario& campo = campos[campoActivo];
    if (static_cast<int>(campo.valor.size()) >= campo.maxLongitud) {
        return;
    }
    if (caracterValidoParaCampo(campo, c)) {
        campo.valor += c;
        mensajeError = "";
    }
}

void FormularioModal::manejarTeclaEspecial(bool esEnter, bool esBackspace, bool esTab,
                                           bool esEscape, bool esArriba, bool esAbajo,
                                           bool& confirmado) {
    confirmado = false;
    if (!visible) {
        return;
    }
    if (esEscape) {
        cerrar();
        return;
    }
    if (esEnter) {
        confirmado = true;
        return;
    }
    if (totalCampos > 0 && campoActivo >= 0 && campoActivo < totalCampos) {
        if (esBackspace) {
            if (!campos[campoActivo].valor.empty()) {
                campos[campoActivo].valor.pop_back();
                mensajeError = "";
            }
            return;
        }
        if (esTab || esAbajo) {
            campoActivo = (campoActivo + 1) % totalCampos;
            return;
        }
        if (esArriba) {
            campoActivo = (campoActivo - 1 + totalCampos) % totalCampos;
            return;
        }
    }
}

void FormularioModal::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    if (!visible) {
        return;
    }

    sf::RectangleShape fondoOscuro(sf::Vector2f(1200.0f, 720.0f));
    fondoOscuro.setFillColor(sf::Color(10, 12, 16, 178));
    ventana.draw(fondoOscuro);

    sf::RectangleShape cajaModal(sf::Vector2f(modalAncho, modalAlto));
    cajaModal.setPosition(sf::Vector2f(modalX, modalY));
    cajaModal.setFillColor(sf::Color(30, 34, 42));
    cajaModal.setOutlineColor(sf::Color(72, 86, 112));
    cajaModal.setOutlineThickness(1.5f);
    ventana.draw(cajaModal);

    CompatSFML::dibujarTexto(ventana, fuente, titulo,
                             modalX + 24.0f, modalY + 16.0f, 16, sf::Color(235, 239, 246));
    CompatSFML::dibujarTexto(ventana, fuente, subtitulo,
                             modalX + 24.0f, modalY + 40.0f, 12, sf::Color(152, 162, 178));

    for (int i = 0; i < totalCampos; ++i) {
        float x = 0.0f;
        float y = 0.0f;
        float ancho = 0.0f;
        float alto = 0.0f;
        calcularRectCampo(i, x, y, ancho, alto);

        CompatSFML::dibujarTexto(ventana, fuente, campos[i].etiqueta,
                                 x, y - 18.0f, 12, sf::Color(196, 204, 218));

        sf::RectangleShape cajaInput(sf::Vector2f(ancho, alto));
        cajaInput.setPosition(sf::Vector2f(x, y));
        cajaInput.setFillColor(sf::Color(20, 23, 29));
        cajaInput.setOutlineColor((i == campoActivo) ? sf::Color(86, 148, 240) : sf::Color(58, 66, 80));
        cajaInput.setOutlineThickness((i == campoActivo) ? 1.8f : 1.0f);
        ventana.draw(cajaInput);

        string textoVisible = campos[i].valor;
        if (i == campoActivo) {
            textoVisible += "_";
        }
        if (textoVisible.size() > 50) {
            textoVisible = "..." + textoVisible.substr(textoVisible.size() - 47);
        }

        CompatSFML::dibujarTexto(ventana, fuente, textoVisible,
                                 x + 10.0f, y + 6.0f, 13, sf::Color(238, 242, 248));

        if (!campos[i].valor.empty()) {
            CompatSFML::dibujarTextoCentrado(ventana, fuente, "x",
                                             x + ancho - 12.0f, y + alto * 0.5f - 1.0f,
                                             12, sf::Color(145, 155, 172));
        }
    }

    if (!mensajeError.empty()) {
        CompatSFML::dibujarTexto(ventana, fuente, mensajeError,
                                 modalX + 24.0f, modalY + modalAlto - 76.0f,
                                 12, sf::Color(240, 108, 108));
    } else {
        CompatSFML::dibujarTexto(ventana, fuente, "Tab: siguiente campo | Enter: confirmar | Esc: cancelar",
                                 modalX + 24.0f, modalY + modalAlto - 76.0f,
                                 11, sf::Color(130, 140, 156));
    }

    btnConfirmar.renderizar(ventana, fuente);
    btnCancelar.renderizar(ventana, fuente);
}
