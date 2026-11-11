# Desglose Punto por Punto del Enunciado y Definición de Estructuras
**Curso:** EIF207 — Estructuras de Datos (Universidad Nacional de Costa Rica, Sede Regional Brunca)  
**Proyecto:** Proyecto II (15% de la nota final del curso)  
**Documento base:** `Proyecto II de Estructuras de Datos.pdf`

---

## Parte I: Desglose Punto por Punto del Enunciado

### 1. Descripción General (Pág. 1–2)
* **Objetivo:** Implementar una plataforma de gestión de una aerolínea en **C++**.
* **Entorno base:** ZinjaI (acuerdo de cátedra: modalidad **en parejas**, con un integrante en ZinjaI y el otro en VS Code / Code OSS con autorización docente, manteniendo compatibilidad mediante [`Proyecto_Rutas.zpr`](Proyecto_Rutas.zpr)).
* **Regla de implementación:** Utilizar exclusivamente estructuras de datos estudiadas en el curso (TADs, listas, pilas, colas, algoritmos de ordenamiento, grafos y árboles) y algoritmos vistos en clase.
* **Arquitectura funcional en 2 áreas principales + estructuras compartidas:**
  1. **Módulo del Cliente:** consultar rutas, encontrar vuelos económicos o rápidos, explorar destinos y planificar viajes con escalas.
  2. **Módulo Administrativo:** gestionar vuelos, aeronaves, tripulación, pasajeros y maletas, además de construir y recorrer los árboles solicitados.
  3. **Estructuras de datos compartidas:** Listas enlazadas, Pilas, Colas, Grafos (lista de adyacencia elegida), Árboles y Algoritmos de ordenamiento.

---

### 2. Requisitos del Módulo del Cliente (Pág. 2–3)
1. **Búsqueda de ruta óptima punto a punto (Dijkstra):**
   * El cliente define la ciudad de origen donde se encuentra y selecciona la ciudad destino.
   * Puede escoger el criterio de optimización: **por tiempo** o **por costo**.
   * El algoritmo de **Dijkstra** debe recomendar el vuelo y mostrar **todo el recorrido**, indicando el tiempo total o costo total según corresponda y **todas las escalas** que debe realizar.
2. **Exploración de todos los destinos posibles (Bellman-Ford):**
   * Pensado para un cliente que planifica su viaje y **no sabe a dónde ir**.
   * Desde la ciudad de origen dada, calcula la ruta más corta hacia **todas las demás ciudades** donde la aerolínea tiene vuelos.
   * Utiliza el algoritmo de **Bellman-Ford**, permitiendo elegir si el criterio es **tiempo** o **costo**, y muestra **todas las opciones posibles** alcanzables.
3. **Búsqueda y catálogo de ciudades (Árbol Binario de Búsqueda - ABB):**
   * Un **ABB** almacena el nombre de todas las ciudades visitadas por la aerolínea.
   * Debe permitir **mostrar el árbol completo** e integrar un **método de búsqueda** para verificar si una ciudad en particular está en la ruta de la aerolínea.
4. **Selección de vuelo y verificación de asignación de pasajero (Árbol AVL):**
   * Una vez definido el viaje, el cliente selecciona el vuelo.
   * Un **Árbol AVL** permite consultar rápidamente en qué vuelos está asignado un cliente (mostrar fácilmente si un cliente está asignado al vuelo o no).
   * Para administración debe contar con un **despliegue total del árbol** (mostrando el estado balanceado).

---

### 3. Requisitos del Módulo Administrativo (Pág. 3)
1. **Control total de vuelos (CRUD y asociaciones):**
   * **Aeronave:** Registro completo con detalle de placas, capacidad, marca, modelo, estado, etc.
   * **Ruta:** Registro de cada una de las rutas aéreas de las aeronaves.
   * **Tripulación:** Registro de la tripulación asignada a cada aeronave (piloto, copiloto, sobrecargos, etc.).
   * **Pasajeros:** Detalle de los pasajeros que han seleccionado cada vuelo.
   * **Maletas (Regla obligatoria de equipaje):** Las maletas **siempre viajan por la ruta más barata**. Para cada pasajero que elija un vuelo optimizado **por tiempo**, el sistema debe calcular y considerar explícitamente la ruta económica que llevará su equipaje (diferenciándola de la del pasajero cuando no coincidan).
