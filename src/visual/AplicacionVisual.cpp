#include "AplicacionVisual.h"
#include "CompatSFML.h"

AplicacionVisual::AplicacionVisual()
    : ventana(CompatSFML::crearVideoMode(1200, 720), "Aerolinea EIF207", sf::Style::Titlebar | sf::Style::Close),
      moduloActual(ModuloActivo::Cliente),
      vistaActual(VistaLienzo::MapaGrafo),
      sistema("data"),
      criterioBellmanCosto(true),
      ultimaAccionCliente(-1),
      ultimaAccionAdmin(-1),
      indicePasajeroActivo(0),
      indiceAeronaveActiva(0),
      indiceVueloActivo(0),
      modoArbolVista(0),
      ultimoMSTEsPrim(true) {
    ventana.setFramerateLimit(60);
    if (!CompatSFML::cargarFuente(fuente, "assets/arial.ttf")) {
        CompatSFML::cargarFuente(fuente, "../assets/arial.ttf");
    }
    sistema.cargarDatos();
    panelMantenimiento.setEntidad(0, sistema);
    inicializarInterfaz();
    actualizarTextosMantenimiento();
    sincronizarMapaVisual();
    mostrarMapaRutas();
}

void AplicacionVisual::inicializarInterfaz() {
    btnTabCliente.configurar(756.0f, 10.0f, 120.0f, 32.0f, "Cliente", true, 13);
    btnTabAdmin.configurar(886.0f, 10.0f, 140.0f, 32.0f, "Administrativo", true, 13);
    btnTabMantenimiento.configurar(1036.0f, 10.0f, 146.0f, 32.0f, "Mantenimientos", true, 13);
    btnTabCliente.setActivo(true);
    btnTabAdmin.setActivo(false);
    btnTabMantenimiento.setActivo(false);

    btnOrigenAnt.configurar(205.0f, 70.0f, 24.0f, 24.0f, "<", true, 13);
    btnOrigenSig.configurar(233.0f, 70.0f, 24.0f, 24.0f, ">", true, 13);
    btnDestinoAnt.configurar(205.0f, 104.0f, 24.0f, 24.0f, "<", true, 13);
    btnDestinoSig.configurar(233.0f, 104.0f, 24.0f, 24.0f, ">", true, 13);

    const char* textosCliente[TOTAL_BOTONES_ACCION] = {
        "Ruta mas rapida",
        "Ruta mas barata",
        "Explorar destinos",
        "Buscar ciudad (ABB)",
        "Vuelos pasajero (AVL)",
        "Mapa de rutas"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesCliente[i].configurar(18.0f, 178.0f + i * 44.0f, 239.0f, 36.0f, textosCliente[i], false, 13);
    }

    const char* textosAdmin[TOTAL_BOTONES_ACCION] = {
        "Aeronaves y tripulacion",
        "Vuelos y pasajeros",
        "Ruta de equipaje",
        "Prim (MST costo)",
        "Kruskal (MST tiempo)",
        "Ver arboles (ABB/AVL/MST)"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesAdmin[i].configurar(18.0f, 178.0f + i * 44.0f, 239.0f, 36.0f, textosAdmin[i], false, 13);
    }

    btnClienteReservarVuelo.configurar(18.0f, 478.0f, 239.0f, 32.0f, "Seleccionar vuelo (AVL)", true, 12);
    btnClienteSiguientePasajero.configurar(18.0f, 518.0f, 239.0f, 32.0f, "Siguiente pasajero AVL", true, 12);

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesMantenimientoEntidad[i].configurar(18.0f, 88.0f + i * 42.0f, 239.0f, 34.0f,
                                                   panelMantenimiento.getNombrePlural(i), false, 13);
        botonesMantenimientoEntidad[i].setActivo(i == 0);
    }

    btnMaintAgregar.configurar(18.0f, 378.0f, 239.0f, 36.0f, "Agregar nuevo", true, 13);
    btnMaintAgregar.setColores(sf::Color(35, 76, 56), sf::Color(46, 118, 82));
    btnMaintEditar.configurar(18.0f, 422.0f, 239.0f, 36.0f, "Editar seleccionado", true, 13);
    btnMaintEditar.setColores(sf::Color(38, 60, 92), sf::Color(52, 114, 206));
    btnMaintEliminar.configurar(18.0f, 466.0f, 239.0f, 36.0f, "Eliminar seleccionado", true, 13);
    btnMaintEliminar.setColores(sf::Color(86, 40, 44), sf::Color(156, 56, 62));

    btnMaintVerEstructura.configurar(18.0f, 546.0f, 239.0f, 34.0f, "Ver en Mapa / Arbol", true, 12);

    btnAdminIrMantenimientos.configurar(18.0f, 478.0f, 239.0f, 34.0f, "Abrir Mantenimientos (CRUD)", true, 12);
    btnAdminIrMantenimientos.setColores(sf::Color(36, 58, 86), sf::Color(52, 114, 206));

    limpiarLineasResultado("Resultados");
}

void AplicacionVisual::sincronizarMapaVisual() {
    int prevOrigen = grafoVisual.getCiudadOrigen();
    int prevDestino = grafoVisual.getCiudadDestino();

    grafoVisual.limpiar();
    const GrafoVuelos& grafo = sistema.obtenerGrafo();
    int totalVertices = grafo.obtenerCantidadVertices();

    for (int i = 0; i < totalVertices; ++i) {
        const Ciudad& c = grafo.obtenerCiudad(i);
        grafoVisual.agregarCiudad(c.obtenerCodigo(), c.obtenerNombre(), c.obtenerX(), c.obtenerY());
    }

    for (int u = 0; u < totalVertices; ++u) {
        const NodoArista* arista = grafo.obtenerAdyacentes(u);
        while (arista != nullptr) {
            grafoVisual.agregarArista(u, arista->destino, arista->costo, arista->tiempoMin);
            arista = arista->siguiente;
        }
    }

    if (totalVertices > 0) {
        int origenFinal = (prevOrigen >= 0 && prevOrigen < totalVertices) ? prevOrigen : 0;
        grafoVisual.setCiudadOrigen(origenFinal);

        if (prevDestino >= 0 && prevDestino < totalVertices && prevDestino != origenFinal) {
            grafoVisual.setCiudadDestino(prevDestino);
        } else {
            int idxNYC = grafo.buscarIndicePorCodigo("NYC");
            if (idxNYC != -1 && idxNYC != origenFinal) {
                grafoVisual.setCiudadDestino(idxNYC);
            } else if (totalVertices > 1) {
                grafoVisual.setCiudadDestino((origenFinal + 1) % totalVertices);
            } else {
                grafoVisual.setCiudadDestino(0);
            }
        }
    }
}

void AplicacionVisual::limpiarLineasResultado(const string& nuevoTitulo) {
    tituloResultado = nuevoTitulo;
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        lineasResultado[i] = "";
    }
}

string AplicacionVisual::truncarTexto(const string& texto, int maxChars) const {
    if (maxChars <= 3 || static_cast<int>(texto.size()) <= maxChars) {
        return texto;
    }
    return texto.substr(0, static_cast<size_t>(maxChars - 3)) + "...";
}

bool AplicacionVisual::validarOrigenDestino(string& codOrigen, string& codDestino, bool exigirDistintos) {
    int total = grafoVisual.getTotalCiudades();
    int idxO = grafoVisual.getCiudadOrigen();
    int idxD = grafoVisual.getCiudadDestino();

    if (total <= 0 || idxO < 0 || idxO >= total) {
        lineasResultado[0] = "No hay ciudades cargadas en el grafo.";
        return false;
    }
    codOrigen = grafoVisual.getCiudad(idxO).codigo;

    if (idxD < 0 || idxD >= total) {
        lineasResultado[0] = "Selecciona una ciudad de destino valida.";
        return false;
    }
    codDestino = grafoVisual.getCiudad(idxD).codigo;

    if (exigirDistintos && idxO == idxD) {
        lineasResultado[0] = "Origen y destino son iguales (" + codOrigen + "). Selecciona ciudades distintas.";
        return false;
    }
    return true;
}

void AplicacionVisual::animarIndices(const ListaEnlazada<int>& indicesLista, bool esMaleta) {
    int indices[30];
    int cantidad = 0;
    const ListaEnlazada<int>::NodoLista* actual = indicesLista.obtenerCabeza();
    while (actual != nullptr && cantidad < 30) {
        indices[cantidad++] = actual->dato;
        actual = actual->siguiente;
    }
    if (cantidad >= 2) {
        grafoVisual.iniciarAnimacionRuta(indices, cantidad, esMaleta);
    }
}

Pasajero AplicacionVisual::obtenerPasajeroActivo(bool& hayPasajeros) const {
    ListaEnlazada<Pasajero> lista = sistema.obtenerListaPasajeros();
    if (lista.estaVacia()) {
        hayPasajeros = false;
        return Pasajero();
    }
    hayPasajeros = true;
    int idx = indicePasajeroActivo % lista.tamano();
    if (idx < 0) {
        idx += lista.tamano();
    }
    return lista.obtener(idx);
}

void AplicacionVisual::mostrarRutaMasRapida() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Ruta mas rapida (Dijkstra por tiempo)");

    string codOrigen;
    string codDestino;
    if (!validarOrigenDestino(codOrigen, codDestino, true)) {
        grafoVisual.limpiarResaltados();
        return;
    }

    ResultadoRuta rapida = sistema.calcularRutaMasRapida(codOrigen, codDestino);
    if (!rapida.encontrada) {
        grafoVisual.limpiarResaltados();
        lineasResultado[0] = "No existe ruta disponible desde " + codOrigen + " hacia " + codDestino + ".";
        return;
    }

    bool hayPasajeros = false;
    Pasajero pasajero = obtenerPasajeroActivo(hayPasajeros);
    string idPasajero = hayPasajeros ? pasajero.getId() : "CLI-001";
    int cantMaletas = hayPasajeros ? pasajero.getCantidadMaletas() : 1;

    Equipaje equipaje = sistema.calcularRutaEquipaje(idPasajero, cantMaletas, codOrigen, codDestino);
    ResultadoRuta barata = sistema.calcularRutaMasBarata(codOrigen, codDestino);

    grafoVisual.limpiarResaltados();
    animarIndices(rapida.caminoIndices, false);
    if (barata.encontrada) {
        animarIndices(barata.caminoIndices, true);
    }

    lineasResultado[0] = truncarTexto(
        "Pasajero (rapida): " + sistema.formatearCaminoIndices(rapida.caminoIndices) +
        " | " + to_string(rapida.duracionTotal) + " min | $" + to_string(rapida.costoTotal) +
        " | Escalas: " + to_string(rapida.cantidadEscalas), 108);

    if (barata.encontrada && sistema.sonRutasDistintas(rapida, barata)) {
        lineasResultado[1] = truncarTexto(
            "Equipaje (economica): " + equipaje.obtenerRutaTexto() +
            " | $" + to_string(equipaje.obtenerCostoEnvio()) +
            " | " + to_string(equipaje.obtenerTiempoEnvio()) + " min", 108);
    } else {
        lineasResultado[1] = truncarTexto(
            "Equipaje (economica): coincide con la ruta rapida ($" +
            to_string(equipaje.obtenerCostoEnvio()) + ")", 108);
    }

    lineasResultado[2] = truncarTexto(
        "Vuelos en tramo: " + sistema.obtenerVuelosParaCamino(rapida.caminoIndices), 108);

    if (barata.encontrada && sistema.sonRutasDistintas(rapida, barata)) {
        int ahorro = rapida.costoTotal - equipaje.obtenerCostoEnvio();
        lineasResultado[3] = truncarTexto(
            "Rutas diferenciadas: equipaje ahorra $" + to_string(ahorro) +
            " (" + (hayPasajeros ? pasajero.getNombre() : string("Cliente")) + ", " +
            to_string(cantMaletas) + " maletas)", 108);
    } else if (barata.encontrada) {
        lineasResultado[3] = truncarTexto(
            "La ruta por tiempo coincide con la opcion mas barata disponible ($" +
            to_string(barata.costoTotal) + ")", 108);
    }
}

void AplicacionVisual::mostrarRutaMasBarata() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Ruta mas barata (Dijkstra por costo)");

    string codOrigen;
    string codDestino;
    if (!validarOrigenDestino(codOrigen, codDestino, true)) {
        grafoVisual.limpiarResaltados();
        return;
    }

    ResultadoRuta barata = sistema.calcularRutaMasBarata(codOrigen, codDestino);
    if (!barata.encontrada) {
        grafoVisual.limpiarResaltados();
        lineasResultado[0] = "No existe ruta disponible desde " + codOrigen + " hacia " + codDestino + ".";
        return;
    }

    ResultadoRuta rapida = sistema.calcularRutaMasRapida(codOrigen, codDestino);

    grafoVisual.limpiarResaltados();
    animarIndices(barata.caminoIndices, false);

    lineasResultado[0] = truncarTexto(
        "Economica: " + sistema.formatearCaminoIndices(barata.caminoIndices) +
        " | $" + to_string(barata.costoTotal) +
        " | " + to_string(barata.duracionTotal) + " min | Escalas: " +
        to_string(barata.cantidadEscalas), 108);
    lineasResultado[1] = "Equipaje: viaja por esta misma ruta economica ($" + to_string(barata.costoTotal) + ")";
    lineasResultado[2] = truncarTexto(
        "Vuelos en tramo: " + sistema.obtenerVuelosParaCamino(barata.caminoIndices), 108);

    if (rapida.encontrada && sistema.sonRutasDistintas(barata, rapida)) {
        lineasResultado[3] = truncarTexto(
            "Alternativa rapida: " + sistema.formatearCaminoIndices(rapida.caminoIndices) +
            " (" + to_string(rapida.duracionTotal) + " min, $" + to_string(rapida.costoTotal) + ")", 108);
    } else {
        lineasResultado[3] = "Es tambien la ruta mas rapida disponible entre ambas ciudades";
    }
}

