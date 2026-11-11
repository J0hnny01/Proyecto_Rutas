#ifndef ARBOL_MST_H
#define ARBOL_MST_H

#include <string>
#include "../estructuras/ResultadosGrafo.h"

struct NodoMST {
	int indiceCiudad;
	std::string codigoCiudad;
	std::string nombreCiudad;
	int pesoDesdePadre;
	NodoMST* primerHijo;
	NodoMST* siguienteHermano;

	NodoMST(int idx, const std::string& codigo, const std::string& nombre, int peso)
		: indiceCiudad(idx), codigoCiudad(codigo), nombreCiudad(nombre), pesoDesdePadre(peso), primerHijo(nullptr), siguienteHermano(nullptr) {}
};

class ArbolMST {
private:
	NodoMST* raiz;
	int cantidadNodos;
	int pesoTotal;

	NodoMST* clonarRec(const NodoMST* nodo) const;
	void liberarArbol(NodoMST* nodo);
	NodoMST* buscarIndiceRec(NodoMST* nodo, int indiceCiudad) const;
	NodoMST* buscarCodigoRec(NodoMST* nodo, const std::string& codigo) const;
	void enlazarHijo(NodoMST* padre, NodoMST* hijo);
	int calcularAltura(const NodoMST* nodo) const;
	int contarHojasRec(const NodoMST* nodo) const;
	void concatenarNodo(std::string& resultado, const NodoMST* nodo) const;
	void preordenRec(const NodoMST* nodo, std::string& resultado) const;
	void inordenRec(const NodoMST* nodo, std::string& resultado) const;
	void postordenRec(const NodoMST* nodo, std::string& resultado) const;

public:
	ArbolMST();
	ArbolMST(const ArbolMST& otro);
	ArbolMST& operator=(const ArbolMST& otro);
	~ArbolMST();

	bool estaVacio() const;
	int getCantidadNodos() const;
	int getCantidadAristas() const;
	int getPesoTotal() const;
	int getAltura() const;
	int contarHojas() const;
	const NodoMST* getRaiz() const;

	void crearRaiz(int indiceCiudad, const std::string& codigoCiudad, const std::string& nombreCiudad);
	bool agregarHijo(int indicePadre, int indiceHijo, const std::string& codigoHijo, const std::string& nombreHijo, int peso);
	bool construirDesdeAristas(int indiceRaiz, const AristaMST* aristas, int totalAristas, const std::string* codigos, const std::string* nombres, int totalVertices);
	bool construirDesdeResultado(int indiceRaiz, const ResultadoMST& resultado, const std::string* codigos, const std::string* nombres, int totalVertices);

	const NodoMST* buscarPorIndice(int indiceCiudad) const;
	const NodoMST* buscarPorCodigo(const std::string& codigo) const;
	void limpiar();

	std::string getPreorden() const;
	std::string getInorden() const;
	std::string getPostorden() const;
	std::string getPorNiveles() const;
};

#endif
