#include "AplicacionVisual.h"
#include "CompatSFML.h"

AplicacionVisual::AplicacionVisual()
    : ventana(CompatSFML::crearVideoMode(1200, 720), "Aerolinea EIF207"),
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
    inicializarInterfaz();
    sincronizarMapaVisual();
    mostrarMapaRutas();
}

void AplicacionVisual::inicializarInterfaz() {
    btnTabCliente.configurar(910.0f, 10.0f, 125.0f, 32.0f, "Cliente", true, 13);
    btnTabAdmin.configurar(1045.0f, 10.0f, 135.0f, 32.0f, "Administrativo", true, 13);
    btnTabCliente.setActivo(true);
    btnTabAdmin.setActivo(false);

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

    btnAdminEstadoAeronave.configurar(18.0f, 482.0f, 239.0f, 32.0f, "Rotar estado aeronave", true, 12);
    btnAdminAjustarRuta.configurar(18.0f, 522.0f, 239.0f, 32.0f, "Tarifa Origen->Dest +10", true, 12);
    btnAdminToggleVueloPasajero.configurar(18.0f, 562.0f, 239.0f, 32.0f, "Asignar/quitar vuelo AVL", true, 12);

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

void AplicacionVisual::mostrarVuelosPasajeroAVL() {
    vistaActual = VistaLienzo::ArbolEstructura;
    arbolVisual.limpiar("Arbol AVL de Pasajeros");
    limpiarLineasResultado("Asignacion de pasajero (AVL)");

    if (ultimaAccionCliente == 4) {
        indicePasajeroActivo++;
    }

    bool hayPasajeros = false;
    Pasajero pasajero = obtenerPasajeroActivo(hayPasajeros);
    if (!hayPasajeros) {
        lineasResultado[0] = "No hay pasajeros registrados en el AVL.";
        return;
    }

    const NodoAVL* nodo = sistema.buscarPasajeroAVL(pasajero.getId());
    if (nodo == nullptr) {
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
    lineasResultado[3] = "Clic de nuevo para consultar el siguiente pasajero del AVL";
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
        moduloActual = ModuloActivo::Cliente;
        btnTabCliente.setActivo(true);
        btnTabAdmin.setActivo(false);
        return;
    }
    if (btnTabAdmin.contienePunto(mx, my)) {
        moduloActual = ModuloActivo::Administrativo;
        btnTabCliente.setActivo(false);
        btnTabAdmin.setActivo(true);
        return;
    }

    if (moduloActual == ModuloActivo::Administrativo && manejarClicEdicionRapidaAdmin(mx, my)) {
        return;
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
    } else {
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
    if (moduloActual == ModuloActivo::Administrativo && btnAdminAjustarRuta.contienePunto(mx, my)) {
        ajustarTarifaRutaRapida(-10);
        return;
    }
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
            mostrarBusquedaCiudadABB();
            break;
        case 4:
            mostrarVuelosPasajeroAVL();
            break;
        case 5:
            mostrarMapaRutas();
            break;
    }
    ultimaAccionCliente = indiceAccion;
}

bool AplicacionVisual::manejarClicEdicionRapidaAdmin(float mx, float my) {
    if (btnAdminEstadoAeronave.contienePunto(mx, my)) {
        rotarEstadoAeronaveRapida();
        return true;
    }
    if (btnAdminAjustarRuta.contienePunto(mx, my)) {
        ajustarTarifaRutaRapida(10);
        return true;
    }
    if (btnAdminToggleVueloPasajero.contienePunto(mx, my)) {
        alternarVueloPasajeroRapido();
        return true;
    }
    return false;
}

void AplicacionVisual::mostrarAeronavesYCrewAdmin() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Aeronaves y tripulacion");

    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int totalAeronaves = aeronaves.tamano();
    if (totalAeronaves == 0) {
        lineasResultado[0] = "No hay aeronaves registradas en el sistema.";
        return;
    }

    if (ultimaAccionAdmin == 0) {
        indiceAeronaveActiva = (indiceAeronaveActiva + 1) % totalAeronaves;
    } else {
        indiceAeronaveActiva = ((indiceAeronaveActiva % totalAeronaves) + totalAeronaves) % totalAeronaves;
    }

    const Aeronave& aero = aeronaves.obtener(indiceAeronaveActiva);
    string resumenTrip = sistema.obtenerResumenTripulacionPorAeronave(aero.getPlaca());

    lineasResultado[0] = truncarTexto(
        "Aeronave [" + to_string(indiceAeronaveActiva + 1) + "/" + to_string(totalAeronaves) +
        "]: " + aero.getPlaca() + " | " + aero.getMarca() + " " + aero.getModelo() +
        " | Cap: " + to_string(aero.getCapacidad()) + " | Estado: " + aero.getEstado(), 108);
    lineasResultado[1] = truncarTexto("Tripulacion asignada: " + resumenTrip, 108);
    lineasResultado[2] = "Flota total: " + to_string(totalAeronaves) +
                         " aeronaves | Tripulantes cargados: " + to_string(sistema.obtenerTripulantes().tamano());
    lineasResultado[3] = "Clic de nuevo para ciclar aeronave | Usa 'Rotar estado aeronave' para editar en vivo";
}

