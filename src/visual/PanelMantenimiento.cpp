#include "PanelMantenimiento.h"
#include "CompatSFML.h"

PanelMantenimiento::PanelMantenimiento()
    : entidadActiva(0),
      indiceSeleccionado(-1),
      paginaActual(0) {
    btnPagAnt.configurar(1004.0f, 549.0f, 30.0f, 26.0f, "<", true, 12);
    btnPagSig.configurar(1142.0f, 549.0f, 30.0f, 26.0f, ">", true, 12);
}

string PanelMantenimiento::acortar(const string& texto, size_t maxChars) const {
    if (maxChars <= 3 || texto.size() <= maxChars) {
        return texto;
    }
    return texto.substr(0, maxChars - 3) + "...";
}

int PanelMantenimiento::totalRegistrosPorEntidad(const SistemaAerolinea& sistema, int entidad,
                                                  const ListaEnlazada<Pasajero>& cachePasajeros) const {
    switch (entidad) {
        case 0: return sistema.obtenerCiudades().tamano();
        case 1: return sistema.obtenerRutas().tamano();
        case 2: return sistema.obtenerAeronaves().tamano();
        case 3: return sistema.obtenerTripulantes().tamano();
        case 4: return sistema.obtenerVuelos().tamano();
        case 5: return cachePasajeros.tamano();
        default: return 0;
    }
}

void PanelMantenimiento::setEntidad(int nuevaEntidad, const SistemaAerolinea& sistema) {
    entidadActiva = ((nuevaEntidad % 6) + 6) % 6;
    paginaActual = 0;
    int total = getTotalRegistros(sistema);
    indiceSeleccionado = (total > 0) ? 0 : -1;
}

void PanelMantenimiento::ajustarSeleccion(int nuevoIndice, const SistemaAerolinea& sistema) {
    int total = getTotalRegistros(sistema);
    if (total <= 0) {
        indiceSeleccionado = -1;
        paginaActual = 0;
        return;
    }
    if (nuevoIndice < 0) {
        nuevoIndice = 0;
    }
    if (nuevoIndice >= total) {
        nuevoIndice = total - 1;
    }
    indiceSeleccionado = nuevoIndice;
    paginaActual = indiceSeleccionado / FILAS_POR_PAGINA;
}

int PanelMantenimiento::getEntidad() const {
    return entidadActiva;
}

int PanelMantenimiento::getIndiceSeleccionado() const {
    return indiceSeleccionado;
}

int PanelMantenimiento::getPaginaActual() const {
    return paginaActual;
}

int PanelMantenimiento::getTotalRegistros(const SistemaAerolinea& sistema) const {
    if (entidadActiva == 5) {
        return sistema.obtenerArbolPasajeros().getCantidad();
    }
    ListaEnlazada<Pasajero> vacio;
    return totalRegistrosPorEntidad(sistema, entidadActiva, vacio);
}

int PanelMantenimiento::getTotalPaginas(const SistemaAerolinea& sistema) const {
    int total = getTotalRegistros(sistema);
    if (total <= 0) {
        return 1;
    }
    return (total + FILAS_POR_PAGINA - 1) / FILAS_POR_PAGINA;
}

string PanelMantenimiento::getNombreSingular(int entidad) const {
    const char* nombres[6] = {
        "Ciudad",
        "Ruta",
        "Aeronave",
        "Tripulante",
        "Vuelo",
        "Pasajero"
    };
    return nombres[((entidad % 6) + 6) % 6];
}

string PanelMantenimiento::getNombrePlural(int entidad) const {
    const char* nombres[6] = {
        "Ciudades",
        "Rutas",
        "Aeronaves",
        "Tripulacion",
        "Vuelos",
        "Pasajeros"
    };
    return nombres[((entidad % 6) + 6) % 6];
}

string PanelMantenimiento::getArchivoDatos(int entidad) const {
    const char* archivos[6] = {
        "ciudades.txt",
        "rutas.txt",
        "aeronaves.txt",
        "tripulacion.txt",
        "vuelos.txt",
        "pasajeros.txt"
    };
    return archivos[((entidad % 6) + 6) % 6];
}

string PanelMantenimiento::getDescripcionMemoria(int entidad) const {
    const char* estructuras[6] = {
        "ListaEnlazada + ArbolABB + GrafoVuelos (vertices)",
        "ListaEnlazada + GrafoVuelos (aristas dirigidas)",
        "ListaEnlazada<Aeronave> (flota operativa)",
        "ListaEnlazada<Tripulante> (asignada por placa)",
        "ListaEnlazada<Vuelo> (origen -> destino)",
        "ArbolAVL<Pasajero> (balanceado por ID)"
    };
    return estructuras[((entidad % 6) + 6) % 6];
}

