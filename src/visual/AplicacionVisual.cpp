#include "AplicacionVisual.h"
#include "CompatSFML.h"

AplicacionVisual::AplicacionVisual()
    : ventana(CompatSFML::crearVideoMode(1200, 720), "Aerolinea EIF207"),
      moduloActual(ModuloActivo::Cliente),
      vistaActual(VistaLienzo::MapaGrafo) {
    ventana.setFramerateLimit(60);
    CompatSFML::cargarFuente(fuente, "assets/arial.ttf");
    inicializarInterfaz();
}

void AplicacionVisual::inicializarInterfaz() {
    btnTabCliente.configurar(910.0f, 10.0f, 125.0f, 32.0f, "Cliente", true, 13);
    btnTabAdmin.configurar(1045.0f, 10.0f, 135.0f, 32.0f, "Administrativo", true, 13);
    btnTabCliente.setActivo(true);
    btnTabAdmin.setActivo(false);

    btnOrigenAnt.configurar(205.0f, 70.0f, 24.0f, 24.0f, "<", true, 13);
    btnOrigenSig.configurar(233.0f, 70.0f, 24.0f, 24.0f, ">", true, 13);
    btnDestinoAnt.configurar(205.0f, 104.0f, 24.0f, 24.0f, "<", true, 13);
    btnDestinoSig.configurar(233.0f, 104.0f, 24.0f, 24.0f, ">", true, 13);

    const char* textosCliente[TOTAL_BOTONES_ACCION] = {
        "Ruta mas rapida",
        "Ruta mas barata",
        "Explorar destinos",
        "Buscar ciudad (ABB)",
        "Vuelos pasajero (AVL)",
        "Mapa de rutas"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesCliente[i].configurar(18.0f, 178.0f + i * 44.0f, 239.0f, 36.0f, textosCliente[i], false, 13);
    }

    const char* textosAdmin[TOTAL_BOTONES_ACCION] = {
        "Aeronaves y tripulacion",
        "Vuelos y pasajeros",
        "Ruta de equipaje",
        "Prim (MST costo)",
        "Kruskal (MST tiempo)",
        "Ver arboles (ABB/AVL/MST)"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesAdmin[i].configurar(18.0f, 178.0f + i * 44.0f, 239.0f, 36.0f, textosAdmin[i], false, 13);
    }

    limpiarLineasResultado("Resultados");
}

void AplicacionVisual::limpiarLineasResultado(const string& nuevoTitulo) {
    tituloResultado = nuevoTitulo;
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        lineasResultado[i] = "";
    }
}

void AplicacionVisual::procesarEventos() {
#if SFML_VERSION_MAJOR >= 3
    while (const optional<sf::Event> evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        } else if (const auto* botonMouse = evento->getIf<sf::Event::MouseButtonPressed>()) {
            float mx = static_cast<float>(botonMouse->position.x);
            float my = static_cast<float>(botonMouse->position.y);
            if (botonMouse->button == sf::Mouse::Button::Left) {
                manejarClicIzquierdo(mx, my);
            } else if (botonMouse->button == sf::Mouse::Button::Right) {
                manejarClicDerecho(mx, my);
            }
        }
    }
#else
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        } else if (evento.type == sf::Event::MouseButtonPressed) {
            float mx = static_cast<float>(evento.mouseButton.x);
            float my = static_cast<float>(evento.mouseButton.y);
            if (evento.mouseButton.button == sf::Mouse::Button::Left) {
                manejarClicIzquierdo(mx, my);
            } else if (evento.mouseButton.button == sf::Mouse::Button::Right) {
                manejarClicDerecho(mx, my);
            }
        }
    }
#endif
}