void AplicacionVisual::mostrarExplorarDestinos() {
    vistaActual = VistaLienzo::MapaGrafo;
    if (ultimaAccionCliente == 2) {
        criterioBellmanCosto = !criterioBellmanCosto;
    }

    CriterioRuta criterio = criterioBellmanCosto ? CRITERIO_COSTO : CRITERIO_TIEMPO;
    string etiquetaCriterio = criterioBellmanCosto ? "Costo" : "Tiempo";
    limpiarLineasResultado("Explorar destinos (Bellman-Ford: " + etiquetaCriterio + ")");

    string codOrigen;
    string codDestino;
    if (!validarOrigenDestino(codOrigen, codDestino, false)) {
        grafoVisual.limpiarResaltados();
        return;
    }

    ResultadoBellmanFord bf = sistema.explorarDestinos(codOrigen, criterio);
    if (bf.hayCicloNegativo) {
        grafoVisual.limpiarResaltados();
        lineasResultado[0] = "Error: se detecto un ciclo negativo en la red de rutas.";
        return;
    }

    grafoVisual.limpiarResaltados();

    int idxOrigen = grafoVisual.getCiudadOrigen();
    int idxDestino = grafoVisual.getCiudadDestino();
    const GrafoVuelos& grafo = sistema.obtenerGrafo();
    int alcanzables = 0;
    int lineaActual = 0;

    for (int v = 0; v < bf.cantidadVertices; ++v) {
        if (v == idxOrigen || !bf.alcanzable[v]) {
            continue;
        }
        if (bf.previo[v] != -1) {
            grafoVisual.resaltarAristaRuta(bf.previo[v], v, false);
        }

        string codigoV = grafo.obtenerCiudad(v).obtenerCodigo();
        string entrada = codigoV + " ($" + to_string(bf.costos[v]) + ", " + to_string(bf.tiempos[v]) + "m)";

        if (lineaActual < 3) {
            if (!lineasResultado[lineaActual].empty() &&
                static_cast<int>(lineasResultado[lineaActual].size() + entrada.size() + 3) > 98) {
                lineaActual++;
            }
            if (lineaActual < 3) {
                if (!lineasResultado[lineaActual].empty()) {
                    lineasResultado[lineaActual] += " | ";
                }
                lineasResultado[lineaActual] += entrada;
            }
        }
        alcanzables++;
    }

    if (idxDestino >= 0 && idxDestino < bf.cantidadVertices &&
        idxDestino != idxOrigen && bf.alcanzable[idxDestino]) {
        ListaEnlazada<int> caminoDest = BellmanFord::reconstruirCamino(bf, idxDestino);
        animarIndices(caminoDest, false);
    }

    if (alcanzables == 0) {
        lineasResultado[0] = "No hay destinos alcanzables desde " + codOrigen + ".";
    }
    lineasResultado[3] = "Alcanzables desde " + codOrigen + ": " + to_string(alcanzables) +
                         " ciudades | Clic de nuevo cambia Costo/Tiempo";
}

int AplicacionVisual::volcarABBRec(const NodoABB* nodo, const string& codigoResaltado) {
    if (nodo == nullptr) {
        return -1;
    }
    int izq = volcarABBRec(nodo->izquierdo, codigoResaltado);
    int der = volcarABBRec(nodo->derecho, codigoResaltado);
    return arbolVisual.agregarNodo(
        nodo->codigo,
        truncarTexto(nodo->nombre, 14),
        izq,
        der,
        nodo->codigo == codigoResaltado);
}

int AplicacionVisual::volcarAVLRec(const NodoAVL* nodo, const string& idResaltado) {
    if (nodo == nullptr) {
        return -1;
    }
    int izq = volcarAVLRec(nodo->izquierdo, idResaltado);
    int der = volcarAVLRec(nodo->derecho, idResaltado);
    string sub = "h" + to_string(nodo->altura) + " " + truncarTexto(nodo->dato.getVuelosTexto(), 14);
    return arbolVisual.agregarNodo(
        nodo->dato.getId(),
        sub,
        izq,
        der,
        nodo->dato.getId() == idResaltado);
}

void AplicacionVisual::volcarMSTRec(const NodoMST* nodo, int padreVisualIdx) {
    if (nodo == nullptr) {
        return;
    }
    string sub = (nodo->pesoDesdePadre == 0)
        ? "Raiz"
        : ((ultimoMSTEsPrim ? "$" : "") + to_string(nodo->pesoDesdePadre) + (ultimoMSTEsPrim ? "" : "m"));
    int idxActual = arbolVisual.agregarNodoGeneral(
        nodo->codigoCiudad,
        sub,
        nodo->pesoDesdePadre == 0);
    if (idxActual != -1) {
        if (padreVisualIdx == -1) {
            arbolVisual.setRaiz(idxActual);
        } else {
            arbolVisual.enlazarHijo(padreVisualIdx, idxActual);
        }
        volcarMSTRec(nodo->primerHijo, idxActual);
    }
    volcarMSTRec(nodo->siguienteHermano, padreVisualIdx);
}

void AplicacionVisual::mostrarArbolABBVisual(const string& codigoResaltado) {
    const ArbolABB& abb = sistema.obtenerArbolCiudades();
    arbolVisual.limpiar("Arbol ABB de Ciudades (por nombre)");
    if (!abb.estaVacio()) {
        int raizIdx = volcarABBRec(abb.getRaiz(), codigoResaltado);
        arbolVisual.setRaiz(raizIdx);
    }
    arbolVisual.setRecorridos(
        truncarTexto(abb.getPreorden(), 102),
        truncarTexto(abb.getInorden(), 102),
        truncarTexto(abb.getPostorden(), 102));
}

void AplicacionVisual::mostrarArbolAVLVisual(const string& idResaltado) {
    const ArbolAVL& avl = sistema.obtenerArbolPasajeros();
    arbolVisual.limpiar("Arbol AVL de Pasajeros (por ID)");
    if (!avl.estaVacio()) {
        int raizIdx = volcarAVLRec(avl.getRaiz(), idResaltado);
        arbolVisual.setRaiz(raizIdx);
    }
    arbolVisual.setRecorridos(
        truncarTexto(avl.getPreorden(), 102),
        truncarTexto(avl.getInorden(), 102),
        truncarTexto(avl.getPostorden(), 102));
}

void AplicacionVisual::mostrarArbolMSTVisual(const string& titulo) {
    arbolVisual.limpiar(titulo);
    if (ultimoArbolMST.estaVacio()) {
        arbolVisual.setRecorridos("-", "-", "-");
        return;
    }
    volcarMSTRec(ultimoArbolMST.getRaiz(), -1);
    arbolVisual.setRecorridos(
        truncarTexto(ultimoArbolMST.getPreorden(), 102),
        truncarTexto(ultimoArbolMST.getInorden(), 102),
        truncarTexto(ultimoArbolMST.getPostorden(), 102));
}

void AplicacionVisual::mostrarBusquedaCiudadABB() {
    vistaActual = VistaLienzo::ArbolEstructura;
    arbolVisual.limpiar("Arbol Binario de Busqueda (ABB)");
    limpiarLineasResultado("Busqueda de ciudad (ABB)");

    int total = grafoVisual.getTotalCiudades();
    if (total <= 0) {
        lineasResultado[0] = "No hay ciudades cargadas en el ABB.";
        return;
    }

    int idxConsulta = (grafoVisual.getCiudadDestino() >= 0)
        ? grafoVisual.getCiudadDestino()
        : grafoVisual.getCiudadOrigen();
    if (idxConsulta < 0 || idxConsulta >= total) {
        lineasResultado[0] = "Selecciona una ciudad valida para consultar.";
        return;
    }

    string nombreCiudad = grafoVisual.getCiudad(idxConsulta).nombre;
    const NodoABB* nodo = sistema.buscarCiudadABB(nombreCiudad);

    if (nodo == nullptr) {
        lineasResultado[0] = "La ciudad " + nombreCiudad + " no se encontro en el ABB.";
        return;
    }

    int salientes = sistema.contarRutasDesdeCiudad(nodo->codigo);
    const ArbolABB& abb = sistema.obtenerArbolCiudades();
    mostrarArbolABBVisual(nodo->codigo);

    lineasResultado[0] = truncarTexto(
        "Encontrada en ABB: " + nodo->nombre + " (" + nodo->codigo +
        ") | Vertice #" + to_string(nodo->indiceVertice), 108);
    lineasResultado[1] = "Rutas salientes: " + to_string(salientes) +
                         " | Nodos en ABB: " + to_string(abb.getCantidad()) +
                         " | Altura: " + to_string(abb.getAltura());
    lineasResultado[2] = truncarTexto("Inorden ABB: " + abb.getInorden(), 108);
    lineasResultado[3] = "Cambia Destino (< >) para consultar otra ciudad en el ABB";
}

void AplicacionVisual::mostrarPasajeroAVLEncontrando(const NodoAVL* nodo, const string& tituloInfo) {
    vistaActual = VistaLienzo::ArbolEstructura;
    arbolVisual.limpiar("Arbol AVL de Pasajeros");
    limpiarLineasResultado(tituloInfo);

    if (nodo == nullptr) {
        mostrarArbolAVLVisual("");
        lineasResultado[0] = "No se encontro el pasajero seleccionado en el AVL.";
        return;
    }

    const ArbolAVL& avl = sistema.obtenerArbolPasajeros();
    mostrarArbolAVLVisual(nodo->dato.getId());
    lineasResultado[0] = truncarTexto(
        "Pasajero AVL: " + nodo->dato.getNombre() + " (ID: " + nodo->dato.getId() +
        ") | Maletas: " + to_string(nodo->dato.getCantidadMaletas()), 108);
    lineasResultado[1] = truncarTexto("Vuelos asignados: " + nodo->dato.getVuelosTexto(), 108);
    lineasResultado[2] = "Nodos AVL: " + to_string(avl.getCantidad()) +
                         " | Altura balanceada: " + to_string(avl.getAltura()) +
                         " | Hojas: " + to_string(avl.contarHojas());
    lineasResultado[3] = "Usa 'Siguiente pasajero AVL' o 'Vuelos pasajero (AVL)' para consultar otro ID";
}

void AplicacionVisual::mostrarVuelosPasajeroAVL() {
    bool hayPasajeros = false;
    Pasajero pasajero = obtenerPasajeroActivo(hayPasajeros);
    if (!hayPasajeros) {
        vistaActual = VistaLienzo::ArbolEstructura;
        arbolVisual.limpiar("Arbol AVL de Pasajeros");
        limpiarLineasResultado("Asignacion de pasajero (AVL)");
        lineasResultado[0] = "No hay pasajeros registrados en el AVL.";
        return;
    }

    const NodoAVL* nodo = sistema.buscarPasajeroAVL(pasajero.getId());
    mostrarPasajeroAVLEncontrando(nodo, "Asignacion de pasajero (AVL)");
}

void AplicacionVisual::mostrarSiguientePasajeroCliente() {
    ListaEnlazada<Pasajero> lista = sistema.obtenerListaPasajeros();
    if (lista.estaVacia()) {
        vistaActual = VistaLienzo::ArbolEstructura;
        arbolVisual.limpiar("Arbol AVL de Pasajeros");
        limpiarLineasResultado("Asignacion de pasajero (AVL)");
        lineasResultado[0] = "No hay pasajeros registrados en el AVL.";
        return;
    }

    indicePasajeroActivo = (indicePasajeroActivo + 1) % lista.tamano();
    mostrarVuelosPasajeroAVL();
}

void AplicacionVisual::mostrarMapaRutas() {
    vistaActual = VistaLienzo::MapaGrafo;
    grafoVisual.limpiarResaltados();
    limpiarLineasResultado("Mapa de rutas");

    lineasResultado[0] = "Red cargada: " + to_string(sistema.obtenerCiudades().tamano()) +
                         " ciudades | " + to_string(sistema.obtenerRutas().tamano()) +
                         " rutas | " + to_string(sistema.obtenerVuelos().tamano()) +
                         " vuelos | " + to_string(sistema.obtenerArbolPasajeros().getCantidad()) + " pasajeros";
    lineasResultado[1] = "Clic izq en nodo o botones (< >) cambia Origen; clic der cambia Destino";
    lineasResultado[2] = "Usa Ruta mas rapida / mas barata para comparar vuelo del pasajero y maleta";
}

void AplicacionVisual::mostrarRutaEquipajeAdmin() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Ruta de equipaje (menor costo)");

    string codOrigen;
    string codDestino;
    if (!validarOrigenDestino(codOrigen, codDestino, true)) {
        grafoVisual.limpiarResaltados();
        return;
    }

    bool hayPasajeros = false;
    Pasajero pasajero = obtenerPasajeroActivo(hayPasajeros);
    string idPasajero = hayPasajeros ? pasajero.getId() : "CLI-001";
    int cantMaletas = hayPasajeros ? pasajero.getCantidadMaletas() : 1;

    Equipaje equipaje = sistema.calcularRutaEquipaje(idPasajero, cantMaletas, codOrigen, codDestino);
    ResultadoRuta barata = sistema.calcularRutaMasBarata(codOrigen, codDestino);
    ResultadoRuta rapida = sistema.calcularRutaMasRapida(codOrigen, codDestino);

    if (!barata.encontrada) {
        grafoVisual.limpiarResaltados();
        lineasResultado[0] = "No hay ruta economica disponible entre " + codOrigen + " y " + codDestino + ".";
        return;
    }

    grafoVisual.limpiarResaltados();
    if (rapida.encontrada) {
        animarIndices(rapida.caminoIndices, false);
    }
    animarIndices(barata.caminoIndices, true);

    lineasResultado[0] = truncarTexto(
        "Equipaje (" + (hayPasajeros ? pasajero.getNombre() : string("Cliente")) + ", " +
        to_string(cantMaletas) + " maletas): " + equipaje.obtenerRutaTexto(), 108);
    lineasResultado[1] = "Costo envio: $" + to_string(equipaje.obtenerCostoEnvio()) +
                         " | Tiempo envio: " + to_string(equipaje.obtenerTiempoEnvio()) + " min";

    if (rapida.encontrada && sistema.sonRutasDistintas(rapida, barata)) {
        lineasResultado[2] = truncarTexto(
            "Diferente al vuelo rapido (" + sistema.formatearCaminoIndices(rapida.caminoIndices) +
            ", $" + to_string(rapida.costoTotal) + ", " + to_string(rapida.duracionTotal) + " min)", 108);
        lineasResultado[3] = "Ahorro en tarifa de equipaje: $" +
                             to_string(rapida.costoTotal - equipaje.obtenerCostoEnvio());
    } else {
        lineasResultado[2] = "La ruta economica de equipaje coincide con el vuelo rapido";
        lineasResultado[3] = "Las maletas siempre se envian por la ruta de menor costo (Dijkstra)";
    }
}

