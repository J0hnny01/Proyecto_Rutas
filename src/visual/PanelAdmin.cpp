#include "PanelAdmin.h"
#include "CompatSFML.h"

PanelAdmin::PanelAdmin()
    : modoActual(ModoPanelAdmin::FlotaTripulacion),
      paginaIzq(0),
      paginaDer(0) {
    btnIzqPagAnt.configurar(564.0f, 546.0f, 50.0f, 26.0f, "< Ant", true, 11);
    btnIzqPagSig.configurar(676.0f, 546.0f, 58.0f, 26.0f, "Sig >", true, 11);
    btnDerPagAnt.configurar(1004.0f, 546.0f, 50.0f, 26.0f, "< Ant", true, 11);
    btnDerPagSig.configurar(1116.0f, 546.0f, 58.0f, 26.0f, "Sig >", true, 11);
}

void PanelAdmin::setModo(ModoPanelAdmin nuevoModo) {
    if (modoActual != nuevoModo) {
        modoActual = nuevoModo;
        paginaDer = 0;
    }
}

ModoPanelAdmin PanelAdmin::getModo() const {
    return modoActual;
}

void PanelAdmin::resetPaginas() {
    paginaIzq = 0;
    paginaDer = 0;
}

void PanelAdmin::resetPaginaDerecha() {
    paginaDer = 0;
}

void PanelAdmin::asegurarSeleccionIzquierda(int indiceSeleccionado) {
    if (indiceSeleccionado < 0) {
        paginaIzq = 0;
        return;
    }
    paginaIzq = indiceSeleccionado / FILAS_IZQUIERDA;
}

string PanelAdmin::acortar(const string& texto, size_t maxChars) const {
    if (texto.size() <= maxChars) {
        return texto;
    }
    if (maxChars <= 3) {
        return texto.substr(0, maxChars);
    }
    return texto.substr(0, maxChars - 3) + "...";
}

string PanelAdmin::aMinuscula(const string& texto) const {
    string res = texto;
    for (size_t i = 0; i < res.size(); ++i) {
        char c = res[i];
        if (c >= 'A' && c <= 'Z') {
            res[i] = static_cast<char>(c - 'A' + 'a');
        }
    }
    return res;
}

sf::Color PanelAdmin::colorPorEstado(const string& estado) const {
    string low = aMinuscula(estado);
    if (low == "activa" || low == "activo") {
        return sf::Color(110, 220, 155);
    }
    if (low == "en vuelo") {
        return sf::Color(120, 192, 255);
    }
    if (low == "mantenimiento") {
        return sf::Color(242, 162, 108);
    }
    return sf::Color(195, 205, 220);
}

void PanelAdmin::dibujarCaja(sf::RenderWindow& ventana, float x, float y, float w, float h,
                             sf::Color fondo, sf::Color borde) const {
    sf::RectangleShape caja(sf::Vector2f(w, h));
    caja.setPosition(sf::Vector2f(x, y));
    caja.setFillColor(fondo);
    caja.setOutlineColor(borde);
    caja.setOutlineThickness(1.0f);
    ventana.draw(caja);
}

int PanelAdmin::totalPaginas(int totalItems, int porPagina) const {
    if (totalItems <= 0 || porPagina <= 0) {
        return 1;
    }
    return (totalItems + porPagina - 1) / porPagina;
}

int PanelAdmin::clampPagina(int pagina, int totalItems, int porPagina) const {
    int tot = totalPaginas(totalItems, porPagina);
    if (pagina < 0) {
        return 0;
    }
    if (pagina >= tot) {
        return tot - 1;
    }
    return pagina;
}

ListaEnlazada<Tripulante> PanelAdmin::recolectarTripulacionAeronave(const SistemaAerolinea& sistema,
                                                                    const string& placa) const {
    ListaEnlazada<Tripulante> salida;
    const ListaEnlazada<Tripulante>& tripulantes = sistema.obtenerTripulantes();
    for (int i = 0; i < tripulantes.tamano(); ++i) {
        const Tripulante& t = tripulantes.obtener(i);
        if (t.getPlacaAeronave() == placa) {
            salida.insertarAlFinal(t);
        }
    }
    return salida;
}