void AplicacionVisual::manejarClicIzquierdo(float mx, float my) {
    if (btnTabCliente.contienePunto(mx, my)) {
        moduloActual = ModuloActivo::Cliente;
        btnTabCliente.setActivo(true);
        btnTabAdmin.setActivo(false);
        return;
    }
    if (btnTabAdmin.contienePunto(mx, my)) {
        moduloActual = ModuloActivo::Administrativo;
        btnTabCliente.setActivo(false);
        btnTabAdmin.setActivo(true);
        return;
    }

    int totalC = grafoVisual.getTotalCiudades();
    if (totalC > 0) {
        if (btnOrigenAnt.contienePunto(mx, my)) {
            grafoVisual.setCiudadOrigen((grafoVisual.getCiudadOrigen() - 1 + totalC) % totalC);
            return;
        }
        if (btnOrigenSig.contienePunto(mx, my)) {
            grafoVisual.setCiudadOrigen((grafoVisual.getCiudadOrigen() + 1) % totalC);
            return;
        }
        if (btnDestinoAnt.contienePunto(mx, my)) {
            grafoVisual.setCiudadDestino((grafoVisual.getCiudadDestino() - 1 + totalC) % totalC);
            return;
        }
        if (btnDestinoSig.contienePunto(mx, my)) {
            grafoVisual.setCiudadDestino((grafoVisual.getCiudadDestino() + 1) % totalC);
            return;
        }
    }

    if (vistaActual == VistaLienzo::MapaGrafo) {
        int ciudadClic = grafoVisual.detectarCiudadClic(mx, my);
        if (ciudadClic != -1) {
            grafoVisual.setCiudadOrigen(ciudadClic);
            return;
        }
    }

    if (moduloActual == ModuloActivo::Cliente) {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            if (botonesCliente[i].contienePunto(mx, my)) {
                for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) botonesCliente[j].setActivo(i == j);
                ejecutarAccionCliente(i);
                return;
            }
        }
    } else {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            if (botonesAdmin[i].contienePunto(mx, my)) {
                for (int j = 0; j < TOTAL_BOTONES_ACCION; ++j) botonesAdmin[j].setActivo(i == j);
                ejecutarAccionAdmin(i);
                return;
            }
        }
    }
}

void AplicacionVisual::manejarClicDerecho(float mx, float my) {
    if (vistaActual == VistaLienzo::MapaGrafo) {
        int ciudadClic = grafoVisual.detectarCiudadClic(mx, my);
        if (ciudadClic != -1) {
            grafoVisual.setCiudadDestino(ciudadClic);
        }
    }
}

void AplicacionVisual::ejecutarAccionCliente(int indiceAccion) {
    switch (indiceAccion) {
        case 0:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Ruta mas rapida (Dijkstra)");
            break;
        case 1:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Ruta mas barata (Dijkstra)");
            break;
        case 2:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Destinos alcanzables (Bellman-Ford)");
            break;
        case 3:
            vistaActual = VistaLienzo::ArbolEstructura;
            arbolVisual.limpiar("Arbol Binario de Busqueda (ABB)");
            limpiarLineasResultado("Busqueda de ciudad (ABB)");
            break;
        case 4:
            vistaActual = VistaLienzo::ArbolEstructura;
            arbolVisual.limpiar("Arbol AVL de Pasajeros");
            limpiarLineasResultado("Asignacion de pasajero (AVL)");
            break;
        case 5:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Mapa de rutas");
            break;
    }
}

void AplicacionVisual::ejecutarAccionAdmin(int indiceAccion) {
    switch (indiceAccion) {
        case 0:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Aeronaves y tripulacion");
            break;
        case 1:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Vuelos y pasajeros");
            break;
        case 2:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Ruta de equipaje (menor costo)");
            break;
        case 3:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Prim (MST por costo)");
            break;
        case 4:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Kruskal (MST por duracion)");
            break;
        case 5:
            vistaActual = VistaLienzo::ArbolEstructura;
            arbolVisual.limpiar("Despliegue de arbol");
            limpiarLineasResultado("Recorridos del arbol");
            break;
    }
}

