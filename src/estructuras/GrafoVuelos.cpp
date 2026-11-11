#include "GrafoVuelos.h"

#include <stdexcept>

void GrafoVuelos::redimensionar() {
    int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
    VerticeCiudad* nuevoArreglo = new VerticeCiudad[nuevaCapacidad];
    for (int i = 0; i < numVertices; i++) {
        nuevoArreglo[i] = vertices[i];
    }
    for (int i = numVertices; i < nuevaCapacidad; i++) {
        nuevoArreglo[i].adyacentes = nullptr;
    }
    delete[] vertices;
    vertices = nuevoArreglo;
    capacidad = nuevaCapacidad;
}

void GrafoVuelos::liberarAdyacentes(int indiceVertice) {
    NodoArista* actual = vertices[indiceVertice].adyacentes;
    while (actual != nullptr) {
        NodoArista* borrar = actual;
        actual = actual->siguiente;
        delete borrar;
    }
    vertices[indiceVertice].adyacentes = nullptr;
}

void GrafoVuelos::liberarTodo() {
    for (int i = 0; i < numVertices; i++) {
        liberarAdyacentes(i);
    }
    delete[] vertices;
    vertices = nullptr;
    numVertices = 0;
    capacidad = 0;
}

void GrafoVuelos::copiarDesde(const GrafoVuelos& otro) {
    if (otro.numVertices == 0) {
        return;
    }
    capacidad = otro.numVertices;
    numVertices = otro.numVertices;
    vertices = new VerticeCiudad[capacidad];
    for (int i = 0; i < numVertices; i++) {
        vertices[i].ciudad = otro.vertices[i].ciudad;
        vertices[i].adyacentes = nullptr;
        NodoArista* actual = otro.vertices[i].adyacentes;
        NodoArista* ultimo = nullptr;
        while (actual != nullptr) {
            NodoArista* nuevo = new NodoArista;
            nuevo->destino = actual->destino;
            nuevo->costo = actual->costo;
            nuevo->tiempoMin = actual->tiempoMin;
            nuevo->siguiente = nullptr;
            if (vertices[i].adyacentes == nullptr) {
                vertices[i].adyacentes = nuevo;
            } else {
                ultimo->siguiente = nuevo;
            }
            ultimo = nuevo;
            actual = actual->siguiente;
        }
    }
}

NodoArista* GrafoVuelos::buscarArista(int indiceOrigen, int indiceDestino) const {
    if (indiceOrigen < 0 || indiceOrigen >= numVertices || indiceDestino < 0 || indiceDestino >= numVertices) {
        return nullptr;
    }
    NodoArista* actual = vertices[indiceOrigen].adyacentes;
    while (actual != nullptr) {
        if (actual->destino == indiceDestino) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

GrafoVuelos::GrafoVuelos() : vertices(nullptr), numVertices(0), capacidad(0) {
}

GrafoVuelos::GrafoVuelos(const GrafoVuelos& otro) : vertices(nullptr), numVertices(0), capacidad(0) {
    copiarDesde(otro);
}

GrafoVuelos& GrafoVuelos::operator=(const GrafoVuelos& otro) {
    if (this != &otro) {
        liberarTodo();
        copiarDesde(otro);
    }
    return *this;
}

GrafoVuelos::~GrafoVuelos() {
    liberarTodo();
}

bool GrafoVuelos::agregarCiudad(const Ciudad& ciudad) {
    if (ciudad.obtenerCodigo().empty() || existeCiudad(ciudad.obtenerCodigo())) {
        return false;
    }
    if (numVertices == capacidad) {
        redimensionar();
    }
    vertices[numVertices].ciudad = ciudad;
    vertices[numVertices].adyacentes = nullptr;
    numVertices++;
    return true;
}

bool GrafoVuelos::eliminarCiudad(const string& codigo) {
    int idx = buscarIndicePorCodigo(codigo);
    if (idx == -1) {
        return false;
    }
    liberarAdyacentes(idx);
    for (int i = 0; i < numVertices; i++) {
        if (i == idx) {
            continue;
        }
        NodoArista* actual = vertices[i].adyacentes;
        NodoArista* anterior = nullptr;
        while (actual != nullptr) {
            if (actual->destino == idx) {
                NodoArista* borrar = actual;
                actual = actual->siguiente;
                if (anterior == nullptr) {
                    vertices[i].adyacentes = actual;
                } else {
                    anterior->siguiente = actual;
                }
                delete borrar;
            } else {
                if (actual->destino > idx) {
                    actual->destino--;
                }
                anterior = actual;
                actual = actual->siguiente;
            }
        }
    }
    for (int i = idx; i < numVertices - 1; i++) {
        vertices[i] = vertices[i + 1];
    }
    vertices[numVertices - 1].adyacentes = nullptr;
    numVertices--;
    return true;
}

int GrafoVuelos::buscarIndicePorCodigo(const string& codigo) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i].ciudad.obtenerCodigo() == codigo) {
            return i;
        }
    }
    return -1;
}