void AplicacionVisual::mostrarVuelosYPasajerosAdmin() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Vuelos y pasajeros");

    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int totalVuelos = vuelos.tamano();
    if (totalVuelos == 0) {
        lineasResultado[0] = "No hay vuelos programados en el sistema.";
        return;
    }

    if (ultimaAccionAdmin == 1) {
        indiceVueloActivo = (indiceVueloActivo + 1) % totalVuelos;
    } else {
        indiceVueloActivo = ((indiceVueloActivo % totalVuelos) + totalVuelos) % totalVuelos;
    }

    const Vuelo& vuelo = vuelos.obtener(indiceVueloActivo);
    string resumenPax = sistema.obtenerResumenPasajerosPorVuelo(vuelo.obtenerCodigoVuelo());

    const GrafoVuelos& grafo = sistema.obtenerGrafo();
    int idxO = grafo.buscarIndicePorCodigo(vuelo.obtenerOrigenCodigo());
    int idxD = grafo.buscarIndicePorCodigo(vuelo.obtenerDestinoCodigo());
    grafoVisual.limpiarResaltados();
    if (idxO != -1 && idxD != -1) {
        grafoVisual.resaltarAristaRuta(idxO, idxD, false);
    }

    lineasResultado[0] = truncarTexto(
        "Vuelo [" + to_string(indiceVueloActivo + 1) + "/" + to_string(totalVuelos) +
        "]: " + vuelo.obtenerCodigoVuelo() + " (" + vuelo.obtenerOrigenCodigo() + " -> " +
        vuelo.obtenerDestinoCodigo() + ") | Aeronave: " + vuelo.obtenerPlacaAeronave(), 108);
    lineasResultado[1] = "Tarifa: $" + to_string(vuelo.obtenerCosto()) +
                         " | Duracion: " + to_string(vuelo.obtenerTiempoMin()) +
                         " min | Pasajeros en AVL: " + to_string(sistema.obtenerArbolPasajeros().getCantidad());
    lineasResultado[2] = truncarTexto("Pasajeros asignados: " + resumenPax, 108);
    lineasResultado[3] = "Clic de nuevo para ciclar vuelo | Usa 'Asignar/quitar vuelo AVL' sobre pasajero activo";
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