2. **Árbol de Expansión Minimal por Costo (Prim):**
   * Construir con la información de los vuelos un árbol de expansión mínima usando el **algoritmo de Prim** ordenado por el **costo** del vuelo.
   * Debe mostrar el árbol de ciudades generado.
3. **Árbol de Expansión Minimal por Duración (Kruskal):**
   * Construir con la información de los vuelos un árbol de expansión mínima usando el **algoritmo de Kruskal** ordenado por la **duración** del vuelo.
   * Debe mostrar el árbol de ciudades generado.
4. **Recorridos de árboles:**
   * Implementar los métodos de recorrido de árboles (**Preorden, Inorden y Postorden**) y **utilizarlos en cada árbol generado** (ABB, AVL y árboles resultantes de Prim/Kruskal).

---

### 4. Diseño del Grafo de Vuelos (Sección 2, Pág. 3)
* **Importancia:** Es la estructura central del proyecto.
* **Vértices:** Cada vértice representa una ciudad o aeropuerto.
* **Aristas:** Cada arista representa una ruta aérea directa entre dos ciudades.
* **Pesos mínimos por arista:** Cada arista debe tener como mínimo un **costo** y una **duración**.
* **Direccionalidad:** Grafo dirigido. Si existen vuelos en ambos sentidos, se registran **dos aristas dirigidas independientes**; no se debe asumir que el vuelo de regreso tiene el mismo costo ni la misma duración que el de ida.
* **Representación elegida por el equipo:** **Lista de Adyacencia propia** (permitida explícitamente en la Sección 1 y en la Rúbrica B).

---

### 5. Objetivos de Aprendizaje (Sección 3, Pág. 4)
* Implementar y utilizar correctamente los árboles binarios de búsqueda (ABB y AVL).
* Implementar y utilizar correctamente la estructura de grafo seleccionada (en nuestro caso, lista de adyacencia propia; nota: saber justificar la elección frente a matriz de adyacencia en la defensa).
* Implementar y utilizar los algoritmos vistos en clase para recorrer árboles y manejar cada operación de inserción, búsqueda, eliminación y balanceo.

---

### 6. Restricciones Técnicas (Sección 4, Pág. 4) — *Incumplimiento = Nota 0*
* **Lenguaje y compilador:** C++ / ZinjaI (compilación compatible con `g++` en VS Code / Code OSS según permiso docente).
* **Estructuras STL PROHIBIDAS:**
  * `std::stack`
  * `std::queue`
  * `std::deque`
  * `std::list`
  * `std::priority_queue`
  * `std::sort`
* **Elementos STL PERMITIDOS:**
  * `std::string`
  * `std::vector` (exclusivamente para colecciones auxiliares no evaluadas, como leer líneas de un archivo; preferiblemente evitarlo también si se usan nuestras propias listas).
  * Biblioteca estándar para E/S (`iostream`, `fstream`, `sstream`) y manejo de tiempo (`chrono` / SFML `Clock`).
* **Persistencia acordada por el equipo:** Archivos de texto plano (`.txt`) delimitados dentro de la carpeta `data/` usando exclusivamente `std::ifstream` / `std::ofstream`.

---

### 7. Interfaz y Elementos Gráficos Obligatorios (Sección 5, Pág. 4)
* **Librería obligatoria:** **SFML** (prohibido entregar un proyecto únicamente de consola; equivale a nota 0).
* **Requisitos visuales:**
  * Dibujar las ciudades y las rutas de la aerolínea.
  * Incluir **animación en las rutas** (trayectoria del vuelo y diferenciación con la ruta del equipaje).
  * Visualizar resultados de los algoritmos (Dijkstra, Bellman-Ford, Prim, Kruskal) y el despliegue de los árboles (ABB y AVL).
* *Estado actual:* Ya contamos con el cascarón visual en `src/visual/` listo para conectar con la lógica.

---

### 8. Entregables (Sección 6 y 8, Pág. 4 y 8)
1. **Código fuente completo:** Organizado modularmente en archivos `.h` y `.cpp`.
2. **Archivo `README.md`:** Indicando cómo compilar, ejecutar el programa y la librería gráfica usada (SFML).
3. **Informe técnico corto (PDF o Word, máximo 6 páginas):**
   * Diagrama o descripción de cómo se relacionan todas las estructuras entre sí.
   * Complejidad teórica (**notación asintótica $O$**) de cada operación implementada.
   * Justificación de los algoritmos y estructuras utilizados.
