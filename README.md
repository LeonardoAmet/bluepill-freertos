> 🧠 Este repositorio contiene únicamente ejemplos que utilizan **FreeRTOS** sobre STM32F103C8T6 y **libopencm3**.
> No incluye ejemplos bare-metal. Para esos, consultá el repositorio `bluepill-libopencm3`.

# Proyecto Base: FreeRTOS + libopencm3 en STM32F103C8T6

Este repositorio contiene una base mínima pero funcional para trabajar con **FreeRTOS** sobre un microcontrolador **STM32F103C8T6**, usando la biblioteca **libopencm3**. Está pensado como punto de partida para proyectos con múltiples tareas, sincronización, y periféricos del microcontrolador.

---

## 🧱 Estructura del proyecto

```
/common                → linker.ld
/freertos              → código fuente de FreeRTOS
/libopencm3            → submódulo con libopencm3
/freertos-labs/        → proyectos basados en FreeRTOS
```

Cada subcarpeta en `freertos-labs/` es un proyecto independiente con su propio `Makefile`.

---

## ⚙️ Compilación

Para compilar un proyecto:

```bash
cd freertos-labs/01_blink_task
make
```

Esto genera el binario `.elf`, `.bin` y `.hex` en la carpeta `bin/`.

Para flashear con OpenOCD:

```bash
make flash
```

## 🧪 Tests