string PanelMantenimiento::getResumenCiudadesDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const {
    const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
    if (ciudades.estaVacia()) {
        return "Ninguna";
    }
    string resumen = "";
    const ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
    while (actual != nullptr) {
        if (!resumen.empty()) {
            resumen += ", ";
        }
        resumen += actual->dato.obtenerCodigo();
        actual = actual->siguiente;
    }
    return acortar(resumen, maxChars);
}

string PanelMantenimiento::getResumenPlacasDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const {
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    if (aeronaves.estaVacia()) {
        return "Ninguna";
    }
    string resumen = "";
    const ListaEnlazada<Aeronave>::NodoLista* actual = aeronaves.obtenerCabeza();
    while (actual != nullptr) {
        if (!resumen.empty()) {
            resumen += ", ";
        }
        resumen += actual->dato.getPlaca();
        actual = actual->siguiente;
    }
    return acortar(resumen, maxChars);
}

string PanelMantenimiento::getResumenVuelosDisponibles(const SistemaAerolinea& sistema, size_t maxChars) const {
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    if (vuelos.estaVacia()) {
        return "Ninguno";
    }
    string resumen = "";
    const ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
    while (actual != nullptr) {
        if (!resumen.empty()) {
            resumen += ", ";
        }
        resumen += actual->dato.obtenerCodigoVuelo();
        actual = actual->siguiente;
    }
    return acortar(resumen, maxChars);
}

string PanelMantenimiento::estadoAeronavePorPlaca(const SistemaAerolinea& sistema, const string& placa) const {
    const ListaEnlazada<Aeronave>::NodoLista* actual = sistema.obtenerAeronaves().obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getPlaca() == placa) {
            return actual->dato.getMarca() + " (" + actual->dato.getEstado() + ")";
        }
        actual = actual->siguiente;
    }
    return "Placa no registrada en flota";
}

string PanelMantenimiento::resumenDisponibles(const SistemaAerolinea& sistema, int entidad) const {
    switch (entidad) {
        case 0:
            return "Codigos existentes: " + getResumenCiudadesDisponibles(sistema, 86);
        case 1:
            return "Ciudades disponibles para rutas: " + getResumenCiudadesDisponibles(sistema, 72);
        case 2:
            return "Estados recomendados: Activa | En vuelo | Mantenimiento | Placas: " +
                   getResumenPlacasDisponibles(sistema, 38);
        case 3:
            return "Aeronaves disponibles para asignar: " + getResumenPlacasDisponibles(sistema, 66);
        case 4:
            return "Ciudades: " + getResumenCiudadesDisponibles(sistema, 36) +
                   "  |  Aviones: " + getResumenPlacasDisponibles(sistema, 36);
        case 5:
            return "Vuelos disponibles para asignar: " + getResumenVuelosDisponibles(sistema, 72);
        default:
            return "";
    }
}