void AplicacionVisual::procesarEventos() {
#if SFML_VERSION_MAJOR >= 3
    while (const optional<sf::Event> evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        } else if (modal.estaVisible()) {
            if (const auto* texto = evento->getIf<sf::Event::TextEntered>()) {
                modal.manejarTextoIngresado(static_cast<unsigned int>(texto->unicode));
            } else if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
                bool confirmado = false;
                modal.manejarTeclaEspecial(
                    tecla->code == sf::Keyboard::Key::Enter,
                    tecla->code == sf::Keyboard::Key::Backspace,
                    tecla->code == sf::Keyboard::Key::Tab,
                    tecla->code == sf::Keyboard::Key::Escape,
                    tecla->code == sf::Keyboard::Key::Up,
                    tecla->code == sf::Keyboard::Key::Down,
                    confirmado);
                if (confirmado) {
                    confirmarModalActivo();
                }
            } else if (const auto* botonMouse = evento->getIf<sf::Event::MouseButtonPressed>()) {
                if (botonMouse->button == sf::Mouse::Button::Left) {
                    bool confirmado = false;
                    modal.manejarClicIzquierdo(static_cast<float>(botonMouse->position.x),
                                               static_cast<float>(botonMouse->position.y),
                                               confirmado);
                    if (confirmado) {
                        confirmarModalActivo();
                    }
                }
            }
        } else if (const auto* botonMouse = evento->getIf<sf::Event::MouseButtonPressed>()) {
            float mx = static_cast<float>(botonMouse->position.x);
            float my = static_cast<float>(botonMouse->position.y);
            if (botonMouse->button == sf::Mouse::Button::Left) {
                manejarClicIzquierdo(mx, my);
            } else if (botonMouse->button == sf::Mouse::Button::Right) {
                manejarClicDerecho(mx, my);
            }
        }
    }
#else
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        } else if (modal.estaVisible()) {
            if (evento.type == sf::Event::TextEntered) {
                modal.manejarTextoIngresado(static_cast<unsigned int>(evento.text.unicode));
            } else if (evento.type == sf::Event::KeyPressed) {
                bool confirmado = false;
                modal.manejarTeclaEspecial(
                    evento.key.code == sf::Keyboard::Return,
                    evento.key.code == sf::Keyboard::BackSpace,
                    evento.key.code == sf::Keyboard::Tab,
                    evento.key.code == sf::Keyboard::Escape,
                    evento.key.code == sf::Keyboard::Up,
                    evento.key.code == sf::Keyboard::Down,
                    confirmado);
                if (confirmado) {
                    confirmarModalActivo();
                }
            } else if (evento.type == sf::Event::MouseButtonPressed &&
                       evento.mouseButton.button == sf::Mouse::Left) {
                bool confirmado = false;
                modal.manejarClicIzquierdo(static_cast<float>(evento.mouseButton.x),
                                           static_cast<float>(evento.mouseButton.y),
                                           confirmado);
                if (confirmado) {
                    confirmarModalActivo();
                }
            }
        } else if (evento.type == sf::Event::MouseButtonPressed) {
            float mx = static_cast<float>(evento.mouseButton.x);
            float my = static_cast<float>(evento.mouseButton.y);
            if (evento.mouseButton.button == sf::Mouse::Button::Left) {
                manejarClicIzquierdo(mx, my);
            } else if (evento.mouseButton.button == sf::Mouse::Button::Right) {
                manejarClicDerecho(mx, my);
            }
        }
    }
#endif
}

void AplicacionVisual::manejarClicIzquierdo(float mx, float my) {
    if (btnTabCliente.contienePunto(mx, my)) {
        cambiarModulo(ModuloActivo::Cliente);
        return;
    }
    if (btnTabAdmin.contienePunto(mx, my)) {
        cambiarModulo(ModuloActivo::Administrativo);
        return;
    }
    if (btnTabMantenimiento.contienePunto(mx, my)) {
        cambiarModulo(ModuloActivo::Mantenimientos);
        return;
    }

    if (moduloActual == ModuloActivo::Mantenimientos) {
        manejarClicPanelMantenimiento(mx, my);
        return;
    }

    if (moduloActual == ModuloActivo::Cliente && manejarClicClienteExtra(mx, my)) {
        return;
    }
    if (moduloActual == ModuloActivo::Administrativo) {
        if (btnAdminIrMantenimientos.contienePunto(mx, my)) {
            cambiarModulo(ModuloActivo::Mantenimientos);
            return;
        }
        if (vistaActual == VistaLienzo::PanelAdmin && manejarClicPanelAdmin(mx, my)) {
            return;
        }
    }

    int totalC = grafoVisual.getTotalCiudades();
    if (totalC > 0) {
        if (btnOrigenAnt.contienePunto(mx, my)) {
            grafoVisual.setCiudadOrigen((grafoVisual.getCiudadOrigen() - 1 + totalC) % totalC);
            grafoVisual.limpiarResaltados();
            return;
        }
        if (btnOrigenSig.contienePunto(mx, my)) {
            grafoVisual.setCiudadOrigen((grafoVisual.getCiudadOrigen() + 1) % totalC);
            grafoVisual.limpiarResaltados();
            return;
        }
        if (btnDestinoAnt.contienePunto(mx, my)) {
            grafoVisual.setCiudadDestino((grafoVisual.getCiudadDestino() - 1 + totalC) % totalC);
            grafoVisual.limpiarResaltados();
            return;
        }
        if (btnDestinoSig.contienePunto(mx, my)) {
            grafoVisual.setCiudadDestino((grafoVisual.getCiudadDestino() + 1) % totalC);
            grafoVisual.limpiarResaltados();
            return;
        }
    }

    if (vistaActual == VistaLienzo::MapaGrafo) {
        int ciudadClic = grafoVisual.detectarCiudadClic(mx, my);
        if (ciudadClic != -1) {
            grafoVisual.setCiudadOrigen(ciudadClic);
            grafoVisual.limpiarResaltados();
            return;
        }
    }

    if (moduloActual == ModuloActivo::Cliente) {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            if (botonesCliente[i].contienePunto(mx, my)) {
                for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) botonesCliente[j].setActivo(i == j);
                ejecutarAccionCliente(i);
                return;
            }
        }
    } else if (moduloActual == ModuloActivo::Administrativo) {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            if (botonesAdmin[i].contienePunto(mx, my)) {
                for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) botonesAdmin[j].setActivo(i == j);
                ejecutarAccionAdmin(i);
                return;
            }
        }
    }
}

void AplicacionVisual::manejarClicDerecho(float mx, float my) {
    if (vistaActual == VistaLienzo::MapaGrafo) {
        int ciudadClic = grafoVisual.detectarCiudadClic(mx, my);
        if (ciudadClic != -1) {
            grafoVisual.setCiudadDestino(ciudadClic);
            grafoVisual.limpiarResaltados();
        }
    }
}

void AplicacionVisual::ejecutarAccionCliente(int indiceAccion) {
    switch (indiceAccion) {
        case 0:
            mostrarRutaMasRapida();
            break;
        case 1:
            mostrarRutaMasBarata();
            break;
        case 2:
            mostrarExplorarDestinos();
            break;
        case 3:
            abrirModalClienteBuscarABB();
            break;
        case 4:
            abrirModalClienteConsultarAVL();
            break;
        case 5:
            mostrarMapaRutas();
            break;
    }
    ultimaAccionCliente = indiceAccion;
}

bool AplicacionVisual::manejarClicClienteExtra(float mx, float my) {
    if (btnClienteReservarVuelo.contienePunto(mx, my)) {
        abrirModalClienteReservarVuelo();
        return true;
    }
    if (btnClienteSiguientePasajero.contienePunto(mx, my)) {
        mostrarSiguientePasajeroCliente();
        return true;
    }
    return false;
}

bool AplicacionVisual::manejarClicPanelMantenimiento(float mx, float my) {
    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        if (botonesMantenimientoEntidad[i].contienePunto(mx, my)) {
            panelMantenimiento.setEntidad(i, sistema);
            actualizarTextosMantenimiento();
            actualizarDetalleRegistroMantenimiento();
            return true;
        }
    }
    if (btnMaintAgregar.contienePunto(mx, my)) {
        abrirModalCrud(0);
        return true;
    }
    if (btnMaintEditar.contienePunto(mx, my)) {
        abrirModalCrud(1);
        return true;
    }
    if (btnMaintEliminar.contienePunto(mx, my)) {
        abrirModalCrud(2);
        return true;
    }
    if (btnMaintVerEstructura.contienePunto(mx, my)) {
        mostrarEstructuraDesdeMantenimiento();
        return true;
    }
    if (vistaActual == VistaLienzo::PanelMantenimiento) {
        bool cambioSeleccion = false;
        if (panelMantenimiento.manejarClic(mx, my, sistema, cambioSeleccion)) {
            if (cambioSeleccion) {
                actualizarDetalleRegistroMantenimiento();
            }
            return true;
        }
    }
    return false;
}

void AplicacionVisual::actualizarResumenInferiorAdmin() {
    if (panelAdmin.getModo() == ModoPanelAdmin::FlotaTripulacion) {
        limpiarLineasResultado("Aeronaves y tripulacion");
        const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
        int totalAeronaves = aeronaves.tamano();
        if (totalAeronaves == 0) {
            lineasResultado[0] = "No hay aeronaves registradas en el sistema.";
            lineasResultado[1] = "Usa 'Abrir Mantenimientos (CRUD)' para agregar flota y tripulacion.";
            return;
        }
        int idx = ((indiceAeronaveActiva % totalAeronaves) + totalAeronaves) % totalAeronaves;
        const Aeronave& aero = aeronaves.obtener(idx);
        string resumenTrip = sistema.obtenerResumenTripulacionPorAeronave(aero.getPlaca());
        lineasResultado[0] = truncarTexto(
            "Aeronave [" + to_string(idx + 1) + "/" + to_string(totalAeronaves) +
            "]: " + aero.getPlaca() + " | " + aero.getMarca() + " " + aero.getModelo() +
            " | Cap: " + to_string(aero.getCapacidad()) + " | Estado: " + aero.getEstado(), 108);
        lineasResultado[1] = truncarTexto("Tripulacion asignada: " + resumenTrip, 108);
        lineasResultado[2] = "Flota total: " + to_string(totalAeronaves) +
                             " aeronaves | Tripulantes cargados: " +
                             to_string(sistema.obtenerTripulantes().tamano());
        lineasResultado[3] = "Clic en una fila del centro para inspeccionar | Edita flota en Mantenimientos";
        return;
    }

    limpiarLineasResultado("Vuelos y pasajeros");
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int totalVuelos = vuelos.tamano();
    if (totalVuelos == 0) {
        lineasResultado[0] = "No hay vuelos programados en el sistema.";
        lineasResultado[1] = "Usa 'Abrir Mantenimientos (CRUD)' para programar vuelos y asignar pasajeros.";
        return;
    }
    int idx = ((indiceVueloActivo % totalVuelos) + totalVuelos) % totalVuelos;
    const Vuelo& vuelo = vuelos.obtener(idx);
    string resumenPax = sistema.obtenerResumenPasajerosPorVuelo(vuelo.obtenerCodigoVuelo());
    lineasResultado[0] = truncarTexto(
        "Vuelo [" + to_string(idx + 1) + "/" + to_string(totalVuelos) +
        "]: " + vuelo.obtenerCodigoVuelo() + " (" + vuelo.obtenerOrigenCodigo() + " -> " +
        vuelo.obtenerDestinoCodigo() + ") | Aeronave: " + vuelo.obtenerPlacaAeronave(), 108);
    lineasResultado[1] = "Tarifa: $" + to_string(vuelo.obtenerCosto()) +
                         " | Duracion: " + to_string(vuelo.obtenerTiempoMin()) +
                         " min | Pasajeros en AVL: " + to_string(sistema.obtenerArbolPasajeros().getCantidad());
    lineasResultado[2] = truncarTexto("Pasajeros asignados: " + resumenPax, 108);
    lineasResultado[3] = "Clic en un vuelo del centro para ver sus pasajeros AVL | Edita en Mantenimientos";
}

bool AplicacionVisual::manejarClicPanelAdmin(float mx, float my) {
    int evento = panelAdmin.manejarClic(mx, my, sistema, indiceAeronaveActiva, indiceVueloActivo);
    if (evento == 0) {
        return false;
    }
    if (evento == 4) {
        ultimaAccionAdmin = 0;
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].setActivo(i == 0);
        }
    } else if (evento == 5) {
        ultimaAccionAdmin = 1;
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].setActivo(i == 1);
        }
    }
    actualizarResumenInferiorAdmin();
    return true;
}

void AplicacionVisual::mostrarAeronavesYCrewAdmin() {
    vistaActual = VistaLienzo::PanelAdmin;
    panelAdmin.setModo(ModoPanelAdmin::FlotaTripulacion);
    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesAdmin[i].setActivo(i == 0);
    }

    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int totalAeronaves = aeronaves.tamano();
    if (totalAeronaves == 0) {
        indiceAeronaveActiva = 0;
        panelAdmin.resetPaginas();
    } else {
        if (ultimaAccionAdmin == 0) {
            indiceAeronaveActiva = (indiceAeronaveActiva + 1) % totalAeronaves;
        } else {
            indiceAeronaveActiva = ((indiceAeronaveActiva % totalAeronaves) + totalAeronaves) % totalAeronaves;
        }
        panelAdmin.resetPaginaDerecha();
        panelAdmin.asegurarSeleccionIzquierda(indiceAeronaveActiva);
    }
    actualizarResumenInferiorAdmin();
}