void AplicacionVisual::rotarEstadoAeronaveRapida() {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Edicion rapida: estado de aeronave");

    const ListaEnlazada<Aeronave>& aeronaves = sistema.obtenerAeronaves();
    int totalAeronaves = aeronaves.tamano();
    if (totalAeronaves == 0) {
        lineasResultado[0] = "No hay aeronaves disponibles para modificar.";
        return;
    }

    indiceAeronaveActiva = ((indiceAeronaveActiva % totalAeronaves) + totalAeronaves) % totalAeronaves;
    Aeronave aero = aeronaves.obtener(indiceAeronaveActiva);

    string estadoActual = aero.getEstado();
    string nuevoEstado = "Activa";
    if (estadoActual == "Activa") {
        nuevoEstado = "En vuelo";
    } else if (estadoActual == "En vuelo") {
        nuevoEstado = "Mantenimiento";
    }

    if (!sistema.cambiarEstadoAeronaveAdmin(aero.getPlaca(), nuevoEstado)) {
        lineasResultado[0] = "No se pudo actualizar el estado de la aeronave " + aero.getPlaca() + ".";
        return;
    }

    for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) {
        botonesAdmin[j].setActivo(j == 0);
    }
    ultimaAccionAdmin = 0;

    lineasResultado[0] = truncarTexto(
        "Aeronave " + aero.getPlaca() + " (" + aero.getMarca() + " " + aero.getModelo() +
        "): " + estadoActual + " -> " + nuevoEstado, 108);
    lineasResultado[1] = truncarTexto(
        "Tripulacion asignada: " + sistema.obtenerResumenTripulacionPorAeronave(aero.getPlaca()), 108);
    lineasResultado[2] = "Cambio guardado en data/aeronaves.txt";
    lineasResultado[3] = "Usa 'Aeronaves y tripulacion' para seleccionar otra aeronave";
}

void AplicacionVisual::ajustarTarifaRutaRapida(int deltaCosto) {
    vistaActual = VistaLienzo::MapaGrafo;
    limpiarLineasResultado("Edicion rapida: tarifa de ruta");

    string codOrigen;
    string codDestino;
    if (!validarOrigenDestino(codOrigen, codDestino, true)) {
        return;
    }

    const ListaEnlazada<Ruta>& rutas = sistema.obtenerRutas();
    bool encontrada = false;
    int costoAnterior = 0;
    int tiempoMin = 0;
    for (int i = 0; i < rutas.tamano(); ++i) {
        const Ruta& r = rutas.obtener(i);
        if (r.obtenerOrigenCodigo() == codOrigen && r.obtenerDestinoCodigo() == codDestino) {
            encontrada = true;
            costoAnterior = r.obtenerCosto();
            tiempoMin = r.obtenerTiempoMin();
            break;
        }
    }

    if (!encontrada) {
        lineasResultado[0] = "No existe ruta directa " + codOrigen + " -> " + codDestino + " para modificar.";
        lineasResultado[1] = "Selecciona Origen y Destino con conexion directa en el mapa.";
        return;
    }

    int nuevoCosto = costoAnterior + deltaCosto;
    if (nuevoCosto < 10) {
        nuevoCosto = 10;
    }

    if (!sistema.modificarRutaAdmin(codOrigen, codDestino, nuevoCosto, tiempoMin)) {
        lineasResultado[0] = "No se pudo actualizar la tarifa de la ruta.";
        return;
    }

    sincronizarMapaVisual();
    ultimoArbolMST.limpiar();
    grafoVisual.limpiarResaltados();
    grafoVisual.resaltarAristaRuta(grafoVisual.getCiudadOrigen(), grafoVisual.getCiudadDestino(), false);

    lineasResultado[0] = "Ruta " + codOrigen + " -> " + codDestino +
                         ": tarifa $" + to_string(costoAnterior) + " -> $" + to_string(nuevoCosto) +
                         " | Tiempo: " + to_string(tiempoMin) + " min";
    lineasResultado[1] = "Grafo de rutas recalculado en memoria y sincronizado con el mapa";
    lineasResultado[2] = "Cambio guardado en data/rutas.txt";
    lineasResultado[3] = "Clic izquierdo en boton: +$10 | Clic derecho en boton: -$10 (minimo $10)";
}

