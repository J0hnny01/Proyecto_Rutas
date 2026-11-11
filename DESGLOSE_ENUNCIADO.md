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
* *Estado actual:* Ya contamos con el cascarón visual en `src/visuales/` listo para conectar con la lógica.

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

Toda estructura de datos evaluada se implementará desde cero en `include/` y `src/` utilizando punteros, memoria dinámica y plantillas (`template`) donde aplique, respetando estrictamente la prohibición de contenedores STL.

### Capa 1: TADs Base Propios (`include/tads/`)

| Estructura | Archivo sugerido | Diseño interno | Para qué se usa en el proyecto |
| :--- | :--- | :--- | :--- |
| **`ListaEnlazada<T>`** | `include/tads/ListaEnlazada.h` | `NodoLista<T>` con `dato` y `siguiente`; punteros `cabeza`, `cola` y contador `tamano`. | **Lista de adyacencia** del grafo; listas de aeronaves, vuelos, tripulación, pasajeros, aristas del MST y rutas reconstruidas. |
| **`Pila<T>`** | `include/tads/Pila.h` | `NodoPila<T>` enlazado con puntero `tope` y `tamano`. | Invertir el camino de predecesores al reconstruir rutas en **Dijkstra** y **Bellman-Ford** (de destino a origen $\rightarrow$ origen a destino). |
| **`Cola<T>`** | `include/tads/Cola.h` | `NodoCola<T>` enlazado con punteros `frente` y `final`. | Recorridos por anchura (BFS) para verificar conectividad/alcanzabilidad y procesamiento ordenado de registros. |
| **`ColaPrioridad<T>`** | `include/tads/ColaPrioridad.h` | **Min-Heap binario** sobre arreglo dinámico propio (`capacidad`, `tamano`, `duplicarCapacidad()`, `flotar()`, `hundir()`). Cada elemento almacena `(clavePrioridad, valor)`. | Selección eficiente del vértice de menor costo/tiempo en **Dijkstra** y de la arista mínima en **Prim** ($O((V+E)\log V)$). |
| **`ConjuntoDisjunto`** | `include/tads/ConjuntoDisjunto.h` | Arreglos dinámicos `padre[]` y `rango[]` con compresión de caminos (`find`) y unión por rango (`unir`). | Detección de ciclos en tiempo casi constante $O(\alpha(V))$ durante la ejecución de **Kruskal**. |
| **`Ordenamiento`** | `include/tads/Ordenamiento.h` | Implementación propia de **MergeSort** (o **QuickSort**) sobre arreglos dinámicos / listas sin usar `std::sort`. | Ordenar las aristas por duración ascendente en **Kruskal**, y ordenar listados de resultados (vuelos, opciones de Bellman-Ford). |

---

### Capa 2: Modelos de Dominio y Persistencia (`include/modelos/` y `data/`)

1. **`Ciudad` (`Ciudad.h`)**
   * **Atributos:** `int id`, `std::string codigo` (ej. `"SJO"`), `std::string nombre` (ej. `"San Jose"`), `std::string pais`, `float posX`, `float posY` (posición en el mapa SFML).
2. **`Ruta` (`Ruta.h`)**
   * **Atributos:** `std::string idRuta`, `std::string origenCodigo`, `std::string destinoCodigo`, `double costo`, `int duracionMin`, `bool activa`.
3. **`Tripulante` (`Tripulante.h`)**
   * **Atributos:** `std::string id`, `std::string nombre`, `std::string rol` (Piloto, Copiloto, Sobrecargo), `std::string licencia`, `std::string placaAeronave`.
4. **`Aeronave` (`Aeronave.h`)**
   * **Atributos:** `std::string placa`, `std::string marca`, `std::string modelo`, `int capacidad`, `std::string estado`, `ListaEnlazada<Tripulante> tripulacion`.
5. **`Equipaje` (`Equipaje.h`)**
   * **Atributos:** `std::string idMaleta`, `double pesoKg`, `ListaEnlazada<std::string> rutaMasBarata` (códigos de ciudad calculados con Dijkstra por costo), `double costoEnvio`, `int duracionEnvio`.
6. **`Pasajero` (`Pasajero.h`)**
   * **Atributos:** `int idPasajero` (clave numérica para el árbol AVL), `std::string cedula`, `std::string nombre`, `ListaEnlazada<std::string> vuelosAsignados` (códigos de vuelos donde está registrado), `std::string criterioSeleccionado` (`"Tiempo"` o `"Costo"`), `Equipaje maleta`.
