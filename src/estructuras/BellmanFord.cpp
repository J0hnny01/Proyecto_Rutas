#include "BellmanFord.h"

#include "Pila.h"

ResultadoBellmanFord BellmanFord::calcularDesdeOrigen(const GrafoVuelos& grafo,
                                                      int indiceOrigen,
                                                      CriterioRuta criterio) {
    ResultadoBellmanFord resultado;
    int n = grafo.obtenerCantidadVertices();
    if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n) {
        return resultado;
    }

    resultado.inicializar(n);
    resultado.alcanzable[indiceOrigen] = true;

    for (int paso = 0; paso < n - 1; paso++) {
        bool huboCambio = false;
        for (int u = 0; u < n; u++) {
            if (!resultado.alcanzable[u]) {
                continue;
            }
            const NodoArista* arista = grafo.obtenerAdyacentes(u);
            while (arista != nullptr) {
                int v = arista->destino;
                int nuevoCosto = resultado.costos[u] + arista->costo;
                int nuevoTiempo = resultado.tiempos[u] + arista->tiempoMin;
                if (!resultado.alcanzable[v] ||
                    esMejorCamino(nuevoCosto, nuevoTiempo, resultado.costos[v], resultado.tiempos[v], criterio)) {
                    resultado.costos[v] = nuevoCosto;
                    resultado.tiempos[v] = nuevoTiempo;
                    resultado.previo[v] = u;
                    resultado.alcanzable[v] = true;
                    huboCambio = true;
                }
                arista = arista->siguiente;
            }
        }
        if (!huboCambio) {
            break;
        }
    }

    for (int u = 0; u < n; u++) {
        if (!resultado.alcanzable[u]) {
            continue;
        }
        const NodoArista* arista = grafo.obtenerAdyacentes(u);
        while (arista != nullptr) {
            int v = arista->destino;
            int nuevoCosto = resultado.costos[u] + arista->costo;
            int nuevoTiempo = resultado.tiempos[u] + arista->tiempoMin;
            if (esMejorCamino(nuevoCosto, nuevoTiempo, resultado.costos[v], resultado.tiempos[v], criterio)) {
                resultado.hayCicloNegativo = true;
                return resultado;
            }
            arista = arista->siguiente;
        }
    }

    return resultado;
}

ResultadoBellmanFord BellmanFord::calcularDesdeOrigen(const GrafoVuelos& grafo,
                                                      const string& codigoOrigen,
                                                      CriterioRuta criterio) {
    int indiceOrigen = grafo.buscarIndicePorCodigo(codigoOrigen);
    return calcularDesdeOrigen(grafo, indiceOrigen, criterio);
}

ListaEnlazada<int> BellmanFord::reconstruirCamino(const ResultadoBellmanFord& resultado, int indiceDestino) {
    ListaEnlazada<int> camino;
    if (resultado.hayCicloNegativo ||
        indiceDestino < 0 ||
        indiceDestino >= resultado.cantidadVertices ||
        !resultado.alcanzable[indiceDestino]) {
        return camino;
    }

    Pila pila;
    int actual = indiceDestino;
    int pasos = 0;
    while (actual != -1 && pasos < resultado.cantidadVertices) {
        pila.apilar(actual);
        actual = resultado.previo[actual];
        pasos++;
    }

    if (actual != -1) {
        return camino;
    }

    while (!pila.estaVacia()) {
        camino.insertarAlFinal(pila.verTope());
        pila.desapilar();
    }
    return camino;
}
