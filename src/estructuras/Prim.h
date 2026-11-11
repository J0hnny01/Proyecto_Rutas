#ifndef PRIM_H
#define PRIM_H

#include "GrafoVuelos.h"
#include "ResultadosGrafo.h"
#include "../arboles/ArbolMST.h"

class Prim {
private:
	static int* construirMatrizPesos(const GrafoVuelos& grafo, int n, CriterioRuta criterio);

public:
	static ResultadoMST calcular(const GrafoVuelos& grafo, int indiceOrigen = 0, CriterioRuta criterio = CRITERIO_COSTO);
	static bool construirArbol(const GrafoVuelos& grafo, int indiceOrigen, ArbolMST& arbol, CriterioRuta criterio = CRITERIO_COSTO);
};

#endif
