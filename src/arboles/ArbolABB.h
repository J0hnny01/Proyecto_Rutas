#ifndef ARBOL_ABB_H
#define ARBOL_ABB_H

#include <string>

struct NodoABB {
	std::string codigo;
	std::string nombre;
	int indiceVertice;
	NodoABB* izquierdo;
	NodoABB* derecho;

	NodoABB(const std::string& cod, const std::string& nom, int idx)
		: codigo(cod), nombre(nom), indiceVertice(idx), izquierdo(nullptr), derecho(nullptr) {}
};

class ArbolABB {
private:
	NodoABB* raiz;
	int cantidad;

	NodoABB* clonarRec(const NodoABB* nodo) const;
	void liberarArbol(NodoABB* nodo);
	NodoABB* insertarRec(NodoABB* nodo, const std::string& codigo, const std::string& nombre, int indiceVertice, bool& insertado);
	NodoABB* buscarRec(NodoABB* nodo, const std::string& nombre) const;
	NodoABB* encontrarMinimo(NodoABB* nodo) const;
	NodoABB* eliminarRec(NodoABB* nodo, const std::string& nombre, bool& eliminado);
	int calcularAltura(const NodoABB* nodo) const;
	int contarHojasRec(const NodoABB* nodo) const;
	void preordenRec(const NodoABB* nodo, std::string& resultado) const;
	void inordenRec(const NodoABB* nodo, std::string& resultado) const;
	void postordenRec(const NodoABB* nodo, std::string& resultado) const;
	void concatenarNodo(std::string& resultado, const NodoABB* nodo) const;

public:
	ArbolABB();
	ArbolABB(const ArbolABB& otro);
	ArbolABB& operator=(const ArbolABB& otro);
	~ArbolABB();

	bool estaVacio() const;
	int getCantidad() const;
	int getAltura() const;
	int contarHojas() const;
	const NodoABB* getRaiz() const;

	bool insertar(const std::string& codigo, const std::string& nombre, int indiceVertice);
	const NodoABB* buscar(const std::string& nombre) const;
	bool contiene(const std::string& nombre) const;
	bool eliminar(const std::string& nombre);
	void limpiar();

	std::string getPreorden() const;
	std::string getInorden() const;
	std::string getPostorden() const;
	std::string getPorNiveles() const;
};

#endif
