#include "SistemaAerolinea.h"

#include <fstream>

SistemaAerolinea::SistemaAerolinea(const string& carpetaDatos)
    : directorioDatos(carpetaDatos) {
}

string SistemaAerolinea::resolverRutaArchivo(const string& nombreArchivo) const {
    return directorioDatos + "/" + nombreArchivo;
}

void SistemaAerolinea::recolectarPasajerosRec(const NodoAVL* nodo, ListaEnlazada<Pasajero>& destino) const {
    if (nodo == nullptr) {
        return;
    }
    recolectarPasajerosRec(nodo->izquierdo, destino);
    destino.insertarAlFinal(nodo->dato);
    recolectarPasajerosRec(nodo->derecho, destino);
}

void SistemaAerolinea::reconstruirArbolCiudades() {
    arbolCiudades.limpiar();
    int indice = 0;
    const ListaEnlazada<Ciudad>::NodoLista* nodo = ciudades.obtenerCabeza();
    while (nodo != nullptr) {
        arbolCiudades.insertar(nodo->dato.obtenerCodigo(), nodo->dato.obtenerNombre(), indice);
        indice++;
        nodo = nodo->siguiente;
    }
}

void SistemaAerolinea::poblarArbolMST(int indiceOrigen, const ResultadoMST& resultado, ArbolMST& arbolSalida) const {
    int n = grafo.obtenerCantidadVertices();
    if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n) {
        arbolSalida.limpiar();
        return;
    }
    string* codigos = new string[n];
    string* nombres = new string[n];
    for (int i = 0; i < n; i++) {
        const Ciudad& ciudad = grafo.obtenerCiudad(i);
        codigos[i] = ciudad.obtenerCodigo();
        nombres[i] = ciudad.obtenerNombre();
    }
    arbolSalida.construirDesdeResultado(indiceOrigen, resultado, codigos, nombres, n);
    delete[] codigos;
    delete[] nombres;
}

bool SistemaAerolinea::cargarDatos() {
    ifstream pruebaDirecta(resolverRutaArchivo("ciudades.txt").c_str());
    if (!pruebaDirecta.is_open()) {
        string alternativa = "../" + directorioDatos;
        ifstream pruebaAlternativa((alternativa + "/ciudades.txt").c_str());
        if (pruebaAlternativa.is_open()) {
            directorioDatos = alternativa;
        }
    }

    bool okCiudades = GestorArchivos::cargarCiudades(resolverRutaArchivo("ciudades.txt"), ciudades, arbolCiudades);
    bool okRutas = GestorArchivos::cargarRutas(resolverRutaArchivo("rutas.txt"), rutas);
    bool okAeronaves = GestorArchivos::cargarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
    bool okTripulantes = GestorArchivos::cargarTripulantes(resolverRutaArchivo("tripulacion.txt"), tripulantes);
    bool okVuelos = GestorArchivos::cargarVuelos(resolverRutaArchivo("vuelos.txt"), vuelos);
    bool okPasajeros = GestorArchivos::cargarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);

    reconstruirGrafo();

    return okCiudades && okRutas && okAeronaves && okTripulantes && okVuelos && okPasajeros;
}

bool SistemaAerolinea::guardarDatos() const {
    bool okCiudades = GestorArchivos::guardarCiudades(resolverRutaArchivo("ciudades.txt"), ciudades);
    bool okRutas = GestorArchivos::guardarRutas(resolverRutaArchivo("rutas.txt"), rutas);
    bool okAeronaves = GestorArchivos::guardarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
    bool okTripulantes = GestorArchivos::guardarTripulantes(resolverRutaArchivo("tripulacion.txt"), tripulantes);
    bool okVuelos = GestorArchivos::guardarVuelos(resolverRutaArchivo("vuelos.txt"), vuelos);
    bool okPasajeros = GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    return okCiudades && okRutas && okAeronaves && okTripulantes && okVuelos && okPasajeros;
}

