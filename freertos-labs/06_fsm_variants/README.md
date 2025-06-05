# Laboratorio 06 - Variantes de FSM en C

Este ejemplo muestra cuatro maneras distintas de implementar una misma máquina de estados finita (FSM) en C puro. Cada variante vive en su propia carpeta y expone la misma función pública `fsm_handle_event()` para procesar un evento.

## Descripción de la FSM

La máquina de estados controla un LED imaginario y responde a comandos simulados por UART:

* Estados:
  * `OFF`
  * `BLINK_SLOW` (1 Hz)
  * `BLINK_FAST` (5 Hz)
  * `ERROR`
* Eventos de entrada:
  * `'0'` → pasar a `OFF`
  * `'1'` → pasar a `BLINK_SLOW`
  * `'2'` → pasar a `BLINK_FAST`
  * otro caracter → `ERROR`

La transición se produce al invocar `fsm_handle_event()` con un `fsm_event_t`.

## Implementaciones

| Carpeta            | Descripción breve                               |
|--------------------|-------------------------------------------------|
| `switch_case/`     | Estructura clásica con `switch` anidados por estado y evento. |
| `func_pointer/`    | Usa un arreglo de punteros a función para cada estado. |
| `transition_table/`| Implementa una tabla de transición `estado × evento`.| 
| `state_pattern/`   | Aplica el *State Pattern* con funciones asociadas a cada estado.| 

### Ventajas y desventajas

* **Switch/Case**: simple y directo, pero puede crecer rápidamente en tamaño.
* **Punteros a función**: separa el código por estado y facilita extender nuevas acciones.
* **Tabla de transición**: compacta y fácil de visualizar, ideal para FSM simples.
* **State Pattern**: más flexible y escalable, aunque algo verboso para casos pequeños.

### ¿Cuándo usar cada una?

* `switch_case/`: para FSM pequeñas o prototipos rápidos.
* `func_pointer/`: cuando cada estado tiene comportamientos bien diferenciados.
* `transition_table/`: si la FSM es muy regular y las acciones son breves.
* `state_pattern/`: en proyectos grandes donde se requiera extensibilidad.

## Compilación y ejecución

Cada variante cuenta con su propio `Makefile`. Por ejemplo:

```bash
make -C switch_case run
```

Los tests unitarios se encuentran en `tests/` y se ejecutan con:

```bash
make -C tests run
```

## ¿Qué es una FSM?

Una *máquina de estados finita* describe el comportamiento de un sistema que puede estar en un conjunto limitado de estados y cambiar entre ellos según eventos externos. En sistemas embebidos se utiliza para modelar protocolos, manejos de periféricos y secuencias de control de manera clara y mantenible.

