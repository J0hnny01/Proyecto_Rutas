#include "ArbolMST.h"
#include "../estructuras/Cola.h"

ArbolMST::ArbolMST() : raiz(nullptr), cantidadNodos(0), pesoTotal(0) {}

ArbolMST::ArbolMST(const ArbolMST& otro)
	: raiz(nullptr), cantidadNodos(otro.cantidadNodos), pesoTotal(otro.pesoTotal) {
	raiz = clonarRec(otro.raiz);
}

ArbolMST& ArbolMST::operator=(const ArbolMST& otro) {
	if (this != &otro) {
		liberarArbol(raiz);
		raiz = clonarRec(otro.raiz);
		cantidadNodos = otro.cantidadNodos;
		pesoTotal = otro.pesoTotal;
	}
	return *this;
}

ArbolMST::~ArbolMST() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidadNodos = 0;
	pesoTotal = 0;
}

NodoMST* ArbolMST::clonarRec(const NodoMST* nodo) const {
	if (nodo == nullptr) {
		return nullptr;
	}
	NodoMST* nuevo = new NodoMST(nodo->indiceCiudad, nodo->codigoCiudad, nodo->nombreCiudad, nodo->pesoDesdePadre);
	nuevo->primerHijo = clonarRec(nodo->primerHijo);
	nuevo->siguienteHermano = clonarRec(nodo->siguienteHermano);
	return nuevo;
}

void ArbolMST::liberarArbol(NodoMST* nodo) {
	if (nodo == nullptr) {
		return;
	}
	liberarArbol(nodo->primerHijo);
	liberarArbol(nodo->siguienteHermano);
	delete nodo;
}

NodoMST* ArbolMST::buscarIndiceRec(NodoMST* nodo, int indiceCiudad) const {
	if (nodo == nullptr) {
		return nullptr;
	}
	if (nodo->indiceCiudad == indiceCiudad) {
		return nodo;
	}
	NodoMST* enHijos = buscarIndiceRec(nodo->primerHijo, indiceCiudad);
	if (enHijos != nullptr) {
		return enHijos;
	}
	return buscarIndiceRec(nodo->siguienteHermano, indiceCiudad);
}

NodoMST* ArbolMST::buscarCodigoRec(NodoMST* nodo, const std::string& codigo) const {
	if (nodo == nullptr) {
		return nullptr;
	}
	if (nodo->codigoCiudad == codigo) {
		return nodo;
	}
	NodoMST* enHijos = buscarCodigoRec(nodo->primerHijo, codigo);
	if (enHijos != nullptr) {
		return enHijos;
	}
	return buscarCodigoRec(nodo->siguienteHermano, codigo);
}

void ArbolMST::enlazarHijo(NodoMST* padre, NodoMST* hijo) {
	if (padre->primerHijo == nullptr) {
		padre->primerHijo = hijo;
		return;
	}
	NodoMST* actual = padre->primerHijo;
	while (actual->siguienteHermano != nullptr) {
		actual = actual->siguienteHermano;
	}
	actual->siguienteHermano = hijo;
}

int ArbolMST::calcularAltura(const NodoMST* nodo) const {
	if (nodo == nullptr) {
		return -1;
	}
	if (nodo->primerHijo == nullptr) {
		return 0;
	}
	int maxAlturaHijo = -1;
	const NodoMST* hijo = nodo->primerHijo;
	while (hijo != nullptr) {
		int alturaHijo = calcularAltura(hijo);
		if (alturaHijo > maxAlturaHijo) {
			maxAlturaHijo = alturaHijo;
		}
		hijo = hijo->siguienteHermano;
	}
	return maxAlturaHijo + 1;
}

int ArbolMST::contarHojasRec(const NodoMST* nodo) const {
	if (nodo == nullptr) {
		return 0;
	}
	if (nodo->primerHijo == nullptr) {
		return 1;
	}
	int total = 0;
	const NodoMST* hijo = nodo->primerHijo;
	while (hijo != nullptr) {
		total += contarHojasRec(hijo);
		hijo = hijo->siguienteHermano;
	}
	return total;
}

void ArbolMST::concatenarNodo(std::string& resultado, const NodoMST* nodo) const {
	if (!resultado.empty()) {
		resultado += ", ";
	}
	resultado += nodo->codigoCiudad;
	if (nodo->pesoDesdePadre > 0) {
		resultado += "(" + std::to_string(nodo->pesoDesdePadre) + ")";
	}
}

void ArbolMST::preordenRec(const NodoMST* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	concatenarNodo(resultado, nodo);
	const NodoMST* hijo = nodo->primerHijo;
	while (hijo != nullptr) {
		preordenRec(hijo, resultado);
		hijo = hijo->siguienteHermano;
	}
}

void ArbolMST::inordenRec(const NodoMST* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	if (nodo->primerHijo == nullptr) {
		concatenarNodo(resultado, nodo);
		return;
	}
	inordenRec(nodo->primerHijo, resultado);
	concatenarNodo(resultado, nodo);
	const NodoMST* hermano = nodo->primerHijo->siguienteHermano;
	while (hermano != nullptr) {
		inordenRec(hermano, resultado);
		hermano = hermano->siguienteHermano;
	}
}

void ArbolMST::postordenRec(const NodoMST* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	const NodoMST* hijo = nodo->primerHijo;
	while (hijo != nullptr) {
		postordenRec(hijo, resultado);
		hijo = hijo->siguienteHermano;
	}
	concatenarNodo(resultado, nodo);
}

bool ArbolMST::estaVacio() const {
	return raiz == nullptr;
}

