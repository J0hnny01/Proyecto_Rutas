#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

struct AristaOrdenable {
	int origen;
	int destino;
	int peso;
};

class Ordenamiento {
private:
	static void merge(AristaOrdenable* arr, AristaOrdenable* aux, int inicio, int medio, int fin);
	static void mergeSort(AristaOrdenable* arr, AristaOrdenable* aux, int inicio, int fin);

public:
	static void ordenarAristas(AristaOrdenable* aristas, int cantidad);
};

#endif
