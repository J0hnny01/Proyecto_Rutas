#include "Ordenamiento.h"

void Ordenamiento::merge(AristaOrdenable* arr, AristaOrdenable* aux, int inicio, int medio, int fin) {
	for (int idx = inicio; idx <= fin; idx++) {
		aux[idx] = arr[idx];
	}
	int i = inicio;
	int j = medio + 1;
	int k = inicio;
	while (i <= medio && j <= fin) {
		if (aux[i].peso <= aux[j].peso) {
			arr[k] = aux[i];
			i++;
		} else {
			arr[k] = aux[j];
			j++;
		}
		k++;
	}
	while (i <= medio) {
		arr[k] = aux[i];
		i++;
		k++;
	}
	while (j <= fin) {
		arr[k] = aux[j];
		j++;
		k++;
	}
}

void Ordenamiento::mergeSort(AristaOrdenable* arr, AristaOrdenable* aux, int inicio, int fin) {
	if (inicio >= fin) {
		return;
	}
	int medio = inicio + (fin - inicio) / 2;
	mergeSort(arr, aux, inicio, medio);
	mergeSort(arr, aux, medio + 1, fin);
	merge(arr, aux, inicio, medio, fin);
}

void Ordenamiento::ordenarAristas(AristaOrdenable* aristas, int cantidad) {
	if (aristas == nullptr || cantidad <= 1) {
		return;
	}
	AristaOrdenable* aux = new AristaOrdenable[cantidad];
	mergeSort(aristas, aux, 0, cantidad - 1);
	delete[] aux;
}