void PanelMantenimiento::dibujarEncabezados(sf::RenderWindow& ventana, const sf::Font& fuente) const {
    sf::RectangleShape barraCol(sf::Vector2f(895.0f, 32.0f));
    barraCol.setPosition(sf::Vector2f(290.0f, 152.0f));
    barraCol.setFillColor(sf::Color(37, 43, 54));
    ventana.draw(barraCol);

    sf::Color colorCol(178, 194, 222);
    float yCol = 160.0f;

    switch (entidadActiva) {
        case 0:
            CompatSFML::dibujarTexto(ventana, fuente, "Codigo", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Nombre de ciudad", 395.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Pos mapa (X, Y)", 615.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Rutas salientes", 765.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Estructuras", 915.0f, yCol, 12, colorCol);
            break;
        case 1:
            CompatSFML::dibujarTexto(ventana, fuente, "Origen", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Destino", 405.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Costo", 510.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Duracion", 625.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Estado", 755.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Tipo", 885.0f, yCol, 12, colorCol);
            break;
        case 2:
            CompatSFML::dibujarTexto(ventana, fuente, "Placa", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Marca y Modelo", 430.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Capacidad", 650.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Estado", 755.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Tripulacion asignada", 900.0f, yCol, 12, colorCol);
            break;
        case 3:
            CompatSFML::dibujarTexto(ventana, fuente, "ID", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Nombre", 405.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Rol", 645.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Placa avion", 785.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Aeronave asociada", 915.0f, yCol, 12, colorCol);
            break;
        case 4:
            CompatSFML::dibujarTexto(ventana, fuente, "Codigo", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Trayecto", 405.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Aeronave", 540.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Tarifa / Tiempo", 655.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Pasajeros en AVL", 830.0f, yCol, 12, colorCol);
            break;
        case 5:
            CompatSFML::dibujarTexto(ventana, fuente, "ID", 306.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Nombre completo", 395.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Maletas", 630.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Vuelos", 720.0f, yCol, 12, colorCol);
            CompatSFML::dibujarTexto(ventana, fuente, "Codigos asignados (AVL)", 830.0f, yCol, 12, colorCol);
            break;
    }
}

void PanelMantenimiento::dibujarFila(sf::RenderWindow& ventana, const sf::Font& fuente,
                                     const SistemaAerolinea& sistema, int indiceGlobal,
                                     float yFila, bool seleccionada,
                                     const ListaEnlazada<Pasajero>& cachePasajeros) const {
    sf::RectangleShape cajaFila(sf::Vector2f(887.0f, 32.0f));
    cajaFila.setPosition(sf::Vector2f(294.0f, yFila));
    if (seleccionada) {
        cajaFila.setFillColor(sf::Color(42, 68, 108));
        cajaFila.setOutlineColor(sf::Color(86, 148, 240));
        cajaFila.setOutlineThickness(1.2f);
    } else {
        cajaFila.setFillColor((indiceGlobal % 2 == 0) ? sf::Color(28, 32, 39) : sf::Color(32, 36, 44));
        cajaFila.setOutlineThickness(0.0f);
    }
    ventana.draw(cajaFila);

    float yTxt = yFila + 8.0f;
    sf::Color cTexto = seleccionada ? sf::Color(246, 249, 255) : sf::Color(210, 216, 226);
    sf::Color cSecundario = seleccionada ? sf::Color(200, 222, 255) : sf::Color(156, 166, 182);

    if (entidadActiva == 0) {
        const Ciudad& c = sistema.obtenerCiudades().obtener(indiceGlobal);
        int salientes = sistema.contarRutasDesdeCiudad(c.obtenerCodigo());
        CompatSFML::dibujarTexto(ventana, fuente, c.obtenerCodigo(), 306.0f, yTxt, 12, sf::Color(135, 198, 255));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(c.obtenerNombre(), 24), 395.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente,
                                 "(" + to_string(static_cast<int>(c.obtenerX())) + ", " +
                                 to_string(static_cast<int>(c.obtenerY())) + ")",
                                 615.0f, yTxt, 12, cSecundario);
        CompatSFML::dibujarTexto(ventana, fuente, to_string(salientes) + " ruta(s)", 765.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, "GrafoVuelos + ABB", 915.0f, yTxt, 12, cSecundario);
    } else if (entidadActiva == 1) {
        const Ruta& r = sistema.obtenerRutas().obtener(indiceGlobal);
        CompatSFML::dibujarTexto(ventana, fuente, r.obtenerOrigenCodigo(), 306.0f, yTxt, 12, sf::Color(115, 220, 165));
        CompatSFML::dibujarTexto(ventana, fuente, r.obtenerDestinoCodigo(), 405.0f, yTxt, 12, sf::Color(240, 130, 130));
        CompatSFML::dibujarTexto(ventana, fuente, "$" + to_string(r.obtenerCosto()), 510.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, to_string(r.obtenerTiempoMin()) + " min", 625.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, r.estaActiva() ? "Activa" : "Inactiva", 755.0f, yTxt, 12, cSecundario);
        CompatSFML::dibujarTexto(ventana, fuente, "Arista dirigida", 885.0f, yTxt, 12, cSecundario);
    } else if (entidadActiva == 2) {
        const Aeronave& a = sistema.obtenerAeronaves().obtener(indiceGlobal);
        string crew = sistema.obtenerResumenTripulacionPorAeronave(a.getPlaca());
        sf::Color cEstado(115, 220, 165);
        if (a.getEstado() == "Mantenimiento") {
            cEstado = sf::Color(240, 165, 95);
        } else if (a.getEstado() == "En vuelo") {
            cEstado = sf::Color(130, 185, 250);
        }
        CompatSFML::dibujarTexto(ventana, fuente, a.getPlaca(), 306.0f, yTxt, 12, sf::Color(135, 198, 255));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(a.getMarca() + " " + a.getModelo(), 24),
                                 430.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, to_string(a.getCapacidad()) + " pax", 650.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, a.getEstado(), 755.0f, yTxt, 12, cEstado);
        CompatSFML::dibujarTexto(ventana, fuente, acortar(crew, 32), 900.0f, yTxt, 12, cSecundario);
    } else if (entidadActiva == 3) {
        const Tripulante& t = sistema.obtenerTripulantes().obtener(indiceGlobal);
        CompatSFML::dibujarTexto(ventana, fuente, t.getId(), 306.0f, yTxt, 12, sf::Color(135, 198, 255));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(t.getNombre(), 26), 405.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, acortar(t.getRol(), 16), 645.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, t.getPlacaAeronave(), 785.0f, yTxt, 12, sf::Color(115, 220, 165));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(estadoAeronavePorPlaca(sistema, t.getPlacaAeronave()), 30),
                                 915.0f, yTxt, 12, cSecundario);
    } else if (entidadActiva == 4) {
        const Vuelo& v = sistema.obtenerVuelos().obtener(indiceGlobal);
        string paxResumen = sistema.obtenerResumenPasajerosPorVuelo(v.obtenerCodigoVuelo());
        CompatSFML::dibujarTexto(ventana, fuente, v.obtenerCodigoVuelo(), 306.0f, yTxt, 12, sf::Color(135, 198, 255));
        CompatSFML::dibujarTexto(ventana, fuente,
                                 v.obtenerOrigenCodigo() + " -> " + v.obtenerDestinoCodigo(),
                                 405.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, v.obtenerPlacaAeronave(), 540.0f, yTxt, 12, cSecundario);
        CompatSFML::dibujarTexto(ventana, fuente,
                                 "$" + to_string(v.obtenerCosto()) + " / " + to_string(v.obtenerTiempoMin()) + " min",
                                 655.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, acortar(paxResumen, 40), 830.0f, yTxt, 12, cSecundario);
    } else if (entidadActiva == 5) {
        const Pasajero& p = cachePasajeros.obtener(indiceGlobal);
        CompatSFML::dibujarTexto(ventana, fuente, p.getId(), 306.0f, yTxt, 12, sf::Color(135, 198, 255));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(p.getNombre(), 26), 395.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, to_string(p.getCantidadMaletas()), 630.0f, yTxt, 12, cTexto);
        CompatSFML::dibujarTexto(ventana, fuente, to_string(p.getCantidadVuelos()) + " vuelo(s)",
                                 720.0f, yTxt, 12, sf::Color(115, 220, 165));
        CompatSFML::dibujarTexto(ventana, fuente, acortar(p.getVuelosTexto(), 40), 830.0f, yTxt, 12, cSecundario);
    }
}