void AplicacionVisual::mostrarVuelosYPasajerosAdmin() {
    vistaActual = VistaLienzo::PanelAdmin;
    panelAdmin.setModo(ModoPanelAdmin::VuelosPasajeros);
    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesAdmin[i].setActivo(i == 1);
    }

    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int totalVuelos = vuelos.tamano();
    if (totalVuelos == 0) {
        indiceVueloActivo = 0;
        panelAdmin.resetPaginas();
    } else {
        if (ultimaAccionAdmin == 1) {
            indiceVueloActivo = (indiceVueloActivo + 1) % totalVuelos;
        } else {
            indiceVueloActivo = ((indiceVueloActivo % totalVuelos) + totalVuelos) % totalVuelos;
        }
        panelAdmin.resetPaginaDerecha();
        panelAdmin.asegurarSeleccionIzquierda(indiceVueloActivo);
    }
    actualizarResumenInferiorAdmin();
}

void AplicacionVisual::mostrarMSTAdmin(bool usarPrim) {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado(usarPrim ? "Prim (MST por costo)" : "Kruskal (MST por duracion)");

    int total = grafoVisual.getTotalCiudades();
    int idxOrigen = grafoVisual.getCiudadOrigen();
    if (total == 0 || idxOrigen < 0 || idxOrigen >= total) {
        grafoVisual.limpiarResaltados();
        lineasResultado[0] = "No hay una ciudad de origen valida para calcular el MST.";
        return;
    }

    string codOrigen = grafoVisual.getCiudad(idxOrigen).codigo;
    ultimoMSTEsPrim = usarPrim;
    ResultadoMST resultado = usarPrim
        ? sistema.calcularPrimCosto(codOrigen, ultimoArbolMST)
        : sistema.calcularKruskalDuracion(codOrigen, ultimoArbolMST);

    grafoVisual.limpiarResaltados();
    for (int i = 0; i < resultado.aristas.tamano(); ++i) {
        const AristaMST& arista = resultado.aristas.obtener(i);
        grafoVisual.resaltarAristaMST(arista.origenIdx, arista.destinoIdx);
    }

    string pesoTexto = usarPrim
        ? ("$" + to_string(resultado.pesoTotal))
        : (to_string(resultado.pesoTotal) + " min");
    lineasResultado[0] = string(usarPrim ? "Prim" : "Kruskal") + " desde " + codOrigen +
                         " | Peso total: " + pesoTexto +
                         " | Aristas MST: " + to_string(resultado.aristas.tamano()) +
                         " | Conexo: " + (resultado.esConexo ? "Si" : "No");
    lineasResultado[1] = truncarTexto("Preorden MST: " + ultimoArbolMST.getPreorden(), 108);
    lineasResultado[2] = truncarTexto("Niveles MST: " + ultimoArbolMST.getPorNiveles(), 108);
    lineasResultado[3] = "Presiona 'Ver arboles (ABB/AVL/MST)' para ver el arbol general y sus recorridos";
}

void AplicacionVisual::mostrarSiguienteArbolAdmin() {
    vistaActual = VistaLienzo::ArbolEstructura;
    if (ultimaAccionAdmin == 5) {
        modoArbolVista = (modoArbolVista + 1) % 3;
    }

    if (modoArbolVista == 0) {
        if (ultimoArbolMST.estaVacio() && grafoVisual.getTotalCiudades() > 0) {
            int idxOrigen = grafoVisual.getCiudadOrigen();
            if (idxOrigen < 0 || idxOrigen >= grafoVisual.getTotalCiudades()) {
                idxOrigen = 0;
            }
            ultimoMSTEsPrim = true;
            sistema.calcularPrimCosto(grafoVisual.getCiudad(idxOrigen).codigo, ultimoArbolMST);
        }
        string titulo = ultimoMSTEsPrim
            ? "Arbol MST General - Prim (costo)"
            : "Arbol MST General - Kruskal (tiempo)";
        mostrarArbolMSTVisual(titulo);
        limpiarLineasResultado("Despliegue grafico: Arbol MST");
        if (ultimoArbolMST.estaVacio()) {
            lineasResultado[0] = "No hay ciudades en el grafo para construir el MST.";
        } else {
            string pesoTexto = ultimoMSTEsPrim
                ? ("$" + to_string(ultimoArbolMST.getPesoTotal()))
                : (to_string(ultimoArbolMST.getPesoTotal()) + " min");
            lineasResultado[0] = "Nodos: " + to_string(ultimoArbolMST.getCantidadNodos()) +
                                 " | Aristas: " + to_string(ultimoArbolMST.getCantidadAristas()) +
                                 " | Peso total: " + pesoTexto +
                                 " | Hojas: " + to_string(ultimoArbolMST.contarHojas());
            lineasResultado[1] = truncarTexto("Preorden: " + ultimoArbolMST.getPreorden(), 108);
            lineasResultado[2] = truncarTexto("Inorden: " + ultimoArbolMST.getInorden(), 108);
            lineasResultado[3] = "Clic de nuevo en 'Ver arboles' para cambiar a ABB de ciudades";
        }
    } else if (modoArbolVista == 1) {
        string codResaltado = "";
        int total = grafoVisual.getTotalCiudades();
        int idxConsulta = (grafoVisual.getCiudadDestino() >= 0 && grafoVisual.getCiudadDestino() < total)
            ? grafoVisual.getCiudadDestino()
            : grafoVisual.getCiudadOrigen();
        if (idxConsulta >= 0 && idxConsulta < total) {
            codResaltado = grafoVisual.getCiudad(idxConsulta).codigo;
        }
        const ArbolABB& abb = sistema.obtenerArbolCiudades();
        mostrarArbolABBVisual(codResaltado);
        limpiarLineasResultado("Despliegue grafico: Arbol ABB de Ciudades");
        lineasResultado[0] = "Nodos ABB: " + to_string(abb.getCantidad()) +
                             " | Altura: " + to_string(abb.getAltura()) +
                             " | Hojas: " + to_string(abb.contarHojas());
        lineasResultado[1] = truncarTexto("Preorden: " + abb.getPreorden(), 108);
        lineasResultado[2] = truncarTexto("Inorden: " + abb.getInorden(), 108);
        lineasResultado[3] = "Clic de nuevo en 'Ver arboles' para cambiar a AVL de pasajeros";
    } else {
        bool hayPax = false;
        Pasajero pax = obtenerPasajeroActivo(hayPax);
        string idResaltado = hayPax ? pax.getId() : "";
        const ArbolAVL& avl = sistema.obtenerArbolPasajeros();
        mostrarArbolAVLVisual(idResaltado);
        limpiarLineasResultado("Despliegue grafico: Arbol AVL de Pasajeros");
        lineasResultado[0] = "Nodos AVL: " + to_string(avl.getCantidad()) +
                             " | Altura: " + to_string(avl.getAltura()) +
                             " | Hojas: " + to_string(avl.contarHojas()) +
                             (hayPax ? (" | Activo: " + pax.getNombre()) : "");
        lineasResultado[1] = truncarTexto("Preorden: " + avl.getPreorden(), 108);
        lineasResultado[2] = truncarTexto("Inorden: " + avl.getInorden(), 108);
        lineasResultado[3] = "Clic de nuevo en 'Ver arboles' para volver al Arbol MST";
    }
}

void AplicacionVisual::ejecutarAccionAdmin(int indiceAccion) {
    ultimaAccionCliente = -1;
    switch (indiceAccion) {
        case 0:
            mostrarAeronavesYCrewAdmin();
            break;
        case 1:
            mostrarVuelosYPasajerosAdmin();
            break;
        case 2:
            mostrarRutaEquipajeAdmin();
            break;
        case 3:
            mostrarMSTAdmin(true);
            break;
        case 4:
            mostrarMSTAdmin(false);
            break;
        case 5:
            mostrarSiguienteArbolAdmin();
            break;
    }
    ultimaAccionAdmin = indiceAccion;
}

void AplicacionVisual::renderizar() {
    ventana.clear(sf::Color(23, 26, 31));

    sf::RectangleShape barraSuperior(sf::Vector2f(1200.0f, 52.0f));
    barraSuperior.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(barraSuperior);

    sf::RectangleShape panelIzquierdo(sf::Vector2f(275.0f, 668.0f));
    panelIzquierdo.setPosition(sf::Vector2f(0.0f, 52.0f));
    panelIzquierdo.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(panelIzquierdo);

    sf::RectangleShape panelResultados(sf::Vector2f(925.0f, 125.0f));
    panelResultados.setPosition(sf::Vector2f(275.0f, 595.0f));
    panelResultados.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(panelResultados);

    sf::RectangleShape lineaTop(sf::Vector2f(1200.0f, 1.0f));
    lineaTop.setPosition(sf::Vector2f(0.0f, 52.0f));
    lineaTop.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaTop);

    sf::RectangleShape lineaLateral(sf::Vector2f(1.0f, 668.0f));
    lineaLateral.setPosition(sf::Vector2f(275.0f, 52.0f));
    lineaLateral.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaLateral);

    sf::RectangleShape lineaResultados(sf::Vector2f(925.0f, 1.0f));
    lineaResultados.setPosition(sf::Vector2f(275.0f, 595.0f));
    lineaResultados.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaResultados);

    CompatSFML::dibujarTexto(ventana, fuente, "Aerolinea EIF207", 18.0f, 15.0f, 17, sf::Color(235, 238, 244));
    btnTabCliente.renderizar(ventana, fuente);
    btnTabAdmin.renderizar(ventana, fuente);
    btnTabMantenimiento.renderizar(ventana, fuente);

    if (moduloActual == ModuloActivo::Mantenimientos) {
        CompatSFML::dibujarTexto(ventana, fuente, "Catalogos (archivos data/)", 20.0f, 64.0f, 12, sf::Color(145, 153, 168));
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesMantenimientoEntidad[i].renderizar(ventana, fuente);
        }

        sf::RectangleShape sepCrud(sf::Vector2f(239.0f, 1.0f));
        sepCrud.setPosition(sf::Vector2f(18.0f, 344.0f));
        sepCrud.setFillColor(sf::Color(46, 51, 60));
        ventana.draw(sepCrud);

        CompatSFML::dibujarTexto(ventana, fuente, "CRUD del registro activo", 20.0f, 354.0f, 12, sf::Color(145, 153, 168));
        btnMaintAgregar.renderizar(ventana, fuente);
        btnMaintEditar.renderizar(ventana, fuente);
        btnMaintEliminar.renderizar(ventana, fuente);

        sf::RectangleShape sepVerif(sf::Vector2f(239.0f, 1.0f));
        sepVerif.setPosition(sf::Vector2f(18.0f, 518.0f));
        sepVerif.setFillColor(sf::Color(46, 51, 60));
        ventana.draw(sepVerif);

        CompatSFML::dibujarTexto(ventana, fuente, "Verificacion y enlace", 20.0f, 526.0f, 12, sf::Color(145, 153, 168));
        btnMaintVerEstructura.renderizar(ventana, fuente);

        CompatSFML::dibujarTexto(ventana, fuente, "Clic en tabla: elegir fila para editar/eliminar",
                                 18.0f, 686.0f, 11, sf::Color(115, 123, 138));
    } else {
        string txtOrigen = "-";
        string txtDestino = "-";
        if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadOrigen() >= 0) {
            txtOrigen = grafoVisual.getCiudad(grafoVisual.getCiudadOrigen()).codigo;
        }
        if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadDestino() >= 0) {
            txtDestino = grafoVisual.getCiudad(grafoVisual.getCiudadDestino()).codigo;
        }

        CompatSFML::dibujarTexto(ventana, fuente, "Origen:  " + txtOrigen, 20.0f, 74.0f, 13, sf::Color(110, 215, 155));
        btnOrigenAnt.renderizar(ventana, fuente);
        btnOrigenSig.renderizar(ventana, fuente);

        CompatSFML::dibujarTexto(ventana, fuente, "Destino: " + txtDestino, 20.0f, 108.0f, 13, sf::Color(235, 120, 120));
        btnDestinoAnt.renderizar(ventana, fuente);
        btnDestinoSig.renderizar(ventana, fuente);

        sf::RectangleShape sepMenu(sf::Vector2f(239.0f, 1.0f));
        sepMenu.setPosition(sf::Vector2f(18.0f, 146.0f));
        sepMenu.setFillColor(sf::Color(46, 51, 60));
        ventana.draw(sepMenu);

        string subtitulo = (moduloActual == ModuloActivo::Cliente) ? "Cliente" : "Administrativo";
        CompatSFML::dibujarTexto(ventana, fuente, subtitulo, 20.0f, 154.0f, 12, sf::Color(145, 153, 168));

        if (moduloActual == ModuloActivo::Cliente) {
            for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
                botonesCliente[i].renderizar(ventana, fuente);
            }
            sf::RectangleShape sepCliente(sf::Vector2f(239.0f, 1.0f));
            sepCliente.setPosition(sf::Vector2f(18.0f, 446.0f));
            sepCliente.setFillColor(sf::Color(46, 51, 60));
            ventana.draw(sepCliente);

            CompatSFML::dibujarTexto(ventana, fuente, "Acciones cliente", 20.0f, 454.0f, 12, sf::Color(145, 153, 168));
            btnClienteReservarVuelo.renderizar(ventana, fuente);
            btnClienteSiguientePasajero.renderizar(ventana, fuente);
        } else {
            for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
                botonesAdmin[i].renderizar(ventana, fuente);
            }

            sf::RectangleShape sepAdmin(sf::Vector2f(239.0f, 1.0f));
            sepAdmin.setPosition(sf::Vector2f(18.0f, 446.0f));
            sepAdmin.setFillColor(sf::Color(46, 51, 60));
            ventana.draw(sepAdmin);

            CompatSFML::dibujarTexto(ventana, fuente, "Gestion de datos", 20.0f, 454.0f, 12, sf::Color(145, 153, 168));
            btnAdminIrMantenimientos.renderizar(ventana, fuente);
        }

        string txtHintLateral = (moduloActual == ModuloActivo::Administrativo &&
                                 vistaActual == VistaLienzo::PanelAdmin)
            ? "Clic en centro: filas / paginas"
            : "Clic izq: origen  |  Clic der: destino";
        CompatSFML::dibujarTexto(ventana, fuente, txtHintLateral,
                                 20.0f, 686.0f, 11, sf::Color(115, 123, 138));
    }

    if (vistaActual == VistaLienzo::PanelMantenimiento) {
        panelMantenimiento.renderizar(ventana, fuente, sistema);
    } else if (vistaActual == VistaLienzo::PanelAdmin) {
        panelAdmin.renderizar(ventana, fuente, sistema, indiceAeronaveActiva, indiceVueloActivo);
    } else if (vistaActual == VistaLienzo::MapaGrafo) {
        grafoVisual.renderizar(ventana, fuente);
    } else {
        arbolVisual.renderizar(ventana, fuente);
    }

    CompatSFML::dibujarTexto(ventana, fuente, tituloResultado, 298.0f, 606.0f, 14, sf::Color(130, 180, 245));
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        if (!lineasResultado[i].empty()) {
            CompatSFML::dibujarTexto(ventana, fuente, lineasResultado[i],
                                     298.0f, 632.0f + i * 20.0f, 13, sf::Color(208, 214, 224));
        }
    }

    modal.renderizar(ventana, fuente);
    ventana.display();
}

