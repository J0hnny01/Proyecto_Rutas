#include "Pasajero.h"

void Pasajero::redimensionar(int nuevaCapacidad) {
	std::string* nuevo = new std::string[nuevaCapacidad];
	for (int i = 0; i < cantidadVuelos; i++) {
		nuevo[i] = codigosVuelos[i];
	}
	delete[] codigosVuelos;
	codigosVuelos = nuevo;
	capacidadVuelos = nuevaCapacidad;
}

void Pasajero::copiarDesde(const Pasajero& otro) {
	id = otro.id;
	nombre = otro.nombre;
	cantidadMaletas = otro.cantidadMaletas;
	cantidadVuelos = otro.cantidadVuelos;
	capacidadVuelos = otro.capacidadVuelos;
	if (capacidadVuelos > 0 && otro.codigosVuelos != nullptr) {
		codigosVuelos = new std::string[capacidadVuelos];
		for (int i = 0; i < cantidadVuelos; i++) {
			codigosVuelos[i] = otro.codigosVuelos[i];
		}
	} else {
		codigosVuelos = nullptr;
		cantidadVuelos = 0;
		capacidadVuelos = 0;
	}
}

void Pasajero::liberarVuelos() {
	delete[] codigosVuelos;
	codigosVuelos = nullptr;
	cantidadVuelos = 0;
	capacidadVuelos = 0;
}

Pasajero::Pasajero()
	: id(""), nombre(""), codigosVuelos(nullptr), cantidadVuelos(0), capacidadVuelos(0), cantidadMaletas(0) {}

Pasajero::Pasajero(const std::string& id, const std::string& nombre, int cantidadMaletas)
	: id(id), nombre(nombre), codigosVuelos(nullptr), cantidadVuelos(0), capacidadVuelos(0),
	  cantidadMaletas(cantidadMaletas < 0 ? 0 : cantidadMaletas) {}

Pasajero::Pasajero(const Pasajero& otro)
	: id(""), nombre(""), codigosVuelos(nullptr), cantidadVuelos(0), capacidadVuelos(0), cantidadMaletas(0) {
	copiarDesde(otro);
}

Pasajero& Pasajero::operator=(const Pasajero& otro) {
	if (this == &otro) {
		return *this;
	}
	liberarVuelos();
	copiarDesde(otro);
	return *this;
}

Pasajero::~Pasajero() {
	liberarVuelos();
}

const std::string& Pasajero::getId() const {
	return id;
}

const std::string& Pasajero::getNombre() const {
	return nombre;
}

int Pasajero::getCantidadVuelos() const {
	return cantidadVuelos;
}

int Pasajero::getCantidadMaletas() const {
	return cantidadMaletas;
}

std::string Pasajero::getVuelo(int indice) const {
	if (indice < 0 || indice >= cantidadVuelos) {
		return "";
	}
	return codigosVuelos[indice];
}

std::string Pasajero::getVuelosTexto() const {
	if (cantidadVuelos == 0) {
		return "Sin vuelos";
	}
	std::string resultado = "";
	for (int i = 0; i < cantidadVuelos; i++) {
		if (i > 0) {
			resultado += ", ";
		}
		resultado += codigosVuelos[i];
	}
	return resultado;
}

void Pasajero::setId(const std::string& id) {
	this->id = id;
}

void Pasajero::setNombre(const std::string& nombre) {
	this->nombre = nombre;
}

void Pasajero::setCantidadMaletas(int cantidadMaletas) {
	this->cantidadMaletas = (cantidadMaletas < 0) ? 0 : cantidadMaletas;
}

bool Pasajero::agregarVuelo(const std::string& codigoVuelo) {
	if (codigoVuelo.empty() || tieneVuelo(codigoVuelo)) {
		return false;
	}
	if (cantidadVuelos == capacidadVuelos) {
		int nuevaCapacidad = (capacidadVuelos == 0) ? 2 : capacidadVuelos * 2;
		redimensionar(nuevaCapacidad);
	}
	codigosVuelos[cantidadVuelos] = codigoVuelo;
	cantidadVuelos++;
	return true;
}

bool Pasajero::eliminarVuelo(const std::string& codigoVuelo) {
	int pos = -1;
	for (int i = 0; i < cantidadVuelos; i++) {
		if (codigosVuelos[i] == codigoVuelo) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		return false;
	}
	for (int i = pos; i < cantidadVuelos - 1; i++) {
		codigosVuelos[i] = codigosVuelos[i + 1];
	}
	codigosVuelos[cantidadVuelos - 1] = "";
	cantidadVuelos--;
	return true;
}

bool Pasajero::tieneVuelo(const std::string& codigoVuelo) const {
	for (int i = 0; i < cantidadVuelos; i++) {
		if (codigosVuelos[i] == codigoVuelo) {
			return true;
		}
	}
	return false;
}