7. **`Vuelo` (`Vuelo.h`)**
   * **Atributos:** `std::string codigoVuelo`, `std::string placaAeronave`, `std::string origenCodigo`, `std::string destinoCodigo`, `double costo`, `int duracionMin`, `std::string horaSalida`, `ListaEnlazada<int> idsPasajeros`.
8. **`GestorArchivos` (`GestorArchivos.h` / `.cpp`)**
   * Lee y escribe con `std::ifstream` / `std::ofstream` y `std::stringstream` (separador `;`) los archivos en `data/`:
     * `data/ciudades.txt` $\rightarrow$ `codigo;nombre;pais;posX;posY`
     * `data/rutas.txt` $\rightarrow$ `idRuta;origen;destino;costo;duracion`
     * `data/aeronaves.txt` $\rightarrow$ `placa;marca;modelo;capacidad;estado`
     * `data/tripulacion.txt` $\rightarrow$ `id;nombre;rol;licencia;placaAeronave`
     * `data/vuelos.txt` $\rightarrow$ `codigoVuelo;placaAeronave;origen;destino;costo;duracion;horaSalida`
     * `data/pasajeros.txt` $\rightarrow$ `idPasajero;cedula;nombre;codigoVuelo;criterio;pesoMaleta`

---

### Capa 3: Grafo de Vuelos y Algoritmos (`include/grafo/`)

1. **Estructuras internas del Grafo (`GrafoVuelos.h` / `.cpp`):**
   * **`AristaAdyacente`:** `int destinoIdx`, `std::string idRuta`, `double costo`, `int duracionMin`.
   * **`VerticeCiudad`:** `Ciudad ciudad`, `ListaEnlazada<AristaAdyacente> adyacentes` (Lista de Adyacencia propia).
   * **`GrafoVuelos`:**
     * Arreglo dinámico propio de `VerticeCiudad` (`vertices`), `int numVertices`, `int capacidad`.
     * Operaciones: `agregarCiudad`, `eliminarCiudad`, `agregarRutaDirigida`, `eliminarRuta`, `modificarRuta`, `buscarIndicePorCodigo`, `buscarIndicePorNombre`.
2. **Estructuras de resultados para algoritmos:**
   * **`ResultadoRuta` (Dijkstra):** `bool encontrada`, `double costoTotal`, `int duracionTotal`, `int cantidadEscalas`, `ListaEnlazada<int> caminoIndices`.
   * **`EntradaDestinoBF` / `ResultadoBellmanFord`:** arreglo de `double costos[]`, `int tiempos[]`, `int predecesores[]`, `bool alcanzable[]`, `bool hayCicloNegativo`.
   * **`AristaMST`:** `int origenIdx`, `int destinoIdx`, `double peso` (costo en Prim, duración en Kruskal).
   * **`ResultadoMST` (Prim / Kruskal):** `ListaEnlazada<AristaMST> aristas`, `double pesoTotal`, `bool esConexo`.
3. **Algoritmos implementados sobre `GrafoVuelos`:**
   * `dijkstra(int origen, int destino, bool porCosto)` $\rightarrow$ retorna `ResultadoRuta`.
   * `calcularRutaMaleta(int origen, int destino)` $\rightarrow$ invoca internamente `dijkstra(origen, destino, true)` (siempre por costo).
   * `bellmanFord(int origen, bool porCosto)` $\rightarrow$ retorna `ResultadoBellmanFord` hacia todas las ciudades.
   * `primPorCosto(int origen)` $\rightarrow$ genera el MST de costo mínimo tratando los enlaces disponibles entre pares de ciudades y retorna `ResultadoMST` + `ArbolMST`.
   * `kruskalPorDuracion()` $\rightarrow$ ordena aristas con `Ordenamiento` propio, evita ciclos con `ConjuntoDisjunto` y retorna `ResultadoMST` + `ArbolMST`.

---

### Capa 4: Árboles y Recorridos (`include/arboles/`)

1. **Árbol Binario de Búsqueda — `ArbolABB` (`ArbolABB.h` / `.cpp`):**
   * **`NodoABB`:** `Ciudad ciudad`, `NodoABB* izq`, `NodoABB* der`.
   * **Criterio de orden:** Alfabético por `ciudad.nombre` (o código).
   * **Operaciones:** `insertar`, `buscar(nombreCiudad)`, `eliminar`, `limpiar`.
   * **Recorridos:** `preorden()`, `inorden()`, `postorden()`.
   * **Integración visual:** método para volcar nodos y recorridos directamente al `VisualizadorArbol` de SFML (resaltando el nodo buscado).

