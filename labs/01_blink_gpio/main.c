#include <libopencm3/stm32/rcc.h>   // Configuración del reloj del sistema
#include <libopencm3/stm32/gpio.h>  // Control de los pines GPIO

int main(void) {
    // Configura el sistema de reloj del microcontrolador a 72 MHz
    // usando un cristal externo de 8 MHz.
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    // Habilita el reloj para el puerto GPIOC (donde está conectado el LED)
    rcc_periph_clock_enable(RCC_GPIOC);

    // Configura el pin PC13 como salida digital de 2 MHz en modo push-pull.
    // Este es el pin donde está conectado el LED integrado en la Blue Pill.
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    while (1) {
        // Cambia el estado del pin PC13 (enciende o apaga el LED)
        gpio_toggle(GPIOC, GPIO13);

        // Retardo aproximado usando un bucle ocupado.
        // Cada iteración ejecuta una instrucción NOP (no-operation).
        // El número 800000 fue elegido empíricamente para generar un retardo visible.
        for (int i = 0; i < 800000; i++) {
            __asm__("nop");
        }
    }
}
