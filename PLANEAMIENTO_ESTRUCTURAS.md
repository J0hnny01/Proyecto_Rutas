# Planeamiento Técnico de Estructuras, Nodos y Tipos — Parte de Johnny

Documento de referencia para la implementación de los módulos asignados a **Johnny** (Sprint 1, Sprint 2 y Sprint 3), alineado con `DIVISION_TRABAJO.md`, el enunciado de EIF207 y el material de `Anti-estructuras` (`Arboles.pdf` y `programas/video3_listas.cpp`–`video7_mst.cpp`).

---

## 1. Reglas de diseño acordadas

1. **Cero STL prohibida:** no usar `std::stack`, `std::queue`, `std::deque`, `std::list`, `std::priority_queue`, `std::sort` (ni `std::vector` en las estructuras evaluadas; toda colección interna usa punteros o arreglos dinámicos propios con `new` / `delete` / `delete[]`).
2. **Plantillas solo cuando hay múltiples usos reales:**
   - **Con `template <typename T>`:** `Cola<T>` (implementada completa en `include/colas/Cola.h`).
   - **Clases concretas (sin `template`):** `ColaPrioridad`, `ConjuntoDisjunto`, `Ordenamiento`, `ArbolABB`, `ArbolAVL` y `ArbolMST`.
3. **Norma de código:** sin comentarios (`//` ni `/* ... */`) en ningún archivo `.h` o `.cpp`.
4. **Gestión de memoria:** toda clase con memoria dinámica implementa destructor, constructor de copia y operador de asignación `operator=` (Regla de los 3) para evitar fugas o dobles liberaciones.
5. **Compilación:** compatible con `ZinjaI` (`Debug_Win32`, `-std=c++14` / SFML 2.5.1) y `Code OSS / g++` (`Debug_Linux`, `-std=c++17` / SFML 3.1.0).

---

## 2. Sprint 1 — Modelos de negocio (`include/modelos/` y `src/modelos/`)

### 2.1 `Aeronave` (`Aeronave.h` / `Aeronave.cpp`)
| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `placa` | `std::string` | Identificador único (ej. `"TI-AIR01"`) |
| `marca` | `std::string` | Fabricante (ej. `"Airbus"`) |
| `modelo` | `std::string` | Modelo (ej. `"A320"`) |
| `capacidad` | `int` | Capacidad máxima de pasajeros |
| `estado` | `std::string` | `"Activa"`, `"En vuelo"`, `"Mantenimiento"` |

### 2.2 `Tripulante` (`Tripulante.h` / `Tripulante.cpp`)
| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `id` | `std::string` | Identificador único (ej. `"TR-001"`) |
| `nombre` | `std::string` | Nombre completo |
| `rol` | `std::string` | `"Piloto"`, `"Copiloto"`, `"Sobrecargo"` |
| `placaAeronave` | `std::string` | Placa de la aeronave asignada (clave foránea a `Aeronave`) |

### 2.3 `Pasajero` (`Pasajero.h` / `Pasajero.cpp`) — *Opción A*
Un pasajero se identifica de forma única por su `id` y puede estar asignado a uno o varios vuelos mediante un arreglo dinámico propio (sin `std::vector` ni `std::list`).

| Campo | Tipo | Descripción |
| :--- | :--- | :--- |
| `id` | `std::string` | Cédula o pasaporte (clave única de búsqueda en `ArbolAVL`) |
| `nombre` | `std::string` | Nombre completo |
| `codigosVuelos` | `std::string*` | Arreglo dinámico con los códigos de vuelo asignados |
| `cantidadVuelos` | `int` | Cantidad actual de vuelos en `codigosVuelos` |
| `capacidadVuelos` | `int` | Capacidad reservada del arreglo dinámico |
| `cantidadMaletas` | `int` | Total de maletas registradas |

Métodos clave adicionales a getters/setters:
- `agregarVuelo(const std::string& codigoVuelo)`: evita duplicados; si `cantidadVuelos == capacidadVuelos`, duplica la capacidad del arreglo dinámico.
- `tieneVuelo(const std::string& codigoVuelo) const`: verifica si el pasajero está asignado a ese vuelo.
- `obtenerVuelosTexto() const`: devuelve los vuelos concatenados (ej. `"AV-101, AV-205"`) para mostrarlos en interfaz y despliegue del AVL.
- Regla de los 3 (`Pasajero(const Pasajero&)`, `operator=`, `~Pasajero()`) para copiar/liberar `codigosVuelos` de forma segura.

---

## 3. Sprint 1 — TADs base y algoritmos auxiliares

### 3.1 `Cola<T>` (`include/colas/Cola.h`) — *Plantilla completa en `.h`*
Usada para recorridos por niveles en `ArbolABB` (`NodoABB*`), `ArbolAVL` (`NodoAVL*`), `ArbolMST` (`NodoMST*`) y BFS por índices (`int`).