void SistemaAerolinea::reconstruirGrafo() {
    grafo.limpiar();

    const ListaEnlazada<Ciudad>::NodoLista* nodoCiudad = ciudades.obtenerCabeza();
    while (nodoCiudad != nullptr) {
        grafo.agregarCiudad(nodoCiudad->dato);
        nodoCiudad = nodoCiudad->siguiente;
    }

    const ListaEnlazada<Ruta>::NodoLista* nodoRuta = rutas.obtenerCabeza();
    while (nodoRuta != nullptr) {
        const Ruta& ruta = nodoRuta->dato;
        if (ruta.estaActiva()) {
            grafo.agregarRutaDirigida(ruta.obtenerOrigenCodigo(),
                                      ruta.obtenerDestinoCodigo(),
                                      ruta.obtenerCosto(),
                                      ruta.obtenerTiempoMin());
        }
        nodoRuta = nodoRuta->siguiente;
    }
}

ResultadoRuta SistemaAerolinea::calcularRutaMasRapida(const string& codigoOrigen, const string& codigoDestino) const {
    return Dijkstra::calcularRutaMasRapida(grafo, codigoOrigen, codigoDestino);
}

ResultadoRuta SistemaAerolinea::calcularRutaMasBarata(const string& codigoOrigen, const string& codigoDestino) const {
    return Dijkstra::calcularRutaMasBarata(grafo, codigoOrigen, codigoDestino);
}

Equipaje SistemaAerolinea::calcularRutaEquipaje(const string& idPasajero, int cantidadMaletas,
                                                const string& codigoOrigen, const string& codigoDestino) const {
    return Dijkstra::calcularRutaEquipaje(grafo, idPasajero, cantidadMaletas, codigoOrigen, codigoDestino);
}

ResultadoBellmanFord SistemaAerolinea::explorarDestinos(const string& codigoOrigen, CriterioRuta criterio) const {
    return BellmanFord::calcularDesdeOrigen(grafo, codigoOrigen, criterio);
}

ResultadoMST SistemaAerolinea::calcularPrimCosto(const string& codigoOrigen, ArbolMST& arbolSalida) const {
    int idxOrigen = grafo.buscarIndicePorCodigo(codigoOrigen);
    if (idxOrigen == -1) {
        idxOrigen = 0;
    }
    if (grafo.obtenerCantidadVertices() <= 0) {
        arbolSalida.limpiar();
        return ResultadoMST();
    }
    ResultadoMST resultado = Prim::calcular(grafo, idxOrigen, CRITERIO_COSTO);
    poblarArbolMST(idxOrigen, resultado, arbolSalida);
    return resultado;
}

ResultadoMST SistemaAerolinea::calcularKruskalDuracion(const string& codigoOrigen, ArbolMST& arbolSalida) const {
    int idxOrigen = grafo.buscarIndicePorCodigo(codigoOrigen);
    if (idxOrigen == -1) {
        idxOrigen = 0;
    }
    if (grafo.obtenerCantidadVertices() <= 0) {
        arbolSalida.limpiar();
        return ResultadoMST();
    }
    ResultadoMST resultado = Kruskal::calcular(grafo, CRITERIO_TIEMPO);
    poblarArbolMST(idxOrigen, resultado, arbolSalida);
    return resultado;
}

bool SistemaAerolinea::sonRutasDistintas(const ResultadoRuta& rutaA, const ResultadoRuta& rutaB) const {
    if (!rutaA.encontrada || !rutaB.encontrada) {
        return false;
    }
    if (rutaA.caminoIndices.tamano() != rutaB.caminoIndices.tamano()) {
        return true;
    }
    const ListaEnlazada<int>::NodoLista* nodoA = rutaA.caminoIndices.obtenerCabeza();
    const ListaEnlazada<int>::NodoLista* nodoB = rutaB.caminoIndices.obtenerCabeza();
    while (nodoA != nullptr && nodoB != nullptr) {
        if (nodoA->dato != nodoB->dato) {
            return true;
        }
        nodoA = nodoA->siguiente;
        nodoB = nodoB->siguiente;
    }
    return false;
}