4. **Archivo comprimido (`.zip`):** Agrupa código fuente, recursos (`assets/`, `data/`), `README.md` e informe técnico, con el formato de nombre indicado para la entrega.

---

### 9. Tabla de Calificación y Defensa (Sección 7, Pág. 5–8)
$$\text{Nota Final} = \text{Puntaje del Proyecto (0–100)} \times \text{Factor de Defensa (0.00–1.00)}$$

| Apartado | Rubro | Puntos |
| :--- | :--- | :---: |
| **A. Documentación y calidad (15)** | Informe de análisis (relaciones, complejidad $O$, justificaciones) | 6 |
| | Calidad del código (`.h`/`.cpp`, modularidad, legibilidad, memoria, `README`) | 9 |
| **B. Estructuras y algoritmos (55)** | Grafo de vuelos propio (ciudades, conexiones, costos y tiempos) | 10 |
| | Algoritmo de Dijkstra (económica y rápida, recorrido, escalas y total) | 10 |
| | Algoritmo de Bellman-Ford (origen hacia todos los destinos alcanzables) | 8 |
| | Árbol Binario de Búsqueda (ABB) (inserción, búsqueda, visualización, recorridos) | 6 |
| | Árbol AVL (pasajeros asignados, balanceo, rotaciones, visualización) | 9 |
| | Algoritmo de Prim (MST por costo) | 6 |
| | Algoritmo de Kruskal (MST por duración) | 6 |
| **C. Funcionamiento (30)** | Módulo administrativo (aeronaves, vuelos, rutas, tripulación, pasajeros) | 10 |
| | Módulo del cliente (destinos, vuelos, alternativas) | 7 |
| | Gestión de equipaje (ruta económica diferenciada de la ruta por tiempo) | 5 |
| | Interfaz gráfica SFML (ciudades, rutas, algoritmos, árboles, animación) | 8 |
| **SUBTOTAL PROYECTO** | | **100** |

* **Defensa Oral (Apartado D — Factor Multiplicador 0.00–1.00):**
  * **D1. Dominio técnico (30%):** Explicación interna de grafos, árboles, algoritmos y estructuras auxiliares.
  * **D2. Justificación de decisiones (25%):** Por qué cada estructura, Dijkstra vs Bellman-Ford, Prim vs Kruskal, complejidad algorítmica.
  * **D3. Respuestas imprevistas (25%):** Modificar/depurar en vivo (agregar ciudades, cambiar costos, eliminar rutas, probar inserciones en árboles).
  * **D4. Comunicación (20%):** Claridad, terminología técnica y demostración del aplicativo.
  * *Advertencia crítica:* No saber explicar cualquier componente del entregable limita el factor de defensa a un máximo de **0.50**.

---
---

## Parte II: Definición de las Estructuras Necesarias para el Proyecto

> **Fuente primaria de referencia:** Todas las estructuras, nombres de punteros, convenciones de altura/balance y esquemas algorítmicos toman como primera fuente el material de cátedra del Prof. Pablo A. Venegas Elizondo en `/home/andresj21/Descargas/Anti-estructuras` (`Grafos.pdf`, `Arboles.pdf`, `Estructuras_y_Algoritmos_Avanzados.pdf`, `Estructuras_Asociativas_y_Busqueda.pdf` y `programas/video3_listas.cpp` a `video7_mst.cpp`).  
> En los programas didácticos de clase se usan ocasionalmente contenedores STL por brevedad (`std::stack`, `std::queue`, `std::vector`, `std::stable_sort`, `std::priority_queue`); en este proyecto se sustituyen obligatoriamente por los TADs propios de la Capa 1 para cumplir la Sección 4 del enunciado.

Toda estructura de datos evaluada se implementará desde cero dentro de `src/` organizada en **5 carpetas** (`estructuras/`, `arboles/`, `modelos/`, `sistema/` y `visual/`), manteniendo `.h` y `.cpp` juntos en la misma carpeta y respetando estrictamente la prohibición de contenedores STL.

### Capa 1: Estructuras Base y Auxiliares (`src/estructuras/`)