2. **Árbol AVL — `ArbolAVL` (`ArbolAVL.h` / `.cpp`):**
   * **`NodoAVL`:** `Pasajero pasajero`, `int altura`, `NodoAVL* izq`, `NodoAVL* der`.
   * **Criterio de orden:** Por `pasajero.idPasajero` (entero único). Cada nodo mantiene la lista de vuelos a los que está asignado ese pasajero.
   * **Operaciones de balanceo:** `altura()`, `factorEquilibrio()`, `rotacionDerecha` (caso LL), `rotacionIzquierda` (caso RR), `rotacionIzqDer` (caso LR), `rotacionDerIzq` (caso RL).
   * **Operaciones de negocio:** `insertarOActualizarAsignacion`, `buscarPasajero(id)`, `estaAsignadoAVuelo(id, codigoVuelo)`, `eliminar`.
   * **Recorridos:** `preorden()`, `inorden()`, `postorden()`.
   * **Integración visual:** volcado completo del árbol balanceado al `VisualizadorArbol` (mostrando ID, nombre/vuelos y `FE`).

3. **Árbol Generado de Expansión Mínima — `ArbolMST` (`ArbolMST.h` / `.cpp`):**
   * *¿Por qué existe esta estructura?* El enunciado exige explícitamente: *"Implemente los métodos de recorrido de los árboles, utilícelos en cada árbol generado"* (incluyendo los árboles de Prim y Kruskal).
   * **`NodoMST`:** `int indiceCiudad`, `std::string codigoCiudad`, `std::string nombreCiudad`, `double pesoDesdePadre`, `ListaEnlazada<NodoMST*> hijos` (representación n-aria enraizada en la ciudad inicial, o representación binaria Hijo-Izquierdo / Hermano-Derecho para recorridos binarios estándar).
   * **Operaciones:** `construirDesdeAristas(ResultadoMST, int raizIdx)`, `preorden()`, `inorden()`, `postorden()`.

---

### Capa 5: Orquestación del Sistema y Enlace con SFML (`include/modelos/SistemaAerolinea.h`)

* **`SistemaAerolinea`:** Clase coordinadora que mantiene en memoria:
  * `GrafoVuelos grafo`
  * `ArbolABB arbolCiudades`
  * `ArbolAVL arbolPasajeros`
  * `ListaEnlazada<Aeronave> aeronaves`
  * `ListaEnlazada<Vuelo> vuelos`
  * `GestorArchivos persistencia`
* Se encarga de mantener sincronizados los datos entre las estructuras (por ejemplo, al cargar o agregar una ciudad se inserta tanto en el `GrafoVuelos` como en el `ArbolABB`; al asignar un pasajero a un vuelo se actualiza el `Vuelo` y el `ArbolAVL`; y al ejecutar acciones en [`AplicacionVisual`](src/visuales/AplicacionVisual.h), entrega los caminos y árboles listos para animar y dibujar).

---

### Orden Recomendado de Implementación
1. **Fase 1 — TADs Base (`include/tads/`):** `ListaEnlazada`, `Pila`, `Cola`, `ColaPrioridad`, `ConjuntoDisjunto`, `Ordenamiento`.
2. **Fase 2 — Modelos y Archivos (`include/modelos/` y `data/`):** Clases del dominio + `GestorArchivos` + archivos `.txt` iniciales de prueba.
3. **Fase 3 — Árboles (`include/arboles/`):** `ArbolABB`, `ArbolAVL` (con sus 4 rotaciones) y `ArbolMST` con sus recorridos.
4. **Fase 4 — Grafo y Algoritmos (`include/grafo/`):** `GrafoVuelos` (lista de adyacencia), `Dijkstra` (tiempo, costo y equipaje), `Bellman-Ford`, `Prim` y `Kruskal`.
5. **Fase 5 — Integración con SFML (`src/visuales/`):** Conectar `SistemaAerolinea` con los botones, formularios/consola auxiliar de ingreso de datos, animaciones de vuelo/equipaje y visualización de árboles.
6. **Fase 6 — Entregables finales:** Actualizar `README.md`, redactar informe técnico (máx. 6 págs con diagrama y complejidades $O$) y preparar defensa oral en pareja.