string SistemaAerolinea::formatearCaminoIndices(const ListaEnlazada<int>& indices) const {
    string texto = "";
    const ListaEnlazada<int>::NodoLista* actual = indices.obtenerCabeza();
    while (actual != nullptr) {
        int idx = actual->dato;
        if (idx >= 0 && idx < grafo.obtenerCantidadVertices()) {
            if (!texto.empty()) {
                texto += " -> ";
            }
            texto += grafo.obtenerCiudad(idx).obtenerCodigo();
        }
        actual = actual->siguiente;
    }
    return texto;
}

ListaEnlazada<Vuelo> SistemaAerolinea::buscarVuelosDirectos(const string& codigoOrigen, const string& codigoDestino) const {
    ListaEnlazada<Vuelo> encontrados;
    const ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerOrigenCodigo() == codigoOrigen &&
            actual->dato.obtenerDestinoCodigo() == codigoDestino) {
            encontrados.insertarAlFinal(actual->dato);
        }
        actual = actual->siguiente;
    }
    return encontrados;
}

bool SistemaAerolinea::existeVuelo(const string& codigoVuelo) const {
    const ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigoVuelo() == codigoVuelo) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

string SistemaAerolinea::obtenerVuelosParaCamino(const ListaEnlazada<int>& indices) const {
    if (indices.tamano() < 2) {
        return "Sin tramos";
    }
    string resumen = "";
    const ListaEnlazada<int>::NodoLista* actual = indices.obtenerCabeza();
    while (actual != nullptr && actual->siguiente != nullptr) {
        int u = actual->dato;
        int v = actual->siguiente->dato;
        if (u >= 0 && u < grafo.obtenerCantidadVertices() &&
            v >= 0 && v < grafo.obtenerCantidadVertices()) {
            string codU = grafo.obtenerCiudad(u).obtenerCodigo();
            string codV = grafo.obtenerCiudad(v).obtenerCodigo();
            ListaEnlazada<Vuelo> directos = buscarVuelosDirectos(codU, codV);
            if (!resumen.empty()) {
                resumen += ", ";
            }
            if (!directos.estaVacia()) {
                resumen += directos.obtener(0).obtenerCodigoVuelo() + " (" + codU + "->" + codV + ")";
            } else {
                resumen += "Conexion " + codU + "->" + codV;
            }
        }
        actual = actual->siguiente;
    }
    return resumen;
}

const NodoABB* SistemaAerolinea::buscarCiudadABB(const string& nombre) const {
    return arbolCiudades.buscar(nombre);
}

const NodoAVL* SistemaAerolinea::buscarPasajeroAVL(const string& id) const {
    return arbolPasajeros.buscar(id);
}

bool SistemaAerolinea::asignarVueloAPasajero(const string& idPasajero, const string& codigoVuelo) {
    bool asignado = arbolPasajeros.asignarVuelo(idPasajero, codigoVuelo);
    if (asignado) {
        GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    }
    return asignado;
}

bool SistemaAerolinea::quitarVueloDePasajero(const string& idPasajero, const string& codigoVuelo) {
    bool quitado = arbolPasajeros.quitarVuelo(idPasajero, codigoVuelo);
    if (quitado) {
        GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    }
    return quitado;
}

ListaEnlazada<Pasajero> SistemaAerolinea::obtenerListaPasajeros() const {
    ListaEnlazada<Pasajero> lista;
    recolectarPasajerosRec(arbolPasajeros.getRaiz(), lista);
    return lista;
}

int SistemaAerolinea::contarRutasDesdeCiudad(const string& codigoCiudad) const {
    int idx = grafo.buscarIndicePorCodigo(codigoCiudad);
    if (idx == -1) {
        return 0;
    }
    int total = 0;
    const NodoArista* arista = grafo.obtenerAdyacentes(idx);
    while (arista != nullptr) {
        total++;
        arista = arista->siguiente;
    }
    return total;
}

