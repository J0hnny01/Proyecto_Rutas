#include "AplicacionVisual.h"
#include "CompatSFML.h"

AplicacionVisual::AplicacionVisual()
    : ventana(CompatSFML::crearVideoMode(1200, 720), "Aerolinea EIF207"),
      moduloActual(ModuloActivo::Cliente),
      vistaActual(VistaLienzo::MapaGrafo),
      sistema("data"),
      criterioBellmanCosto(true),
      ultimaAccionCliente(-1),
      indicePasajeroActivo(0) {
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

void AplicacionVisual::ejecutarAccionAdmin(int indiceAccion) {
    ultimaAccionCliente = -1;
    switch (indiceAccion) {
        case 0:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Aeronaves y tripulacion");
            lineasResultado[0] = "Aeronaves cargadas: " + to_string(sistema.obtenerAeronaves().tamano()) +
                                 " | Tripulantes cargados: " + to_string(sistema.obtenerTripulantes().tamano());
            break;
        case 1:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Vuelos y pasajeros");
            lineasResultado[0] = "Vuelos cargados: " + to_string(sistema.obtenerVuelos().tamano()) +
                                 " | Pasajeros en AVL: " + to_string(sistema.obtenerArbolPasajeros().getCantidad());
            break;
        case 2:
            mostrarRutaEquipajeAdmin();
            break;
        case 3:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Prim (MST por costo)");
            break;
        case 4:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Kruskal (MST por duracion)");
            break;
        case 5:
            vistaActual = VistaLienzo::ArbolEstructura;
            arbolVisual.limpiar("Despliegue de arbol");
            limpiarLineasResultado("Recorridos del arbol");
            break;
    }
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