int PanelAdmin::contarTripulantesSinPlacaValida(const SistemaAerolinea& sistema) const {
    const ListaEnlazada<Tripulante>& tripulantes = sistema.obtenerTripulantes();
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int sinAsignar = 0;
    for (int i = 0; i < tripulantes.tamano(); ++i) {
        const string& placa = tripulantes.obtener(i).getPlacaAeronave();
        bool encontrada = false;
        if (!placa.empty()) {
            for (int j = 0; j < aeronaves.tamano(); ++j) {
                if (aeronaves.obtener(j).getPlaca() == placa) {
                    encontrada = true;
                    break;
                }
            }
        }
        if (!encontrada) {
            sinAsignar++;
        }
    }
    return sinAsignar;
}

int PanelAdmin::contarVuelosPorPlaca(const SistemaAerolinea& sistema, const string& placa) const {
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int conteo = 0;
    for (int i = 0; i < vuelos.tamano(); ++i) {
        if (vuelos.obtener(i).obtenerPlacaAeronave() == placa) {
            conteo++;
        }
    }
    return conteo;
}

ListaEnlazada<Pasajero> PanelAdmin::recolectarPasajerosVuelo(const ListaEnlazada<Pasajero>& cachePasajeros,
                                                             const string& codigoVuelo) const {
    ListaEnlazada<Pasajero> salida;
    for (int i = 0; i < cachePasajeros.tamano(); ++i) {
        const Pasajero& p = cachePasajeros.obtener(i);
        if (p.tieneVuelo(codigoVuelo)) {
            salida.insertarAlFinal(p);
        }
    }
    return salida;
}

bool PanelAdmin::buscarAeronavePorPlaca(const SistemaAerolinea& sistema, const string& placa,
                                         Aeronave& salida) const {
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    for (int i = 0; i < aeronaves.tamano(); ++i) {
        if (aeronaves.obtener(i).getPlaca() == placa) {
            salida = aeronaves.obtener(i);
            return true;
        }
    }
    return false;
}

string PanelAdmin::nombreCiudadPorCodigo(const SistemaAerolinea& sistema, const string& codigo) const {
    const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
    for (int i = 0; i < ciudades.tamano(); ++i) {
        if (ciudades.obtener(i).obtenerCodigo() == codigo) {
            return ciudades.obtener(i).obtenerNombre();
        }
    }
    return codigo;
}

void PanelAdmin::contarEstadosFlota(const SistemaAerolinea& sistema, int& activas, int& enVuelo,
                                    int& mantenimiento, int& otros) const {
    activas = 0;
    enVuelo = 0;
    mantenimiento = 0;
    otros = 0;
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    for (int i = 0; i < aeronaves.tamano(); ++i) {
        string low = aMinuscula(aeronaves.obtener(i).getEstado());
        if (low == "activa" || low == "activo") {
            activas++;
        } else if (low == "en vuelo") {
            enVuelo++;
        } else if (low == "mantenimiento") {
            mantenimiento++;
        } else {
            otros++;
        }
    }
}

void PanelAdmin::dibujarPaginador(sf::RenderWindow& ventana, const sf::Font& fuente,
                                  const BotonVisual& btnAnt, const BotonVisual& btnSig,
                                  float centroPagX, float yTexto,
                                  int paginaMostrada, int totPaginas) const {
    btnAnt.renderizar(ventana, fuente);
    string textoPag = to_string(paginaMostrada + 1) + " / " + to_string(totPaginas);
    CompatSFML::dibujarTextoCentrado(ventana, fuente, textoPag, centroPagX, yTexto, 11,
                                     sf::Color(208, 216, 230));
    btnSig.renderizar(ventana, fuente);
}