void AplicacionVisual::cambiarModulo(ModuloActivo nuevoModulo) {
    moduloActual = nuevoModulo;
    btnTabCliente.setActivo(nuevoModulo == ModuloActivo::Cliente);
    btnTabAdmin.setActivo(nuevoModulo == ModuloActivo::Administrativo);
    btnTabMantenimiento.setActivo(nuevoModulo == ModuloActivo::Mantenimientos);

    if (nuevoModulo == ModuloActivo::Mantenimientos) {
        actualizarTextosMantenimiento();
        panelMantenimiento.ajustarSeleccion(panelMantenimiento.getIndiceSeleccionado(), sistema);
        vistaActual = VistaLienzo::PanelMantenimiento;
        actualizarDetalleRegistroMantenimiento();
    } else if (nuevoModulo == ModuloActivo::Administrativo) {
        ultimaAccionAdmin = -1;
        mostrarAeronavesYCrewAdmin();
    } else if (vistaActual == VistaLienzo::PanelMantenimiento || vistaActual == VistaLienzo::PanelAdmin) {
        mostrarMapaRutas();
    }
}

void AplicacionVisual::actualizarTextosMantenimiento() {
    int totales[6] = {
        sistema.obtenerCiudades().tamano(),
        sistema.obtenerRutas().tamano(),
        sistema.obtenerAeronaves().tamano(),
        sistema.obtenerTripulantes().tamano(),
        sistema.obtenerVuelos().tamano(),
        sistema.obtenerArbolPasajeros().getCantidad()
    };
    int activa = panelMantenimiento.getEntidad();
    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesMantenimientoEntidad[i].setTexto(
            panelMantenimiento.getNombrePlural(i) + " (" + to_string(totales[i]) + ")");
        botonesMantenimientoEntidad[i].setActivo(i == activa);
    }
}

void AplicacionVisual::actualizarDetalleRegistroMantenimiento() {
    int entidad = panelMantenimiento.getEntidad();
    int total = panelMantenimiento.getTotalRegistros(sistema);
    int sel = panelMantenimiento.getIndiceSeleccionado();
    string nombreCat = panelMantenimiento.getNombrePlural(entidad);
    string ruta = sistema.obtenerDirectorioDatos() + "/" + panelMantenimiento.getArchivoDatos(entidad);

    limpiarLineasResultado("Detalle de " + nombreCat + " (" + ruta + ")");

    if (total <= 0 || sel < 0 || sel >= total) {
        lineasResultado[0] = "No hay registros existentes en " + nombreCat + ".";
        lineasResultado[1] = "Archivo destino: " + ruta + " | Estructura: " +
                             panelMantenimiento.getDescripcionMemoria(entidad);
        lineasResultado[2] = "Haz clic en 'Agregar nuevo' en el panel izquierdo para crear el primer registro.";
        return;
    }

    if (entidad == 0) {
        const Ciudad& c = sistema.obtenerCiudades().obtener(sel);
        int salientes = sistema.contarRutasDesdeCiudad(c.obtenerCodigo());
        lineasResultado[0] = "Seleccionado [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             c.obtenerCodigo() + " - " + c.obtenerNombre() + " | Posicion mapa: (" +
                             to_string(static_cast<int>(c.obtenerX())) + ", " +
                             to_string(static_cast<int>(c.obtenerY())) + ")";
        lineasResultado[1] = "Rutas salientes en grafo: " + to_string(salientes) +
                             " | Nodos en ABB: " + to_string(sistema.obtenerArbolCiudades().getCantidad());
    } else if (entidad == 1) {
        const Ruta& r = sistema.obtenerRutas().obtener(sel);
        lineasResultado[0] = "Seleccionada [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             r.obtenerOrigenCodigo() + " -> " + r.obtenerDestinoCodigo() +
                             " | Tarifa: $" + to_string(r.obtenerCosto()) +
                             " | Duracion: " + to_string(r.obtenerTiempoMin()) + " min";
        lineasResultado[1] = truncarTexto("Ciudades disponibles para rutas: " +
                                          panelMantenimiento.getResumenCiudadesDisponibles(sistema, 80), 108);
    } else if (entidad == 2) {
        const Aeronave& a = sistema.obtenerAeronaves().obtener(sel);
        lineasResultado[0] = "Seleccionada [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             a.getPlaca() + " | " + a.getMarca() + " " + a.getModelo() +
                             " | Cap: " + to_string(a.getCapacidad()) + " | Estado: " + a.getEstado();
        lineasResultado[1] = truncarTexto("Tripulacion asignada: " +
                                          sistema.obtenerResumenTripulacionPorAeronave(a.getPlaca()), 108);
    } else if (entidad == 3) {
        const Tripulante& t = sistema.obtenerTripulantes().obtener(sel);
        lineasResultado[0] = "Seleccionado [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             t.getId() + " | " + t.getNombre() + " | Rol: " + t.getRol() +
                             " | Aeronave: " + t.getPlacaAeronave();
        lineasResultado[1] = truncarTexto("Aeronaves disponibles en flota: " +
                                          panelMantenimiento.getResumenPlacasDisponibles(sistema, 76), 108);
    } else if (entidad == 4) {
        const Vuelo& v = sistema.obtenerVuelos().obtener(sel);
        lineasResultado[0] = "Seleccionado [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             v.obtenerCodigoVuelo() + " (" + v.obtenerOrigenCodigo() + " -> " +
                             v.obtenerDestinoCodigo() + ") | Avion: " + v.obtenerPlacaAeronave() +
                             " | $" + to_string(v.obtenerCosto()) + " | " + to_string(v.obtenerTiempoMin()) + " min";
        lineasResultado[1] = truncarTexto("Pasajeros asignados (AVL): " +
                                          sistema.obtenerResumenPasajerosPorVuelo(v.obtenerCodigoVuelo()), 108);
    } else if (entidad == 5) {
        ListaEnlazada<Pasajero> lista = sistema.obtenerListaPasajeros();
        const Pasajero& p = lista.obtener(sel);
        lineasResultado[0] = "Seleccionado [" + to_string(sel + 1) + "/" + to_string(total) + "]: " +
                             p.getId() + " - " + p.getNombre() + " | Maletas: " +
                             to_string(p.getCantidadMaletas());
        lineasResultado[1] = truncarTexto("Vuelos asignados en AVL: " + p.getVuelosTexto(), 108);
    }

    lineasResultado[2] = truncarTexto("Archivo: " + ruta + " | Memoria: " +
                                      panelMantenimiento.getDescripcionMemoria(entidad), 108);
    lineasResultado[3] = "Usa Agregar nuevo, Editar seleccionado o Eliminar seleccionado en el panel izquierdo";
}

void AplicacionVisual::finalizarCrudMantenimiento(int idxSeleccionado, const string& accionMensaje) {
    actualizarTextosMantenimiento();
    panelMantenimiento.ajustarSeleccion(idxSeleccionado, sistema);
    if (moduloActual == ModuloActivo::Mantenimientos) {
        vistaActual = VistaLienzo::PanelMantenimiento;
        actualizarDetalleRegistroMantenimiento();
        tituloResultado = accionMensaje;
    }
}

void AplicacionVisual::mostrarEstructuraDesdeMantenimiento() {
    int entidad = panelMantenimiento.getEntidad();
    int total = panelMantenimiento.getTotalRegistros(sistema);
    int sel = panelMantenimiento.getIndiceSeleccionado();

    if (entidad == 0) {
        cambiarModulo(ModuloActivo::Cliente);
        if (total > 0 && sel >= 0 && sel < total) {
            const Ciudad& c = sistema.obtenerCiudades().obtener(sel);
            vistaActual = VistaLienzo::ArbolEstructura;
            mostrarArbolABBVisual(c.obtenerCodigo());
            limpiarLineasResultado("Vista en ABB desde Mantenimientos");
            lineasResultado[0] = "Ciudad seleccionada: " + c.obtenerNombre() + " (" + c.obtenerCodigo() + ")";
            lineasResultado[1] = truncarTexto("Inorden ABB: " + sistema.obtenerArbolCiudades().getInorden(), 108);
        }
        return;
    }

    if (entidad == 1) {
        cambiarModulo(ModuloActivo::Administrativo);
        vistaActual = VistaLienzo::MapaGrafo;
        ultimaAccionAdmin = -1;
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].setActivo(false);
        }
        if (total > 0 && sel >= 0 && sel < total) {
            const Ruta& r = sistema.obtenerRutas().obtener(sel);
            int idxO = sistema.obtenerGrafo().buscarIndicePorCodigo(r.obtenerOrigenCodigo());
            int idxD = sistema.obtenerGrafo().buscarIndicePorCodigo(r.obtenerDestinoCodigo());
            if (idxO != -1 && idxD != -1) {
                grafoVisual.setCiudadOrigen(idxO);
                grafoVisual.setCiudadDestino(idxD);
                grafoVisual.limpiarResaltados();
                grafoVisual.resaltarAristaRuta(idxO, idxD, false);
            }
            limpiarLineasResultado("Ruta en mapa desde Mantenimientos");
            lineasResultado[0] = "Ruta " + r.obtenerOrigenCodigo() + " -> " + r.obtenerDestinoCodigo() +
                                 ": $" + to_string(r.obtenerCosto()) + " | " + to_string(r.obtenerTiempoMin()) + " min";
        }
        return;
    }

    if (entidad == 2 || entidad == 3) {
        cambiarModulo(ModuloActivo::Administrativo);
        if (entidad == 2 && total > 0 && sel >= 0 && sel < total) {
            indiceAeronaveActiva = sel;
        } else if (entidad == 3 && total > 0 && sel >= 0 && sel < total) {
            string placa = sistema.obtenerTripulantes().obtener(sel).getPlacaAeronave();
            const ListaEnlazada<Aeronave>& aeros = sistema.obtenerAeronaves();
            for (int i = 0; i < aeros.tamano(); ++i) {
                if (aeros.obtener(i).getPlaca() == placa) {
                    indiceAeronaveActiva = i;
                    break;
                }
            }
        }
        ultimaAccionAdmin = -1;
        mostrarAeronavesYCrewAdmin();
        return;
    }

    if (entidad == 4) {
        cambiarModulo(ModuloActivo::Administrativo);
        if (total > 0 && sel >= 0 && sel < total) {
            indiceVueloActivo = sel;
        }
        ultimaAccionAdmin = -1;
        mostrarVuelosYPasajerosAdmin();
        return;
    }

    if (entidad == 5) {
        cambiarModulo(ModuloActivo::Cliente);
        if (total > 0 && sel >= 0 && sel < total) {
            indicePasajeroActivo = sel;
        }
        mostrarVuelosPasajeroAVL();
    }
}

string AplicacionVisual::limpiarCampoTexto(const string& texto) const {
    size_t inicio = 0;
    while (inicio < texto.size() && (texto[inicio] == ' ' || texto[inicio] == '\t')) {
        inicio++;
    }
    size_t fin = texto.size();
    while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\t')) {
        fin--;
    }
    return texto.substr(inicio, fin - inicio);
}

bool AplicacionVisual::igualSinCaso(const string& a, const string& b) const {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        char ca = a[i];
        char cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca = static_cast<char>(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z') cb = static_cast<char>(cb - 'A' + 'a');
        if (ca != cb) {
            return false;
        }
    }
    return true;
}

bool AplicacionVisual::parsearEntero(const string& texto, int& salida) const {
    string s = limpiarCampoTexto(texto);
    if (s.empty()) {
        return false;
    }
    long long acumulado = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
        acumulado = acumulado * 10 + (s[i] - '0');
        if (acumulado > 1000000000LL) {
            return false;
        }
    }
    salida = static_cast<int>(acumulado);
    return true;
}

bool AplicacionVisual::parsearFlotante(const string& texto, float& salida) const {
    string s = limpiarCampoTexto(texto);
    if (s.empty()) {
        return false;
    }
    bool vioDigito = false;
    bool vioPunto = false;
    double acumulado = 0.0;
    double decimalFactor = 0.1;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '.') {
            if (vioPunto) {
                return false;
            }
            vioPunto = true;
        } else if (c >= '0' && c <= '9') {
            vioDigito = true;
            int d = c - '0';
            if (!vioPunto) {
                acumulado = acumulado * 10.0 + d;
            } else {
                acumulado += d * decimalFactor;
                decimalFactor *= 0.1;
            }
            if (acumulado > 100000.0) {
                return false;
            }
        } else {
            return false;
        }
    }
    if (!vioDigito) {
        return false;
    }
    salida = static_cast<float>(acumulado);
    return true;
}

void AplicacionVisual::calcularPosicionCiudadNueva(float& x, float& y) const {
    const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
    int inicio = ciudades.tamano();
    for (int intento = 0; intento < 20; ++intento) {
        int slot = (inicio + intento) % 20;
        float cx = 355.0f + static_cast<float>(slot % 5) * 185.0f;
        float cy = 115.0f + static_cast<float>(slot / 5) * 125.0f;
        bool colision = false;
        const ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
        while (actual != nullptr) {
            float dx = actual->dato.obtenerX() - cx;
            float dy = actual->dato.obtenerY() - cy;
            if (dx * dx + dy * dy < 3200.0f) {
                colision = true;
                break;
            }
            actual = actual->siguiente;
        }
        if (!colision) {
            x = cx;
            y = cy;
            return;
        }
    }
    x = 640.0f;
    y = 310.0f;
}