string SistemaAerolinea::obtenerResumenTripulacionPorAeronave(const string& placa) const {
    string resumen = "";
    const ListaEnlazada<Tripulante>::NodoLista* actual = tripulantes.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getPlacaAeronave() == placa) {
            if (!resumen.empty()) {
                resumen += ", ";
            }
            resumen += actual->dato.getNombre() + " (" + actual->dato.getRol() + ")";
        }
        actual = actual->siguiente;
    }
    if (resumen.empty()) {
        return "Sin tripulacion asignada";
    }
    return resumen;
}

string SistemaAerolinea::obtenerResumenPasajerosPorVuelo(const string& codigoVuelo) const {
    ListaEnlazada<Pasajero> lista = obtenerListaPasajeros();
    string resumen = "";
    const ListaEnlazada<Pasajero>::NodoLista* actual = lista.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.tieneVuelo(codigoVuelo)) {
            if (!resumen.empty()) {
                resumen += ", ";
            }
            resumen += actual->dato.getNombre() + " [" + actual->dato.getId() + "]";
        }
        actual = actual->siguiente;
    }
    if (resumen.empty()) {
        return "Sin pasajeros asignados";
    }
    return resumen;
}

bool SistemaAerolinea::agregarCiudadAdmin(const Ciudad& ciudad) {
    if (ciudad.obtenerCodigo().empty() || ciudad.obtenerNombre().empty()) {
        return false;
    }
    const ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigo() == ciudad.obtenerCodigo() ||
            actual->dato.obtenerNombre() == ciudad.obtenerNombre()) {
            return false;
        }
        actual = actual->siguiente;
    }
    int nuevoIdx = ciudades.tamano();
    ciudades.insertarAlFinal(ciudad);
    arbolCiudades.insertar(ciudad.obtenerCodigo(), ciudad.obtenerNombre(), nuevoIdx);
    reconstruirGrafo();
    GestorArchivos::guardarCiudades(resolverRutaArchivo("ciudades.txt"), ciudades);
    return true;
}

bool SistemaAerolinea::modificarCiudadAdmin(const string& codigo, const string& nuevoNombre, float nuevaX, float nuevaY) {
    if (codigo.empty() || nuevoNombre.empty()) {
        return false;
    }
    const ListaEnlazada<Ciudad>::NodoLista* check = ciudades.obtenerCabeza();
    while (check != nullptr) {
        if (check->dato.obtenerCodigo() != codigo && check->dato.obtenerNombre() == nuevoNombre) {
            return false;
        }
        check = check->siguiente;
    }
    ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigo() == codigo) {
            actual->dato.establecerNombre(nuevoNombre);
            actual->dato.establecerPosicion(nuevaX, nuevaY);
            reconstruirArbolCiudades();
            reconstruirGrafo();
            GestorArchivos::guardarCiudades(resolverRutaArchivo("ciudades.txt"), ciudades);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::eliminarCiudadAdmin(const string& codigo) {
    int idxCiudad = -1;
    int pos = 0;
    const ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigo() == codigo) {
            idxCiudad = pos;
            break;
        }
        pos++;
        actual = actual->siguiente;
    }
    if (idxCiudad == -1) {
        return false;
    }

    ciudades.eliminarEn(idxCiudad);

    int i = 0;
    while (i < rutas.tamano()) {
        const Ruta& r = rutas.obtener(i);
        if (r.obtenerOrigenCodigo() == codigo || r.obtenerDestinoCodigo() == codigo) {
            rutas.eliminarEn(i);
        } else {
            i++;
        }
    }

    int j = 0;
    while (j < vuelos.tamano()) {
        const Vuelo& v = vuelos.obtener(j);
        if (v.obtenerOrigenCodigo() == codigo || v.obtenerDestinoCodigo() == codigo) {
            vuelos.eliminarEn(j);
        } else {
            j++;
        }
    }

    reconstruirArbolCiudades();
    reconstruirGrafo();
    guardarDatos();
    return true;
}

