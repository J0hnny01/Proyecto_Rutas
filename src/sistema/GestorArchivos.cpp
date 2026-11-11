#include "GestorArchivos.h"
#include <sstream>

std::string GestorArchivos::limpiarTexto(const std::string& texto) {
	std::size_t inicio = 0;
	while (inicio < texto.size() && (texto[inicio] == ' ' || texto[inicio] == '\t' || texto[inicio] == '\r' || texto[inicio] == '\n')) {
		inicio++;
	}
	std::size_t fin = texto.size();
	while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\t' || texto[fin - 1] == '\r' || texto[fin - 1] == '\n')) {
		fin--;
	}
	return texto.substr(inicio, fin - inicio);
}

int GestorArchivos::dividirLinea(const std::string& linea, char delimitador, std::string* campos, int maxCampos) {
	if (campos == nullptr || maxCampos <= 0) {
		return 0;
	}
	int cantidad = 0;
	std::size_t inicio = 0;
	while (cantidad < maxCampos) {
		std::size_t pos = linea.find(delimitador, inicio);
		if (pos == std::string::npos) {
			campos[cantidad++] = limpiarTexto(linea.substr(inicio));
			break;
		}
		campos[cantidad++] = limpiarTexto(linea.substr(inicio, pos - inicio));
		inicio = pos + 1;
		if (inicio == linea.size() && cantidad < maxCampos) {
			campos[cantidad++] = "";
			break;
		}
	}
	return cantidad;
}

bool GestorArchivos::parsearEntero(const std::string& texto, int& valor) {
	std::string limpio = limpiarTexto(texto);
	if (limpio.empty()) {
		return false;
	}
	std::istringstream ss(limpio);
	char extra;
	if (!(ss >> valor) || (ss >> extra)) {
		return false;
	}
	return true;
}

bool GestorArchivos::parsearFlotante(const std::string& texto, float& valor) {
	std::string limpio = limpiarTexto(texto);
	if (limpio.empty()) {
		return false;
	}
	std::istringstream ss(limpio);
	char extra;
	if (!(ss >> valor) || (ss >> extra)) {
		return false;
	}
	return true;
}

void GestorArchivos::guardarPasajerosRec(std::ofstream& archivo, const NodoAVL* nodo) {
	if (nodo == nullptr) {
		return;
	}
	guardarPasajerosRec(archivo, nodo->izquierdo);
	const Pasajero& pasajero = nodo->dato;
	int totalVuelos = pasajero.getCantidadVuelos();
	if (totalVuelos == 0) {
		archivo << pasajero.getId() << ";"
			<< pasajero.getNombre() << ";"
			<< "SIN_VUELO;"
			<< pasajero.getCantidadMaletas() << "\n";
	} else {
		for (int i = 0; i < totalVuelos; i++) {
			archivo << pasajero.getId() << ";"
				<< pasajero.getNombre() << ";"
				<< pasajero.getVuelo(i) << ";"
				<< pasajero.getCantidadMaletas() << "\n";
		}
	}
	guardarPasajerosRec(archivo, nodo->derecho);
}

bool GestorArchivos::cargarCiudades(const std::string& rutaArchivo, ListaEnlazada<Ciudad>& ciudades, ArbolABB& arbolCiudades) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	ciudades.limpiar();
	arbolCiudades.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 4) {
			continue;
		}
		float x = 0.0f;
		float y = 0.0f;
		if (!parsearFlotante(campos[2], x) || !parsearFlotante(campos[3], y)) {
			continue;
		}
		int indice = ciudades.tamano();
		ciudades.insertarAlFinal(Ciudad(campos[0], campos[1], x, y));
		arbolCiudades.insertar(campos[0], campos[1], indice);
	}
	return true;
}

bool GestorArchivos::guardarCiudades(const std::string& rutaArchivo, const ListaEnlazada<Ciudad>& ciudades) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	const ListaEnlazada<Ciudad>::NodoLista* actual = ciudades.obtenerCabeza();
	while (actual != nullptr) {
		const Ciudad& ciudad = actual->dato;
		archivo << ciudad.obtenerCodigo() << ";"
			<< ciudad.obtenerNombre() << ";"
			<< ciudad.obtenerX() << ";"
			<< ciudad.obtenerY() << "\n";
		actual = actual->siguiente;
	}
	return true;
}

bool GestorArchivos::cargarRutas(const std::string& rutaArchivo, ListaEnlazada<Ruta>& rutas) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	rutas.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 4) {
			continue;
		}
		int costo = 0;
		int tiempoMin = 0;
		if (!parsearEntero(campos[2], costo) || !parsearEntero(campos[3], tiempoMin)) {
			continue;
		}
		rutas.insertarAlFinal(Ruta(campos[0], campos[1], costo, tiempoMin, true));
	}
	return true;
}