int PanelAdmin::manejarClic(float mx, float my, const SistemaAerolinea& sistema,
                            int& idxAeronave, int& idxVuelo) {
    if (mx < 290.0f || mx > 1185.0f || my < 64.0f || my > 582.0f) {
        return 0;
    }

    if (modoActual == ModoPanelAdmin::FlotaTripulacion) {
        const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
        int totalA = aeronaves.tamano();
        int pagIzqAct = clampPagina(paginaIzq, totalA, FILAS_IZQUIERDA);

        if (btnIzqPagAnt.contienePunto(mx, my)) {
            if (pagIzqAct > 0) {
                paginaIzq = pagIzqAct - 1;
                return 1;
            }
            return 0;
        }
        if (btnIzqPagSig.contienePunto(mx, my)) {
            if (pagIzqAct + 1 < totalPaginas(totalA, FILAS_IZQUIERDA)) {
                paginaIzq = pagIzqAct + 1;
                return 1;
            }
            return 0;
        }

        if (totalA > 0) {
            int idxSel = ((idxAeronave % totalA) + totalA) % totalA;
            ListaEnlazada<Tripulante> tripAero =
                recolectarTripulacionAeronave(sistema, aeronaves.obtener(idxSel).getPlaca());
            int totalT = tripAero.tamano();
            int pagDerAct = clampPagina(paginaDer, totalT, FILAS_DERECHA);

            if (btnDerPagAnt.contienePunto(mx, my)) {
                if (pagDerAct > 0) {
                    paginaDer = pagDerAct - 1;
                    return 1;
                }
                return 0;
            }
            if (btnDerPagSig.contienePunto(mx, my)) {
                if (pagDerAct + 1 < totalPaginas(totalT, FILAS_DERECHA)) {
                    paginaDer = pagDerAct + 1;
                    return 1;
                }
                return 0;
            }
        }

        if (mx >= 298.0f && mx <= 738.0f && totalA > 0) {
            for (int fila = 0; fila < FILAS_IZQUIERDA; ++fila) {
                float yFila = 188.0f + static_cast<float>(fila) * 38.0f;
                if (my >= yFila && my <= yFila + 34.0f) {
                    int idxGlobal = pagIzqAct * FILAS_IZQUIERDA + fila;
                    if (idxGlobal < totalA) {
                        idxAeronave = idxGlobal;
                        paginaDer = 0;
                        return 4;
                    }
                }
            }
        }
        return 0;
    }

    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int totalV = vuelos.tamano();
    int pagIzqAct = clampPagina(paginaIzq, totalV, FILAS_IZQUIERDA);

    if (btnIzqPagAnt.contienePunto(mx, my)) {
        if (pagIzqAct > 0) {
            paginaIzq = pagIzqAct - 1;
            return 1;
        }
        return 0;
    }
    if (btnIzqPagSig.contienePunto(mx, my)) {
        if (pagIzqAct + 1 < totalPaginas(totalV, FILAS_IZQUIERDA)) {
            paginaIzq = pagIzqAct + 1;
            return 1;
        }
        return 0;
    }

    if (totalV > 0) {
        int idxSel = ((idxVuelo % totalV) + totalV) % totalV;
        ListaEnlazada<Pasajero> cachePax = sistema.obtenerListaPasajeros();
        ListaEnlazada<Pasajero> paxVuelo =
            recolectarPasajerosVuelo(cachePax, vuelos.obtener(idxSel).obtenerCodigoVuelo());
        int totalP = paxVuelo.tamano();
        int pagDerAct = clampPagina(paginaDer, totalP, FILAS_DERECHA);

        if (btnDerPagAnt.contienePunto(mx, my)) {
            if (pagDerAct > 0) {
                paginaDer = pagDerAct - 1;
                return 1;
            }
            return 0;
        }
        if (btnDerPagSig.contienePunto(mx, my)) {
            if (pagDerAct + 1 < totalPaginas(totalP, FILAS_DERECHA)) {
                paginaDer = pagDerAct + 1;
                return 1;
            }
            return 0;
        }
    }

    if (mx >= 298.0f && mx <= 738.0f && totalV > 0) {
        for (int fila = 0; fila < FILAS_IZQUIERDA; ++fila) {
            float yFila = 188.0f + static_cast<float>(fila) * 38.0f;
            if (my >= yFila && my <= yFila + 34.0f) {
                int idxGlobal = pagIzqAct * FILAS_IZQUIERDA + fila;
                if (idxGlobal < totalV) {
                    idxVuelo = idxGlobal;
                    paginaDer = 0;
                    return 5;
                }
            }
        }
    }
    return 0;
}