bool SistemaAerolinea::agregarRutaAdmin(const Ruta& ruta) {
    if (ruta.obtenerOrigenCodigo() == ruta.obtenerDestinoCodigo() ||
        ruta.obtenerCosto() < 0 || ruta.obtenerTiempoMin() < 0) {
        return false;
    }
    if (!grafo.existeCiudad(ruta.obtenerOrigenCodigo()) ||
        !grafo.existeCiudad(ruta.obtenerDestinoCodigo())) {
        return false;
    }
    const ListaEnlazada<Ruta>::NodoLista* actual = rutas.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerOrigenCodigo() == ruta.obtenerOrigenCodigo() &&
            actual->dato.obtenerDestinoCodigo() == ruta.obtenerDestinoCodigo()) {
            return false;
        }
        actual = actual->siguiente;
    }
    rutas.insertarAlFinal(ruta);
    reconstruirGrafo();
    GestorArchivos::guardarRutas(resolverRutaArchivo("rutas.txt"), rutas);
    return true;
}

bool SistemaAerolinea::modificarRutaAdmin(const string& origen, const string& destino, int nuevoCosto, int nuevoTiempoMin) {
    if (nuevoCosto < 0 || nuevoTiempoMin < 0) {
        return false;
    }
    ListaEnlazada<Ruta>::NodoLista* actual = rutas.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerOrigenCodigo() == origen &&
            actual->dato.obtenerDestinoCodigo() == destino) {
            actual->dato.establecerCosto(nuevoCosto);
            actual->dato.establecerTiempoMin(nuevoTiempoMin);
            actual->dato.establecerActiva(true);
            reconstruirGrafo();
            GestorArchivos::guardarRutas(resolverRutaArchivo("rutas.txt"), rutas);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::eliminarRutaAdmin(const string& origen, const string& destino) {
    for (int i = 0; i < rutas.tamano(); i++) {
        const Ruta& r = rutas.obtener(i);
        if (r.obtenerOrigenCodigo() == origen && r.obtenerDestinoCodigo() == destino) {
            rutas.eliminarEn(i);
            reconstruirGrafo();
            GestorArchivos::guardarRutas(resolverRutaArchivo("rutas.txt"), rutas);
            return true;
        }
    }
    return false;
}

bool SistemaAerolinea::agregarAeronaveAdmin(const Aeronave& aeronave) {
    if (aeronave.getPlaca().empty() || aeronave.getCapacidad() <= 0) {
        return false;
    }
    const ListaEnlazada<Aeronave>::NodoLista* actual = aeronaves.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getPlaca() == aeronave.getPlaca()) {
            return false;
        }
        actual = actual->siguiente;
    }
    aeronaves.insertarAlFinal(aeronave);
    GestorArchivos::guardarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
    return true;
}