| Estructura | Archivos (`.h` / `.cpp`) | Diseño interno | Para qué se usa en el proyecto |
| :--- | :--- | :--- | :--- |
| **`ListaEnlazada<T>`** | `src/estructuras/ListaEnlazada.h` | `NodoLista` con `dato` y `siguiente`; punteros `cabeza`, `cola` y contador `cantidad` (plantilla header-only). | **Lista de adyacencia** del grafo (`insertarAlFinal` para conservar orden); listas de aeronaves, vuelos, tripulación, pasajeros, aristas del MST y rutas reconstruidas. |
| **`Pila`** | `src/estructuras/Pila.h`, `src/estructuras/Pila.cpp` | `NodoPila` enlazado con `int` (`tope` y `cantidad`; concreta en `int`). | Sustituye a `std::stack` de `video4_dfs.cpp`; invierte el camino de predecesores en **Dijkstra** y **Bellman-Ford** (de destino a origen $\rightarrow$ origen a destino). |
| **`Cola<T>`** | `src/estructuras/Cola.h` | `NodoCola` enlazado con punteros `frente` y `final` (plantilla header-only). | Sustituye a `std::queue` de `video5_bfs.cpp`; recorridos por niveles y BFS para conectividad/alcanzabilidad. |
| **`ColaPrioridad`** | `src/estructuras/ColaPrioridad.h`, `src/estructuras/ColaPrioridad.cpp` | **Min-Heap binario** concreto (`ElementoHeap{vertice, prioridad}`) sobre arreglo dinámico propio. | Sustituye a `std::priority_queue` citado en `Grafos.pdf`; selección del vértice mínimo en **Dijkstra** y de la arista mínima en **Prim** ($O((V+E)\log V)$). |
| **`ConjuntoDisjunto`** | `src/estructuras/ConjuntoDisjunto.h`, `src/estructuras/ConjuntoDisjunto.cpp` | Arreglo dinámico `representante[]` y `rango[]` con búsqueda de raíz `encontrar(int x)` (con compresión de caminos) y unión (`unir`), siguiendo `video7_mst.cpp` / `Grafos.pdf`. | Detección de ciclos durante la ejecución de **Kruskal** ($O(\alpha(V))$ amortizado). |
| **`Ordenamiento`** | `src/estructuras/Ordenamiento.h`, `src/estructuras/Ordenamiento.cpp` | Implementación propia de **MergeSort** estable sobre arreglos dinámicos sin usar `std::sort` ni `std::stable_sort`. | Sustituye a `std::stable_sort` de `video7_mst.cpp` para ordenar aristas por duración ascendente en **Kruskal**. |

---

### Capa 2: Modelos de Dominio y Persistencia (`src/modelos/`, `src/sistema/` y `data/`)

1. **`Ciudad` (`src/modelos/Ciudad.h`, `src/modelos/Ciudad.cpp`)**
   * **Atributos:** `std::string codigo` (ej. `"SJO"`), `std::string nombre` (ej. `"San Jose"`), `float x`, `float y` (posición en el mapa SFML).
2. **`Ruta` (`src/modelos/Ruta.h`, `src/modelos/Ruta.cpp`)**
   * **Atributos:** `std::string origenCodigo`, `std::string destinoCodigo`, `int costo`, `int tiempoMin`, `bool activa`.
3. **`Tripulante` (`src/modelos/Tripulante.h`, `src/modelos/Tripulante.cpp`)**
   * **Atributos:** `std::string id`, `std::string nombre`, `std::string rol`, `std::string placaAeronave`.
4. **`Aeronave` (`src/modelos/Aeronave.h`, `src/modelos/Aeronave.cpp`)**
   * **Atributos:** `std::string placa`, `std::string marca`, `std::string modelo`, `int capacidad`, `std::string estado`.
5. **`Equipaje` (`src/modelos/Equipaje.h`, `src/modelos/Equipaje.cpp`)**
   * **Atributos:** `std::string idPasajero`, `int cantidadMaletas`, `ListaEnlazada<std::string> rutaMasBarata`, `int costoEnvio`, `int tiempoEnvio`.
6. **`Pasajero` (`src/modelos/Pasajero.h`, `src/modelos/Pasajero.cpp`)**
   * **Atributos:** `std::string id` (clave en el árbol AVL), `std::string nombre`, arreglo dinámico `std::string* codigosVuelos` (`cantidadVuelos`, `capacidadVuelos`), `int cantidadMaletas`.
