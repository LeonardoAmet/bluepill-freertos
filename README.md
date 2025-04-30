# bluepill-libopencm3

Este repositorio contiene ejemplos para la placa STM32F103C8T6 (conocida como "Blue Pill") usando la biblioteca [libopencm3](https://github.com/libopencm3/libopencm3). Está pensado como material de apoyo para la materia **Sistemas Embebidos** (UADE).

---

## ✨ ¿Qué es libopencm3?

`libopencm3` es una biblioteca de bajo nivel para microcontroladores ARM Cortex-M. Proporciona acceso directo a los periféricos mediante funciones C, sin dependencias externas ni HALs pesados. No usa drivers automáticos ni configuradores visuales: **el objetivo es que vos controles todo el hardware**.

A diferencia de CMSIS:

- `libopencm3` implementa su propio startup, vector de interrupciones, funciones de inicialización y periféricos.
- Internamente usa CMSIS **solo para definiciones básicas** (nombres de registros, etc.), pero no depende de HAL ni de `system_stm32f1xx.c` ni de `startup.s` personalizados.
- Te permite escribir firmware liviano, portándolo entre placas de ST, NXP, TI, etc. con la misma API.

**Ideal para aprender cómo funciona el microcontrolador sin abstracciones innecesarias.**

---

## 🚀 Cómo clonar y compilar

Este repo incluye `libopencm3` como submódulo. Para clonar correctamente:

```bash
# Clonar el proyecto junto con submódulos
git clone --recurse-submodules https://github.com/usuario/bluepill-libopencm3.git
cd bluepill-libopencm3

# Compilar libopencm3 una vez
cd libopencm3
make
```

Luego podés compilar cualquier ejemplo:

```bash
cd labs/01_blink_gpio
make
```

Y flashear con:

```bash
make flash
```

---

## 📂 Estructura del proyecto

- `labs/` → Contiene los ejemplos por tema (GPIO, UART, PWM, etc.)
- `common/` → Archivos compartidos como el `linker.ld`
- `libopencm3/` → Submódulo con la biblioteca original

Cada lab tiene su propio `Makefile`, pero todos comparten:

- `linker.ld` hecho a medida para STM32F103C8T6
- Vector de interrupciones y startup provistos por `libopencm3`, ya compilados

No necesitas `startup.s` ni `system_stm32f1xx.c` personalizados.

---

## 🔍 Dónde aprender más

- Documentación oficial: https://libopencm3.org
- Código fuente: [https://github.com/libopencm3/libopencm3](https://github.com/libopencm3/libopencm3)
- Ejemplos: mirá los que están dentro de `libopencm3-examples` o este mismo repo
- Archivos clave:
  - `libopencm3/lib/cm3/vector.c` → contiene el reset handler y la lógica de inicio (copiar .data, limpiar .bss, llamar a main)
  - `libopencm3/lib/cm3/vector_nvic.c` → funciones para manipular NVIC (habilitar, deshabilitar, setear prioridades)

---

## 📆 Pensado para la cursada

Este repo está en construcción. Vas a encontrar ejemplos guiados que siguen la metodología de clase, sin automatismos. La idea es que entiendas cada paso del proceso de inicialización, uso de periféricos y compilación. Cualquier cambio que hagas, podés versionarlo con `git`.

Si tenés dudas, revisá:
- El repo de la cátedra: [https://github.com/LeonardoAmet/bluepill-cmsis-drivers](https://github.com/LeonardoAmet/bluepill-cmsis-drivers)
- Esta guía paso a paso
- Los apuntes de clase

---

✅ Primera prueba recomendada: `labs/01_blink_gpio`

- Enciende y apaga el LED de PC13
- Usa funciones de `libopencm3` para habilitar GPIOC, configurar el pin y hacer toggle
- No usa delays de HAL: todo es bajo nivel y controlado por vos





