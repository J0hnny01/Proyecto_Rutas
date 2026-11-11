#include "Aeronave.h"

Aeronave::Aeronave() : placa(""), marca(""), modelo(""), capacidad(0), estado("Activa") {}

Aeronave::Aeronave(const std::string& placa, const std::string& marca, const std::string& modelo, int capacidad, const std::string& estado)
	: placa(placa), marca(marca), modelo(modelo), capacidad(capacidad < 0 ? 0 : capacidad), estado(estado) {}

const std::string& Aeronave::getPlaca() const {
	return placa;
}

const std::string& Aeronave::getMarca() const {
	return marca;
}

const std::string& Aeronave::getModelo() const {
	return modelo;
}

int Aeronave::getCapacidad() const {
	return capacidad;
}

const std::string& Aeronave::getEstado() const {
	return estado;
}

void Aeronave::setPlaca(const std::string& placa) {
	this->placa = placa;
}

void Aeronave::setMarca(const std::string& marca) {
	this->marca = marca;
}

void Aeronave::setModelo(const std::string& modelo) {
	this->modelo = modelo;
}

void Aeronave::setCapacidad(int capacidad) {
	this->capacidad = (capacidad < 0) ? 0 : capacidad;
}

void Aeronave::setEstado(const std::string& estado) {
	this->estado = estado;
}

std::string Aeronave::aLineaArchivo() const {
	return placa + ";" + marca + ";" + modelo + ";" + std::to_string(capacidad) + ";" + estado;
}

std::string Aeronave::aTexto() const {
	return placa + " - " + marca + " " + modelo + " (" + std::to_string(capacidad) + " pax, " + estado + ")";
}
