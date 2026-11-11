#include "ArbolAVL.h"
#include "../estructuras/Cola.h"

ArbolAVL::ArbolAVL() : raiz(nullptr), cantidad(0) {}

ArbolAVL::ArbolAVL(const ArbolAVL& otro) : raiz(nullptr), cantidad(otro.cantidad) {
	raiz = clonarRec(otro.raiz);
}

ArbolAVL& ArbolAVL::operator=(const ArbolAVL& otro) {
	if (this != &otro) {
		liberarArbol(raiz);
		raiz = clonarRec(otro.raiz);
		cantidad = otro.cantidad;
	}
	return *this;
}

ArbolAVL::~ArbolAVL() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidad = 0;
}

NodoAVL* ArbolAVL::clonarRec(const NodoAVL* nodo) const {
	if (nodo == nullptr) {
		return nullptr;
	}
	NodoAVL* nuevo = new NodoAVL(nodo->dato);
	nuevo->altura = nodo->altura;
	nuevo->izquierdo = clonarRec(nodo->izquierdo);
	nuevo->derecho = clonarRec(nodo->derecho);
	return nuevo;
}

void ArbolAVL::liberarArbol(NodoAVL* nodo) {
	if (nodo == nullptr) {
		return;
	}
	liberarArbol(nodo->izquierdo);
	liberarArbol(nodo->derecho);
	delete nodo;
}

int ArbolAVL::alturaNodo(const NodoAVL* nodo) const {
	if (nodo == nullptr) {
		return -1;
	}
	return nodo->altura;
}

int ArbolAVL::factorBalance(const NodoAVL* nodo) const {
	if (nodo == nullptr) {
		return 0;
	}
	return alturaNodo(nodo->izquierdo) - alturaNodo(nodo->derecho);
}

void ArbolAVL::actualizarAltura(NodoAVL* nodo) {
	int alturaIzq = alturaNodo(nodo->izquierdo);
	int alturaDer = alturaNodo(nodo->derecho);
	if (alturaIzq > alturaDer) {
		nodo->altura = alturaIzq + 1;
	} else {
		nodo->altura = alturaDer + 1;
	}
}

NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* nodo) {
	NodoAVL* nuevaRaiz = nodo->izquierdo;
	nodo->izquierdo = nuevaRaiz->derecho;
	nuevaRaiz->derecho = nodo;
	actualizarAltura(nodo);
	actualizarAltura(nuevaRaiz);
	return nuevaRaiz;
}

NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* nodo) {
	NodoAVL* nuevaRaiz = nodo->derecho;
	nodo->derecho = nuevaRaiz->izquierdo;
	nuevaRaiz->izquierdo = nodo;
	actualizarAltura(nodo);
	actualizarAltura(nuevaRaiz);
	return nuevaRaiz;
}

NodoAVL* ArbolAVL::insertarRec(NodoAVL* nodo, const Pasajero& pasajero, bool& nodoNuevo, bool& actualizado) {
	if (nodo == nullptr) {
		nodoNuevo = true;
		return new NodoAVL(pasajero);
	}
	const std::string& idNuevo = pasajero.getId();
	const std::string& idActual = nodo->dato.getId();
	if (idNuevo < idActual) {
		nodo->izquierdo = insertarRec(nodo->izquierdo, pasajero, nodoNuevo, actualizado);
	} else if (idNuevo > idActual) {
		nodo->derecho = insertarRec(nodo->derecho, pasajero, nodoNuevo, actualizado);
	} else {
		for (int i = 0; i < pasajero.getCantidadVuelos(); i++) {
			if (nodo->dato.agregarVuelo(pasajero.getVuelo(i))) {
				actualizado = true;
			}
		}
		return nodo;
	}

	actualizarAltura(nodo);
	int balance = factorBalance(nodo);

	if (balance > 1 && idNuevo < nodo->izquierdo->dato.getId()) {
		return rotacionDerecha(nodo);
	}
	if (balance < -1 && idNuevo > nodo->derecho->dato.getId()) {
		return rotacionIzquierda(nodo);
	}
	if (balance > 1 && idNuevo > nodo->izquierdo->dato.getId()) {
		nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
		return rotacionDerecha(nodo);
	}
	if (balance < -1 && idNuevo < nodo->derecho->dato.getId()) {
		nodo->derecho = rotacionDerecha(nodo->derecho);
		return rotacionIzquierda(nodo);
	}
	return nodo;
}

NodoAVL* ArbolAVL::buscarRec(NodoAVL* nodo, const std::string& id) const {
	if (nodo == nullptr || nodo->dato.getId() == id) {
		return nodo;
	}
	if (id < nodo->dato.getId()) {
		return buscarRec(nodo->izquierdo, id);
	}
	return buscarRec(nodo->derecho, id);
}

NodoAVL* ArbolAVL::encontrarMinimo(NodoAVL* nodo) const {
	while (nodo != nullptr && nodo->izquierdo != nullptr) {
		nodo = nodo->izquierdo;
	}
	return nodo;
}

