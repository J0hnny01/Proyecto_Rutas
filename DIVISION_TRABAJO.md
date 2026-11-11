# División del Trabajo — Proyecto II EIF207 (Pareja: Andrés & Johnny)

Plan de reparto equitativo para desarrollar el proyecto en paralelo con **cero conflictos en Git** y balance 50/50 en carga técnica y puntos de rúbrica.

---

## Reglas compartidas antes de empezar

1. **Fuente primaria de diseño:** Seguir las estructuras y convenciones del material de clase en `Anti-estructuras` (`Grafos.pdf`, `Arboles.pdf`, `programas/video3_listas.cpp` a `video7_mst.cpp`).
2. **Prohibición estricta de STL (nota `0` si se incumple):** No usar `std::stack`, `std::queue`, `std::deque`, `std::list`, `std::priority_queue` ni `std::sort`. Todo va con punteros y memoria dinámica propia.
3. **Norma de código del proyecto:** No colocar comentarios (`//` ni `/* ... */`) en ningún archivo `.h` o `.cpp`.
4. **Plantillas (`template <typename T>`):** únicamente `ListaEnlazada.h` y `Cola.h` (que se usan con múltiples tipos) se implementan completos dentro de su `.h` en `src/estructuras/`. Todo lo demás lleva clases concretas en `.h` + `.cpp` en la misma carpeta (`src/estructuras/`, `src/arboles/`, `src/modelos/`, `src/sistema/`, `src/visual/`).
5. **Compilación dual:** Mantener compatible con **Code OSS / g++** (`.vscode/tasks.json`) y **ZinjaI** (`Proyecto_Rutas.zpr`).

---

## Reparto 50 / 50 por archivos y módulos

### Parte de Andrés (Code OSS / Linux)

| Categoría | Archivos asignados | Puntos / Responsabilidad |
| :--- | :--- | :--- |
| **TADs base** | `src/estructuras/ListaEnlazada.h`<br>`src/estructuras/Pila.h` / `.cpp` | TADs lineales propios (lista usada como adyacencia y listas generales; pila concreta de `int` para reconstruir rutas) |
| **Modelos de vuelos** | `src/modelos/Ciudad.h` / `.cpp`<br>`src/modelos/Ruta.h` / `.cpp`<br>`src/modelos/Equipaje.h` / `.cpp`<br>`src/modelos/Vuelo.h` / `.cpp` | Entidades ligadas directamente al grafo, rutas, equipaje y vuelos |
| **Grafo principal** | `src/estructuras/ResultadosGrafo.h` / `.cpp`<br>`src/estructuras/GrafoVuelos.h` / `.cpp` | Grafo dirigido ponderado por Lista de Adyacencia (**10 pts**) |
| **Caminos mínimos** | `src/estructuras/Dijkstra.h` / `.cpp`<br>`src/estructuras/BellmanFord.h` / `.cpp` | Dijkstra costo/tiempo (**10 pts**) + ruta económica de maleta (**5 pts**) + Bellman-Ford (**8 pts**) |
| **Sistema y SFML** | Métodos cliente/equipaje en `src/sistema/SistemaAerolinea.h` / `.cpp`<br>Acciones del **Módulo del Cliente** y animaciones de ruta/equipaje en `src/visual/AplicacionVisual.cpp` | Módulo Cliente (**7 pts**) + animación de vuelos/maletas en SFML |
| **Documentación** | `README.md` + sección del informe técnico sobre Grafo, Dijkstra, Bellman-Ford y diagrama general | Parte de Apartado A (**15 pts** compartidos) |

---

### Parte de Johnny (ZinjaI)

| Categoría | Archivos asignados | Puntos / Responsabilidad |
| :--- | :--- | :--- |
| **TADs base y orden** | `src/estructuras/Cola.h`<br>`src/estructuras/ColaPrioridad.h` / `.cpp`<br>`src/estructuras/ConjuntoDisjunto.h` / `.cpp`<br>`src/estructuras/Ordenamiento.h` / `.cpp` | Cola genérica, Min-Heap concreto, Union-Find (`representante[]` / `encontrar`) y ordenamiento propio (sin `std::sort`) |
| **Modelos y archivos** | `src/modelos/Aeronave.h` / `.cpp`<br>`src/modelos/Tripulante.h` / `.cpp`<br>`src/modelos/Pasajero.h` / `.cpp`<br>`src/sistema/GestorArchivos.h` / `.cpp`<br>Archivos iniciales `.txt` en `data/` | Flota, tripulación, pasajeros y persistencia en texto plano |
| **Árboles y recorridos** | `src/arboles/ArbolABB.h` / `.cpp`<br>`src/arboles/ArbolAVL.h` / `.cpp`<br>`src/arboles/ArbolMST.h` / `.cpp` | ABB de ciudades (**6 pts**), AVL de pasajeros con rotaciones (**9 pts**) y árbol general `ArbolMST` (`primerHijo`/`siguienteHermano`) + recorridos |
| **Árboles de expansión (MST)** | `src/estructuras/Prim.h` / `.cpp`<br>`src/estructuras/Kruskal.h` / `.cpp` | Prim por costo (**6 pts**) y Kruskal por duración (**6 pts**) |
| **Sistema y SFML** | Métodos administrativos en `src/sistema/SistemaAerolinea.h` / `.cpp`<br>Acciones del **Módulo Administrativo** y volcado de ABB/AVL/MST al visualizador en `src/visual/AplicacionVisual.cpp` | Módulo Administrativo (**10 pts**) + visualización de árboles en SFML |
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
