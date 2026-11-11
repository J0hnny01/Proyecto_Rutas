#include "ArbolABB.h"
#include "../estructuras/Cola.h"

ArbolABB::ArbolABB() : raiz(nullptr), cantidad(0) {}

ArbolABB::ArbolABB(const ArbolABB& otro) : raiz(nullptr), cantidad(otro.cantidad) {
	raiz = clonarRec(otro.raiz);
}

ArbolABB& ArbolABB::operator=(const ArbolABB& otro) {
	if (this != &otro) {
		liberarArbol(raiz);
		raiz = clonarRec(otro.raiz);
		cantidad = otro.cantidad;
	}
	return *this;
}

ArbolABB::~ArbolABB() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidad = 0;
}

NodoABB* ArbolABB::clonarRec(const NodoABB* nodo) const {
	if (nodo == nullptr) {
		return nullptr;
	}
	NodoABB* nuevo = new NodoABB(nodo->codigo, nodo->nombre, nodo->indiceVertice);
	nuevo->izquierdo = clonarRec(nodo->izquierdo);
	nuevo->derecho = clonarRec(nodo->derecho);
	return nuevo;
}

void ArbolABB::liberarArbol(NodoABB* nodo) {
	if (nodo == nullptr) {
		return;
	}
	liberarArbol(nodo->izquierdo);
	liberarArbol(nodo->derecho);
	delete nodo;
}

NodoABB* ArbolABB::insertarRec(NodoABB* nodo, const std::string& codigo, const std::string& nombre, int indiceVertice, bool& insertado) {
	if (nodo == nullptr) {
		insertado = true;
		return new NodoABB(codigo, nombre, indiceVertice);
	}
	if (nombre < nodo->nombre) {
		nodo->izquierdo = insertarRec(nodo->izquierdo, codigo, nombre, indiceVertice, insertado);
	} else if (nombre > nodo->nombre) {
		nodo->derecho = insertarRec(nodo->derecho, codigo, nombre, indiceVertice, insertado);
	}
	return nodo;
}

NodoABB* ArbolABB::buscarRec(NodoABB* nodo, const std::string& nombre) const {
	if (nodo == nullptr || nodo->nombre == nombre) {
		return nodo;
	}
	if (nombre < nodo->nombre) {
		return buscarRec(nodo->izquierdo, nombre);
	}
	return buscarRec(nodo->derecho, nombre);
}

NodoABB* ArbolABB::encontrarMinimo(NodoABB* nodo) const {
	while (nodo != nullptr && nodo->izquierdo != nullptr) {
		nodo = nodo->izquierdo;
	}
	return nodo;
}

NodoABB* ArbolABB::eliminarRec(NodoABB* nodo, const std::string& nombre, bool& eliminado) {
	if (nodo == nullptr) {
		return nullptr;
	}
	if (nombre < nodo->nombre) {
		nodo->izquierdo = eliminarRec(nodo->izquierdo, nombre, eliminado);
	} else if (nombre > nodo->nombre) {
		nodo->derecho = eliminarRec(nodo->derecho, nombre, eliminado);
	} else {
		eliminado = true;
		if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
			delete nodo;
			return nullptr;
		}
		if (nodo->izquierdo == nullptr) {
			NodoABB* temp = nodo->derecho;
			delete nodo;
			return temp;
		}
		if (nodo->derecho == nullptr) {
			NodoABB* temp = nodo->izquierdo;
			delete nodo;
			return temp;
		}
		NodoABB* sucesor = encontrarMinimo(nodo->derecho);
		nodo->codigo = sucesor->codigo;
		nodo->nombre = sucesor->nombre;
		nodo->indiceVertice = sucesor->indiceVertice;
		bool dummy = false;
		nodo->derecho = eliminarRec(nodo->derecho, sucesor->nombre, dummy);
	}
	return nodo;
}

int ArbolABB::calcularAltura(const NodoABB* nodo) const {
	if (nodo == nullptr) {
		return -1;
	}
	int altIzq = calcularAltura(nodo->izquierdo);
	int altDer = calcularAltura(nodo->derecho);
	return (altIzq > altDer ? altIzq : altDer) + 1;
}

int ArbolABB::contarHojasRec(const NodoABB* nodo) const {
	if (nodo == nullptr) {
		return 0;
	}
	if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
		return 1;
	}
	return contarHojasRec(nodo->izquierdo) + contarHojasRec(nodo->derecho);
}

void ArbolABB::concatenarNodo(std::string& resultado, const NodoABB* nodo) const {
	if (!resultado.empty()) {
		resultado += ", ";
	}
	resultado += nodo->nombre + " (" + nodo->codigo + ")";
}

void ArbolABB::preordenRec(const NodoABB* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	concatenarNodo(resultado, nodo);
	preordenRec(nodo->izquierdo, resultado);
	preordenRec(nodo->derecho, resultado);
}

void ArbolABB::inordenRec(const NodoABB* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	inordenRec(nodo->izquierdo, resultado);
	concatenarNodo(resultado, nodo);
	inordenRec(nodo->derecho, resultado);
}

void ArbolABB::postordenRec(const NodoABB* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	postordenRec(nodo->izquierdo, resultado);
	postordenRec(nodo->derecho, resultado);
	concatenarNodo(resultado, nodo);
}

bool ArbolABB::estaVacio() const {
	return raiz == nullptr;
}

int ArbolABB::getCantidad() const {
	return cantidad;
}

int ArbolABB::getAltura() const {
	return calcularAltura(raiz);
}

int ArbolABB::contarHojas() const {
	return contarHojasRec(raiz);
}

const NodoABB* ArbolABB::getRaiz() const {
	return raiz;
}

bool ArbolABB::insertar(const std::string& codigo, const std::string& nombre, int indiceVertice) {
	bool insertado = false;
	raiz = insertarRec(raiz, codigo, nombre, indiceVertice, insertado);
	if (insertado) {
		cantidad++;
	}
	return insertado;
}

const NodoABB* ArbolABB::buscar(const std::string& nombre) const {
	return buscarRec(raiz, nombre);
}

bool ArbolABB::contiene(const std::string& nombre) const {
	return buscarRec(raiz, nombre) != nullptr;
}

bool ArbolABB::eliminar(const std::string& nombre) {
	bool eliminado = false;
	raiz = eliminarRec(raiz, nombre, eliminado);
	if (eliminado) {
		cantidad--;
	}
	return eliminado;
}

void ArbolABB::limpiar() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidad = 0;
}

std::string ArbolABB::getPreorden() const {
	std::string resultado;
	preordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolABB::getInorden() const {
	std::string resultado;
	inordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolABB::getPostorden() const {
	std::string resultado;
	postordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolABB::getPorNiveles() const {
	if (raiz == nullptr) {
		return "";
	}
	Cola<const NodoABB*> cola;
	cola.encolar(raiz);
	std::string resultado;
	while (!cola.estaVacia()) {
		const NodoABB* actual = cola.obtenerFrente();
		cola.desencolar();
		concatenarNodo(resultado, actual);
		if (actual->izquierdo != nullptr) {
			cola.encolar(actual->izquierdo);
		}
		if (actual->derecho != nullptr) {
			cola.encolar(actual->derecho);
		}
	}
	return resultado;
}
