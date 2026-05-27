> Este repositorio contiene ejemplos con FreeRTOS sobre STM32F103C8T6 y libopencm3.
> Los ejemplos bare-metal están en `bluepill-libopencm3`.

# bluepill-freertos

Este repositorio reúne laboratorios de FreeRTOS para la placa Blue Pill (STM32F103C8T6) usando libopencm3 como capa de acceso al hardware.

Está pensado como continuación del recorrido bare-metal:

- `bluepill-libopencm3` para GPIO, UART, timers, EXTI y antirrebote,
- `bluepill-freertos` para tareas, colas, mutexes, semáforos y variantes de FSM con RTOS.

## Estructura del proyecto

```text
/common         -> linker.ld y FreeRTOSConfig.h compartidos
/freertos       -> kernel de FreeRTOS
/libopencm3     -> submódulo libopencm3
/freertos-labs/ -> proyectos independientes basados en FreeRTOS
```

Cada subdirectorio dentro de `freertos-labs/` tiene su propio `Makefile`. La configuración del kernel se centraliza en `common/FreeRTOSConfig.h`.

## Compilación

Ejemplo:

```bash
cd freertos-labs/01_blink_task
make
```

Para flashear:

```bash
make flash
```

## Tests

El repositorio incluye tests unitarios con [Unity](https://www.throwtheswitch.org/unity). La idea es poder validar parte de la lógica en la PC usando stubs de funciones de libopencm3 y de FreeRTOS.

Para correrlos:

```bash
make test
```

También hay integración en GitHub Actions para compilar los proyectos y ejecutar los tests automáticamente.

## Detalles técnicos importantes

### FreeRTOS y la tabla de vectores

FreeRTOS necesita que estos handlers estén exactamente en la tabla de vectores:

- `vPortSVCHandler`
- `xPortPendSVHandler`
- `xPortSysTickHandler`

libopencm3, por su parte, define handlers `__weak` con otros nombres:

- `sv_call_handler`
- `pend_sv_handler`
- `sys_tick_handler`

### Solución adoptada

En este repo no se redefinen funciones ni se usan aliases. Se resuelve con macros en `FreeRTOSConfig.h`:

```c
#define vPortSVCHandler     sv_call_handler
#define xPortPendSVHandler  pend_sv_handler
#define xPortSysTickHandler sys_tick_handler
```

Eso evita conflictos de símbolos y mantiene la tabla de vectores coherente con libopencm3.

## Dependencias

- [libopencm3](https://github.com/libopencm3/libopencm3)
- [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel)
- `arm-none-eabi-gcc`
- `make`
- `openocd`
- `gdb-multiarch`

## Debug con VS Code

El repositorio puede usarse con Cortex-Debug en Visual Studio Code.

Ejemplo de configuración esperada:

```json
"executable": "${workspaceFolder}/freertos-labs/01_blink_task/bin/main.elf",
"configFiles": [
    "interface/stlink.cfg",
    "target/stm32f1x.cfg"
]
```

## Mecanismos de sincronización en FreeRTOS

FreeRTOS ofrece varias primitivas para resolver exclusión mutua, señalización y paso de datos.

- **Mutex**: protege recursos compartidos y tiene herencia de prioridad.
- **Recursive Mutex**: igual que el mutex, pero puede ser tomado varias veces por la misma tarea.
- **Semáforo binario**: señalización simple entre tareas o desde ISR.
- **Semáforo de conteo**: acumula múltiples eventos.
- **Queue**: paso de datos FIFO entre tareas o desde ISR.
- **Event group**: sincronización basada en bits de evento.
- **Delay / Sleep**: suspensión temporal de tareas; no es una primitiva de sincronización entre contextos.

### Resumen comparativo

| Mecanismo | Uso típico | Herencia de prioridad | Uso desde ISR |
| --- | --- | --- | --- |
| Mutex | Exclusión mutua | Sí | No |
| Recursive Mutex | Exclusión mutua recursiva | Sí | No |
| Semáforo binario | Señalización | No | Sí |
| Semáforo de conteo | Acumulación de eventos | No | Sí |
| Queue | Paso de datos | No | Sí |
| Event group | Sincronización por eventos | No | Sí |
| Delay / Sleep | Suspensión temporal | No | No |

Notas:

- solo los mutex implementan herencia de prioridad,
- queues y semáforos pueden usarse desde interrupciones con sus variantes `FromISR()`,
- queues y semáforos cumplen roles diferentes: una queue transfiere datos; un semáforo solo sincroniza.

Referencias:

- [Mutexes - FreeRTOS](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/04-Mutexes)
- [Binary Semaphores](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/02-Binary-semaphores)
- [Counting Semaphores](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/03-Counting-semaphores)
- [Queues](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/01-Queues)
- [Event Groups](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/06-Event-groups)

## Licencia

El código propio del repositorio usa licencia MIT. FreeRTOS y libopencm3 mantienen sus licencias respectivas.
