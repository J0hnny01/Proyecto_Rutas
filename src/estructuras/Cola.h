#ifndef COLA_H
#define COLA_H

template <typename T>
class Cola {
private:
	struct NodoCola {
		T dato;
		NodoCola* siguiente;
		NodoCola(const T& valor) : dato(valor), siguiente(nullptr) {}
	};
	
	NodoCola* frente;
	NodoCola* final;
	int cantidad;
	
	void copiarDesde(const Cola& otra) {
		NodoCola* actual = otra.frente;
		while (actual != nullptr) {
			encolar(actual->dato);
			actual = actual->siguiente;
		}
	}
	
public:
	Cola() : frente(nullptr), final(nullptr), cantidad(0) {}
		
	Cola(const Cola& otra) : frente(nullptr), final(nullptr), cantidad(0) {
		copiarDesde(otra);
	}
		
	Cola& operator=(const Cola& otra) {
		if (this == &otra) {
			return *this;
		}
		limpiar();
		copiarDesde(otra);
		return *this;
	}
		
	~Cola() {
		limpiar();
	}
		
	bool estaVacia() const {
		return frente == nullptr;
	}
		
	int tamano() const {
		return cantidad;
	}
		
	void encolar(const T& valor) {
		NodoCola* nuevo = new NodoCola(valor);
		if (estaVacia()) {
			frente = nuevo;
			final = nuevo;
		} else {
			final->siguiente = nuevo;
			final = nuevo;
		}
		cantidad++;
	}
		
	void desencolar() {
		if (estaVacia()) {
			return;
		}
		NodoCola* borrar = frente;
		frente = frente->siguiente;
		if (frente == nullptr) {
			final = nullptr;
		}
		delete borrar;
		cantidad--;
	}
		
	T& obtenerFrente() {
		return frente->dato;
	}
		
	const T& obtenerFrente() const {
		return frente->dato;
	}
		
	void limpiar() {
		while (!estaVacia()) {
			desencolar();
		}
	}
};

#endif