- **Nodo interno:**
  ```cpp
  struct NodoCola {
      T dato;
      NodoCola* siguiente;
  };
  ```
- **Atributos privados:**
  - `NodoCola* frente;`
  - `NodoCola* final;`
  - `int cantidad;`
- **Métodos y complejidad:**
  - `encolar(const T& valor)` — $O(1)$
  - `desencolar()` — $O(1)$
  - `obtenerFrente()` / `obtenerFrente() const` — $O(1)$
  - `estaVacia() const`, `tamano() const` — $O(1)$
  - `limpiar()` — $O(n)$
  - Regla de los 3: `Cola(const Cola&)`, `operator=`, `~Cola()`.

### 3.2 `ColaPrioridad` (`include/colas/ColaPrioridad.h` / `src/colas/ColaPrioridad.cpp`) — *Concreta (Min-Heap)*
Usada por **Prim** (prioridad = costo de arista) y disponible para **Dijkstra** (prioridad = costo o tiempo acumulado).

- **Elemento del heap:**
  ```cpp
  struct ElementoHeap {
      int vertice;
      int prioridad;
  };
  ```
- **Atributos privados:**
  - `ElementoHeap* datos;`
  - `int cantidad;`
  - `int capacidad;`
- **Métodos y complejidad:**
  - `insertar(int vertice, int prioridad)` — $O(\log n)$ amortizado (usa `subir(int)` y `redimensionar()`).
  - `extraerMinimo()` — $O(\log n)$ (retorna el `ElementoHeap` con menor `prioridad` y reordena con `bajar(0)`).
  - `verMinimo() const`, `estaVacia() const`, `tamano() const` — $O(1)$
  - `limpiar()` — $O(1)$ (lógico) / `~ColaPrioridad()` — $O(1)$ (`delete[] datos`).
  - Regla de los 3: copia profunda del arreglo `datos`.

### 3.3 `ConjuntoDisjunto` (`include/conjuntos/ConjuntoDisjunto.h` / `src/conjuntos/ConjuntoDisjunto.cpp`) — *Concreta (Union-Find)*
Usada por **Kruskal** para detectar ciclos entre los índices `0..n-1` de ciudades (basada en `representante[]` y `encontrar()` de `video7_mst.cpp`).

- **Atributos privados:**
  - `int* representante;`
  - `int* rango;`
  - `int tamano;`
- **Métodos y complejidad:**
  - `ConjuntoDisjunto(int n)`: inicializa `representante[i] = i` y `rango[i] = 0` — $O(n)$.
  - `encontrar(int x)`: busca la raíz aplicando compresión de caminos — $O(\alpha(n))$.
  - `unir(int a, int b)`: une dos conjuntos distintos por rango; retorna `false` si ya estaban en el mismo conjunto (formarían ciclo) — $O(\alpha(n))$.
  - `mismoConjunto(int a, int b)` — $O(\alpha(n))$.
  - `reiniciar(int n)` — $O(n)$.
  - Regla de los 3 (`ConjuntoDisjunto(const ConjuntoDisjunto&)`, `operator=`, `~ConjuntoDisjunto()`).

### 3.4 `Ordenamiento` (`include/ordenamiento/Ordenamiento.h` / `src/ordenamiento/Ordenamiento.cpp`) — *Concreta (MergeSort)*
Reemplaza `std::sort` / `stable_sort` en **Kruskal** (donde `video7_mst.cpp` ordenaba las aristas por peso ascendente).

- **Elemento ordenable:**
  ```cpp
  struct AristaOrdenable {
      int origen;
      int destino;
      int peso;
  };
  ```
- **Métodos y complejidad:**
  - `static void ordenarAristas(AristaOrdenable* aristas, int cantidad)`: ordena por `peso` ascendente mediante **MergeSort** estable usando arreglo temporal dinámico (`new AristaOrdenable[...]` / `delete[]`) — $O(E \log E)$ garantizado.
  - Métodos privados auxiliares: `mergeSortRecursivo(AristaOrdenable* arr, int izq, int der)` y `mezclar(AristaOrdenable* arr, int izq, int medio, int der)`.

---

## 4. Sprint 1 — Formato `;` y archivos iniciales en `data/`

Todos los archivos usan `;` como delimitador y una entidad por línea. Las coordenadas de `ciudades.txt` deben ubicarse dentro del lienzo del grafo en SFML (`x` en `430..1110`, `y` en `120..465`, acorde al panel `365..1185 x 70..510` de `AplicacionVisual.cpp`).

