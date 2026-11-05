#include "AplicacionVisual.h"
#include "CompatSFML.h"
#include <iostream>

AplicacionVisual::AplicacionVisual()
    : ventana(CompatSFML::crearVideoMode(1200, 720), "Sistema de Gestion de Aerolinea - EIF207 (C++ / SFML)"),
      moduloActual(ModuloActivo::Cliente),
      vistaActual(VistaLienzo::MapaGrafo) {
    ventana.setFramerateLimit(60);

    CompatSFML::cargarFuente(fuente, "assets/arial.ttf");

    inicializarInterfaz();
}

void AplicacionVisual::inicializarInterfaz() {
    btnTabCliente.configurar(365.0f, 15.0f, 220.0f, 38.0f, "Modulo del Cliente");
    btnTabAdmin.configurar(600.0f, 15.0f, 220.0f, 38.0f, "Modulo Administrativo");
    btnTabCliente.setActivo(true);
    btnTabAdmin.setActivo(false);

    btnOrigenAnt.configurar(250.0f, 110.0f, 35.0f, 28.0f, "<");
    btnOrigenSig.configurar(295.0f, 110.0f, 35.0f, 28.0f, ">");
    btnDestinoAnt.configurar(250.0f, 150.0f, 35.0f, 28.0f, "<");
    btnDestinoSig.configurar(295.0f, 150.0f, 35.0f, 28.0f, ">");

    const char* textosCliente[TOTAL_BOTONES_ACCION] = {
        "1. Ruta mas Rapida (Dijkstra)",
        "2. Ruta mas Barata (Dijkstra)",
        "3. Explorar Destinos (Bellman-Ford)",
        "4. Buscar Ciudad (Arbol ABB)",
        "5. Vuelos de Pasajero (Arbol AVL)",
        "6. Ver Mapa de Rutas (Grafo)"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesCliente[i].configurar(30.0f, 240.0f + i * 52.0f, 300.0f, 42.0f, textosCliente[i]);
    }

    const char* textosAdmin[TOTAL_BOTONES_ACCION] = {
        "1. Aeronaves y Tripulacion",
        "2. Registro Vuelos y Pasajeros",
        "3. Gestion de Maletas (Ruta Barata)",
        "4. Arbol Prim (MST por Costo)",
        "5. Arbol Kruskal (MST por Tiempo)",
        "6. Alternar Arboles (ABB / AVL)"
    };

    for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
        botonesAdmin[i].configurar(30.0f, 240.0f + i * 52.0f, 300.0f, 42.0f, textosAdmin[i]);
    }

    limpiarLineasResultado("Panel de Informacion");
}

void AplicacionVisual::limpiarLineasResultado(const std::string& nuevoTitulo) {
    tituloResultado = nuevoTitulo;
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        lineasResultado[i] = "";
    }
}

