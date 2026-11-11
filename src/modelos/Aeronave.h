#ifndef AERONAVE_H
#define AERONAVE_H

#include <string>

class Aeronave {
private:
	std::string placa;
	std::string marca;
	std::string modelo;
	int capacidad;
	std::string estado;

public:
	Aeronave();
	Aeronave(const std::string& placa, const std::string& marca, const std::string& modelo, int capacidad, const std::string& estado);

	const std::string& getPlaca() const;
	const std::string& getMarca() const;
	const std::string& getModelo() const;
	int getCapacidad() const;
	const std::string& getEstado() const;

	void setPlaca(const std::string& placa);
	void setMarca(const std::string& marca);
	void setModelo(const std::string& modelo);
	void setCapacidad(int capacidad);
	void setEstado(const std::string& estado);

	std::string aLineaArchivo() const;
	std::string aTexto() const;
};

#endif
