#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "GrafoVuelos.h"
#include "ResultadosGrafo.h"
#include "Ordenamiento.h"
#include "../arboles/ArbolMST.h"

class Kruskal {
private:
	static AristaOrdenable* extraerAristas(const GrafoVuelos& grafo, int n, CriterioRuta criterio, int& totalAristas);

public:
	static ResultadoMST calcular(const GrafoVuelos& grafo, CriterioRuta criterio = CRITERIO_TIEMPO);
	static bool construirArbol(const GrafoVuelos& grafo, int indiceOrigen, ArbolMST& arbol, CriterioRuta criterio = CRITERIO_TIEMPO);
};

#endif