void AplicacionVisual::procesarEventos() {
#if SFML_VERSION_MAJOR >= 3
    while (const std::optional<sf::Event> evento = ventana.pollEvent()) {
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
            if (evento.mouseButton.button == sf::Mouse::Left) {
                manejarClicIzquierdo(mx, my);
            } else if (evento.mouseButton.button == sf::Mouse::Right) {
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
            limpiarLineasResultado("Ruta mas Rapida (Dijkstra)");
            break;
        case 1:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Ruta mas Barata (Dijkstra)");
            break;
        case 2:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Explorar Destinos (Bellman-Ford)");
            break;
        case 3:
            vistaActual = VistaLienzo::ArbolBinario;
            arbolVisual.limpiar("Arbol Binario de Busqueda (ABB)");
            limpiarLineasResultado("Buscar Ciudad (Arbol ABB)");
            break;
        case 4:
            vistaActual = VistaLienzo::ArbolBinario;
            arbolVisual.limpiar("Arbol AVL");
            limpiarLineasResultado("Vuelos de Pasajero (Arbol AVL)");
            break;
        case 5:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Mapa de Rutas (Grafo)");
            break;
    }
}

void AplicacionVisual::ejecutarAccionAdmin(int indiceAccion) {
    switch (indiceAccion) {
        case 0:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Aeronaves y Tripulacion");
            break;
        case 1:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Registro Vuelos y Pasajeros");
            break;
        case 2:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Gestion de Maletas");
            break;
        case 3:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Arbol Prim (MST por Costo)");
            break;
        case 4:
            vistaActual = VistaLienzo::MapaGrafo;
            limpiarLineasResultado("Arbol Kruskal (MST por Tiempo)");
            break;
        case 5:
            vistaActual = VistaLienzo::ArbolBinario;
            arbolVisual.limpiar("Visualizacion de Arboles");
            limpiarLineasResultado("Despliegue de Arboles");
            break;
    }
}

void AplicacionVisual::renderizar() {
    ventana.clear(sf::Color(18, 22, 34));

    sf::RectangleShape barraSuperior(sf::Vector2f(1200.0f, 60.0f));
    barraSuperior.setFillColor(sf::Color(28, 35, 52));
    ventana.draw(barraSuperior);

    CompatSFML::dibujarTexto(ventana, fuente, "SISTEMA DE AEROLINEA - EIF207", 25.0f, 18.0f, 20, sf::Color(120, 200, 255));
    btnTabCliente.renderizar(ventana, fuente);
    btnTabAdmin.renderizar(ventana, fuente);

    sf::RectangleShape panelIzquierdo(sf::Vector2f(330.0f, 635.0f));
    panelIzquierdo.setPosition(sf::Vector2f(15.0f, 70.0f));
    panelIzquierdo.setFillColor(sf::Color(26, 32, 48));
    panelIzquierdo.setOutlineColor(sf::Color(60, 75, 105));
    panelIzquierdo.setOutlineThickness(1.5f);
    ventana.draw(panelIzquierdo);

    CompatSFML::dibujarTexto(ventana, fuente, "Seleccion de Ruta:", 30.0f, 80.0f, 16, sf::Color(180, 205, 245));

    std::string txtOrigen = "Sin seleccionar";
    std::string txtDestino = "Sin seleccionar";
    if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadOrigen() >= 0) {
        const CiudadVisual& cOrig = grafoVisual.getCiudad(grafoVisual.getCiudadOrigen());
        txtOrigen = cOrig.codigo + " (" + cOrig.nombre + ")";
    }
    if (grafoVisual.getTotalCiudades() > 0 && grafoVisual.getCiudadDestino() >= 0) {
        const CiudadVisual& cDest = grafoVisual.getCiudad(grafoVisual.getCiudadDestino());
        txtDestino = cDest.codigo + " (" + cDest.nombre + ")";
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Origen:  " + txtOrigen,
                             30.0f, 114.0f, 14, sf::Color(110, 250, 150));
    btnOrigenAnt.renderizar(ventana, fuente);
    btnOrigenSig.renderizar(ventana, fuente);

    CompatSFML::dibujarTexto(ventana, fuente, "Destino: " + txtDestino,
                             30.0f, 154.0f, 14, sf::Color(255, 140, 140));
    btnDestinoAnt.renderizar(ventana, fuente);
    btnDestinoSig.renderizar(ventana, fuente);

    std::string subtituloModulo = (moduloActual == ModuloActivo::Cliente)
        ? "Herramientas del Cliente:"
        : "Herramientas Administrativas:";
    CompatSFML::dibujarTexto(ventana, fuente, subtituloModulo, 30.0f, 205.0f, 16, sf::Color(180, 205, 245));

    if (moduloActual == ModuloActivo::Cliente) {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesCliente[i].renderizar(ventana, fuente);
        }
    } else {
        for (int i = 0; i < TOTAL_BOTONES_ACCION; ++i) {
            botonesAdmin[i].renderizar(ventana, fuente);
        }
    }

    CompatSFML::dibujarTexto(ventana, fuente, "Leyenda del Mapa:", 30.0f, 570.0f, 14, sf::Color(180, 200, 230));
    CompatSFML::dibujarTexto(ventana, fuente, "* Verde: Ciudad Origen / Ruta Vuelo", 30.0f, 595.0f, 13, sf::Color(90, 235, 130));
    CompatSFML::dibujarTexto(ventana, fuente, "* Rojo: Ciudad Destino", 30.0f, 618.0f, 13, sf::Color(255, 130, 130));
    CompatSFML::dibujarTexto(ventana, fuente, "* Naranja: Ruta Equipaje (Mas barata)", 30.0f, 641.0f, 13, sf::Color(255, 175, 70));
    CompatSFML::dibujarTexto(ventana, fuente, "* Amarillo: Arbol Prim / Kruskal", 30.0f, 664.0f, 13, sf::Color(250, 215, 70));

    sf::RectangleShape panelLienzo(sf::Vector2f(820.0f, 440.0f));
    panelLienzo.setPosition(sf::Vector2f(365.0f, 70.0f));
    panelLienzo.setFillColor(sf::Color(22, 28, 42));
    panelLienzo.setOutlineColor(sf::Color(65, 85, 120));
    panelLienzo.setOutlineThickness(1.5f);
    ventana.draw(panelLienzo);

    if (vistaActual == VistaLienzo::MapaGrafo) {
        grafoVisual.renderizar(ventana, fuente);
    } else {
        arbolVisual.renderizar(ventana, fuente);
    }

    sf::RectangleShape panelResultados(sf::Vector2f(820.0f, 180.0f));
    panelResultados.setPosition(sf::Vector2f(365.0f, 525.0f));
    panelResultados.setFillColor(sf::Color(25, 31, 46));
    panelResultados.setOutlineColor(sf::Color(65, 85, 120));
    panelResultados.setOutlineThickness(1.5f);
    ventana.draw(panelResultados);

    CompatSFML::dibujarTexto(ventana, fuente, tituloResultado, 380.0f, 535.0f, 16, sf::Color(125, 210, 255));
    for (int i = 0; i < MAX_LINEAS_INFO; ++i) {
        if (!lineasResultado[i].empty()) {
            CompatSFML::dibujarTexto(ventana, fuente, lineasResultado[i],
                                     380.0f, 565.0f + i * 22.0f, 13, sf::Color(220, 230, 245));
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