7. **`Vuelo` (`src/modelos/Vuelo.h`, `src/modelos/Vuelo.cpp`)**
   * **Atributos:** `std::string codigoVuelo`, `std::string origenCodigo`, `std::string destinoCodigo`, `std::string placaAeronave`, `int costo`, `int tiempoMin`.
8. **`GestorArchivos` (`src/sistema/GestorArchivos.h`, `src/sistema/GestorArchivos.cpp`)**
   * Lee y escribe con `std::ifstream` / `std::ofstream` y `std::stringstream` (separador `;`) los archivos en `data/`:
     * `data/ciudades.txt` $\rightarrow$ `codigo;nombre;x;y`
     * `data/rutas.txt` $\rightarrow$ `origen;destino;costo;tiempoMin`
     * `data/aeronaves.txt` $\rightarrow$ `placa;marca;modelo;capacidad;estado`
     * `data/tripulacion.txt` $\rightarrow$ `id;nombre;rol;placaAeronave`
     * `data/vuelos.txt` $\rightarrow$ `codigoVuelo;origen;destino;placaAeronave;costo;tiempoMin`
     * `data/pasajeros.txt` $\rightarrow$ `id;nombre;codigoVuelo;cantidadMaletas`

---

### Capa 3: Grafo de Vuelos y Algoritmos (`src/estructuras/`)

1. **Estructuras internas del Grafo (`src/estructuras/GrafoVuelos.h`, `src/estructuras/GrafoVuelos.cpp`, según `Grafos.pdf` y `video3_listas.cpp`):**
   * **`NodoArista`:** `int destino`, `int costo`, `int tiempoMin`, `NodoArista* siguiente` (con inserción al final `insertarAlFinal` para conservar orden y liberación explícita).
   * **`VerticeCiudad`:** `Ciudad ciudad`, `NodoArista* adyacentes` (Lista de Adyacencia propia basada en el patrón `GrafoLista` / `listas[v]`).
   * **`GrafoVuelos`:**
     * Arreglo dinámico propio de `VerticeCiudad` (`vertices`), `int numVertices`, `int capacidad`.
     * Operaciones: `agregarCiudad`, `eliminarCiudad`, `agregarRutaDirigida`, `eliminarRuta`, `modificarRuta`, `buscarIndicePorCodigo`, `buscarIndicePorNombre`.
2. **Estructuras de resultados (`src/estructuras/ResultadosGrafo.h`, `src/estructuras/ResultadosGrafo.cpp`, patrón `video6_caminos.cpp` y `video7_mst.cpp`):**
   * **`ResultadoRuta` (Dijkstra):** `bool encontrada`, `int costoTotal`, `int duracionTotal`, `int cantidadEscalas`, `ListaEnlazada<int> caminoIndices`.
   * **`ResultadoBellmanFord`:** arreglos dinámicos `int* costos`, `int* tiempos`, `int* previo`, `bool* alcanzable`, `int cantidadVertices`, `bool hayCicloNegativo`.
   * **`AristaMST`:** `int origenIdx`, `int destinoIdx`, `int peso`.
   * **`ResultadoMST` (Prim / Kruskal):** `ListaEnlazada<AristaMST> aristas`, `int pesoTotal`, `bool esConexo`.
3. **Algoritmos separados sobre `GrafoVuelos` (`src/estructuras/`):**
   * `Dijkstra.h/.cpp` $\rightarrow$ ruta económica, rápida y ruta de maleta (siempre por costo).
   * `BellmanFord.h/.cpp` $\rightarrow$ rutas mínimas hacia todas las ciudades alcanzables desde origen.
   * `Prim.h/.cpp` $\rightarrow$ MST de costo mínimo + construcción de `ArbolMST`.
   * `Kruskal.h/.cpp` $\rightarrow$ ordena aristas con `Ordenamiento` propio, evita ciclos con `ConjuntoDisjunto` y genera MST por duración + `ArbolMST`.

---

### Capa 4: Árboles y Recorridos (`src/arboles/`, según `Arboles.pdf`)