void AplicacionVisual::seleccionarPasajeroActivoPorId(const string& idPasajero) {
    ListaEnlazada<Pasajero> lista = sistema.obtenerListaPasajeros();
    for (int i = 0; i < lista.tamano(); ++i) {
        if (lista.obtener(i).getId() == idPasajero) {
            indicePasajeroActivo = i;
            return;
        }
    }
}

void AplicacionVisual::refrescarTrasCambioCiudadesORutas() {
    sincronizarMapaVisual();
    ultimoArbolMST.limpiar();
    grafoVisual.limpiarResaltados();
}

void AplicacionVisual::abrirModalClienteBuscarABB() {
    string valorInicial = "";
    int total = grafoVisual.getTotalCiudades();
    int idxConsulta = (grafoVisual.getCiudadDestino() >= 0)
        ? grafoVisual.getCiudadDestino()
        : grafoVisual.getCiudadOrigen();
    if (idxConsulta >= 0 && idxConsulta < total) {
        valorInicial = grafoVisual.getCiudad(idxConsulta).nombre;
    }

    modal.abrir(MODAL_CLIENTE_BUSCAR_ABB,
                "Buscar ciudad en el ABB",
                "Escribe el nombre o codigo de la ciudad a buscar",
                "Buscar");
    modal.agregarCampo("Nombre o codigo de ciudad", valorInicial, false, 40);
}

void AplicacionVisual::abrirModalClienteConsultarAVL() {
    bool hayPax = false;
    Pasajero pax = obtenerPasajeroActivo(hayPax);
    string idInicial = hayPax ? pax.getId() : "";

    modal.abrir(MODAL_CLIENTE_CONSULTAR_AVL,
                "Consultar pasajero en el AVL",
                "Ingresa el ID del pasajero (ej: P001) para ver sus vuelos",
                "Consultar");
    modal.agregarCampo("ID de pasajero", idInicial, false, 20);
}

void AplicacionVisual::abrirModalClienteReservarVuelo() {
    bool hayPax = false;
    Pasajero pax = obtenerPasajeroActivo(hayPax);

    string idInicial = hayPax ? pax.getId() : "P101";
    string nombreInicial = hayPax ? pax.getNombre() : "";
    string maletasInicial = hayPax ? to_string(pax.getCantidadMaletas()) : "1";

    string vueloSugerido = "";
    if (grafoVisual.getTotalCiudades() > 1 &&
        grafoVisual.getCiudadOrigen() >= 0 &&
        grafoVisual.getCiudadDestino() >= 0) {
        string codO = grafoVisual.getCiudad(grafoVisual.getCiudadOrigen()).codigo;
        string codD = grafoVisual.getCiudad(grafoVisual.getCiudadDestino()).codigo;
        ListaEnlazada<Vuelo> directos = sistema.buscarVuelosDirectos(codO, codD);
        if (!directos.estaVacia()) {
            vueloSugerido = directos.obtener(0).obtenerCodigoVuelo();
        }
    }
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    if (vueloSugerido.empty() && !vuelos.estaVacia()) {
        int idx = ((indiceVueloActivo % vuelos.tamano()) + vuelos.tamano()) % vuelos.tamano();
        vueloSugerido = vuelos.obtener(idx).obtenerCodigoVuelo();
    }

    modal.abrir(MODAL_CLIENTE_RESERVAR_VUELO,
                "Seleccionar vuelo para pasajero (AVL)",
                "Asigna un vuelo a un pasajero nuevo o existente en el AVL",
                "Reservar");
    modal.agregarCampo("ID de pasajero", idInicial, false, 20);
    modal.agregarCampo("Nombre del pasajero", nombreInicial, false, 40);
    modal.agregarCampo("Codigo de vuelo (ej: VU101)", vueloSugerido, false, 20);
    modal.agregarCampo("Cantidad de maletas", maletasInicial, true, 4);
}

void AplicacionVisual::abrirModalCrud(int tipoOperacion) {
    int entidad = panelMantenimiento.getEntidad();
    switch (entidad) {
        case 0:
            abrirModalCrudCiudad(tipoOperacion);
            break;
        case 1:
            abrirModalCrudRuta(tipoOperacion);
            break;
        case 2:
            abrirModalCrudAeronave(tipoOperacion);
            break;
        case 3:
            abrirModalCrudTripulante(tipoOperacion);
            break;
        case 4:
            abrirModalCrudVuelo(tipoOperacion);
            break;
        case 5:
            abrirModalCrudPasajero(tipoOperacion);
            break;
    }
}

void AplicacionVisual::abrirModalCrudCiudad(int tipoOperacion) {
    const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
    int total = ciudades.tamano();

    if (tipoOperacion == 0) {
        float xDef = 640.0f;
        float yDef = 310.0f;
        calcularPosicionCiudadNueva(xDef, yDef);
        modal.abrir(MODAL_CRUD_CIUDAD_AGREGAR,
                    "Mantenimiento Ciudades: Agregar",
                    "Se guarda en data/ciudades.txt | Existentes: " +
                    panelMantenimiento.getResumenCiudadesDisponibles(sistema, 34),
                    "Agregar");
        modal.agregarCampo("Codigo de ciudad (ej: LIB)", "", false, 10);
        modal.agregarCampo("Nombre de la ciudad", "", false, 40);
        modal.agregarCampo("Posicion X en mapa (300 - 1155)", to_string(static_cast<int>(xDef)), true, 6, true);
        modal.agregarCampo("Posicion Y en mapa (75 - 565)", to_string(static_cast<int>(yDef)), true, 6, true);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Ciudades");
        lineasResultado[0] = "No hay ciudades registradas para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Ciudad& c = ciudades.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_CIUDAD_EDITAR,
                    "Mantenimiento Ciudades: Editar (" + c.obtenerCodigo() + ")",
                    "Actualiza en Grafo, ABB y data/ciudades.txt",
                    "Guardar");
        modal.agregarCampo("Codigo de ciudad existente", c.obtenerCodigo(), false, 10);
        modal.agregarCampo("Nuevo nombre", c.obtenerNombre(), false, 40);
        modal.agregarCampo("Nueva posicion X (300 - 1155)", to_string(static_cast<int>(c.obtenerX())), true, 6, true);
        modal.agregarCampo("Nueva posicion Y (75 - 565)", to_string(static_cast<int>(c.obtenerY())), true, 6, true);
    } else {
        modal.abrir(MODAL_CRUD_CIUDAD_ELIMINAR,
                    "Mantenimiento Ciudades: Eliminar (" + c.obtenerCodigo() + ")",
                    "Elimina ciudad, sus rutas asociadas y actualiza data/",
                    "Eliminar");
        modal.agregarCampo("Codigo de ciudad a eliminar", c.obtenerCodigo(), false, 10);
    }
}

void AplicacionVisual::abrirModalCrudRuta(int tipoOperacion) {
    const ListaEnlazada<Ruta>& rutas = sistema.obtenerRutas();
    int total = rutas.tamano();

    if (tipoOperacion == 0) {
        string codO = "";
        string codD = "";
        if (sistema.obtenerCiudades().tamano() >= 2) {
            codO = sistema.obtenerCiudades().obtener(0).obtenerCodigo();
            codD = sistema.obtenerCiudades().obtener(1).obtenerCodigo();
        }
        modal.abrir(MODAL_CRUD_RUTA_AGREGAR,
                    "Mantenimiento Rutas: Agregar",
                    "Guarda en data/rutas.txt | Ciudades: " +
                    panelMantenimiento.getResumenCiudadesDisponibles(sistema, 36),
                    "Agregar");
        modal.agregarCampo("Codigo ciudad origen", codO, false, 10);
        modal.agregarCampo("Codigo ciudad destino", codD, false, 10);
        modal.agregarCampo("Costo ($)", "250", true, 8);
        modal.agregarCampo("Duracion (min)", "120", true, 8);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Rutas");
        lineasResultado[0] = "No hay rutas registradas para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Ruta& r = rutas.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_RUTA_EDITAR,
                    "Mantenimiento Rutas: Editar (" + r.obtenerOrigenCodigo() + " -> " + r.obtenerDestinoCodigo() + ")",
                    "Actualiza tarifa/duracion en GrafoVuelos y data/rutas.txt",
                    "Guardar");
        modal.agregarCampo("Codigo ciudad origen", r.obtenerOrigenCodigo(), false, 10);
        modal.agregarCampo("Codigo ciudad destino", r.obtenerDestinoCodigo(), false, 10);
        modal.agregarCampo("Nuevo costo ($)", to_string(r.obtenerCosto()), true, 8);
        modal.agregarCampo("Nueva duracion (min)", to_string(r.obtenerTiempoMin()), true, 8);
    } else {
        modal.abrir(MODAL_CRUD_RUTA_ELIMINAR,
                    "Mantenimiento Rutas: Eliminar (" + r.obtenerOrigenCodigo() + " -> " + r.obtenerDestinoCodigo() + ")",
                    "Elimina la ruta dirigida del grafo y data/rutas.txt",
                    "Eliminar");
        modal.agregarCampo("Codigo ciudad origen", r.obtenerOrigenCodigo(), false, 10);
        modal.agregarCampo("Codigo ciudad destino", r.obtenerDestinoCodigo(), false, 10);
    }
}

void AplicacionVisual::abrirModalCrudAeronave(int tipoOperacion) {
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int total = aeronaves.tamano();

    if (tipoOperacion == 0) {
        modal.abrir(MODAL_CRUD_AERONAVE_AGREGAR,
                    "Mantenimiento Aeronaves: Agregar",
                    "Se guarda en data/aeronaves.txt | Estados: Activa / En vuelo / Mantenimiento",
                    "Agregar");
        modal.agregarCampo("Placa (ej: TI-AIR06)", "", false, 20);
        modal.agregarCampo("Marca (ej: Airbus)", "", false, 30);
        modal.agregarCampo("Modelo (ej: A320)", "", false, 30);
        modal.agregarCampo("Capacidad de pasajeros", "160", true, 5);
        modal.agregarCampo("Estado (Activa/En vuelo/Mantenimiento)", "Activa", false, 20);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Aeronaves");
        lineasResultado[0] = "No hay aeronaves registradas para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Aeronave& activa = aeronaves.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_AERONAVE_EDITAR,
                    "Mantenimiento Aeronaves: Editar (" + activa.getPlaca() + ")",
                    "Actualiza flota y guarda en data/aeronaves.txt",
                    "Guardar");
        modal.agregarCampo("Placa existente", activa.getPlaca(), false, 20);
        modal.agregarCampo("Nueva marca", activa.getMarca(), false, 30);
        modal.agregarCampo("Nuevo modelo", activa.getModelo(), false, 30);
        modal.agregarCampo("Nueva capacidad", to_string(activa.getCapacidad()), true, 5);
        modal.agregarCampo("Nuevo estado", activa.getEstado(), false, 20);
    } else {
        modal.abrir(MODAL_CRUD_AERONAVE_ELIMINAR,
                    "Mantenimiento Aeronaves: Eliminar (" + activa.getPlaca() + ")",
                    "Elimina la aeronave de flota y data/aeronaves.txt",
                    "Eliminar");
        modal.agregarCampo("Placa de aeronave a eliminar", activa.getPlaca(), false, 20);
    }
}

void AplicacionVisual::abrirModalCrudTripulante(int tipoOperacion) {
    const ListaEnlazada<Tripulante>& tripulantes = sistema.obtenerTripulantes();
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int total = tripulantes.tamano();

    string placaDefault = aeronaves.estaVacia() ? "" : aeronaves.obtener(0).getPlaca();

    if (tipoOperacion == 0) {
        modal.abrir(MODAL_CRUD_TRIPULANTE_AGREGAR,
                    "Mantenimiento Tripulacion: Agregar",
                    "Guarda data/tripulacion.txt | Aviones: " +
                    panelMantenimiento.getResumenPlacasDisponibles(sistema, 36),
                    "Agregar");
        modal.agregarCampo("ID tripulante (ej: TR-010)", "", false, 20);
        modal.agregarCampo("Nombre completo", "", false, 40);
        modal.agregarCampo("Rol (Piloto/Copiloto/Sobrecargo)", "Piloto", false, 25);
        modal.agregarCampo("Placa de aeronave asignada", placaDefault, false, 20);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Tripulacion");
        lineasResultado[0] = "No hay tripulantes registrados para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Tripulante& activo = tripulantes.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_TRIPULANTE_EDITAR,
                    "Mantenimiento Tripulacion: Editar (" + activo.getId() + ")",
                    "Aviones: " + panelMantenimiento.getResumenPlacasDisponibles(sistema, 52),
                    "Guardar");
        modal.agregarCampo("ID tripulante existente", activo.getId(), false, 20);
        modal.agregarCampo("Nuevo nombre", activo.getNombre(), false, 40);
        modal.agregarCampo("Nuevo rol", activo.getRol(), false, 25);
        modal.agregarCampo("Nueva placa de aeronave", activo.getPlacaAeronave(), false, 20);
    } else {
        modal.abrir(MODAL_CRUD_TRIPULANTE_ELIMINAR,
                    "Mantenimiento Tripulacion: Eliminar (" + activo.getId() + ")",
                    "Elimina el tripulante de data/tripulacion.txt",
                    "Eliminar");
        modal.agregarCampo("ID de tripulante a eliminar", activo.getId(), false, 20);
    }
}