void PanelAdmin::renderizarFlota(sf::RenderWindow& ventana, const sf::Font& fuente,
                                 const SistemaAerolinea& sistema, int idxAeronave) const {
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int totalA = aeronaves.tamano();
    int totalT = sistema.obtenerTripulantes().tamano();

    int activas = 0;
    int enVuelo = 0;
    int mantenimiento = 0;
    int otros = 0;
    contarEstadosFlota(sistema, activas, enVuelo, mantenimiento, otros);

    dibujarCaja(ventana, 290.0f, 64.0f, 895.0f, 76.0f,
                sf::Color(30, 34, 42), sf::Color(55, 68, 90));
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Flota de Aeronaves y Tripulacion Asignada   (" +
                             to_string(totalA) + " aeronaves  |  " + to_string(totalT) + " tripulantes)",
                             306.0f, 71.0f, 15, sf::Color(135, 192, 250));
    string lineaEstados = "Estados -> Activa: " + to_string(activas) +
                          "  |  En vuelo: " + to_string(enVuelo) +
                          "  |  Mantenimiento: " + to_string(mantenimiento) +
                          "  |  Otros: " + to_string(otros);
    CompatSFML::dibujarTexto(ventana, fuente, lineaEstados, 306.0f, 95.0f, 12, sf::Color(115, 220, 165));
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Selecciona una fila en la tabla izquierda para inspeccionar su ficha y tripulacion asignada.",
                             306.0f, 116.0f, 12, sf::Color(198, 208, 220));

    dibujarCaja(ventana, 290.0f, 150.0f, 455.0f, 432.0f,
                sf::Color(24, 27, 33), sf::Color(48, 55, 68));
    dibujarCaja(ventana, 291.0f, 151.0f, 453.0f, 30.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "Placa", 302.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Marca / Modelo", 394.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Cap", 538.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Estado", 580.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Trip", 692.0f, 158.0f, 12, sf::Color(145, 180, 228));

    int pagIzqAct = clampPagina(paginaIzq, totalA, FILAS_IZQUIERDA);
    int totPagIzq = totalPaginas(totalA, FILAS_IZQUIERDA);
    int idxSel = (totalA > 0) ? (((idxAeronave % totalA) + totalA) % totalA) : -1;

    if (totalA == 0) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Sin aeronaves en flota. Agregalas en Mantenimientos.",
                                         517.0f, 345.0f, 13, sf::Color(150, 160, 175));
    } else {
        int inicio = pagIzqAct * FILAS_IZQUIERDA;
        int fin = inicio + FILAS_IZQUIERDA;
        if (fin > totalA) {
            fin = totalA;
        }
        for (int i = inicio; i < fin; ++i) {
            int filaLocal = i - inicio;
            float yFila = 188.0f + static_cast<float>(filaLocal) * 38.0f;
            bool sel = (i == idxSel);
            sf::Color fondo = sel ? sf::Color(46, 86, 146) :
                              ((filaLocal % 2 == 0) ? sf::Color(29, 33, 41) : sf::Color(26, 29, 36));
            sf::Color borde = sel ? sf::Color(92, 152, 232) : sf::Color(38, 43, 52);
            dibujarCaja(ventana, 298.0f, yFila, 439.0f, 34.0f, fondo, borde);

            const Aeronave& a = aeronaves.obtener(i);
            int tripCnt = recolectarTripulacionAeronave(sistema, a.getPlaca()).tamano();
            sf::Color colTxt = sel ? sf::Color(248, 250, 255) : sf::Color(214, 220, 230);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(a.getPlaca(), 10), 304.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(a.getMarca() + " " + a.getModelo(), 17),
                                     394.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, to_string(a.getCapacidad()), 538.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(a.getEstado(), 13),
                                     580.0f, yFila + 9.0f, 12, sel ? colTxt : colorPorEstado(a.getEstado()));
            CompatSFML::dibujarTexto(ventana, fuente, to_string(tripCnt), 696.0f, yFila + 9.0f, 12, colTxt);
        }
    }

    int iniMos = (totalA == 0) ? 0 : (pagIzqAct * FILAS_IZQUIERDA + 1);
    int finMos = (totalA == 0) ? 0 : ((pagIzqAct + 1) * FILAS_IZQUIERDA);
    if (finMos > totalA) {
        finMos = totalA;
    }
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Mostrando " + to_string(iniMos) + "-" + to_string(finMos) +
                             " de " + to_string(totalA),
                             302.0f, 552.0f, 11, sf::Color(145, 155, 172));
    dibujarPaginador(ventana, fuente, btnIzqPagAnt, btnIzqPagSig, 645.0f, 552.0f, pagIzqAct, totPagIzq);

    dibujarCaja(ventana, 755.0f, 150.0f, 430.0f, 432.0f,
                sf::Color(24, 27, 33), sf::Color(48, 55, 68));
    dibujarCaja(ventana, 756.0f, 151.0f, 428.0f, 30.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "Detalle de aeronave y tripulacion asignada",
                             768.0f, 158.0f, 12, sf::Color(145, 180, 228));

    int sinFlota = contarTripulantesSinPlacaValida(sistema);
    if (idxSel == -1) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Sin aeronave seleccionada.",
                                         970.0f, 345.0f, 13, sf::Color(150, 160, 175));
        CompatSFML::dibujarTexto(ventana, fuente,
                                 "Tripulantes registrados: " + to_string(totalT) +
                                 " | Sin flota: " + to_string(sinFlota),
                                 768.0f, 552.0f, 11, sf::Color(145, 155, 172));
        dibujarPaginador(ventana, fuente, btnDerPagAnt, btnDerPagSig, 1085.0f, 552.0f, 0, 1);
        return;
    }

    const Aeronave& aeroSel = aeronaves.obtener(idxSel);
    ListaEnlazada<Tripulante> tripAero = recolectarTripulacionAeronave(sistema, aeroSel.getPlaca());
    int totalTripAero = tripAero.tamano();
    int vuelosAero = contarVuelosPorPlaca(sistema, aeroSel.getPlaca());

    dibujarCaja(ventana, 766.0f, 188.0f, 408.0f, 68.0f,
                sf::Color(30, 35, 44), sf::Color(56, 68, 88));
    CompatSFML::dibujarTexto(ventana, fuente,
                             acortar(aeroSel.getPlaca() + "  -  " + aeroSel.getMarca() + " " + aeroSel.getModelo(), 44),
                             776.0f, 194.0f, 14, sf::Color(235, 240, 248));
    CompatSFML::dibujarTexto(ventana, fuente, "Estado: " + aeroSel.getEstado(),
                             776.0f, 217.0f, 12, colorPorEstado(aeroSel.getEstado()));
    CompatSFML::dibujarTexto(ventana, fuente,
                             "|  Cap: " + to_string(aeroSel.getCapacidad()) + " asientos  |  Vuelos: " +
                             to_string(vuelosAero),
                             932.0f, 217.0f, 12, sf::Color(198, 208, 222));
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Tripulantes asignados a esta aeronave: " + to_string(totalTripAero),
                             776.0f, 236.0f, 12, sf::Color(145, 195, 245));

    dibujarCaja(ventana, 766.0f, 264.0f, 408.0f, 26.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "ID", 776.0f, 270.0f, 11, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Nombre completo", 862.0f, 270.0f, 11, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Rol", 1052.0f, 270.0f, 11, sf::Color(145, 180, 228));

    int pagDerAct = clampPagina(paginaDer, totalTripAero, FILAS_DERECHA);
    int totPagDer = totalPaginas(totalTripAero, FILAS_DERECHA);

    if (totalTripAero == 0) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Sin tripulacion asignada a esta placa.",
                                         970.0f, 412.0f, 13, sf::Color(150, 160, 175));
    } else {
        int iniT = pagDerAct * FILAS_DERECHA;
        int finT = iniT + FILAS_DERECHA;
        if (finT > totalTripAero) {
            finT = totalTripAero;
        }
        for (int i = iniT; i < finT; ++i) {
            int filaLocal = i - iniT;
            float yT = 296.0f + static_cast<float>(filaLocal) * 34.0f;
            dibujarCaja(ventana, 766.0f, yT, 408.0f, 30.0f,
                        (filaLocal % 2 == 0) ? sf::Color(29, 33, 41) : sf::Color(26, 29, 36),
                        sf::Color(38, 43, 52));
            const Tripulante& t = tripAero.obtener(i);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(t.getId(), 9),
                                     776.0f, yT + 7.0f, 12, sf::Color(218, 226, 238));
            CompatSFML::dibujarTexto(ventana, fuente, acortar(t.getNombre(), 22),
                                     862.0f, yT + 7.0f, 12, sf::Color(214, 220, 230));
            CompatSFML::dibujarTexto(ventana, fuente, acortar(t.getRol(), 15),
                                     1052.0f, yT + 7.0f, 12, sf::Color(135, 212, 168));
        }
    }

    CompatSFML::dibujarTexto(ventana, fuente,
                             "Total crew: " + to_string(totalT) + " | Sin flota valida: " + to_string(sinFlota),
                             768.0f, 552.0f, 11, sf::Color(145, 155, 172));
    dibujarPaginador(ventana, fuente, btnDerPagAnt, btnDerPagSig, 1085.0f, 552.0f, pagDerAct, totPagDer);
}

