#include <libopencm3/stm32/rcc.h>     // Configuración del reloj del sistema
#include <libopencm3/stm32/gpio.h>    // Control de pines GPIO

// Función de retardo bloqueante mediante bucle ocupado
void delay(volatile int count) {
    while (count--) {
        __asm__("nop"); // No Operation: mantiene la CPU ocupada
    }
}

int main(void) {
    // Configura el sistema de reloj a 72 MHz usando cristal externo de 8 MHz
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    // Habilita el reloj para los puertos GPIOC (LED) y GPIOA (botón)
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configura el pin PC13 como salida push-pull (LED)
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13); // Apaga el LED (activo en bajo)

    // Activa resistencia de pull-up interna en PA0
    gpio_set(GPIOA, GPIO0); // Setea el pin en alto
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);

    int led_on = 0; // Estado del LED (0 = apagado, 1 = encendido)

    while (1) {
        // Detecta presión del botón (nivel bajo en PA0)
        if (gpio_get(GPIOA, GPIO0) == 0) {
            // Retardo de aproximadamente 20 ms para filtrar rebotes
            delay(100000);

            // Verifica si el botón sigue presionado después del retardo
            if (gpio_get(GPIOA, GPIO0) == 0) {
                led_on = !led_on; // Alterna el estado del LED

                if (led_on) {
                    gpio_clear(GPIOC, GPIO13); // Enciende LED
                } else {
                    gpio_set(GPIOC, GPIO13);   // Apaga LED
                }

                // Espera a que se suelte el botón antes de permitir otro toggle
                while (gpio_get(GPIOA, GPIO0) == 0);
            }
        }
    }
}
// Fin del programa