void AplicacionVisual::abrirModalCrudVuelo(int tipoOperacion) {
    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int total = vuelos.tamano();

    string codO = "";
    string codD = "";
    if (sistema.obtenerCiudades().tamano() >= 2) {
        codO = sistema.obtenerCiudades().obtener(0).obtenerCodigo();
        codD = sistema.obtenerCiudades().obtener(1).obtenerCodigo();
    }
    string placaDefault = aeronaves.estaVacia() ? "" : aeronaves.obtener(0).getPlaca();

    if (tipoOperacion == 0) {
        modal.abrir(MODAL_CRUD_VUELO_AGREGAR,
                    "Mantenimiento Vuelos: Agregar",
                    "Guarda data/vuelos.txt | Ciudades: " +
                    panelMantenimiento.getResumenCiudadesDisponibles(sistema, 34),
                    "Agregar");
        modal.agregarCampo("Codigo de vuelo (ej: VU110)", "", false, 20);
        modal.agregarCampo("Codigo origen", codO, false, 10);
        modal.agregarCampo("Codigo destino", codD, false, 10);
        modal.agregarCampo("Placa aeronave (" + placaDefault + ")", placaDefault, false, 20);
        modal.agregarCampo("Costo ($)", "250", true, 8);
        modal.agregarCampo("Duracion (min)", "120", true, 8);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Vuelos");
        lineasResultado[0] = "No hay vuelos registrados para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Vuelo& activo = vuelos.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_VUELO_EDITAR,
                    "Mantenimiento Vuelos: Editar (" + activo.obtenerCodigoVuelo() + ")",
                    "Ciudades: " + panelMantenimiento.getResumenCiudadesDisponibles(sistema, 50),
                    "Guardar");
        modal.agregarCampo("Codigo de vuelo existente", activo.obtenerCodigoVuelo(), false, 20);
        modal.agregarCampo("Nuevo origen", activo.obtenerOrigenCodigo(), false, 10);
        modal.agregarCampo("Nuevo destino", activo.obtenerDestinoCodigo(), false, 10);
        modal.agregarCampo("Nueva placa aeronave", activo.obtenerPlacaAeronave(), false, 20);
        modal.agregarCampo("Nuevo costo ($)", to_string(activo.obtenerCosto()), true, 8);
        modal.agregarCampo("Nueva duracion (min)", to_string(activo.obtenerTiempoMin()), true, 8);
    } else {
        modal.abrir(MODAL_CRUD_VUELO_ELIMINAR,
                    "Mantenimiento Vuelos: Eliminar (" + activo.obtenerCodigoVuelo() + ")",
                    "Elimina el vuelo programado de data/vuelos.txt",
                    "Eliminar");
        modal.agregarCampo("Codigo de vuelo a eliminar", activo.obtenerCodigoVuelo(), false, 20);
    }
}

void AplicacionVisual::abrirModalCrudPasajero(int tipoOperacion) {
    ListaEnlazada<Pasajero> pasajeros = sistema.obtenerListaPasajeros();
    int total = pasajeros.tamano();

    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    string vueloInicial = vuelos.estaVacia() ? "" : vuelos.obtener(0).obtenerCodigoVuelo();

    if (tipoOperacion == 0) {
        modal.abrir(MODAL_CRUD_PASAJERO_AGREGAR,
                    "Mantenimiento Pasajeros (AVL): Agregar",
                    "Guarda data/pasajeros.txt | Vuelos: " +
                    panelMantenimiento.getResumenVuelosDisponibles(sistema, 34),
                    "Agregar");
        modal.agregarCampo("ID pasajero (ej: P010)", "", false, 20);
        modal.agregarCampo("Nombre completo", "", false, 40);
        modal.agregarCampo("Cantidad de maletas", "1", true, 4);
        modal.agregarCampo("Vuelo inicial (opcional)", vueloInicial, false, 20);
        return;
    }

    if (total == 0) {
        limpiarLineasResultado("Mantenimiento de Pasajeros");
        lineasResultado[0] = "No hay pasajeros registrados en el AVL para editar o eliminar.";
        return;
    }

    int sel = panelMantenimiento.getIndiceSeleccionado();
    if (sel < 0 || sel >= total) {
        sel = 0;
    }
    const Pasajero& pax = pasajeros.obtener(sel);

    if (tipoOperacion == 1) {
        modal.abrir(MODAL_CRUD_PASAJERO_EDITAR,
                    "Mantenimiento Pasajeros (AVL): Editar (" + pax.getId() + ")",
                    "Vuelos disponibles: " + panelMantenimiento.getResumenVuelosDisponibles(sistema, 46),
                    "Guardar");
        modal.agregarCampo("ID pasajero existente", pax.getId(), false, 20);
        modal.agregarCampo("Nuevo nombre", pax.getNombre(), false, 40);
        modal.agregarCampo("Nueva cantidad de maletas", to_string(pax.getCantidadMaletas()), true, 4);
        modal.agregarCampo("Asignar o quitar vuelo (opcional)", "", false, 20);
    } else {
        modal.abrir(MODAL_CRUD_PASAJERO_ELIMINAR,
                    "Mantenimiento Pasajeros (AVL): Eliminar (" + pax.getId() + ")",
                    "Elimina el pasajero del Arbol AVL y data/pasajeros.txt",
                    "Eliminar");
        modal.agregarCampo("ID pasajero a eliminar", pax.getId(), false, 20);
    }
}

void AplicacionVisual::confirmarModalActivo() {
    int accion = modal.getAccionId();
    if (accion >= MODAL_CLIENTE_BUSCAR_ABB && accion <= MODAL_CLIENTE_RESERVAR_VUELO) {
        procesarModalCliente(accion);
    } else if (accion >= MODAL_CRUD_CIUDAD_AGREGAR && accion <= MODAL_CRUD_CIUDAD_ELIMINAR) {
        procesarModalCiudad(accion);
    } else if (accion >= MODAL_CRUD_RUTA_AGREGAR && accion <= MODAL_CRUD_RUTA_ELIMINAR) {
        procesarModalRuta(accion);
    } else if (accion >= MODAL_CRUD_AERONAVE_AGREGAR && accion <= MODAL_CRUD_AERONAVE_ELIMINAR) {
        procesarModalAeronave(accion);
    } else if (accion >= MODAL_CRUD_TRIPULANTE_AGREGAR && accion <= MODAL_CRUD_TRIPULANTE_ELIMINAR) {
        procesarModalTripulante(accion);
    } else if (accion >= MODAL_CRUD_VUELO_AGREGAR && accion <= MODAL_CRUD_VUELO_ELIMINAR) {
        procesarModalVuelo(accion);
    } else if (accion >= MODAL_CRUD_PASAJERO_AGREGAR && accion <= MODAL_CRUD_PASAJERO_ELIMINAR) {
        procesarModalPasajero(accion);
    }
}

void AplicacionVisual::procesarModalCliente(int accion) {
    if (accion == MODAL_CLIENTE_BUSCAR_ABB) {
        string entrada = limpiarCampoTexto(modal.getValor(0));
        if (entrada.empty()) {
            modal.setMensajeError("Ingresa el nombre o codigo de la ciudad");
            return;
        }
        const NodoABB* nodo = sistema.buscarCiudadABB(entrada);
        if (nodo == nullptr) {
            const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
            for (int i = 0; i < ciudades.tamano(); ++i) {
                const Ciudad& c = ciudades.obtener(i);
                if (igualSinCaso(c.obtenerNombre(), entrada) || igualSinCaso(c.obtenerCodigo(), entrada)) {
                    nodo = sistema.buscarCiudadABB(c.obtenerNombre());
                    break;
                }
            }
        }
        modal.cerrar();
        vistaActual = VistaLienzo::ArbolEstructura;
        limpiarLineasResultado("Busqueda de ciudad (ABB)");
        if (nodo == nullptr) {
            mostrarArbolABBVisual("");
            lineasResultado[0] = "No se encontro '" + entrada + "' en el ABB de ciudades.";
            lineasResultado[1] = truncarTexto("Inorden ABB: " + sistema.obtenerArbolCiudades().getInorden(), 108);
            return;
        }
        int salientes = sistema.contarRutasDesdeCiudad(nodo->codigo);
        const ArbolABB& abb = sistema.obtenerArbolCiudades();
        mostrarArbolABBVisual(nodo->codigo);
        lineasResultado[0] = truncarTexto(
            "Encontrada en ABB: " + nodo->nombre + " (" + nodo->codigo +
            ") | Vertice #" + to_string(nodo->indiceVertice), 108);
        lineasResultado[1] = "Rutas salientes: " + to_string(salientes) +
                             " | Nodos en ABB: " + to_string(abb.getCantidad()) +
                             " | Altura: " + to_string(abb.getAltura());
        lineasResultado[2] = truncarTexto("Inorden ABB: " + abb.getInorden(), 108);
        lineasResultado[3] = "Consulta visual realizada directamente por entrada en SFML";
        return;
    }

    if (accion == MODAL_CLIENTE_CONSULTAR_AVL) {
        string id = limpiarCampoTexto(modal.getValor(0));
        if (id.empty()) {
            modal.setMensajeError("Ingresa el ID del pasajero");
            return;
        }
        const NodoAVL* nodo = sistema.buscarPasajeroAVL(id);
        if (nodo == nullptr) {
            ListaEnlazada<Pasajero> lista = sistema.obtenerListaPasajeros();
            for (int i = 0; i < lista.tamano(); ++i) {
                if (igualSinCaso(lista.obtener(i).getId(), id)) {
                    nodo = sistema.buscarPasajeroAVL(lista.obtener(i).getId());
                    break;
                }
            }
        }
        modal.cerrar();
        if (nodo == nullptr) {
            vistaActual = VistaLienzo::ArbolEstructura;
            mostrarArbolAVLVisual("");
            limpiarLineasResultado("Consulta de pasajero (AVL)");
            lineasResultado[0] = "No se encontro pasajero con ID '" + id + "' en el AVL.";
            lineasResultado[1] = "Usa 'Seleccionar vuelo (AVL)' para registrar un nuevo pasajero.";
            return;
        }
        seleccionarPasajeroActivoPorId(nodo->dato.getId());
        mostrarPasajeroAVLEncontrando(nodo, "Consulta de pasajero (AVL)");
        return;
    }

    if (accion == MODAL_CLIENTE_RESERVAR_VUELO) {
        string id = limpiarCampoTexto(modal.getValor(0));
        string nombre = limpiarCampoTexto(modal.getValor(1));
        string codVuelo = limpiarCampoTexto(modal.getValor(2));
        int maletas = 0;
        if (id.empty() || nombre.empty() || codVuelo.empty()) {
            modal.setMensajeError("ID, nombre y codigo de vuelo son obligatorios");
            return;
        }
        if (!parsearEntero(modal.getValor(3), maletas) || maletas < 0) {
            modal.setMensajeError("Cantidad de maletas invalida");
            return;
        }
        if (!sistema.existeVuelo(codVuelo)) {
            modal.setMensajeError("El codigo de vuelo no existe en vuelos.txt");
            return;
        }

        const NodoAVL* existente = sistema.buscarPasajeroAVL(id);
        if (existente != nullptr) {
            sistema.modificarPasajeroAdmin(id, nombre, maletas);
            const NodoAVL* actualizado = sistema.buscarPasajeroAVL(id);
            if (actualizado != nullptr && !actualizado->dato.tieneVuelo(codVuelo)) {
                sistema.asignarVueloAPasajero(id, codVuelo);
            }
        } else {
            Pasajero nuevo(id, nombre, maletas);
            nuevo.agregarVuelo(codVuelo);
            if (!sistema.registrarPasajeroAdmin(nuevo)) {
                modal.setMensajeError("No se pudo registrar el pasajero en el AVL");
                return;
            }
        }

        modal.cerrar();
        seleccionarPasajeroActivoPorId(id);
        const NodoAVL* nodo = sistema.buscarPasajeroAVL(id);
        mostrarPasajeroAVLEncontrando(nodo, "Vuelo reservado en AVL");
        lineasResultado[3] = "Reserva guardada en data/pasajeros.txt | Vuelo: " + codVuelo;
    }
}

void AplicacionVisual::procesarModalCiudad(int accion) {
    if (accion == MODAL_CRUD_CIUDAD_AGREGAR) {
        if (sistema.obtenerCiudades().tamano() >= 30) {
            modal.setMensajeError("Se alcanzo el limite visual de 30 ciudades");
            return;
        }
        string codigo = limpiarCampoTexto(modal.getValor(0));
        string nombre = limpiarCampoTexto(modal.getValor(1));
        float x = 0.0f;
        float y = 0.0f;
        if (codigo.empty() || nombre.empty()) {
            modal.setMensajeError("Codigo y nombre son obligatorios");
            return;
        }
        if (!parsearFlotante(modal.getValor(2), x) || !parsearFlotante(modal.getValor(3), y) ||
            x < 300.0f || x > 1155.0f || y < 75.0f || y > 565.0f) {
            modal.setMensajeError("Coords validas en mapa: X [300..1155], Y [75..565]");
            return;
        }
        if (!sistema.agregarCiudadAdmin(Ciudad(codigo, nombre, x, y))) {
            modal.setMensajeError("Ya existe una ciudad con ese codigo o nombre");
            return;
        }
        modal.cerrar();
        refrescarTrasCambioCiudadesORutas();
        int idxNueva = sistema.obtenerCiudades().tamano() - 1;
        finalizarCrudMantenimiento(idxNueva, "Ciudad agregada y guardada en data/ciudades.txt");
        return;
    }

    if (accion == MODAL_CRUD_CIUDAD_EDITAR) {
        string codigo = limpiarCampoTexto(modal.getValor(0));
        string nombre = limpiarCampoTexto(modal.getValor(1));
        float x = 0.0f;
        float y = 0.0f;
        if (codigo.empty() || nombre.empty()) {
            modal.setMensajeError("Codigo y nuevo nombre son obligatorios");
            return;
        }
        if (!parsearFlotante(modal.getValor(2), x) || !parsearFlotante(modal.getValor(3), y) ||
            x < 300.0f || x > 1155.0f || y < 75.0f || y > 565.0f) {
            modal.setMensajeError("Coords validas en mapa: X [300..1155], Y [75..565]");
            return;
        }
        if (!sistema.modificarCiudadAdmin(codigo, nombre, x, y)) {
            modal.setMensajeError("Codigo no encontrado o nombre duplicado");
            return;
        }
        modal.cerrar();
        refrescarTrasCambioCiudadesORutas();
        int idxEditada = panelMantenimiento.getIndiceSeleccionado();
        const ListaEnlazada<Ciudad>& ciudades = sistema.obtenerCiudades();
        for (int i = 0; i < ciudades.tamano(); ++i) {
            if (ciudades.obtener(i).obtenerCodigo() == codigo) {
                idxEditada = i;
                break;
            }
        }
        finalizarCrudMantenimiento(idxEditada, "Ciudad editada y guardada en data/ciudades.txt");
        return;
    }

    if (accion == MODAL_CRUD_CIUDAD_ELIMINAR) {
        string codigo = limpiarCampoTexto(modal.getValor(0));
        if (codigo.empty()) {
            modal.setMensajeError("Ingresa el codigo de ciudad a eliminar");
            return;
        }
        if (!sistema.eliminarCiudadAdmin(codigo)) {
            modal.setMensajeError("No existe una ciudad con ese codigo");
            return;
        }
        modal.cerrar();
        refrescarTrasCambioCiudadesORutas();
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Ciudad eliminada (rutas/vuelos vinculados actualizados en data/)");
    }
}