bool GestorArchivos::guardarRutas(const std::string& rutaArchivo, const ListaEnlazada<Ruta>& rutas) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	const ListaEnlazada<Ruta>::NodoLista* actual = rutas.obtenerCabeza();
	while (actual != nullptr) {
		const Ruta& ruta = actual->dato;
		if (ruta.estaActiva()) {
			archivo << ruta.obtenerOrigenCodigo() << ";"
				<< ruta.obtenerDestinoCodigo() << ";"
				<< ruta.obtenerCosto() << ";"
				<< ruta.obtenerTiempoMin() << "\n";
		}
		actual = actual->siguiente;
	}
	return true;
}

bool GestorArchivos::cargarAeronaves(const std::string& rutaArchivo, ListaEnlazada<Aeronave>& aeronaves) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	aeronaves.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 5) {
			continue;
		}
		int capacidad = 0;
		if (!parsearEntero(campos[3], capacidad)) {
			continue;
		}
		aeronaves.insertarAlFinal(Aeronave(campos[0], campos[1], campos[2], capacidad, campos[4]));
	}
	return true;
}

bool GestorArchivos::guardarAeronaves(const std::string& rutaArchivo, const ListaEnlazada<Aeronave>& aeronaves) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	const ListaEnlazada<Aeronave>::NodoLista* actual = aeronaves.obtenerCabeza();
	while (actual != nullptr) {
		archivo << actual->dato.aLineaArchivo() << "\n";
		actual = actual->siguiente;
	}
	return true;
}

bool GestorArchivos::cargarTripulantes(const std::string& rutaArchivo, ListaEnlazada<Tripulante>& tripulantes) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	tripulantes.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 4) {
			continue;
		}
		tripulantes.insertarAlFinal(Tripulante(campos[0], campos[1], campos[2], campos[3]));
	}
	return true;
}

bool GestorArchivos::guardarTripulantes(const std::string& rutaArchivo, const ListaEnlazada<Tripulante>& tripulantes) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	const ListaEnlazada<Tripulante>::NodoLista* actual = tripulantes.obtenerCabeza();
	while (actual != nullptr) {
		archivo << actual->dato.aLineaArchivo() << "\n";
		actual = actual->siguiente;
	}
	return true;
}

bool GestorArchivos::cargarVuelos(const std::string& rutaArchivo, ListaEnlazada<Vuelo>& vuelos) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	vuelos.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 6) {
			continue;
		}
		int costo = 0;
		int tiempoMin = 0;
		if (!parsearEntero(campos[4], costo) || !parsearEntero(campos[5], tiempoMin)) {
			continue;
		}
		vuelos.insertarAlFinal(Vuelo(campos[0], campos[1], campos[2], campos[3], costo, tiempoMin));
	}
	return true;
}

bool GestorArchivos::guardarVuelos(const std::string& rutaArchivo, const ListaEnlazada<Vuelo>& vuelos) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	const ListaEnlazada<Vuelo>::NodoLista* actual = vuelos.obtenerCabeza();
	while (actual != nullptr) {
		const Vuelo& vuelo = actual->dato;
		archivo << vuelo.obtenerCodigoVuelo() << ";"
			<< vuelo.obtenerOrigenCodigo() << ";"
			<< vuelo.obtenerDestinoCodigo() << ";"
			<< vuelo.obtenerPlacaAeronave() << ";"
			<< vuelo.obtenerCosto() << ";"
			<< vuelo.obtenerTiempoMin() << "\n";
		actual = actual->siguiente;
	}
	return true;
}

bool GestorArchivos::cargarPasajeros(const std::string& rutaArchivo, ArbolAVL& arbolPasajeros) {
	std::ifstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	arbolPasajeros.limpiar();

	std::string linea;
	std::string campos[8];
	while (std::getline(archivo, linea)) {
		linea = limpiarTexto(linea);
		if (linea.empty()) {
			continue;
		}
		if (dividirLinea(linea, ';', campos, 8) != 4) {
			continue;
		}
		int maletas = 0;
		if (!parsearEntero(campos[3], maletas)) {
			continue;
		}
		Pasajero pasajero(campos[0], campos[1], maletas);
		if (!campos[2].empty() && campos[2] != "SIN_VUELO") {
			pasajero.agregarVuelo(campos[2]);
		}
		arbolPasajeros.insertar(pasajero);
	}
	return true;
}

bool GestorArchivos::guardarPasajeros(const std::string& rutaArchivo, const ArbolAVL& arbolPasajeros) {
	std::ofstream archivo(rutaArchivo.c_str());
	if (!archivo.is_open()) {
		return false;
	}
	guardarPasajerosRec(archivo, arbolPasajeros.getRaiz());
	return true;
}