Se provee un entorno de **tests unitarios** basado en [Unity](https://www.throwtheswitch.org/unity). Estos tests permiten
ejecutar en la PC lógica que normalmente correría en el microcontrolador, utilizando implementaciones ficticias
(*stubs*) de las funciones de `libopencm3` y de FreeRTOS. De esta manera es posible validar el comportamiento de las
tareas sin necesidad de hardware real.

Para compilar y correr todos los tests simplemente ejecutá:

```bash
make test
```

Esto compila los test dentro de `tests/` y corre el binario resultante.

Además, el repositorio cuenta con una configuración de **GitHub Actions** que
compila los proyectos y ejecuta estos tests de forma automática en cada push o
pull request. El workflow se encuentra en `.github/workflows/build.yml`.

---

## 🧠 Detalles técnicos importantes

### ✔️ FreeRTOS y la tabla de vectores

FreeRTOS necesita que los siguientes handlers estén **exactamente en la tabla de vectores** del micro:

* `vPortSVCHandler`
* `xPortPendSVHandler`
* `xPortSysTickHandler`

Sin embargo, libopencm3 define sus propios handlers (`sv_call_handler`, `pend_sv_handler`, `sys_tick_handler`) como `__weak`.

### ✅ Solución adoptada

Este proyecto **no redefine funciones ni usa `alias()`**. En cambio, usa una técnica más simple: **macros en `FreeRTOSConfig.h`** que redirigen los nombres requeridos por FreeRTOS a los handlers definidos por libopencm3:

```c
#define vPortSVCHandler     sv_call_handler
#define xPortPendSVHandler  pend_sv_handler
#define xPortSysTickHandler sys_tick_handler
```

Esto asegura que FreeRTOS funcione correctamente sin conflictos de símbolos ni problemas de linker.

---

## 📌 Dependencias

* [libopencm3](https://github.com/libopencm3/libopencm3) (agregado como submódulo)
* [FreeRTOS kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel)
* Toolchain: `arm-none-eabi-gcc`, `make`, `openocd`, `gdb-multiarch`

---

## 🔧 Debug con VS Code

Este proyecto utiliza la extensión Cortex-Debug para Visual Studio Code, la cual permite depurar firmware ARM Cortex-M a través de gdb-multiarch y OpenOCD.

Incluye configuración en `.vscode/launch.json` para depuración con `gdb-multiarch` y `OpenOCD`. Verificá que el archivo ELF y los paths sean correctos para tu proyecto:

```json
"executable": "${workspaceFolder}/freertos-labs/01_blink_task/bin/main.elf",
"configFiles": [
    "interface/stlink.cfg",
    "target/stm32f1x.cfg"
]
```

---
### 🔧 Mecanismos de sincronización en FreeRTOS

En sistemas de tiempo real como FreeRTOS, los mecanismos de sincronización son fundamentales para coordinar el acceso a recursos compartidos, sincronizar tareas y responder a eventos generados por interrupciones. FreeRTOS ofrece distintas primitivas que permiten implementar desde exclusión mutua hasta transferencia de datos o señalización de eventos.

A continuación se presenta una descripción breve de cada uno:

* **Mutex**: se utiliza para proteger recursos compartidos entre tareas. Solo una tarea puede tener el mutex a la vez. Implementa herencia de prioridad, lo que lo hace ideal para evitar la inversión de prioridades.
* **Recursive Mutex**: similar al mutex, pero permite que una misma tarea lo tome múltiples veces (por ejemplo, en funciones recursivas o anidadas).
* **Semáforo binario**: permite la señalización de eventos entre tareas o desde interrupciones. No protege recursos ni tiene herencia de prioridad. Es útil, por ejemplo, para indicar que ocurrió un evento.
* **Semáforo de conteo**: permite acumular eventos (por ejemplo, varias interrupciones), ya que su contador puede incrementarse varias veces. No transfiere datos, pero es útil para sincronizar con tareas que procesan eventos múltiples.
* **Queue**: permite pasar datos entre tareas o desde interrupciones de manera segura y ordenada (FIFO). También sincroniza, pero su función principal es el paso de datos.
* **Event group**: permite que una tarea espere a que se cumplan una o varias condiciones (bits). Es útil para coordinar tareas que deben reaccionar a múltiples eventos.
* **Delay / Sleep**: no es un mecanismo de sincronización entre tareas, pero permite suspender la ejecución de una tarea por un período definido.

### 📋 Resumen comparativo

| Mecanismo              | Uso típico                          | ¿Herencia de prioridad? | ¿Desde ISR? |
| ---------------------- | ----------------------------------- | ----------------------- | ----------- |
| **Mutex**              | Exclusión mutua                     | ✅ Sí                    | ❌ No        |
| **Recursive Mutex**    | Exclusión mutua (recursiva)         | ✅ Sí                    | ❌ No        |
| **Semáforo binario**   | Señalización (evento)               | ❌ No                    | ✅ Sí        |
| **Semáforo de conteo** | Acumulación de eventos              | ❌ No                    | ✅ Sí        |
| **Queue**              | Paso de datos                       | ❌ No                    | ✅ Sí        |
| **Event group**        | Sincronización de múltiples eventos | ❌ No                    | ✅ Sí        |
| **Delay / Sleep**      | Suspensión temporal de tareas       | ❌ No                    | ❌ No        |

**Notas:**

* ✅ **Herencia de prioridad**: permite que una tarea de baja prioridad que posee un recurso compartido herede temporalmente la prioridad de una tarea más prioritaria que espera ese recurso. Esto previene la inversión de prioridades.
* **Mutex** y **Recursive Mutex** son los únicos mecanismos de FreeRTOS que implementan herencia de prioridad.
* Los **semáforos** y **queues** son seguros para usar en ISRs con sus funciones terminadas en `FromISR()`.
* Las **queues** permiten pasar datos y sincronizar a la vez, mientras que los **semáforos** sólo sincronizan.

Para más información:

* [Mutexes - FreeRTOS](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/04-Mutexes)
* [Semáforos binarios](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/02-Binary-semaphores)
* [Semáforos de conteo](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/03-Counting-semaphores)
* [Queues](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/01-Queues)
* [Event Groups](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/06-Event-groups)

---

## 📜 Licencia

MIT para el código propio. FreeRTOS y libopencm3 tienen sus propias licencias compatibles.
