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

## 📜 Licencia

MIT para el código propio. FreeRTOS y libopencm3 tienen sus propias licencias compatibles.
