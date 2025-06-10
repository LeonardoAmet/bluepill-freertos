# Laboratorio 06 - Variantes de implementación de FSM

Este laboratorio ilustra cuatro maneras distintas de programar una misma máquina de estados finita (FSM) en lenguaje C.  Todas las variantes comparten la interfaz declarada en `include/fsm.h` y ejecutan su lógica dentro de una tarea de FreeRTOS.

## Descripción de la FSM

La FSM controla el LED integrado y responde a comandos recibidos por UART. Los estados posibles son:

- `OFF`
- `BLINK_SLOW` (1&nbsp;Hz)
- `BLINK_FAST` (5&nbsp;Hz)
- `ERROR`

Los eventos que disparan transiciones se obtienen de los caracteres `0`, `1` y `2` recibidos. Cualquier otro valor se considera inválido y lleva al estado `ERROR`.

Cada proyecto crea una tarea `fsm_task` que lee la UART y llama a `fsm_handle_event()`. Aparte existe `led_task`, responsable de parpadear `PC13` con la velocidad asociada al estado actual.

## Implementaciones detalladas

### 1. `switch_case/`
Implementa la máquina mediante `switch` anidados. El primer `switch` selecciona el estado actual y dentro de cada caso otro `switch` resuelve el evento recibido. Es la forma más directa de expresar la lógica, aunque a medida que crecen los estados y eventos el código se vuelve difícil de seguir.

### 2. `func_pointer/`
Utiliza un arreglo de punteros a función, donde cada posición corresponde a un estado. `fsm_handle_event()` simplemente invoca `handlers[current_state](event)` y cada función realiza las transiciones necesarias. Esta organización reduce los `switch` y permite separar claramente el código de cada estado.

### 3. `transition_table/`
En esta variante se define una matriz `estado × evento` donde cada entrada indica el próximo estado. `fsm_handle_event()` consulta la tabla y luego llama a `enter_state()` para ejecutar las acciones asociadas (mostrar mensajes, reiniciar temporizadores, etc.). Es muy compacta y conveniente cuando la FSM es regular y no requiere acciones complejas por transición.

### 4. `state_pattern/`
Se inspira en el *State Pattern*: cada estado tiene asociada una función que implementa su comportamiento. `fsm_handle_event()` delega en la función correspondiente y el estado puede cambiarse modificando `current_state`. En proyectos grandes resulta flexible, pues permite agregar funciones de entrada/salida o incluso dividir el código de cada estado en archivos separados.

## Ventajas y desventajas (resumen)

- **Switch/Case:** simple de entender, pero el tamaño del código crece rápidamente.
- **Punteros a función:** organiza mejor las funciones y facilita extender acciones.
- **Tabla de transición:** muy compacta; ideal para FSM regulares y pequeñas.
- **State Pattern:** modular y escalable a costa de algo más de código.

## Compilación y ejecución

Cada carpeta contiene su propio `Makefile`. Para compilar una implementación:

```bash
make
```

El binario resultante puede flashearse con:

```bash
make flash
```

Al ejecutar, envíe por la UART los caracteres `0`, `1` y `2` para cambiar entre estados. Cualquier otro carácter lleva al estado de error donde el LED permanece apagado.

