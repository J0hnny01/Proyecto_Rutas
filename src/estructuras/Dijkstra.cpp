#include "Dijkstra.h"

#include "Pila.h"

ResultadoRuta Dijkstra::calcularRutaOptima(const GrafoVuelos& grafo,
                                            int indiceOrigen,
                                            int indiceDestino,
                                            CriterioRuta criterio) {
    ResultadoRuta resultado;
    int n = grafo.obtenerCantidadVertices();
    if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n || indiceDestino < 0 || indiceDestino >= n) {
        return resultado;
    }

    if (indiceOrigen == indiceDestino) {
        resultado.encontrada = true;
        resultado.costoTotal = 0;
        resultado.duracionTotal = 0;
        resultado.cantidadEscalas = 0;
        resultado.caminoIndices.insertarAlFinal(indiceOrigen);
        return resultado;
    }

    int* costos = new int[n];
    int* tiempos = new int[n];
    int* previo = new int[n];
    bool* fijado = new bool[n];
    bool* alcanzado = new bool[n];

    for (int i = 0; i < n; i++) {
        costos[i] = 0;
        tiempos[i] = 0;
        previo[i] = -1;
        fijado[i] = false;
        alcanzado[i] = false;
    }

    alcanzado[indiceOrigen] = true;

    for (int paso = 0; paso < n; paso++) {
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!fijado[i] && alcanzado[i]) {
                if (u == -1 || esMejorCamino(costos[i], tiempos[i], costos[u], tiempos[u], criterio)) {
                    u = i;
                }
            }
        }

        if (u == -1) {
            break;
        }

        fijado[u] = true;
        if (u == indiceDestino) {
            break;
        }

        const NodoArista* arista = grafo.obtenerAdyacentes(u);
        while (arista != nullptr) {
            int v = arista->destino;
            if (!fijado[v]) {
                int nuevoCosto = costos[u] + arista->costo;
                int nuevoTiempo = tiempos[u] + arista->tiempoMin;
                if (!alcanzado[v] || esMejorCamino(nuevoCosto, nuevoTiempo, costos[v], tiempos[v], criterio)) {
                    costos[v] = nuevoCosto;
                    tiempos[v] = nuevoTiempo;
                    previo[v] = u;
                    alcanzado[v] = true;
                }
            }
            arista = arista->siguiente;
        }
    }

    if (alcanzado[indiceDestino]) {
        resultado.encontrada = true;
        resultado.costoTotal = costos[indiceDestino];
        resultado.duracionTotal = tiempos[indiceDestino];

        Pila pila;
        int actual = indiceDestino;
        while (actual != -1) {
            pila.apilar(actual);
            actual = previo[actual];
        }
        while (!pila.estaVacia()) {
            resultado.caminoIndices.insertarAlFinal(pila.verTope());
            pila.desapilar();
        }

        int totalCiudades = resultado.caminoIndices.tamano();
        resultado.cantidadEscalas = (totalCiudades >= 2) ? (totalCiudades - 2) : 0;
    }

    delete[] costos;
    delete[] tiempos;
    delete[] previo;
    delete[] fijado;
    delete[] alcanzado;
    return resultado;
}

ResultadoRuta Dijkstra::calcularRutaOptima(const GrafoVuelos& grafo,
                                            const string& codigoOrigen,
                                            const string& codigoDestino,
                                            CriterioRuta criterio) {
    int indiceOrigen = grafo.buscarIndicePorCodigo(codigoOrigen);
    int indiceDestino = grafo.buscarIndicePorCodigo(codigoDestino);
    return calcularRutaOptima(grafo, indiceOrigen, indiceDestino, criterio);
}

ResultadoRuta Dijkstra::calcularRutaMasBarata(const GrafoVuelos& grafo,
                                               const string& codigoOrigen,
                                               const string& codigoDestino) {
    return calcularRutaOptima(grafo, codigoOrigen, codigoDestino, CRITERIO_COSTO);
}

ResultadoRuta Dijkstra::calcularRutaMasRapida(const GrafoVuelos& grafo,
                                               const string& codigoOrigen,
                                               const string& codigoDestino) {
    return calcularRutaOptima(grafo, codigoOrigen, codigoDestino, CRITERIO_TIEMPO);
}

Equipaje Dijkstra::calcularRutaEquipaje(const GrafoVuelos& grafo,
                                        const string& idPasajero,
                                        int cantidadMaletas,
                                        const string& codigoOrigen,
                                        const string& codigoDestino) {
    ResultadoRuta rutaBarata = calcularRutaMasBarata(grafo, codigoOrigen, codigoDestino);
    Equipaje equipaje(idPasajero, cantidadMaletas, rutaBarata.costoTotal, rutaBarata.duracionTotal);
    if (rutaBarata.encontrada) {
        const ListaEnlazada<int>::NodoLista* actual = rutaBarata.caminoIndices.obtenerCabeza();
        while (actual != nullptr) {
            equipaje.agregarCiudadRuta(grafo.obtenerCiudad(actual->dato).obtenerCodigo());
            actual = actual->siguiente;
        }
    }
    return equipaje;
}
