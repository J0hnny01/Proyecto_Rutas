#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include <string>
#include "../modelos/Pasajero.h"

struct NodoAVL {
	Pasajero dato;
	int altura;
	NodoAVL* izquierdo;
	NodoAVL* derecho;

	NodoAVL(const Pasajero& p)
		: dato(p), altura(0), izquierdo(nullptr), derecho(nullptr) {}
};

class ArbolAVL {
private:
	NodoAVL* raiz;
	int cantidad;

	NodoAVL* clonarRec(const NodoAVL* nodo) const;
	void liberarArbol(NodoAVL* nodo);
	int alturaNodo(const NodoAVL* nodo) const;
	int factorBalance(const NodoAVL* nodo) const;
	void actualizarAltura(NodoAVL* nodo);
	NodoAVL* rotacionDerecha(NodoAVL* nodo);
	NodoAVL* rotacionIzquierda(NodoAVL* nodo);
	NodoAVL* insertarRec(NodoAVL* nodo, const Pasajero& pasajero, bool& nodoNuevo, bool& actualizado);
	NodoAVL* buscarRec(NodoAVL* nodo, const std::string& id) const;
	NodoAVL* encontrarMinimo(NodoAVL* nodo) const;
	NodoAVL* eliminarRec(NodoAVL* nodo, const std::string& id, bool& eliminado);
	int contarHojasRec(const NodoAVL* nodo) const;
	void concatenarNodo(std::string& resultado, const NodoAVL* nodo) const;
	void preordenRec(const NodoAVL* nodo, std::string& resultado) const;
	void inordenRec(const NodoAVL* nodo, std::string& resultado) const;
	void postordenRec(const NodoAVL* nodo, std::string& resultado) const;

public:
	ArbolAVL();
	ArbolAVL(const ArbolAVL& otro);
	ArbolAVL& operator=(const ArbolAVL& otro);
	~ArbolAVL();

	bool estaVacio() const;
	int getCantidad() const;
	int getAltura() const;
	int contarHojas() const;
	const NodoAVL* getRaiz() const;

	bool insertar(const Pasajero& pasajero);
	const NodoAVL* buscar(const std::string& id) const;
	bool contiene(const std::string& id) const;
	bool eliminar(const std::string& id);
	bool asignarVuelo(const std::string& id, const std::string& codigoVuelo);
	bool quitarVuelo(const std::string& id, const std::string& codigoVuelo);
	void limpiar();

	std::string getPreorden() const;
	std::string getInorden() const;
	std::string getPostorden() const;
	std::string getPorNiveles() const;
};

#endif
