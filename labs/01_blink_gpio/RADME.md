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

> 🔍 El bucle con `nop` genera un retardo visible, pero no preciso. A 72 MHz, este retardo es notoriamente más corto que medio segundo. Esta técnica es útil solo como ejercicio básico, ya que depende de muchos factores y no es precisa ni estable.. No es una técnica precisa ni recomendable para código de producción, pero es útil como primer ejercicio.

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