1. **Árbol Binario de Búsqueda — `ArbolABB` (`src/arboles/ArbolABB.h`, `src/arboles/ArbolABB.cpp`):**
   * **`NodoABB`:** `std::string codigo`, `std::string nombre`, `int indiceVertice`, `NodoABB* izquierdo`, `NodoABB* derecho`.
   * **Criterio de orden:** Alfabético por `nombre`.
   * **Operaciones:** `insertar`, `buscarPorNombre`, `eliminar`, recorridos (`preorden`, `inorden`, `postorden`), `obtenerRaiz()`.
   * **Integración visual:** volcado directo al `VisualizadorArbol` de SFML (resaltando el nodo buscado).

2. **Árbol AVL — `ArbolAVL` (`src/arboles/ArbolAVL.h`, `src/arboles/ArbolAVL.cpp`):**
   * **`NodoAVL`:** `Pasajero dato`, `int altura`, `NodoAVL* izquierdo`, `NodoAVL* derecho`.
   * **Criterio de orden y balance (convención de cátedra):** Ordenado por `dato.id`; si el `id` ya existe, agrega el nuevo vuelo a `dato.agregarVuelo(...)`. Se usa `alturaNodo(nullptr) = -1` (hojas con `altura = 0`) y `factorBalance(nodo) = alturaNodo(nodo->izquierdo) - alturaNodo(nodo->derecho)`.
   * **Operaciones de balanceo:** `alturaNodo()`, `factorBalance()`, `rotacionDerecha`, `rotacionIzquierda` (y dobles LR / RL), `insertar`, `eliminar`.
   * **Recorridos:** `preorden()`, `inorden()`, `postorden()`.
   * **Integración visual:** volcado completo del árbol balanceado al `VisualizadorArbol`.

3. **Árbol Generado de Expansión Mínima — `ArbolMST` (`src/arboles/ArbolMST.h`, `src/arboles/ArbolMST.cpp`):**
   * *¿Por qué existe esta estructura?* El enunciado exige explícitamente: *"Implemente los métodos de recorrido de los árboles, utilícelos en cada árbol generado"* (incluyendo los árboles de Prim y Kruskal).
   * **`NodoMST` (patrón `NodoGeneral` de `Arboles.pdf`):** `int indiceCiudad`, `std::string codigoCiudad`, `std::string nombreCiudad`, `int pesoDesdePadre`, `NodoMST* primerHijo`, `NodoMST* siguienteHermano`.
   * **Operaciones:** `construirDesdeAristas(...)`, `preorden()`, `inorden()`, `postorden()`.

---

### Capa 5: Orquestación del Sistema y Enlace con SFML (`src/sistema/` y `src/visual/`)

* **`SistemaAerolinea` (`src/sistema/SistemaAerolinea.h`, `src/sistema/SistemaAerolinea.cpp`):** Clase coordinadora que mantiene en memoria:
  * `GrafoVuelos grafo`
  * `ArbolABB arbolCiudades`
  * `ArbolAVL arbolPasajeros`
  * `ListaEnlazada<Aeronave> aeronaves`
  * `ListaEnlazada<Vuelo> vuelos`
  * `GestorArchivos persistencia`
* Se encarga de mantener sincronizados los datos entre las estructuras y conectar la ejecución desde [`AplicacionVisual`](src/visual/AplicacionVisual.h).

---

### Orden Recomendado de Implementación
1. **Fase 1 — Estructuras Base (`src/estructuras/`):** `ListaEnlazada`, `Pila`, `Cola`, `ColaPrioridad`, `ConjuntoDisjunto`, `Ordenamiento`.
2. **Fase 2 — Modelos y Persistencia (`src/modelos/`, `src/sistema/` y `data/`):** Clases del dominio + `GestorArchivos` + archivos `.txt` iniciales.
3. **Fase 3 — Árboles (`src/arboles/`):** `ArbolABB`, `ArbolAVL` (con sus rotaciones) y `ArbolMST` con sus recorridos.
4. **Fase 4 — Grafo y Algoritmos (`src/estructuras/`):** `GrafoVuelos`, `ResultadosGrafo`, `Dijkstra` (tiempo, costo y equipaje), `BellmanFord`, `Prim` y `Kruskal`.
5. **Fase 5 — Orquestación e Integración con SFML (`src/sistema/` y `src/visual/`):** Conectar `SistemaAerolinea` con los botones, captura/ingreso de datos, animaciones de vuelo/equipaje y visualización de árboles.
6. **Fase 6 — Entregables finales:** Actualizar `README.md`, redactar informe técnico (máx. 6 págs con diagrama y complejidades $O$) y preparar defensa oral en pareja.
