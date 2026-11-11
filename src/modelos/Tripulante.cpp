#include "Tripulante.h"

Tripulante::Tripulante() : id(""), nombre(""), rol(""), placaAeronave("") {}

Tripulante::Tripulante(const std::string& id, const std::string& nombre, const std::string& rol, const std::string& placaAeronave)
	: id(id), nombre(nombre), rol(rol), placaAeronave(placaAeronave) {}

const std::string& Tripulante::getId() const {
	return id;
}

const std::string& Tripulante::getNombre() const {
	return nombre;
}

const std::string& Tripulante::getRol() const {
	return rol;
}

const std::string& Tripulante::getPlacaAeronave() const {
	return placaAeronave;
}

void Tripulante::setId(const std::string& id) {
	this->id = id;
}

void Tripulante::setNombre(const std::string& nombre) {
	this->nombre = nombre;
}

void Tripulante::setRol(const std::string& rol) {
	this->rol = rol;
}

void Tripulante::setPlacaAeronave(const std::string& placaAeronave) {
	this->placaAeronave = placaAeronave;
}

std::string Tripulante::aLineaArchivo() const {
	return id + ";" + nombre + ";" + rol + ";" + placaAeronave;
}

std::string Tripulante::aTexto() const {
	return id + " - " + nombre + " (" + rol + ", " + placaAeronave + ")";
}