bool SistemaAerolinea::modificarAeronaveAdmin(const string& placa, const string& nuevaMarca, const string& nuevoModelo, int nuevaCapacidad, const string& nuevoEstado) {
    if (placa.empty() || nuevaMarca.empty() || nuevoModelo.empty() || nuevaCapacidad <= 0 || nuevoEstado.empty()) {
        return false;
    }
    ListaEnlazada<Aeronave>::NodoLista* actual = aeronaves.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getPlaca() == placa) {
            actual->dato.setMarca(nuevaMarca);
            actual->dato.setModelo(nuevoModelo);
            actual->dato.setCapacidad(nuevaCapacidad);
            actual->dato.setEstado(nuevoEstado);
            GestorArchivos::guardarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::cambiarEstadoAeronaveAdmin(const string& placa, const string& nuevoEstado) {
    ListaEnlazada<Aeronave>::NodoLista* actual = aeronaves.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getPlaca() == placa) {
            actual->dato.setEstado(nuevoEstado);
            GestorArchivos::guardarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::eliminarAeronaveAdmin(const string& placa) {
    for (int i = 0; i < aeronaves.tamano(); i++) {
        if (aeronaves.obtener(i).getPlaca() == placa) {
            aeronaves.eliminarEn(i);
            GestorArchivos::guardarAeronaves(resolverRutaArchivo("aeronaves.txt"), aeronaves);
            return true;
        }
    }
    return false;
}

bool SistemaAerolinea::agregarTripulanteAdmin(const Tripulante& tripulante) {
    if (tripulante.getId().empty() || tripulante.getNombre().empty()) {
        return false;
    }
    const ListaEnlazada<Tripulante>::NodoLista* actual = tripulantes.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getId() == tripulante.getId()) {
            return false;
        }
        actual = actual->siguiente;
    }
    tripulantes.insertarAlFinal(tripulante);
    GestorArchivos::guardarTripulantes(resolverRutaArchivo("tripulacion.txt"), tripulantes);
    return true;
}

bool SistemaAerolinea::modificarTripulanteAdmin(const string& id, const string& nuevoNombre, const string& nuevoRol, const string& nuevaPlaca) {
    if (id.empty() || nuevoNombre.empty() || nuevoRol.empty() || nuevaPlaca.empty()) {
        return false;
    }
    ListaEnlazada<Tripulante>::NodoLista* actual = tripulantes.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.getId() == id) {
            actual->dato.setNombre(nuevoNombre);
            actual->dato.setRol(nuevoRol);
            actual->dato.setPlacaAeronave(nuevaPlaca);
            GestorArchivos::guardarTripulantes(resolverRutaArchivo("tripulacion.txt"), tripulantes);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::eliminarTripulanteAdmin(const string& id) {
    for (int i = 0; i < tripulantes.tamano(); i++) {
        if (tripulantes.obtener(i).getId() == id) {
            tripulantes.eliminarEn(i);
            GestorArchivos::guardarTripulantes(resolverRutaArchivo("tripulacion.txt"), tripulantes);
            return true;
        }
    }
    return false;
}

bool SistemaAerolinea::agregarVueloAdmin(const Vuelo& vuelo) {
    if (vuelo.obtenerCodigoVuelo().empty() || vuelo.obtenerCosto() < 0 || vuelo.obtenerTiempoMin() < 0) {
        return false;
    }
    if (!grafo.existeCiudad(vuelo.obtenerOrigenCodigo()) ||
        !grafo.existeCiudad(vuelo.obtenerDestinoCodigo())) {
        return false;
    }
    const ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigoVuelo() == vuelo.obtenerCodigoVuelo()) {
            return false;
        }
        actual = actual->siguiente;
    }
    vuelos.insertarAlFinal(vuelo);
    GestorArchivos::guardarVuelos(resolverRutaArchivo("vuelos.txt"), vuelos);
    return true;
}

