# Laboratorio 06 - Variantes de FSM en C

Este ejemplo muestra cuatro estilos para implementar una misma máquina de estados finita (FSM) en C. Todas las variantes exponen la función común `fsm_handle_event()` y ejecutan la lógica dentro de una tarea de FreeRTOS.

## Descripción de la FSM

La máquina controla el LED onblard y responde a comandos UART simulados:

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

Cada variante crea una tarea llamada `fsm_task` que espera caracteres recibidos en `USART1` y ejecuta la transición correspondiente. Un
`led_task` adicional parpadea el LED conectado a `PC13` con la velocidad correspondiente a cada estado.

## Implementaciones

| Carpeta            | Descripción breve |
|--------------------|-------------------|
| `switch_case/`     | `switch` anidados por estado y evento. |
| `function_pointer/`| Array de punteros a función para cada estado. |
| `transition_table/`| Tabla de transición `estado × evento`. |
| `state_pattern/`   | Implementación del *State Pattern*. |

### Ventajas y desventajas

* **Switch/Case**: sencillo pero puede crecer mucho.
* **Punteros a función**: separa el código por estado y facilita extender acciones.
* **Tabla de transición**: compacta y fácil de visualizar para FSM regulares.
* **State Pattern**: flexible y escalable, algo más verboso.

### ¿Cuándo usar cada una?

* `switch_case/`: FSM pequeñas o prototipos rápidos.
* `function_pointer/`: cuando los estados tienen comportamientos bien diferenciados.
* `transition_table/`: si la FSM es regular y las acciones son breves.
* `state_pattern/`: proyectos grandes que requieran extensibilidad.

## Compilación y ejecución

Cada implementación posee su propio `Makefile` compatible con el resto de los laboratorios. Se compila con `make`.
Los binarios generados pueden flashearse con `make flash` como en los demás ejemplos.
Abrir una terminar (PuTTY o similar) y utilizar `0`, `1` y `2` para transionar entre estados. Cualquier otro caracter nos lleva a un estador de "error", el cual simplemente apaga el LED.