#ifndef TRIPULANTE_H
#define TRIPULANTE_H

#include <string>

class Tripulante {
private:
	std::string id;
	std::string nombre;
	std::string rol;
	std::string placaAeronave;

public:
	Tripulante();
	Tripulante(const std::string& id, const std::string& nombre, const std::string& rol, const std::string& placaAeronave);

	const std::string& getId() const;
	const std::string& getNombre() const;
	const std::string& getRol() const;
	const std::string& getPlacaAeronave() const;

	void setId(const std::string& id);
	void setNombre(const std::string& nombre);
	void setRol(const std::string& rol);
	void setPlacaAeronave(const std::string& placaAeronave);

	std::string aLineaArchivo() const;
	std::string aTexto() const;
};

#endif