bool SistemaAerolinea::modificarVueloAdmin(const string& codigoVuelo, const string& origen, const string& destino, const string& placa, int nuevoCosto, int nuevoTiempoMin) {
    if (codigoVuelo.empty() || origen == destino || nuevoCosto < 0 || nuevoTiempoMin < 0) {
        return false;
    }
    if (!grafo.existeCiudad(origen) || !grafo.existeCiudad(destino)) {
        return false;
    }
    ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato.obtenerCodigoVuelo() == codigoVuelo) {
            actual->dato.establecerOrigenCodigo(origen);
            actual->dato.establecerDestinoCodigo(destino);
            actual->dato.establecerPlacaAeronave(placa);
            actual->dato.establecerCosto(nuevoCosto);
            actual->dato.establecerTiempoMin(nuevoTiempoMin);
            GestorArchivos::guardarVuelos(resolverRutaArchivo("vuelos.txt"), vuelos);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool SistemaAerolinea::eliminarVueloAdmin(const string& codigoVuelo) {
    for (int i = 0; i < vuelos.tamano(); i++) {
        if (vuelos.obtener(i).obtenerCodigoVuelo() == codigoVuelo) {
            vuelos.eliminarEn(i);
            GestorArchivos::guardarVuelos(resolverRutaArchivo("vuelos.txt"), vuelos);
            return true;
        }
    }
    return false;
}

bool SistemaAerolinea::registrarPasajeroAdmin(const Pasajero& pasajero) {
    if (pasajero.getId().empty()) {
        return false;
    }
    bool insertado = arbolPasajeros.insertar(pasajero);
    if (insertado) {
        GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    }
    return insertado;
}

bool SistemaAerolinea::modificarPasajeroAdmin(const string& idPasajero, const string& nuevoNombre, int nuevasMaletas) {
    if (idPasajero.empty() || nuevoNombre.empty() || nuevasMaletas < 0) {
        return false;
    }
    const NodoAVL* nodo = arbolPasajeros.buscar(idPasajero);
    if (nodo == nullptr) {
        return false;
    }
    Pasajero actualizado(idPasajero, nuevoNombre, nuevasMaletas);
    for (int i = 0; i < nodo->dato.getCantidadVuelos(); i++) {
        actualizado.agregarVuelo(nodo->dato.getVuelo(i));
    }
    arbolPasajeros.eliminar(idPasajero);
    arbolPasajeros.insertar(actualizado);
    GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    return true;
}

bool SistemaAerolinea::eliminarPasajeroAdmin(const string& idPasajero) {
    bool eliminado = arbolPasajeros.eliminar(idPasajero);
    if (eliminado) {
        GestorArchivos::guardarPasajeros(resolverRutaArchivo("pasajeros.txt"), arbolPasajeros);
    }
    return eliminado;
}

const GrafoVuelos& SistemaAerolinea::obtenerGrafo() const {
    return grafo;
}

GrafoVuelos& SistemaAerolinea::obtenerGrafoMutable() {
    return grafo;
}

const ArbolABB& SistemaAerolinea::obtenerArbolCiudades() const {
    return arbolCiudades;
}

ArbolABB& SistemaAerolinea::obtenerArbolCiudadesMutable() {
    return arbolCiudades;
}

const ArbolAVL& SistemaAerolinea::obtenerArbolPasajeros() const {
    return arbolPasajeros;
}

ArbolAVL& SistemaAerolinea::obtenerArbolPasajerosMutable() {
    return arbolPasajeros;
}

const ListaEnlazada<Ciudad>& SistemaAerolinea::obtenerCiudades() const {
    return ciudades;
}

ListaEnlazada<Ciudad>& SistemaAerolinea::obtenerCiudadesMutable() {
    return ciudades;
}

const ListaEnlazada<Ruta>& SistemaAerolinea::obtenerRutas() const {
    return rutas;
}

ListaEnlazada<Ruta>& SistemaAerolinea::obtenerRutasMutable() {
    return rutas;
}

const ListaEnlazada<Aeronave>& SistemaAerolinea::obtenerAeronaves() const {
    return aeronaves;
}

ListaEnlazada<Aeronave>& SistemaAerolinea::obtenerAeronavesMutable() {
    return aeronaves;
}

const ListaEnlazada<Tripulante>& SistemaAerolinea::obtenerTripulantes() const {
    return tripulantes;
}

ListaEnlazada<Tripulante>& SistemaAerolinea::obtenerTripulantesMutable() {
    return tripulantes;
}

const ListaEnlazada<Vuelo>& SistemaAerolinea::obtenerVuelos() const {
    return vuelos;
}

ListaEnlazada<Vuelo>& SistemaAerolinea::obtenerVuelosMutable() {
    return vuelos;
}

const string& SistemaAerolinea::obtenerDirectorioDatos() const {
    return directorioDatos;
}
