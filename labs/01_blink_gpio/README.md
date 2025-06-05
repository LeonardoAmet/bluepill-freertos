# Ejemplo Blink GPIO con STM32 (Blue Pill) usando libopencm3

Este es el proyecto más simple para comenzar con STM32: hacer parpadear un LED conectado al pin PC13. Este ejemplo sirve para entender cómo configurar un pin como salida digital y cómo generar retardos mediante un bucle bloqueante (busy wait).

---

## Tabla de contenidos

* [Objetivo del ejemplo](#objetivo-del-ejemplo)
* [Configuración de hardware](#configuración-de-hardware)
* [Funcionamiento del programa](#funcionamiento-del-programa)
* [Compilación y grabación](#compilación-y-grabación)
* [Notas adicionales](#notas-adicionales)

---

## Objetivo del ejemplo

* Habilitar el reloj para el puerto GPIOC
* Configurar el pin PC13 como salida digital (push-pull, 2 MHz)
* Alternar su estado dentro de un bucle infinito para generar parpadeo

---

## Configuración de hardware

* Este ejemplo está pensado para la placa **Blue Pill** (STM32F103C8T6)
* El pin **PC13** está conectado al LED integrado en la placa (generalmente LED verde)
* El LED está conectado en modo activo bajo (enciende en nivel lógico bajo)

---

## Funcionamiento del programa

El programa configura el pin PC13 como salida digital y lo alterna continuamente dentro de un bucle infinito. Entre cada cambio de estado, se introduce un retardo mediante un bucle de instrucciones vacías (`nop`) que ralentiza la ejecución, permitiendo que el LED parpadee a una frecuencia visible.

```c
int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    while (1) {
        gpio_toggle(GPIOC, GPIO13);
        for (int i = 0; i < 800000; i++) {
            __asm__("nop");
        }
    }
}
```

> 🔍 El bucle con `nop` genera un retardo visible, pero no preciso. A 72 MHz, este retardo es notoriamente más corto que medio segundo. Esta técnica es útil solo como ejercicio básico, ya que depende de muchos factores y no es precisa ni estable. No es una técnica precisa ni recomendable para código de producción, pero es útil como primer ejercicio.

---

## Compilación y grabación

```bash
make         # Compila el proyecto
make flash   # Graba el firmware usando OpenOCD
```

Asegurate de tener instalados: `arm-none-eabi-gcc`, `make` y `OpenOCD`.

---

## Notas adicionales

* ⚠️ **Nota:** Esta versión del ejemplo incluye una línea explícita para configurar el sistema a 72 MHz utilizando un cristal externo de 8 MHz: `rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);`. Si se omite, el sistema funcionará con el reloj interno (HSI), generando un retardo mucho mayor al esperado.
* Se usa `gpio_toggle()` para invertir el estado del pin
* El uso de retardos con bucles de NOP solo es válido como técnica didáctica
# Parpadeo de LED con retardo por software

Este ejemplo muestra cómo generar un parpadeo simple en el LED de la placa Blue Pill utilizando un bucle de retardo por software. Es una de las formas más sencillas de generar una temporización sin depender de temporizadores ni interrupciones.

## 🎯 Objetivo

* Configurar un pin como salida digital.
* Alternar su estado en un bucle infinito.
* Introducir un retardo usando instrucciones NOP para observar el parpadeo.

## 🧠 Teoría: GPIO en STM32

Los pines GPIO (*General Purpose Input/Output*) permiten configurar cada pin del microcontrolador como entrada o salida digital. En STM32, cada pin se configura mediante tres aspectos clave:

* **Modo de funcionamiento**: entrada, salida, función alternativa o analógica.
* **Velocidad de salida**: en MHz (generalmente 2, 10 o 50 MHz).
* **Tipo de salida**: push-pull o open-drain.

Para configurar un pin como salida push-pull:

1. Se habilita el reloj del puerto correspondiente (por ejemplo, GPIOC).
2. Se usa `gpio_set_mode(...)` con los parámetros adecuados.
3. Se usa `gpio_set(...)`, `gpio_clear(...)` o `gpio_toggle(...)` para controlar su valor lógico.

En este ejemplo se configura el pin **PC13** como salida digital push-pull de 2 MHz.

## ⚙️ Descripción del funcionamiento

El programa realiza los siguientes pasos:

1. Configura el sistema de reloj a 72 MHz utilizando un cristal externo de 8 MHz.
2. Habilita el reloj del puerto GPIOC.
3. Configura el pin PC13 como salida push-pull.
4. En un bucle infinito:

   * Alterna el estado del pin PC13 (encendiendo o apagando el LED).
   * Ejecuta un bucle de retardo (donde se suelen usar instrucciones NOP, aunque no son estrictamente necesarias).

El LED se encuentra conectado al pin **PC13**. Este LED está activo en bajo (se enciende con un 0 lógico).

## ⏳ Sobre el retardo

El retardo se genera mediante un bucle con 800000 iteraciones, utilizando la instrucción `__asm__("nop")`. Esto mantiene a la CPU ocupada sin realizar ninguna operación útil (bucle bloqueante). El retardo exacto dependerá de la frecuencia del sistema (en este caso, 72 MHz).

> 📌 Esta técnica es útil para pruebas rápidas, pero **no es precisa** ni recomendable para proyectos complejos.

## 🔧 Requisitos

* Placa STM32F103C8T6 (Blue Pill)
* Toolchain `arm-none-eabi-gcc`
* Biblioteca libopencm3 como submódulo
* `make` y `openocd`

## 🧪 Compilación y carga

```bash
make
make flash
```

## 📚 Observación adicional

* Esta técnica se conoce como **espera activa** o *busy wait*, y es **bloqueante**: impide que el microcontrolador realice otras tareas durante el retardo.
* Más adelante se introducen técnicas con temporizadores (Timers) que permiten generar retardos precisos y no bloqueantes.