void AplicacionVisual::renderizar() {
    ventana.clear(sf::Color(23, 26, 31));

    sf::RectangleShape barraSuperior(sf::Vector2f(1200.0f, 52.0f));
    barraSuperior.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(barraSuperior);

    sf::RectangleShape panelIzquierdo(sf::Vector2f(275.0f, 668.0f));
    panelIzquierdo.setPosition(sf::Vector2f(0.0f, 52.0f));
    panelIzquierdo.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(panelIzquierdo);

    sf::RectangleShape panelResultados(sf::Vector2f(925.0f, 125.0f));
    panelResultados.setPosition(sf::Vector2f(275.0f, 595.0f));
    panelResultados.setFillColor(sf::Color(27, 30, 36));
    ventana.draw(panelResultados);

    sf::RectangleShape lineaTop(sf::Vector2f(1200.0f, 1.0f));
    lineaTop.setPosition(sf::Vector2f(0.0f, 52.0f));
    lineaTop.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaTop);

    sf::RectangleShape lineaLateral(sf::Vector2f(1.0f, 668.0f));
    lineaLateral.setPosition(sf::Vector2f(275.0f, 52.0f));
    lineaLateral.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaLateral);

    sf::RectangleShape lineaResultados(sf::Vector2f(925.0f, 1.0f));
    lineaResultados.setPosition(sf::Vector2f(275.0f, 595.0f));
    lineaResultados.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(lineaResultados);

    CompatSFML::dibujarTexto(ventana, fuente, "Aerolinea EIF207", 18.0f, 15.0f, 17, sf::Color(235, 238, 244));
    btnTabCliente.renderizar(ventana, fuente);
    btnTabAdmin.renderizar(ventana, fuente);

    string txtOrigen = "-";
    string txtDestino = "-";
    if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadOrigen() >= 0) {
        txtOrigen = grafoVisual.getCiudad(grafoVisual.getCiudadOrigen()).codigo;
    }
    if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadDestino() >= 0) {
        txtDestino = grafoVisual.getCiudad(grafoVisual.getCiudadDestino()).codigo;
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Origen:  " + txtOrigen, 20.0f, 74.0f, 13, sf::Color(110, 215, 155));
    btnOrigenAnt.renderizar(ventana, fuente);
    btnOrigenSig.renderizar(ventana, fuente);

    CompatSFML::dibujarTexto(ventana, fuente, "Destino: " + txtDestino, 20.0f, 108.0f, 13, sf::Color(235, 120, 120));
    btnDestinoAnt.renderizar(ventana, fuente);
    btnDestinoSig.renderizar(ventana, fuente);

    sf::RectangleShape sepMenu(sf::Vector2f(239.0f, 1.0f));
    sepMenu.setPosition(sf::Vector2f(18.0f, 146.0f));
    sepMenu.setFillColor(sf::Color(46, 51, 60));
    ventana.draw(sepMenu);

    string subtitulo = (moduloActual == ModuloActivo::Cliente) ? "Cliente" : "Administrativo";
    CompatSFML::dibujarTexto(ventana, fuente, subtitulo, 20.0f, 154.0f, 12, sf::Color(145, 153, 168));

    if (moduloActual == ModuloActivo::Cliente) {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesCliente[i].renderizar(ventana, fuente);
        }
    } else {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].renderizar(ventana, fuente);
        }
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Clic izq: origen  |  Clic der: destino",
                             20.0f, 686.0f, 11, sf::Color(115, 123, 138));

    if (vistaActual == VistaLienzo::MapaGrafo) {
        grafoVisual.renderizar(ventana, fuente);
    } else {
        arbolVisual.renderizar(ventana, fuente);
    }

    CompatSFML::dibujarTexto(ventana, fuente, tituloResultado, 298.0f, 606.0f, 14, sf::Color(130, 180, 245));
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        if (!lineasResultado[i].empty()) {
            CompatSFML::dibujarTexto(ventana, fuente, lineasResultado[i],
                                     298.0f, 632.0f + i * 20.0f, 13, sf::Color(208, 214, 224));
        }
    }

    ventana.display();
}

void AplicacionVisual::ejecutar() {
    while (ventana.isOpen()) {
        float dt = relojDelta.restart().asSeconds();
        procesarEventos();
        grafoVisual.actualizarAnimacion(dt);
        renderizar();
    }
}
