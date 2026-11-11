# Reglas de Prefijos para los Prompts

Este proyecto utiliza un sistema estricto de **prefijos al inicio de cada prompt** para controlar cuándo el asistente solo debe explicar/analizar y cuándo tiene permiso para modificar archivos o ejecutar acciones.

---

## 1. Los 3 Prefijos Oficiales

Todo prompt debe comenzar idealmente con uno de los siguientes tres prefijos (en mayúsculas y seguido de dos puntos `:`):

### `EXPLICA:` — Modo Solo Lectura y Análisis
* **Comportamiento:** El asistente entra en modo estrictamente de **solo lectura**.
* **Qué SÍ puede hacer:**
  * Leer archivos del proyecto y analizar código o logs.
  * Responder dudas teóricas o prácticas, explicar cómo funciona una parte del código, comparar enfoques o proponer soluciones/planes en texto.
* **Qué NO puede hacer (prohibido):**
  * Crear, modificar, renombrar o eliminar archivos del proyecto.
  * Ejecutar comandos que alteren el repositorio o el código (como commits, scripts que escriban en el proyecto, instalaciones o cambios de estado).
* **Cuándo usarlo:** Cuando quieras entender el código, pedir una revisión, planear antes de programar o estudiar sin riesgo de que se toque ningún archivo.

---

### `HAZ:` — Modo Acción (Lectura + Escritura)
* **Comportamiento:** El asistente tiene permiso para **ejecutar cambios reales**.
* **Qué SÍ puede hacer:**
  * Todo lo permitido en `EXPLICA:` (leer, analizar y explicar lo que hizo o va a hacer).
  * Crear, editar o eliminar archivos del proyecto según lo solicitado.
  * Ejecutar comandos necesarios (compilar, correr pruebas, formatear, commits, etc.) cuando forme parte de la tarea pedida.
* **Límite:** Solo debe realizar las acciones solicitadas, respetando las reglas acumuladas con `AÑADE CONTEXTO:`.
* **Cuándo usarlo:** Cuando ya aprobaste una idea o tienes claro exactamente qué cambio quieres que el asistente programe o ejecute.

---

### `AÑADE CONTEXTO:` — Modo Memoria y Directrices del Proyecto
* **Comportamiento:** El asistente registra nuevas reglas, restricciones o información clave del proyecto para respetarlas en **todas las respuestas y cambios futuros**.
* **Qué SÍ hace:**
  * Memoriza estándares del proyecto (lenguaje/versión, librerías permitidas o prohibidas, estilo de código, reglas de comentarios, entorno de compilación, arquitectura, etc.).
  * Confirma que entendió la regla y la mantiene activa a lo largo del trabajo.
* **Qué NO hace:** No modifica archivos de código del proyecto por sí solo (a menos que luego envíes un `HAZ:`).
* **Cuándo usarlo:** Al iniciar el proyecto o cada vez que surja una nueva restricción o preferencia permanente.

---

## 2. Regla por Defecto (Protección contra cambios accidentales)

> **Si un prompt NO empieza con `AÑADE CONTEXTO:`, `HAZ:` ni `EXPLICA:`, el asistente DEBE asumir por defecto el prefijo `EXPLICA:`.**

### ¿Por qué existe esta regla?
Sirve como seguro de protección: si envías una pregunta rápida o una idea suelta y olvidas poner el prefijo, el asistente **no tocará ni modificará tus archivos** hasta que le des una instrucción explícita con `HAZ:`.

---

## 3. Tabla Resumen de Comportamiento

| Prefijo | ¿Lee y analiza? | ¿Explica? | ¿Crea / edita archivos? | ¿Guarda reglas permanentes? |
| :--- | :---: | :---: | :---: | :---: |
| **`EXPLICA:`** | Sí | Sí | **No** | No |
| **`HAZ:`** | Sí | Sí | **Sí** | No |
| **`AÑADE CONTEXTO:`** | Si hace falta | Confirma | **No** | **Sí** |
| *(Sin prefijo)* | Sí | Sí | **No** *(asume `EXPLICA:`)* | No |

---

## 4. Ejemplos de Uso

* **Para definir reglas al inicio del proyecto:**
  ```text
  AÑADE CONTEXTO: El proyecto usa Python 3.11. No uses librerías externas fuera de requirements.txt y no agregues comentarios obvios en el código.
  ```

* **Para preguntar o planear sin que toque nada:**
  ```text
  EXPLICA: Revisa el archivo main.py y dime por qué falla la conexión a la base de datos y cómo deberíamos solucionarlo.
  ```

* **Para mandar un mensaje rápido sabiendo que está en modo seguro:**
  ```text
  ¿Qué hace exactamente esta función de ordenamiento?
  ```
  *(Al no tener prefijo, el asistente asume `EXPLICA:` y solo responde sin editar código).*

* **Para autorizar cambios reales en el código:**
  ```text
  HAZ: Implementa la corrección que propusiste en main.py y ejecuta las pruebas para verificar que pase.
  ```

---

## 5. Instrucción Directa para el Asistente

A partir de este momento, adopta estas reglas como obligatorias en este proyecto:
1. Evalúa el prefijo al inicio de cada prompt (`EXPLICA:`, `HAZ:` o `AÑADE CONTEXTO:`).
2. Si el prompt no incluye ninguno de esos tres prefijos al inicio, trátalo automáticamente como `EXPLICA:` (modo de solo lectura y análisis, sin modificar archivos del proyecto).
3. Respeta de forma acumulativa todas las directrices recibidas mediante `AÑADE CONTEXTO:`.
