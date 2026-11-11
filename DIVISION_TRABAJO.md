# División del Trabajo — Proyecto II EIF207 (Pareja: Andrés & Johnny)

Plan de reparto equitativo para desarrollar el proyecto en paralelo con **cero conflictos en Git** y balance 50/50 en carga técnica y puntos de rúbrica.

---

## Reglas compartidas antes de empezar

1. **Fuente primaria de diseño:** Seguir las estructuras y convenciones del material de clase en `Anti-estructuras` (`Grafos.pdf`, `Arboles.pdf`, `programas/video3_listas.cpp` a `video7_mst.cpp`).
2. **Prohibición estricta de STL (nota `0` si se incumple):** No usar `std::stack`, `std::queue`, `std::deque`, `std::list`, `std::priority_queue` ni `std::sort`. Todo va con punteros y memoria dinámica propia.
3. **Norma de código del proyecto:** No colocar comentarios (`//` ni `/* ... */`) en ningún archivo `.h` o `.cpp`.
4. **Plantillas (`template <typename T>`):** `ListaEnlazada.h`, `Pila.h`, `Cola.h` y `ColaPrioridad.h` se implementan completos dentro de su `.h` en `include/` (sin `.cpp` aparte).
5. **Compilación dual:** Mantener compatible con **Code OSS / g++** (`.vscode/tasks.json`) y **ZinjaI** (`Proyecto_Rutas.zpr`).

---

## Reparto 50 / 50 por archivos y módulos

### Parte de Andrés (Code OSS / Linux)

| Categoría | Archivos asignados | Puntos / Responsabilidad |
| :--- | :--- | :--- |
| **TADs base** | `include/listas/ListaEnlazada.h`<br>`include/pilas/Pila.h` | TADs lineales propios (lista usada como adyacencia y listas generales; pila para reconstruir rutas) |
| **Modelos de vuelos** | `include/modelos/Ciudad.h` / `src/modelos/Ciudad.cpp`<br>`include/modelos/Ruta.h` / `src/modelos/Ruta.cpp`<br>`include/modelos/Equipaje.h` / `src/modelos/Equipaje.cpp`<br>`include/modelos/Vuelo.h` / `src/modelos/Vuelo.cpp` | Entidades ligadas directamente al grafo, rutas, equipaje y vuelos |
| **Grafo principal** | `include/grafo/ResultadosGrafo.h`<br>`include/grafo/GrafoVuelos.h` / `src/grafo/GrafoVuelos.cpp` | Grafo dirigido ponderado por Lista de Adyacencia (**10 pts**) |
| **Caminos mínimos** | `include/grafo/algoritmos/Dijkstra.h` / `src/grafo/algoritmos/Dijkstra.cpp`<br>`include/grafo/algoritmos/BellmanFord.h` / `src/grafo/algoritmos/BellmanFord.cpp` | Dijkstra costo/tiempo (**10 pts**) + ruta económica de maleta (**5 pts**) + Bellman-Ford (**8 pts**) |
| **Sistema y SFML** | Métodos cliente/equipaje en `SistemaAerolinea`<br>Acciones del **Módulo del Cliente** y animaciones de ruta/equipaje en `src/visuales/AplicacionVisual.cpp` | Módulo Cliente (**7 pts**) + animación de vuelos/maletas en SFML |
| **Documentación** | `README.md` + sección del informe técnico sobre Grafo, Dijkstra, Bellman-Ford y diagrama general | Parte de Apartado A (**15 pts** compartidos) |

---

### Parte de Johnny (ZinjaI)

| Categoría | Archivos asignados | Puntos / Responsabilidad |
| :--- | :--- | :--- |
| **TADs base y orden** | `include/colas/Cola.h`<br>`include/colas/ColaPrioridad.h`<br>`include/conjuntos/ConjuntoDisjunto.h` / `src/conjuntos/ConjuntoDisjunto.cpp`<br>`include/ordenamiento/Ordenamiento.h` / `src/ordenamiento/Ordenamiento.cpp` | Colas, Min-Heap propio, Union-Find (`representante[]` / `encontrar`) y ordenamiento propio (sin `std::sort`) |
| **Modelos y archivos** | `include/modelos/Aeronave.h` / `src/modelos/Aeronave.cpp`<br>`include/modelos/Tripulante.h` / `src/modelos/Tripulante.cpp`<br>`include/modelos/Pasajero.h` / `src/modelos/Pasajero.cpp`<br>`include/persistencia/GestorArchivos.h` / `src/persistencia/GestorArchivos.cpp`<br>Archivos iniciales `.txt` en `data/` | Flota, tripulación, pasajeros y persistencia en texto plano |
| **Árboles y recorridos** | `include/arboles/ArbolABB.h` / `src/arboles/ArbolABB.cpp`<br>`include/arboles/ArbolAVL.h` / `src/arboles/ArbolAVL.cpp`<br>`include/arboles/ArbolMST.h` / `src/arboles/ArbolMST.cpp` | ABB de ciudades (**6 pts**), AVL de pasajeros con rotaciones (**9 pts**) y árbol general `ArbolMST` (`primerHijo`/`siguienteHermano`) + recorridos |
| **Árboles de expansión (MST)** | `include/grafo/algoritmos/Prim.h` / `src/grafo/algoritmos/Prim.cpp`<br>`include/grafo/algoritmos/Kruskal.h` / `src/grafo/algoritmos/Kruskal.cpp` | Prim por costo (**6 pts**) y Kruskal por duración (**6 pts**) |
| **Sistema y SFML** | Métodos administrativos en `SistemaAerolinea`<br>Acciones del **Módulo Administrativo** y volcado de ABB/AVL/MST al visualizador en `src/visuales/AplicacionVisual.cpp` | Módulo Administrativo (**10 pts**) + visualización de árboles en SFML |
| **Documentación y entrega** | Sección del informe técnico sobre ABB, AVL, Prim, Kruskal y TADs + prueba completa en **ZinjaI** + paquete `.zip` final | Parte de Apartado A (**15 pts** compartidos) |