NodoAVL* ArbolAVL::eliminarRec(NodoAVL* nodo, const std::string& id, bool& eliminado) {
	if (nodo == nullptr) {
		return nullptr;
	}
	if (id < nodo->dato.getId()) {
		nodo->izquierdo = eliminarRec(nodo->izquierdo, id, eliminado);
	} else if (id > nodo->dato.getId()) {
		nodo->derecho = eliminarRec(nodo->derecho, id, eliminado);
	} else {
		eliminado = true;
		if (nodo->izquierdo == nullptr || nodo->derecho == nullptr) {
			NodoAVL* hijo = nodo->izquierdo;
			if (hijo == nullptr) {
				hijo = nodo->derecho;
			}
			delete nodo;
			return hijo;
		} else {
			NodoAVL* sucesor = encontrarMinimo(nodo->derecho);
			nodo->dato = sucesor->dato;
			bool dummy = false;
			nodo->derecho = eliminarRec(nodo->derecho, sucesor->dato.getId(), dummy);
		}
	}

	actualizarAltura(nodo);
	int balance = factorBalance(nodo);

	if (balance > 1 && factorBalance(nodo->izquierdo) >= 0) {
		return rotacionDerecha(nodo);
	}
	if (balance > 1 && factorBalance(nodo->izquierdo) < 0) {
		nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
		return rotacionDerecha(nodo);
	}
	if (balance < -1 && factorBalance(nodo->derecho) <= 0) {
		return rotacionIzquierda(nodo);
	}
	if (balance < -1 && factorBalance(nodo->derecho) > 0) {
		nodo->derecho = rotacionDerecha(nodo->derecho);
		return rotacionIzquierda(nodo);
	}
	return nodo;
}

int ArbolAVL::contarHojasRec(const NodoAVL* nodo) const {
	if (nodo == nullptr) {
		return 0;
	}
	if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
		return 1;
	}
	return contarHojasRec(nodo->izquierdo) + contarHojasRec(nodo->derecho);
}

void ArbolAVL::concatenarNodo(std::string& resultado, const NodoAVL* nodo) const {
	if (!resultado.empty()) {
		resultado += " | ";
	}
	resultado += nodo->dato.getId() + " (h" + std::to_string(nodo->altura) + ") [" + nodo->dato.getVuelosTexto() + "]";
}

void ArbolAVL::preordenRec(const NodoAVL* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	concatenarNodo(resultado, nodo);
	preordenRec(nodo->izquierdo, resultado);
	preordenRec(nodo->derecho, resultado);
}

void ArbolAVL::inordenRec(const NodoAVL* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	inordenRec(nodo->izquierdo, resultado);
	concatenarNodo(resultado, nodo);
	inordenRec(nodo->derecho, resultado);
}

void ArbolAVL::postordenRec(const NodoAVL* nodo, std::string& resultado) const {
	if (nodo == nullptr) {
		return;
	}
	postordenRec(nodo->izquierdo, resultado);
	postordenRec(nodo->derecho, resultado);
	concatenarNodo(resultado, nodo);
}

bool ArbolAVL::estaVacio() const {
	return raiz == nullptr;
}

int ArbolAVL::getCantidad() const {
	return cantidad;
}

int ArbolAVL::getAltura() const {
	return alturaNodo(raiz);
}

int ArbolAVL::contarHojas() const {
	return contarHojasRec(raiz);
}

const NodoAVL* ArbolAVL::getRaiz() const {
	return raiz;
}

bool ArbolAVL::insertar(const Pasajero& pasajero) {
	bool nodoNuevo = false;
	bool actualizado = false;
	raiz = insertarRec(raiz, pasajero, nodoNuevo, actualizado);
	if (nodoNuevo) {
		cantidad++;
	}
	return nodoNuevo || actualizado;
}

const NodoAVL* ArbolAVL::buscar(const std::string& id) const {
	return buscarRec(raiz, id);
}

bool ArbolAVL::contiene(const std::string& id) const {
	return buscarRec(raiz, id) != nullptr;
}

bool ArbolAVL::eliminar(const std::string& id) {
	bool eliminado = false;
	raiz = eliminarRec(raiz, id, eliminado);
	if (eliminado) {
		cantidad--;
	}
	return eliminado;
}

bool ArbolAVL::asignarVuelo(const std::string& id, const std::string& codigoVuelo) {
	NodoAVL* nodo = buscarRec(raiz, id);
	if (nodo == nullptr) {
		return false;
	}
	return nodo->dato.agregarVuelo(codigoVuelo);
}

bool ArbolAVL::quitarVuelo(const std::string& id, const std::string& codigoVuelo) {
	NodoAVL* nodo = buscarRec(raiz, id);
	if (nodo == nullptr) {
		return false;
	}
	return nodo->dato.eliminarVuelo(codigoVuelo);
}

void ArbolAVL::limpiar() {
	liberarArbol(raiz);
	raiz = nullptr;
	cantidad = 0;
}

std::string ArbolAVL::getPreorden() const {
	std::string resultado;
	preordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolAVL::getInorden() const {
	std::string resultado;
	inordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolAVL::getPostorden() const {
	std::string resultado;
	postordenRec(raiz, resultado);
	return resultado;
}

std::string ArbolAVL::getPorNiveles() const {
	if (raiz == nullptr) {
		return "";
	}
	Cola<const NodoAVL*> cola;
	cola.encolar(raiz);
	std::string resultado;
	while (!cola.estaVacia()) {
		const NodoAVL* actual = cola.obtenerFrente();
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
