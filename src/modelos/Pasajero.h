#ifndef PASAJERO_H
#define PASAJERO_H

#include <string>

class Pasajero {
private:
	std::string id;
	std::string nombre;
	std::string* codigosVuelos;
	int cantidadVuelos;
	int capacidadVuelos;
	int cantidadMaletas;

	void redimensionar(int nuevaCapacidad);
	void copiarDesde(const Pasajero& otro);
	void liberarVuelos();

public:
	Pasajero();
	Pasajero(const std::string& id, const std::string& nombre, int cantidadMaletas);
	Pasajero(const Pasajero& otro);
	Pasajero& operator=(const Pasajero& otro);
	~Pasajero();

	const std::string& getId() const;
	const std::string& getNombre() const;
	int getCantidadVuelos() const;
	int getCantidadMaletas() const;
	std::string getVuelo(int indice) const;
	std::string getVuelosTexto() const;

	void setId(const std::string& id);
	void setNombre(const std::string& nombre);
	void setCantidadMaletas(int cantidadMaletas);

	bool agregarVuelo(const std::string& codigoVuelo);
	bool eliminarVuelo(const std::string& codigoVuelo);
	bool tieneVuelo(const std::string& codigoVuelo) const;
};

#endif