| Archivo | Columnas y tipos | Notas |
| :--- | :--- | :--- |
| `data/ciudades.txt` | `codigo(string);nombre(string);x(float);y(float)` | Ej. `SJO;San Jose;520;340` |
| `data/rutas.txt` | `origen(string);destino(string);costo(int);tiempoMin(int)` | Aristas dirigidas; incluir casos donde ruta barata y rápida difieran |
| `data/aeronaves.txt` | `placa(string);marca(string);modelo(string);capacidad(int);estado(string)` | Flota inicial |
| `data/tripulacion.txt` | `id(string);nombre(string);rol(string);placaAeronave(string)` | Asociada por `placaAeronave` |
| `data/vuelos.txt` | `codigoVuelo(string);origen(string);destino(string);placaAeronave(string);costo(int);tiempoMin(int)` | Coherente con `ciudades.txt` y `aeronaves.txt` |
| `data/pasajeros.txt` | `id(string);nombre(string);codigoVuelo(string);cantidadMaletas(int)` | Si un mismo `id` aparece en más de una línea con distinto `codigoVuelo`, el AVL agrega el vuelo al mismo nodo |

---

## 5. Sprint 2 y 3 — Conexión con árboles, MST y visualizadores SFML

### 5.1 `ArbolABB` (`include/arboles/ArbolABB.h` / `src/arboles/ArbolABB.cpp`) — *Concreto*
Almacena las ciudades visitadas/registradas para verificar si una ciudad pertenece a las rutas de la aerolínea y desplegar el árbol completo.

- **Nodo:**
  ```cpp
  struct NodoABB {
      std::string codigo;
      std::string nombre;
      int indiceVertice;
      NodoABB* izquierdo;
      NodoABB* derecho;
  };
  ```
- **Clave de comparación:** `nombre` (orden lexicográfico).
- **Operaciones:** `insertar`, `buscarPorNombre`, `eliminar` (3 casos con sucesor inorden de `Arboles.pdf`), recorridos (`preorden`, `inorden`, `postorden`), `obtenerRaiz()` para volcar hacia `VisualizadorArbol` (`etiqueta = codigo`, `subtexto = nombre`), y liberación recursiva en `~ArbolABB()`.

### 5.2 `ArbolAVL` (`include/arboles/ArbolAVL.h` / `src/arboles/ArbolAVL.cpp`) — *Concreto (Opción A)*
Almacena los pasajeros indexados por `dato.id` para consultar en $O(\log n)$ si un pasajero existe y en qué vuelos está asignado, manteniendo despliegue total del árbol.

- **Nodo (convención exacta de `Arboles.pdf`):**
  ```cpp
  struct NodoAVL {
      Pasajero dato;
      int altura;
      NodoAVL* izquierdo;
      NodoAVL* derecho;
  };
  ```
- **Convención de alturas y balance:**
  - `alturaNodo(nullptr) = -1`, nodo hoja = `0`.
  - `factorBalance(nodo) = alturaNodo(izq) - alturaNodo(der)` $\in \{-1, 0, 1\}$.
  - Rotaciones simples (`rotacionDerecha`, `rotacionIzquierda`) y dobles (LR, RL) en inserción y eliminación.
- **Comportamiento en inserción (Opción A):** si `id` ya existe en un nodo, llama a `nodo->dato.agregarVuelo(nuevoVuelo)` sin insertar un nodo duplicado.
- **Conexión visual:** `VisualizadorArbol` muestra `etiqueta = dato.id` y `subtexto = dato.obtenerVuelosTexto() + " h=" + altura`.

### 5.3 `ArbolMST` (`include/arboles/ArbolMST.h` / `src/arboles/ArbolMST.cpp`) — *Concreto (Árbol General)*
Representa el árbol de expansión mínima generado por **Prim** (costo) o **Kruskal** (duración) mediante `primerHijo` / `siguienteHermano` (`Arboles.pdf`, pág. 4).

- **Nodo:**
  ```cpp
  struct NodoMST {
      int indiceCiudad;
      std::string codigoCiudad;
      std::string nombreCiudad;
      int pesoDesdePadre;
      NodoMST* primerHijo;
      NodoMST* siguienteHermano;
  };
  ```
- **Operaciones:**
  - Construcción a partir de raíz y lista de aristas aceptadas `(padre, hijo, peso)`.
  - Recorridos requeridos por el enunciado: `preorden`, `inorden` y `postorden` (devolviendo cadenas formateadas para el panel informativo de `AplicacionVisual`).
  - Destructor recursivo (`primerHijo` y `siguienteHermano`).

### 5.4 `Prim` y `Kruskal` (`include/grafo/algoritmos/`)
- **`Prim`:** parte de un vértice origen y usa `ColaPrioridad` (`ElementoHeap`) + arreglo `enArbol[]`, `clave[]`, `padre[]` eligiendo aristas por **costo** — $O((V + E)\log V)$.
- **`Kruskal`:** recolecta aristas en un arreglo dinámico `AristaOrdenable*`, las ordena por **duración** con `Ordenamiento::ordenarAristas` y usa `ConjuntoDisjunto` para evitar ciclos hasta completar $V - 1$ aristas (o agotar componentes) — $O(E \log E)$.
- **Salida hacia UI:** ambos generan un `ArbolMST` (para mostrar los 3 recorridos y costo/tiempo total) y envían las aristas seleccionadas a `VisualizadorGrafo::resaltarAristaMST(origen, destino)`.
