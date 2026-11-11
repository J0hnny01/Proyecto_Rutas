#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <fstream>
#include <string>

#include "../estructuras/ListaEnlazada.h"
#include "../arboles/ArbolABB.h"
#include "../arboles/ArbolAVL.h"
#include "../modelos/Ciudad.h"
#include "../modelos/Ruta.h"
#include "../modelos/Aeronave.h"
#include "../modelos/Tripulante.h"
#include "../modelos/Vuelo.h"

class GestorArchivos {
private:
	static std::string limpiarTexto(const std::string& texto);
	static int dividirLinea(const std::string& linea, char delimitador, std::string* campos, int maxCampos);
	static bool parsearEntero(const std::string& texto, int& valor);
	static bool parsearFlotante(const std::string& texto, float& valor);
	static void guardarPasajerosRec(std::ofstream& archivo, const NodoAVL* nodo);

public:
	static bool cargarCiudades(const std::string& rutaArchivo, ListaEnlazada<Ciudad>& ciudades, ArbolABB& arbolCiudades);
	static bool guardarCiudades(const std::string& rutaArchivo, const ListaEnlazada<Ciudad>& ciudades);

	static bool cargarRutas(const std::string& rutaArchivo, ListaEnlazada<Ruta>& rutas);
	static bool guardarRutas(const std::string& rutaArchivo, const ListaEnlazada<Ruta>& rutas);

	static bool cargarAeronaves(const std::string& rutaArchivo, ListaEnlazada<Aeronave>& aeronaves);
	static bool guardarAeronaves(const std::string& rutaArchivo, const ListaEnlazada<Aeronave>& aeronaves);

	static bool cargarTripulantes(const std::string& rutaArchivo, ListaEnlazada<Tripulante>& tripulantes);
	static bool guardarTripulantes(const std::string& rutaArchivo, const ListaEnlazada<Tripulante>& tripulantes);

	static bool cargarVuelos(const std::string& rutaArchivo, ListaEnlazada<Vuelo>& vuelos);
	static bool guardarVuelos(const std::string& rutaArchivo, const ListaEnlazada<Vuelo>& vuelos);

	static bool cargarPasajeros(const std::string& rutaArchivo, ArbolAVL& arbolPasajeros);
	static bool guardarPasajeros(const std::string& rutaArchivo, const ArbolAVL& arbolPasajeros);
};

#endif
