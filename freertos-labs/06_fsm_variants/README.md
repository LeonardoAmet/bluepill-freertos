# Laboratorio 06 - Variantes de FSM en C

Este ejemplo muestra cuatro estilos para implementar una misma máquina de estados finita (FSM) en C. Todas las variantes exponen la función común `fsm_handle_event()` y ejecutan la lógica dentro de una tarea de FreeRTOS.

## Descripción de la FSM

La máquina controla un LED imaginario y responde a comandos UART simulados:

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

Cada variante crea una tarea llamada `fsm_task` que procesa una secuencia de comandos predefinida y muestra por consola los cambios de estado.

## Implementaciones

| Carpeta            | Descripción breve |
|--------------------|-------------------|
| `switch_case/`     | `switch` anidados por estado y evento. |
| `func_pointer/`    | Array de punteros a función para cada estado. |
| `transition_table/`| Tabla de transición `estado × evento`. |
| `state_pattern/`   | Implementación del *State Pattern*. |

### Ventajas y desventajas

* **Switch/Case**: sencillo pero puede crecer mucho.
* **Punteros a función**: separa el código por estado y facilita extender acciones.
* **Tabla de transición**: compacta y fácil de visualizar para FSM regulares.
* **State Pattern**: flexible y escalable, algo más verboso.

### ¿Cuándo usar cada una?

* `switch_case/`: FSM pequeñas o prototipos rápidos.
* `func_pointer/`: cuando los estados tienen comportamientos bien diferenciados.
* `transition_table/`: si la FSM es regular y las acciones son breves.
* `state_pattern/`: proyectos grandes que requieran extensibilidad.

## Compilación y ejecución

Cada implementación posee su propio `Makefile` compatible con el resto de los laboratorios. Para compilar, por ejemplo:

```bash
make -C switch_case
```

Los binarios generados pueden flashearse con `make flash` como en los demás ejemplos.

Los tests unitarios se encuentran en `tests/` y se ejecutan con:

```bash
make -C tests run
```

## ¿Qué es una FSM?

Una *máquina de estados finita* describe el comportamiento de un sistema que puede estar en un conjunto limitado de estados y cambiar entre ellos según eventos externos. En sistemas embebidos se utiliza para modelar protocolos y secuencias de control de forma clara y mantenible.
