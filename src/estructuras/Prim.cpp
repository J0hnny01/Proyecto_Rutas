#include "Prim.h"
#include "ColaPrioridad.h"

int* Prim::construirMatrizPesos(const GrafoVuelos& grafo, int n, CriterioRuta criterio) {
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
					int posUV = u * n + v;
					int posVU = v * n + u;
					if (pesos[posUV] == -1 || peso < pesos[posUV]) {
						pesos[posUV] = peso;
						pesos[posVU] = peso;
					}
				}
			}
			ady = ady->siguiente;
		}
	}
	return pesos;
}

ResultadoMST Prim::calcular(const GrafoVuelos& grafo, int indiceOrigen, CriterioRuta criterio) {
	ResultadoMST resultado;
	int n = grafo.obtenerCantidadVertices();
	if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n) {
		return resultado;
	}

	const int INF_PRIM = 1000000000;
	int* pesos = construirMatrizPesos(grafo, n, criterio);
	int* clave = new int[n];
	int* padre = new int[n];
	bool* enArbol = new bool[n];

	for (int i = 0; i < n; i++) {
		clave[i] = INF_PRIM;
		padre[i] = -1;
		enArbol[i] = false;
	}
	clave[indiceOrigen] = 0;

	ColaPrioridad cola;
	cola.insertar(indiceOrigen, 0);
	int visitados = 0;

	while (!cola.estaVacia()) {
		ElementoPrioridad actual = cola.extraerMinimo();
		int u = actual.vertice;
		if (enArbol[u]) {
			continue;
		}
		enArbol[u] = true;
		visitados++;

		if (padre[u] != -1) {
			resultado.aristas.insertarAlFinal(AristaMST(padre[u], u, clave[u]));
			resultado.pesoTotal += clave[u];
		}

		for (int v = 0; v < n; v++) {
			int peso = pesos[u * n + v];
			if (peso != -1 && !enArbol[v] && peso < clave[v]) {
				clave[v] = peso;
				padre[v] = u;
				cola.insertar(v, clave[v]);
			}
		}
	}

	resultado.esConexo = (visitados == n);

	delete[] pesos;
	delete[] clave;
	delete[] padre;
	delete[] enArbol;

	return resultado;
}

bool Prim::construirArbol(const GrafoVuelos& grafo, int indiceOrigen, ArbolMST& arbol, CriterioRuta criterio) {
	int n = grafo.obtenerCantidadVertices();
	if (n <= 0 || indiceOrigen < 0 || indiceOrigen >= n) {
		arbol.limpiar();
		return false;
	}

	ResultadoMST resultado = calcular(grafo, indiceOrigen, criterio);
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