void AplicacionVisual::procesarModalRuta(int accion) {
    string codO = limpiarCampoTexto(modal.getValor(0));
    string codD = limpiarCampoTexto(modal.getValor(1));
    if (codO.empty() || codD.empty()) {
        modal.setMensajeError("Origen y destino son obligatorios");
        return;
    }

    if (accion == MODAL_CRUD_RUTA_AGREGAR || accion == MODAL_CRUD_RUTA_EDITAR) {
        if (codO == codD) {
            modal.setMensajeError("Origen y destino deben ser ciudades distintas");
            return;
        }
        int costo = 0;
        int tiempo = 0;
        if (!parsearEntero(modal.getValor(2), costo) || costo < 0 ||
            !parsearEntero(modal.getValor(3), tiempo) || tiempo < 0) {
            modal.setMensajeError("Costo y duracion deben ser enteros positivos");
            return;
        }
        bool ok = (accion == MODAL_CRUD_RUTA_AGREGAR)
            ? sistema.agregarRutaAdmin(Ruta(codO, codD, costo, tiempo, true))
            : sistema.modificarRutaAdmin(codO, codD, costo, tiempo);
        if (!ok) {
            modal.setMensajeError(accion == MODAL_CRUD_RUTA_AGREGAR
                ? "Ciudades no existen o la ruta ya existe"
                : "No existe ruta directa Origen -> Destino");
            return;
        }
        modal.cerrar();
        refrescarTrasCambioCiudadesORutas();
        int idxRuta = panelMantenimiento.getIndiceSeleccionado();
        const ListaEnlazada<Ruta>& rutas = sistema.obtenerRutas();
        for (int i = 0; i < rutas.tamano(); ++i) {
            if (rutas.obtener(i).obtenerOrigenCodigo() == codO &&
                rutas.obtener(i).obtenerDestinoCodigo() == codD) {
                idxRuta = i;
                break;
            }
        }
        finalizarCrudMantenimiento(idxRuta,
                                   accion == MODAL_CRUD_RUTA_AGREGAR
                                       ? "Ruta agregada y guardada en data/rutas.txt"
                                       : "Ruta editada y guardada en data/rutas.txt");
        return;
    }

    if (accion == MODAL_CRUD_RUTA_ELIMINAR) {
        if (!sistema.eliminarRutaAdmin(codO, codD)) {
            modal.setMensajeError("No existe la ruta dirigida " + codO + " -> " + codD);
            return;
        }
        modal.cerrar();
        refrescarTrasCambioCiudadesORutas();
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Ruta eliminada y guardada en data/rutas.txt");
    }
}

void AplicacionVisual::procesarModalAeronave(int accion) {
    string placa = limpiarCampoTexto(modal.getValor(0));
    if (placa.empty()) {
        modal.setMensajeError("La placa es obligatoria");
        return;
    }

    if (accion == MODAL_CRUD_AERONAVE_AGREGAR || accion == MODAL_CRUD_AERONAVE_EDITAR) {
        string marca = limpiarCampoTexto(modal.getValor(1));
        string modelo = limpiarCampoTexto(modal.getValor(2));
        string estado = limpiarCampoTexto(modal.getValor(4));
        int capacidad = 0;
        if (marca.empty() || modelo.empty() || estado.empty()) {
            modal.setMensajeError("Todos los campos son obligatorios");
            return;
        }
        if (!parsearEntero(modal.getValor(3), capacidad) || capacidad <= 0) {
            modal.setMensajeError("La capacidad debe ser mayor que 0");
            return;
        }
        bool ok = (accion == MODAL_CRUD_AERONAVE_AGREGAR)
            ? sistema.agregarAeronaveAdmin(Aeronave(placa, marca, modelo, capacidad, estado))
            : sistema.modificarAeronaveAdmin(placa, marca, modelo, capacidad, estado);
        if (!ok) {
            modal.setMensajeError(accion == MODAL_CRUD_AERONAVE_AGREGAR
                ? "Ya existe una aeronave con esa placa"
                : "No se encontro una aeronave con esa placa");
            return;
        }
        modal.cerrar();
        int idxAero = 0;
        const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
        for (int i = 0; i < aeronaves.tamano(); ++i) {
            if (aeronaves.obtener(i).getPlaca() == placa) {
                indiceAeronaveActiva = i;
                idxAero = i;
                break;
            }
        }
        finalizarCrudMantenimiento(idxAero,
                                   accion == MODAL_CRUD_AERONAVE_AGREGAR
                                       ? "Aeronave agregada y guardada en data/aeronaves.txt"
                                       : "Aeronave editada y guardada en data/aeronaves.txt");
        return;
    }

    if (accion == MODAL_CRUD_AERONAVE_ELIMINAR) {
        if (!sistema.eliminarAeronaveAdmin(placa)) {
            modal.setMensajeError("No existe una aeronave con esa placa");
            return;
        }
        modal.cerrar();
        indiceAeronaveActiva = 0;
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Aeronave eliminada y guardada en data/aeronaves.txt");
    }
}

void AplicacionVisual::procesarModalTripulante(int accion) {
    string id = limpiarCampoTexto(modal.getValor(0));
    if (id.empty()) {
        modal.setMensajeError("El ID del tripulante es obligatorio");
        return;
    }

    if (accion == MODAL_CRUD_TRIPULANTE_AGREGAR || accion == MODAL_CRUD_TRIPULANTE_EDITAR) {
        string nombre = limpiarCampoTexto(modal.getValor(1));
        string rol = limpiarCampoTexto(modal.getValor(2));
        string placa = limpiarCampoTexto(modal.getValor(3));
        if (nombre.empty() || rol.empty() || placa.empty()) {
            modal.setMensajeError("Todos los campos son obligatorios");
            return;
        }
        bool ok = (accion == MODAL_CRUD_TRIPULANTE_AGREGAR)
            ? sistema.agregarTripulanteAdmin(Tripulante(id, nombre, rol, placa))
            : sistema.modificarTripulanteAdmin(id, nombre, rol, placa);
        if (!ok) {
            modal.setMensajeError(accion == MODAL_CRUD_TRIPULANTE_AGREGAR
                ? "Ya existe un tripulante con ese ID"
                : "No existe un tripulante con ese ID");
            return;
        }
        modal.cerrar();
        int idxCrew = 0;
        const ListaEnlazada<Tripulante>& tripulantes = sistema.obtenerTripulantes();
        for (int i = 0; i < tripulantes.tamano(); ++i) {
            if (tripulantes.obtener(i).getId() == id) {
                idxCrew = i;
                break;
            }
        }
        finalizarCrudMantenimiento(idxCrew,
                                   accion == MODAL_CRUD_TRIPULANTE_AGREGAR
                                       ? "Tripulante agregado y guardado en data/tripulacion.txt"
                                       : "Tripulante editado y guardado en data/tripulacion.txt");
        return;
    }

    if (accion == MODAL_CRUD_TRIPULANTE_ELIMINAR) {
        if (!sistema.eliminarTripulanteAdmin(id)) {
            modal.setMensajeError("No existe un tripulante con ese ID");
            return;
        }
        modal.cerrar();
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Tripulante eliminado y guardado en data/tripulacion.txt");
    }
}

void AplicacionVisual::procesarModalVuelo(int accion) {
    string codigo = limpiarCampoTexto(modal.getValor(0));
    if (codigo.empty()) {
        modal.setMensajeError("El codigo de vuelo es obligatorio");
        return;
    }

    if (accion == MODAL_CRUD_VUELO_AGREGAR || accion == MODAL_CRUD_VUELO_EDITAR) {
        string codO = limpiarCampoTexto(modal.getValor(1));
        string codD = limpiarCampoTexto(modal.getValor(2));
        string placa = limpiarCampoTexto(modal.getValor(3));
        int costo = 0;
        int tiempo = 0;
        if (codO.empty() || codD.empty() || placa.empty()) {
            modal.setMensajeError("Todos los campos son obligatorios");
            return;
        }
        if (codO == codD) {
            modal.setMensajeError("Origen y destino deben ser ciudades distintas");
            return;
        }
        if (!parsearEntero(modal.getValor(4), costo) || costo < 0 ||
            !parsearEntero(modal.getValor(5), tiempo) || tiempo < 0) {
            modal.setMensajeError("Costo y duracion deben ser enteros positivos");
            return;
        }
        bool ok = (accion == MODAL_CRUD_VUELO_AGREGAR)
            ? sistema.agregarVueloAdmin(Vuelo(codigo, codO, codD, placa, costo, tiempo))
            : sistema.modificarVueloAdmin(codigo, codO, codD, placa, costo, tiempo);
        if (!ok) {
            modal.setMensajeError(accion == MODAL_CRUD_VUELO_AGREGAR
                ? "Codigo duplicado o ciudades no existen en el grafo"
                : "Vuelo no encontrado o ciudades no existen");
            return;
        }
        modal.cerrar();
        int idxVuelo = 0;
        const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
        for (int i = 0; i < vuelos.tamano(); ++i) {
            if (vuelos.obtener(i).obtenerCodigoVuelo() == codigo) {
                indiceVueloActivo = i;
                idxVuelo = i;
                break;
            }
        }
        finalizarCrudMantenimiento(idxVuelo,
                                   accion == MODAL_CRUD_VUELO_AGREGAR
                                       ? "Vuelo agregado y guardado en data/vuelos.txt"
                                       : "Vuelo editado y guardado en data/vuelos.txt");
        return;
    }

    if (accion == MODAL_CRUD_VUELO_ELIMINAR) {
        if (!sistema.eliminarVueloAdmin(codigo)) {
            modal.setMensajeError("No existe un vuelo con ese codigo");
            return;
        }
        modal.cerrar();
        indiceVueloActivo = 0;
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Vuelo eliminado y guardado en data/vuelos.txt");
    }
}

void AplicacionVisual::procesarModalPasajero(int accion) {
    string id = limpiarCampoTexto(modal.getValor(0));
    if (id.empty()) {
        modal.setMensajeError("El ID del pasajero es obligatorio");
        return;
    }

    if (accion == MODAL_CRUD_PASAJERO_AGREGAR) {
        string nombre = limpiarCampoTexto(modal.getValor(1));
        string vueloInicial = limpiarCampoTexto(modal.getValor(3));
        int maletas = 0;
        if (nombre.empty()) {
            modal.setMensajeError("El nombre es obligatorio");
            return;
        }
        if (!parsearEntero(modal.getValor(2), maletas) || maletas < 0) {
            modal.setMensajeError("Cantidad de maletas invalida");
            return;
        }
        if (sistema.buscarPasajeroAVL(id) != nullptr) {
            modal.setMensajeError("Ya existe un pasajero con ese ID en el AVL");
            return;
        }
        if (!vueloInicial.empty() && !sistema.existeVuelo(vueloInicial)) {
            modal.setMensajeError("El vuelo inicial indicado no existe");
            return;
        }
        Pasajero nuevo(id, nombre, maletas);
        if (!vueloInicial.empty()) {
            nuevo.agregarVuelo(vueloInicial);
        }
        if (!sistema.registrarPasajeroAdmin(nuevo)) {
            modal.setMensajeError("No se pudo registrar el pasajero");
            return;
        }
        modal.cerrar();
        seleccionarPasajeroActivoPorId(id);
        finalizarCrudMantenimiento(indicePasajeroActivo,
                                   "Pasajero insertado en Arbol AVL y data/pasajeros.txt");
        return;
    }

    if (accion == MODAL_CRUD_PASAJERO_EDITAR) {
        string nombre = limpiarCampoTexto(modal.getValor(1));
        string vueloAccion = limpiarCampoTexto(modal.getValor(3));
        int maletas = 0;
        if (nombre.empty()) {
            modal.setMensajeError("El nuevo nombre es obligatorio");
            return;
        }
        if (!parsearEntero(modal.getValor(2), maletas) || maletas < 0) {
            modal.setMensajeError("Cantidad de maletas invalida");
            return;
        }
        if (!vueloAccion.empty() && !sistema.existeVuelo(vueloAccion)) {
            modal.setMensajeError("El codigo de vuelo indicado no existe");
            return;
        }
        if (!sistema.modificarPasajeroAdmin(id, nombre, maletas)) {
            modal.setMensajeError("No existe un pasajero con ese ID en el AVL");
            return;
        }
        if (!vueloAccion.empty()) {
            const NodoAVL* actualizado = sistema.buscarPasajeroAVL(id);
            if (actualizado != nullptr && actualizado->dato.tieneVuelo(vueloAccion)) {
                sistema.quitarVueloDePasajero(id, vueloAccion);
            } else {
                sistema.asignarVueloAPasajero(id, vueloAccion);
            }
        }
        modal.cerrar();
        seleccionarPasajeroActivoPorId(id);
        finalizarCrudMantenimiento(indicePasajeroActivo,
                                   "Pasajero actualizado en Arbol AVL y data/pasajeros.txt");
        return;
    }

    if (accion == MODAL_CRUD_PASAJERO_ELIMINAR) {
        if (!sistema.eliminarPasajeroAdmin(id)) {
            modal.setMensajeError("No existe un pasajero con ese ID en el AVL");
            return;
        }
        modal.cerrar();
        indicePasajeroActivo = 0;
        finalizarCrudMantenimiento(panelMantenimiento.getIndiceSeleccionado(),
                                   "Pasajero eliminado del Arbol AVL y data/pasajeros.txt");
    }
}

void AplicacionVisual::ejecutar() {
    while (ventana.isOpen()) {
        float dt = relojDelta.restart().asSeconds();
        procesarEventos();
        grafoVisual.actualizarAnimacion(dt);
        renderizar();
    }
}