void PanelAdmin::renderizarVuelos(sf::RenderWindow& ventana, const sf::Font& fuente,
                                  const SistemaAerolinea& sistema, int idxVuelo) const {
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    ListaEnlazada<Pasajero> cachePax = sistema.obtenerListaPasajeros();
    int totalV = vuelos.tamano();
    int totalPax = cachePax.tamano();

    dibujarCaja(ventana, 290.0f, 64.0f, 895.0f, 76.0f,
                sf::Color(30, 34, 42), sf::Color(55, 68, 90));
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Vuelos Programados y Pasajeros (Arbol AVL)   (" +
                             to_string(totalV) + " vuelos  |  " + to_string(totalPax) + " pasajeros)",
                             306.0f, 71.0f, 15, sf::Color(135, 192, 250));

    int idxSel = (totalV > 0) ? (((idxVuelo % totalV) + totalV) % totalV) : -1;
    if (idxSel == -1) {
        CompatSFML::dibujarTexto(ventana, fuente,
                                 "Sin vuelos programados actualmente en el sistema.",
                                 306.0f, 95.0f, 12, sf::Color(220, 175, 125));
    } else {
        const Vuelo& vSel = vuelos.obtener(idxSel);
        string subAct = "Activo: " + vSel.obtenerCodigoVuelo() + " (" + vSel.obtenerOrigenCodigo() +
                        " -> " + vSel.obtenerDestinoCodigo() + ")  |  Aeronave: " +
                        vSel.obtenerPlacaAeronave() + "  |  Tarifa: $" + to_string(vSel.obtenerCosto()) +
                        "  |  " + to_string(vSel.obtenerTiempoMin()) + " min";
        CompatSFML::dibujarTexto(ventana, fuente, acortar(subAct, 112),
                                 306.0f, 95.0f, 12, sf::Color(115, 220, 165));
    }
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Selecciona un vuelo (izquierda) para ver su ocupacion y pasajeros desde el AVL.",
                             306.0f, 116.0f, 12, sf::Color(198, 208, 220));

    dibujarCaja(ventana, 290.0f, 150.0f, 455.0f, 432.0f,
                sf::Color(24, 27, 33), sf::Color(48, 55, 68));
    dibujarCaja(ventana, 291.0f, 151.0f, 453.0f, 30.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "Vuelo", 302.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Tramo", 378.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Aeronave", 482.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Tarifa", 586.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Min", 646.0f, 158.0f, 12, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Pax", 696.0f, 158.0f, 12, sf::Color(145, 180, 228));

    int pagIzqAct = clampPagina(paginaIzq, totalV, FILAS_IZQUIERDA);
    int totPagIzq = totalPaginas(totalV, FILAS_IZQUIERDA);

    if (totalV == 0) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Sin vuelos programados. Agregalos en Mantenimientos.",
                                         517.0f, 345.0f, 13, sf::Color(150, 160, 175));
    } else {
        int inicio = pagIzqAct * FILAS_IZQUIERDA;
        int fin = inicio + FILAS_IZQUIERDA;
        if (fin > totalV) {
            fin = totalV;
        }
        for (int i = inicio; i < fin; ++i) {
            int filaLocal = i - inicio;
            float yFila = 188.0f + static_cast<float>(filaLocal) * 38.0f;
            bool sel = (i == idxSel);
            sf::Color fondo = sel ? sf::Color(46, 86, 146) :
                              ((filaLocal % 2 == 0) ? sf::Color(29, 33, 41) : sf::Color(26, 29, 36));
            sf::Color borde = sel ? sf::Color(92, 152, 232) : sf::Color(38, 43, 52);
            dibujarCaja(ventana, 298.0f, yFila, 439.0f, 34.0f, fondo, borde);

            const Vuelo& v = vuelos.obtener(i);
            int paxCnt = recolectarPasajerosVuelo(cachePax, v.obtenerCodigoVuelo()).tamano();
            sf::Color colTxt = sel ? sf::Color(248, 250, 255) : sf::Color(214, 220, 230);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(v.obtenerCodigoVuelo(), 8),
                                     302.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente,
                                     acortar(v.obtenerOrigenCodigo() + "->" + v.obtenerDestinoCodigo(), 11),
                                     378.0f, yFila + 9.0f, 12,
                                     sel ? colTxt : sf::Color(135, 210, 168));
            CompatSFML::dibujarTexto(ventana, fuente, acortar(v.obtenerPlacaAeronave(), 11),
                                     482.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, "$" + to_string(v.obtenerCosto()),
                                     586.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, to_string(v.obtenerTiempoMin()) + "m",
                                     646.0f, yFila + 9.0f, 12, colTxt);
            CompatSFML::dibujarTexto(ventana, fuente, to_string(paxCnt),
                                     700.0f, yFila + 9.0f, 12,
                                     sel ? colTxt : sf::Color(145, 195, 250));
        }
    }

    int iniMos = (totalV == 0) ? 0 : (pagIzqAct * FILAS_IZQUIERDA + 1);
    int finMos = (totalV == 0) ? 0 : ((pagIzqAct + 1) * FILAS_IZQUIERDA);
    if (finMos > totalV) {
        finMos = totalV;
    }
    CompatSFML::dibujarTexto(ventana, fuente,
                             "Mostrando " + to_string(iniMos) + "-" + to_string(finMos) +
                             " de " + to_string(totalV),
                             302.0f, 552.0f, 11, sf::Color(145, 155, 172));
    dibujarPaginador(ventana, fuente, btnIzqPagAnt, btnIzqPagSig, 645.0f, 552.0f, pagIzqAct, totPagIzq);

    dibujarCaja(ventana, 755.0f, 150.0f, 430.0f, 432.0f,
                sf::Color(24, 27, 33), sf::Color(48, 55, 68));
    dibujarCaja(ventana, 756.0f, 151.0f, 428.0f, 30.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "Detalle del vuelo y pasajeros (AVL)",
                             768.0f, 158.0f, 12, sf::Color(145, 180, 228));

    int altAVL = sistema.obtenerArbolPasajeros().getAltura();
    if (idxSel == -1) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Sin vuelo seleccionado.",
                                         970.0f, 345.0f, 13, sf::Color(150, 160, 175));
        CompatSFML::dibujarTexto(ventana, fuente,
                                 "Pasajeros en AVL: " + to_string(totalPax) + " | Altura AVL: " + to_string(altAVL),
                                 768.0f, 552.0f, 11, sf::Color(145, 155, 172));
        dibujarPaginador(ventana, fuente, btnDerPagAnt, btnDerPagSig, 1085.0f, 552.0f, 0, 1);
        return;
    }

    const Vuelo& vSel = vuelos.obtener(idxSel);
    ListaEnlazada<Pasajero> paxVuelo = recolectarPasajerosVuelo(cachePax, vSel.obtenerCodigoVuelo());
    int totalPaxVuelo = paxVuelo.tamano();

    int totalMaletasVuelo = 0;
    for (int i = 0; i < totalPaxVuelo; ++i) {
        totalMaletasVuelo += paxVuelo.obtener(i).getCantidadMaletas();
    }

    Aeronave aeroInfo;
    bool tieneAero = buscarAeronavePorPlaca(sistema, vSel.obtenerPlacaAeronave(), aeroInfo);
    string infoAero = tieneAero
        ? (aeroInfo.getPlaca() + " (" + aeroInfo.getEstado() + ")")
        : (vSel.obtenerPlacaAeronave() + " (no encontrada)");
    string infoOcup = to_string(totalPaxVuelo) +
                      (tieneAero ? (" / " + to_string(aeroInfo.getCapacidad()) + " asientos") : " pasajero(s)");

    string nomO = nombreCiudadPorCodigo(sistema, vSel.obtenerOrigenCodigo());
    string nomD = nombreCiudadPorCodigo(sistema, vSel.obtenerDestinoCodigo());

    dibujarCaja(ventana, 766.0f, 188.0f, 408.0f, 68.0f,
                sf::Color(30, 35, 44), sf::Color(56, 68, 88));
    CompatSFML::dibujarTexto(ventana, fuente,
                             acortar(vSel.obtenerCodigoVuelo() + ": " + nomO + " -> " + nomD, 44),
                             776.0f, 194.0f, 14, sf::Color(235, 240, 248));
    CompatSFML::dibujarTexto(ventana, fuente, acortar("Aeronave: " + infoAero, 48),
                             776.0f, 217.0f, 12,
                             tieneAero ? colorPorEstado(aeroInfo.getEstado()) : sf::Color(220, 175, 125));
    CompatSFML::dibujarTexto(ventana, fuente,
                             acortar("Ocupacion: " + infoOcup + "  |  Maletas: " + to_string(totalMaletasVuelo), 52),
                             776.0f, 236.0f, 12, sf::Color(145, 195, 245));

    dibujarCaja(ventana, 766.0f, 264.0f, 408.0f, 26.0f,
                sf::Color(32, 37, 46), sf::Color(48, 55, 68));
    CompatSFML::dibujarTexto(ventana, fuente, "ID", 776.0f, 270.0f, 11, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Pasajero", 854.0f, 270.0f, 11, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Maletas", 1028.0f, 270.0f, 11, sf::Color(145, 180, 228));
    CompatSFML::dibujarTexto(ventana, fuente, "Vuelos", 1094.0f, 270.0f, 11, sf::Color(145, 180, 228));

    int pagDerAct = clampPagina(paginaDer, totalPaxVuelo, FILAS_DERECHA);
    int totPagDer = totalPaginas(totalPaxVuelo, FILAS_DERECHA);

    if (totalPaxVuelo == 0) {
        CompatSFML::dibujarTextoCentrado(ventana, fuente,
                                         "Ningun pasajero del AVL asignado a este vuelo.",
                                         970.0f, 412.0f, 13, sf::Color(150, 160, 175));
    } else {
        int iniP = pagDerAct * FILAS_DERECHA;
        int finP = iniP + FILAS_DERECHA;
        if (finP > totalPaxVuelo) {
            finP = totalPaxVuelo;
        }
        for (int i = iniP; i < finP; ++i) {
            int filaLocal = i - iniP;
            float yP = 296.0f + static_cast<float>(filaLocal) * 34.0f;
            dibujarCaja(ventana, 766.0f, yP, 408.0f, 30.0f,
                        (filaLocal % 2 == 0) ? sf::Color(29, 33, 41) : sf::Color(26, 29, 36),
                        sf::Color(38, 43, 52));
            const Pasajero& p = paxVuelo.obtener(i);
            CompatSFML::dibujarTexto(ventana, fuente, acortar(p.getId(), 8),
                                     776.0f, yP + 7.0f, 12, sf::Color(218, 226, 238));
            CompatSFML::dibujarTexto(ventana, fuente, acortar(p.getNombre(), 20),
                                     854.0f, yP + 7.0f, 12, sf::Color(214, 220, 230));
            CompatSFML::dibujarTexto(ventana, fuente, to_string(p.getCantidadMaletas()),
                                     1048.0f, yP + 7.0f, 12, sf::Color(200, 208, 222));
            CompatSFML::dibujarTexto(ventana, fuente, acortar(p.getVuelosTexto(), 9),
                                     1094.0f, yP + 7.0f, 11, sf::Color(135, 212, 168));
        }
    }

    CompatSFML::dibujarTexto(ventana, fuente,
                             "Orden Inorden AVL | Altura AVL: " + to_string(altAVL),
                             768.0f, 552.0f, 11, sf::Color(145, 155, 172));
    dibujarPaginador(ventana, fuente, btnDerPagAnt, btnDerPagSig, 1085.0f, 552.0f, pagDerAct, totPagDer);
}

void PanelAdmin::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente,
                            const SistemaAerolinea& sistema,
                            int idxAeronave, int idxVuelo) const {
    if (modoActual == ModoPanelAdmin::FlotaTripulacion) {
        renderizarFlota(ventana, fuente, sistema, idxAeronave);
    } else {
        renderizarVuelos(ventana, fuente, sistema, idxVuelo);
    }
}
