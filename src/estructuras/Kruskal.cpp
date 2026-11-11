#include "Kruskal.h"
#include "ConjuntoDisjunto.h"

AristaOrdenable* Kruskal::extraerAristas(const GrafoVuelos& grafo, int n, CriterioRuta criterio, int& totalAristas) {
	totalAristas = 0;
	if (n <= 1) {
		return nullptr;
	}

	int* pesos = new int[n * n];
	for (int i = 0; i < n * n; i++) {
		pesos[i] = -1;
	}

	for (int u = 0; u < n; u++) {
		const NodoArista* ady = grafo.obtenerAdyacentes(u);
		while (ady != nullptr) {
			int v = ady->destino;
			if (v >= 0 && v < n && u != v) {
				int peso = (criterio == CRITERIO_COSTO) ? ady->costo : ady->tiempoMin;
				if (peso >= 0) {
					int a = (u < v) ? u : v;
					int b = (u < v) ? v : u;
					int pos = a * n + b;
					if (pesos[pos] == -1) {
						totalAristas++;
						pesos[pos] = peso;
					} else if (peso < pesos[pos]) {
						pesos[pos] = peso;
					}
				}
			}
			ady = ady->siguiente;
		}
	}

	if (totalAristas == 0) {
		delete[] pesos;
		return nullptr;
	}

	AristaOrdenable* aristas = new AristaOrdenable[totalAristas];
	int idx = 0;
	for (int a = 0; a < n; a++) {
		for (int b = a + 1; b < n; b++) {
			int peso = pesos[a * n + b];
			if (peso != -1) {
				aristas[idx].origen = a;
				aristas[idx].destino = b;
				aristas[idx].peso = peso;
				idx++;
			}
		}
	}

	delete[] pesos;
	return aristas;
}

ResultadoMST Kruskal::calcular(const GrafoVuelos& grafo, CriterioRuta criterio) {
	ResultadoMST resultado;
	int n = grafo.obtenerCantidadVertices();
	if (n <= 0) {
		return resultado;
	}
	if (n == 1) {
		resultado.esConexo = true;
		return resultado;
	}

	int totalAristas = 0;
	AristaOrdenable* aristas = extraerAristas(grafo, n, criterio, totalAristas);
	if (aristas == nullptr || totalAristas == 0) {
		delete[] aristas;
		return resultado;
	}

	Ordenamiento::ordenarAristas(aristas, totalAristas);
	ConjuntoDisjunto conjuntos(n);
	int usadas = 0;

	for (int i = 0; i < totalAristas; i++) {
		if (conjuntos.unir(aristas[i].origen, aristas[i].destino)) {
			resultado.aristas.insertarAlFinal(AristaMST(aristas[i].origen, aristas[i].destino, aristas[i].peso));
			resultado.pesoTotal += aristas[i].peso;
			usadas++;
			if (usadas == n - 1) {
				break;
			}
		}
	}

	resultado.esConexo = (usadas == n - 1);
	delete[] aristas;
	return resultado;
}

bool Kruskal::construirArbol(const GrafoVuelos& grafo, int indiceOrigen, ArbolMST& arbol, CriterioRuta criterio) {
	int n = grafo.obtenerCantidadVertices();
	if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n) {
		arbol.limpiar();
		return false;
	}

	ResultadoMST resultado = calcular(grafo, criterio);
	std::string* codigos = new std::string[n];
	std::string* nombres = new std::string[n];
	for (int i = 0; i < n; i++) {
		const Ciudad& ciudad = grafo.obtenerCiudad(i);
		codigos[i] = ciudad.obtenerCodigo();
		nombres[i] = ciudad.obtenerNombre();
	}

	bool ok = arbol.construirDesdeResultado(indiceOrigen, resultado, codigos, nombres, n);
	delete[] codigos;
	delete[] nombres;
	return ok;
}