bool PanelMantenimiento::manejarClic(float mx, float my, const SistemaAerolinea& sistema,
                                       bool& cambioSeleccion) {
    cambioSeleccion = false;
    int total = getTotalRegistros(sistema);
    int totalPaginas = getTotalPaginas(sistema);

    if (btnPagAnt.contienePunto(mx, my)) {
        if (paginaActual > 0) {
            paginaActual--;
            int primero = paginaActual * FILAS_POR_PAGINA;
            if (primero < total) {
                indiceSeleccionado = primero;
                cambioSeleccion = true;
            }
        }
        return true;
    }

    if (btnPagSig.contienePunto(mx, my)) {
        if (paginaActual + 1 < totalPaginas) {
            paginaActual++;
            int primero = paginaActual * FILAS_POR_PAGINA;
            if (primero < total) {
                indiceSeleccionado = primero;
                cambioSeleccion = true;
            }
        }
        return true;
    }

    if (mx >= 294.0f && mx <= 1181.0f && my >= 188.0f && my < 188.0f + FILAS_POR_PAGINA * 35.0f) {
        int filaVisual = static_cast<int>((my - 188.0f) / 35.0f);
        float offsetDentroFila = (my - 188.0f) - static_cast<float>(filaVisual) * 35.0f;
        if (filaVisual >= 0 && filaVisual < FILAS_POR_PAGINA && offsetDentroFila <= 32.0f) {
            int idxGlobal = paginaActual * FILAS_POR_PAGINA + filaVisual;
            if (idxGlobal >= 0 && idxGlobal < total) {
                indiceSeleccionado = idxGlobal;
                cambioSeleccion = true;
                return true;
            }
        }
    }

    return false;
}