int ArbolMST::getCantidadNodos() const {
	return cantidadNodos;
}

int ArbolMST::getCantidadAristas() const {
	if (cantidadNodos <= 1) {
		return 0;
	}
	return cantidadNodos - 1;
}

int ArbolMST::getPesoTotal() const {
	return pesoTotal;
}

int ArbolMST::getAltura() const {
	return calcularAltura(raiz);
}

int ArbolMST::contarHojas() const {
	return contarHojasRec(raiz);
}

const NodoMST* ArbolMST::getRaiz() const {
	return raiz;
}

void ArbolMST::crearRaiz(int indiceCiudad, const std::string& codigoCiudad, const std::string& nombreCiudad) {
	limpiar();
	raiz = new NodoMST(indiceCiudad, codigoCiudad, nombreCiudad, 0);
	cantidadNodos = 1;
	pesoTotal = 0;
}

bool ArbolMST::agregarHijo(int indicePadre, int indiceHijo, const std::string& codigoHijo, const std::string& nombreHijo, int peso) {
	if (raiz == nullptr) {
		return false;
	}
	if (buscarIndiceRec(raiz, indiceHijo) != nullptr) {
		return false;
	}
	NodoMST* padre = buscarIndiceRec(raiz, indicePadre);
	if (padre == nullptr) {
		return false;
	}
	NodoMST* nuevo = new NodoMST(indiceHijo, codigoHijo, nombreHijo, peso);
	enlazarHijo(padre, nuevo);
	cantidadNodos++;
	pesoTotal += peso;
	return true;
}

bool ArbolMST::construirDesdeAristas(int indiceRaiz, const AristaMST* aristas, int totalAristas, const std::string* codigos, const std::string* nombres, int totalVertices) {
	if (totalVertices <= 0 || indiceRaiz < 0 || indiceRaiz >= totalVertices || codigos == nullptr || nombres == nullptr) {
		limpiar();
		return false;
	}
	if (totalAristas > 0 && aristas == nullptr) {
		limpiar();
		return false;
	}

	crearRaiz(indiceRaiz, codigos[indiceRaiz], nombres[indiceRaiz]);

	bool* visitado = new bool[totalVertices];
	for (int i = 0; i < totalVertices; i++) {
		visitado[i] = false;
	}
	visitado[indiceRaiz] = true;

	Cola<int> cola;
	cola.encolar(indiceRaiz);

	while (!cola.estaVacia()) {
		int actual = cola.obtenerFrente();
		cola.desencolar();
		NodoMST* nodoActual = buscarIndiceRec(raiz, actual);

		for (int i = 0; i < totalAristas; i++) {
			int vecino = -1;
			if (aristas[i].origenIdx == actual && aristas[i].destinoIdx >= 0 && aristas[i].destinoIdx < totalVertices && !visitado[aristas[i].destinoIdx]) {
				vecino = aristas[i].destinoIdx;
			} else if (aristas[i].destinoIdx == actual && aristas[i].origenIdx >= 0 && aristas[i].origenIdx < totalVertices && !visitado[aristas[i].origenIdx]) {
				vecino = aristas[i].origenIdx;
			}
			if (vecino != -1) {
				visitado[vecino] = true;
				NodoMST* hijo = new NodoMST(vecino, codigos[vecino], nombres[vecino], aristas[i].peso);
				enlazarHijo(nodoActual, hijo);
				cantidadNodos++;
				pesoTotal += aristas[i].peso;
				cola.encolar(vecino);
			}
		}
	}

	delete[] visitado;
	return true;
}

bool ArbolMST::construirDesdeResultado(int indiceRaiz, const ResultadoMST& resultado, const std::string* codigos, const std::string* nombres, int totalVertices) {
	int totalAristas = resultado.aristas.tamano();
	if (totalAristas == 0) {
		return construirDesdeAristas(indiceRaiz, nullptr, 0, codigos, nombres, totalVertices);
	}
	AristaMST* lista = new AristaMST[totalAristas];
	const ListaEnlazada<AristaMST>::NodoLista* actual = resultado.aristas.obtenerCabeza();
	int i = 0;
	while (actual != nullptr && i < totalAristas) {
		lista[i] = actual->dato;
		actual = actual->siguiente;
		i++;
	}
	bool ok = construirDesdeAristas(indiceRaiz, lista, totalAristas, codigos, nombres, totalVertices);
	delete[] lista;
	return ok;
}

const NodoMST* ArbolMST::buscarPorIndice(int indiceCiudad) const {
	return buscarIndiceRec(raiz, indiceCiudad);
}

const NodoMST* ArbolMST::buscarPorCodigo(const std::string& codigo) const {
	return buscarCodigoRec(raiz, codigo);
}

void ArbolMST::limpiar() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidadNodos = 0;
	pesoTotal = 0;
}

std::string ArbolMST::getPreorden() const {
	std::string resultado;
	preordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolMST::getInorden() const {
	std::string resultado;
	inordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolMST::getPostorden() const {
	std::string resultado;
	postordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolMST::getPorNiveles() const {
	if (raiz == nullptr) {
		return "";
	}
	Cola<const NodoMST*> cola;
	cola.encolar(raiz);
	std::string resultado;
	while (!cola.estaVacia()) {
		const NodoMST* actual = cola.obtenerFrente();
		cola.desencolar();
		concatenarNodo(resultado, actual);
		const NodoMST* hijo = actual->primerHijo;
		while (hijo != nullptr) {
			cola.encolar(hijo);
			hijo = hijo->siguienteHermano;
		}
	}
	return resultado;
}