int GrafoVuelos::buscarIndicePorNombre(const string& nombre) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i].ciudad.obtenerNombre() == nombre) {
            return i;
        }
    }
    return -1;
}

bool GrafoVuelos::existeCiudad(const string& codigo) const {
    return buscarIndicePorCodigo(codigo) != -1;
}

bool GrafoVuelos::agregarRutaDirigida(const string& origenCodigo, const string& destinoCodigo, int costo, int tiempoMin) {
    int origenIdx = buscarIndicePorCodigo(origenCodigo);
    int destinoIdx = buscarIndicePorCodigo(destinoCodigo);
    if (origenIdx == -1 || destinoIdx == -1 || origenIdx == destinoIdx) {
        return false;
    }
    if (buscarArista(origenIdx, destinoIdx) != nullptr) {
        return false;
    }
    NodoArista* nuevo = new NodoArista;
    nuevo->destino = destinoIdx;
    nuevo->costo = costo;
    nuevo->tiempoMin = tiempoMin;
    nuevo->siguiente = nullptr;
    if (vertices[origenIdx].adyacentes == nullptr) {
        vertices[origenIdx].adyacentes = nuevo;
    } else {
        NodoArista* actual = vertices[origenIdx].adyacentes;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    return true;
}

bool GrafoVuelos::eliminarRuta(const string& origenCodigo, const string& destinoCodigo) {
    int origenIdx = buscarIndicePorCodigo(origenCodigo);
    int destinoIdx = buscarIndicePorCodigo(destinoCodigo);
    if (origenIdx == -1 || destinoIdx == -1) {
        return false;
    }
    NodoArista* actual = vertices[origenIdx].adyacentes;
    NodoArista* anterior = nullptr;
    while (actual != nullptr) {
        if (actual->destino == destinoIdx) {
            if (anterior == nullptr) {
                vertices[origenIdx].adyacentes = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

bool GrafoVuelos::modificarRuta(const string& origenCodigo, const string& destinoCodigo, int nuevoCosto, int nuevoTiempoMin) {
    int origenIdx = buscarIndicePorCodigo(origenCodigo);
    int destinoIdx = buscarIndicePorCodigo(destinoCodigo);
    NodoArista* arista = buscarArista(origenIdx, destinoIdx);
    if (arista == nullptr) {
        return false;
    }
    arista->costo = nuevoCosto;
    arista->tiempoMin = nuevoTiempoMin;
    return true;
}

bool GrafoVuelos::existeRuta(const string& origenCodigo, const string& destinoCodigo) const {
    int origenIdx = buscarIndicePorCodigo(origenCodigo);
    int destinoIdx = buscarIndicePorCodigo(destinoCodigo);
    return buscarArista(origenIdx, destinoIdx) != nullptr;
}

int GrafoVuelos::obtenerCantidadVertices() const {
    return numVertices;
}

const Ciudad& GrafoVuelos::obtenerCiudad(int indice) const {
    if (indice < 0 || indice >= numVertices) {
        throw out_of_range("Indice de ciudad fuera de rango");
    }
    return vertices[indice].ciudad;
}

const NodoArista* GrafoVuelos::obtenerAdyacentes(int indice) const {
    if (indice < 0 || indice >= numVertices) {
        return nullptr;
    }
    return vertices[indice].adyacentes;
}

void GrafoVuelos::limpiar() {
    liberarTodo();
}
