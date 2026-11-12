# Proyecto_Rutas — Plataforma de Gestión de Aerolínea

Proyecto II del curso **EIF207 Estructuras de Datos** (Universidad Nacional de Costa Rica).  
Aplicación gráfica en C++ para la gestión de rutas aéreas, cálculo de caminos óptimos, administración de flota y consulta de pasajeros mediante estructuras de datos implementadas desde cero (sin contenedores STL prohibidos).

## Librería gráfica y entorno utilizados

* **Entorno / IDE:** ZinjaI (`Proyecto_Rutas.zpr`)
* **Lenguaje:** C++ (`C++14`)
* **Librería gráfica:** **SFML** (*Simple and Fast Multimedia Library*)
  * **Windows:** SFML 2.5.1 (`sfml-graphics`, `sfml-window`, `sfml-system`).
  * **Linux:** SFML 2.x / 3.x (`sfml-graphics`, `sfml-audio`, `sfml-network`, `sfml-window`, `sfml-system`), compatible mediante `src/visual/CompatSFML.h`.

## Cómo compilar y ejecutar en ZinjaI

1. Abre el archivo `Proyecto_Rutas.zpr` con **ZinjaI**.
2. Selecciona el perfil según tu sistema operativo:
   * **Windows:** `Debug_Win32` (o `Release_Win32`).  
     *Nota:* El proyecto espera SFML 2.5.1 en `C:\SFML-2.5.1` (`include`, `build\lib` y variable `PATH`). Si lo tienes instalado en otra carpeta, ajusta esas rutas en **Opciones del proyecto -> Compilación / Enlazado**.
   * **Linux:** `Debug_Linux` (o `Release_Linux`).  
     *Nota:* Requiere tener instalados los paquetes de desarrollo de SFML en el sistema.
3. Compila y ejecuta directamente desde ZinjaI.

> **Importante al ejecutar:** El programa debe iniciar usando la **carpeta raíz del proyecto** como directorio de trabajo (comportamiento predeterminado al ejecutarlo desde ZinjaI), ya que carga los archivos de datos desde `data/` y la tipografía desde `assets/arial.ttf`.

## Estructura del proyecto

* `Proyecto_Rutas.zpr` — Archivo de proyecto para ZinjaI (perfiles Windows y Linux).
* `src/main.cpp` — Punto de entrada de la aplicación gráfica.
* `src/estructuras/` — `GrafoVuelos` (lista de adyacencia), `Dijkstra`, `BellmanFord`, `Prim`, `Kruskal`, `ListaEnlazada`, `Pila`, `Cola`, `ColaPrioridad`, `ConjuntoDisjunto`, `Ordenamiento` (MergeSort).
* `src/arboles/` — `ArbolABB` (ciudades por nombre), `ArbolAVL` (pasajeros por ID con rotaciones LL/RR/LR/RL) y `ArbolMST` (árbol general para Prim y Kruskal).
* `src/modelos/` — `Ciudad`, `Ruta`, `Aeronave`, `Tripulante`, `Vuelo`, `Pasajero`, `Equipaje`.
* `src/sistema/` — `SistemaAerolinea` y `GestorArchivos` (lectura y escritura `.txt` en `data/`).
* `src/visual/` — Interfaz gráfica en SFML (`AplicacionVisual`, visualizadores de grafo/árboles, paneles y modales).
* `data/` — Archivos de persistencia (`ciudades.txt`, `rutas.txt`, `aeronaves.txt`, `tripulacion.txt`, `vuelos.txt`, `pasajeros.txt`).
* `assets/` — Recursos gráficos y tipografía (`arial.ttf`).

## Módulos de la aplicación

* **Cliente:** Ruta más rápida / más barata (Dijkstra, incluyendo ruta económica separada para maletas), exploración de todos los destinos (Bellman-Ford), búsqueda de ciudad en el ABB, reserva y consulta de vuelos por pasajero en el AVL.
* **Administrativo:** Gestión de aeronaves, tripulación, vuelos, pasajeros, ruta de equipaje, generación y despliegue visual de árboles MST (Prim por costo y Kruskal por duración), además del despliegue gráfico de ABB y AVL con recorridos Preorden, Inorden y Postorden.
* **Mantenimientos:** CRUD completo de ciudades, rutas, aeronaves, tripulación, vuelos y pasajeros con guardado automático en `data/`.