void AplicacionVisual::alternarVueloPasajeroRapido() {
    vistaActual = VistaLienzo::ArbolEstructura;
    limpiarLineasResultado("Edicion rapida: vuelo en pasajero (AVL)");

    bool hayPax = false;
    Pasajero pax = obtenerPasajeroActivo(hayPax);
    if (!hayPax) {
        lineasResultado[0] = "No hay pasajeros registrados en el AVL.";
        return;
    }

    const ListaEnlazada<Vuelo>& vuelos = sistema.obtenerVuelos();
    int totalVuelos = vuelos.tamano();
    if (totalVuelos == 0) {
        lineasResultado[0] = "No hay vuelos programados para asignar.";
        return;
    }

    indiceVueloActivo = ((indiceVueloActivo % totalVuelos) + totalVuelos) % totalVuelos;
    Vuelo vuelo = vuelos.obtener(indiceVueloActivo);
    const NodoAVL* nodo = sistema.buscarPasajeroAVL(pax.getId());
    bool yaTieneVuelo = (nodo != nullptr && nodo->dato.tieneVuelo(vuelo.obtenerCodigoVuelo()));

    bool exito = yaTieneVuelo
        ? sistema.quitarVueloDePasajero(pax.getId(), vuelo.obtenerCodigoVuelo())
        : sistema.asignarVueloAPasajero(pax.getId(), vuelo.obtenerCodigoVuelo());

    if (!exito) {
        lineasResultado[0] = "No se pudo actualizar el vuelo " + vuelo.obtenerCodigoVuelo() +
                             " en el pasajero " + pax.getId() + ".";
        return;
    }

    for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) {
        botonesAdmin[j].setActivo(j == 5);
    }
    modoArbolVista = 2;
    ultimaAccionAdmin = 5;
    mostrarArbolAVLVisual(pax.getId());

    const NodoAVL* actualizado = sistema.buscarPasajeroAVL(pax.getId());
    string vuelosTexto = (actualizado != nullptr) ? actualizado->dato.getVuelosTexto() : "Ninguno";

    lineasResultado[0] = string(yaTieneVuelo ? "Quitado" : "Asignado") + " vuelo " +
                         vuelo.obtenerCodigoVuelo() + " (" + vuelo.obtenerOrigenCodigo() + " -> " +
                         vuelo.obtenerDestinoCodigo() + ") a " + pax.getNombre() + " (" + pax.getId() + ")";
    lineasResultado[1] = truncarTexto("Vuelos actuales en AVL: " + vuelosTexto, 108);
    lineasResultado[2] = "Cambio guardado en data/pasajeros.txt";
    lineasResultado[3] = "Usa 'Vuelos y pasajeros' para cambiar vuelo o 'Vuelos pasajero (AVL)' para cambiar pasajero";
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
    } else {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].renderizar(ventana, fuente);
        }
    }

    if (moduloActual == ModuloActivo::Administrativo) {
        sf::RectangleShape sepAdmin(sf::Vector2f(239.0f, 1.0f));
        sepAdmin.setPosition(sf::Vector2f(18.0f, 450.0f));
        sepAdmin.setFillColor(sf::Color(46, 51, 60));
        ventana.draw(sepAdmin);

        CompatSFML::dibujarTexto(ventana, fuente, "Edicion rapida", 20.0f, 458.0f, 12, sf::Color(145, 153, 168));
        btnAdminEstadoAeronave.renderizar(ventana, fuente);
        btnAdminAjustarRuta.renderizar(ventana, fuente);
        btnAdminToggleVueloPasajero.renderizar(ventana, fuente);
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Clic izq: origen  |  Clic der: destino",
                             20.0f, 686.0f, 11, sf::Color(115, 123, 138));

    if (vistaActual == VistaLienzo::MapaGrafo) {
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

    ventana.display();
}

void AplicacionVisual::ejecutar() {
    while (ventana.isOpen()) {
        float dt = relojDelta.restart().asSeconds();
        procesarEventos();
        grafoVisual.actualizarAnimacion(dt);
        renderizar();
    }
}