void PanelMantenimiento::renderizar(sf::RenderWindow& ventana, const sf::Font& fuente,
                                    const SistemaAerolinea& sistema) const {
    ListaEnlazada<Pasajero> cachePasajeros;
    if (entidadActiva == 5) {
        cachePasajeros = sistema.obtenerListaPasajeros();
    }
    int total = totalRegistrosPorEntidad(sistema, entidadActiva, cachePasajeros);
    int totalPaginas = (total <= 0) ? 1 : ((total + FILAS_POR_PAGINA - 1) / FILAS_POR_PAGINA);
    int paginaMostrar = paginaActual;
    if (paginaMostrar < 0) paginaMostrar = 0;
    if (paginaMostrar >= totalPaginas) paginaMostrar = totalPaginas - 1;

    sf::RectangleShape tarjetaInfo(sf::Vector2f(895.0f, 80.0f));
    tarjetaInfo.setPosition(sf::Vector2f(290.0f, 64.0f));
    tarjetaInfo.setFillColor(sf::Color(30, 34, 42));
    tarjetaInfo.setOutlineColor(sf::Color(55, 68, 90));
    tarjetaInfo.setOutlineThickness(1.0f);
    ventana.draw(tarjetaInfo);

    string tituloCard = "Mantenimiento de " + getNombrePlural(entidadActiva) +
                        "   (" + to_string(total) + " registro(s) existentes)";
    CompatSFML::dibujarTexto(ventana, fuente, tituloCard, 306.0f, 72.0f, 15, sf::Color(135, 192, 250));

    string rutaGuardado = "Se guarda en: " + sistema.obtenerDirectorioDatos() + "/" +
                          getArchivoDatos(entidadActiva) +
                          "   |   En memoria: " + getDescripcionMemoria(entidadActiva);
    CompatSFML::dibujarTexto(ventana, fuente, acortar(rutaGuardado, 114),
                             306.0f, 96.0f, 12, sf::Color(115, 220, 165));

    CompatSFML::dibujarTexto(ventana, fuente, acortar(resumenDisponibles(sistema, entidadActiva), 114),
                             306.0f, 117.0f, 12, sf::Color(200, 208, 220));

    sf::RectangleShape marcoTabla(sf::Vector2f(895.0f, 430.0f));
    marcoTabla.setPosition(sf::Vector2f(290.0f, 152.0f));
    marcoTabla.setFillColor(sf::Color(24, 27, 33));
    marcoTabla.setOutlineColor(sf::Color(48, 55, 68));
    marcoTabla.setOutlineThickness(1.0f);
    ventana.draw(marcoTabla);

    dibujarEncabezados(ventana, fuente);

    if (total == 0) {
        CompatSFML::dibujarTextoCentrado(
            ventana, fuente,
            "No hay registros en este catalogo. Haz clic en 'Agregar nuevo' en el panel izquierdo.",
            737.0f, 340.0f, 14, sf::Color(150, 160, 175));
    } else {
        int inicio = paginaMostrar * FILAS_POR_PAGINA;
        int fin = inicio + FILAS_POR_PAGINA;
        if (fin > total) {
            fin = total;
        }
        for (int i = inicio; i < fin; ++i) {
            int filaLocal = i - inicio;
            float yFila = 188.0f + static_cast<float>(filaLocal) * 35.0f;
            dibujarFila(ventana, fuente, sistema, i, yFila, i == indiceSeleccionado, cachePasajeros);
        }
    }

    int inicioMostrado = (total == 0) ? 0 : (paginaMostrar * FILAS_POR_PAGINA + 1);
    int finMostrado = (total == 0) ? 0 : ((paginaMostrar + 1) * FILAS_POR_PAGINA);
    if (finMostrado > total) {
        finMostrado = total;
    }

    string pieTabla = "Mostrando " + to_string(inicioMostrado) + "-" + to_string(finMostrado) +
                      " de " + to_string(total) + "  |  Clic en una fila para seleccionarla";
    CompatSFML::dibujarTexto(ventana, fuente, pieTabla, 304.0f, 555.0f, 12, sf::Color(145, 155, 172));

    btnPagAnt.renderizar(ventana, fuente);
    string textoPagina = "Pag " + to_string(paginaMostrar + 1) + " / " + to_string(totalPaginas);
    CompatSFML::dibujarTextoCentrado(ventana, fuente, textoPagina,
                                     1088.0f, 562.0f, 12, sf::Color(210, 218, 232));
    btnPagSig.renderizar(ventana, fuente);
}