---

## Balance en puntos de rúbrica

* **Apartado B (55 pts):** Andrés `28 pts` (`Grafo 10 + Dijkstra 10 + Bellman-Ford 8`) vs Johnny `27 pts` (`ABB 6 + AVL 9 + Prim 6 + Kruskal 6`).
* **Apartado C (30 pts):** Andrés `~16 pts` (`Cliente 7 + Equipaje 5 + Animación SFML`) vs Johnny `~14 pts` (`Administrativo 10 + Visualización árboles SFML`).

---

## Sprints de ejecución (sin bloqueos)

### Sprint 1 — Base común (desbloquear todo el proyecto)
* **Andrés:**
  * [ ] `ListaEnlazada.h` y `Pila.h`
  * [ ] `Ciudad`, `Ruta`, `Equipaje`, `Vuelo`
  * [ ] `ResultadosGrafo.h` y firmas públicas mínimas de `GrafoVuelos.h`
* **Johnny:**
  * [ ] `Cola.h`, `ColaPrioridad.h`, `ConjuntoDisjunto`, `Ordenamiento`
  * [ ] `Aeronave`, `Tripulante`, `Pasajero`
  * [ ] Definir formato `;` y archivos iniciales en `data/` (`ciudades.txt`, `rutas.txt`, `aeronaves.txt`, `tripulacion.txt`, `vuelos.txt`, `pasajeros.txt`)
* **Hito:** Integrar ambos en `main` para tener todos los TADs y modelos compilando.

### Sprint 2 — Núcleo paralelo (cero conflictos de archivos)
* **Andrés:**
  * [ ] `GrafoVuelos.cpp` (lista de adyacencia con `NodoArista`, agregar/eliminar/modificar ciudades y rutas)
  * [ ] `Dijkstra.cpp` (ruta rápida, ruta barata y ruta económica de maleta)
  * [ ] `BellmanFord.cpp` (todas las rutas alcanzables desde un origen)
* **Johnny:**
  * [ ] `GestorArchivos.cpp` (carga y guardado de `.txt`)
  * [ ] `ArbolABB.cpp` (inserción, búsqueda de ciudades, eliminación y recorridos)
  * [ ] `ArbolAVL.cpp` (inserción, balanceo, rotaciones LL/RR/LR/RL, consulta de vuelos por pasajero y recorridos)
  * [ ] `ArbolMST.cpp` (construcción desde aristas + `preorden`, `inorden`, `postorden`)
* **Hito:** Subir a `main`. Ahora `GrafoVuelos` y `ArbolMST` ya existen para conectar Prim y Kruskal.

### Sprint 3 — MST, `SistemaAerolinea` e integración con SFML
* **Johnny (en sus propios archivos):**
  * [ ] `Prim.cpp` y `Kruskal.cpp`
* **Integración ordenada en `SistemaAerolinea` y `AplicacionVisual.cpp` (para no pisarse):**
  1. [ ] **Primero Andrés** integra carga de grafo, lógica cliente/equipaje y animaciones de Dijkstra / Bellman-Ford en SFML, y hace `push` a `main`.
  2. [ ] **Luego Johnny** hace `pull` e integra el CRUD administrativo, Prim, Kruskal y el volcado visual de ABB, AVL y MST en SFML.

### Sprint 4 — Pruebas, informe y repaso cruzado (defensa oral)
* [ ] Verificar compilación limpia y ejecución completa en **ZinjaI** (Johnny) y **Code OSS** (Andrés).
* [ ] Verificar cumplimiento total anti-STL (sin `std::stack`, `std::queue`, `std::deque`, `std::list`, `std::priority_queue`, `std::sort`) y ausencia de fugas de memoria.
* [ ] Completar `README.md`, informe técnico (máximo 6 páginas con complejidades $O$ y diagrama) y generar el `.zip`.
* [ ] **Repaso cruzado obligatorio:** Andrés explica a Johnny cómo funcionan `GrafoVuelos`, `Dijkstra` y `BellmanFord`; Johnny explica a Andrés cómo funcionan `ArbolABB`, `ArbolAVL`, `Prim` y `Kruskal`. *(Recuerden: si un integrante no puede explicar algún componente en la defensa, el factor multiplicador tiene tope de `0.50`).*
