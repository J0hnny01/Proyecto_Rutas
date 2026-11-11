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
