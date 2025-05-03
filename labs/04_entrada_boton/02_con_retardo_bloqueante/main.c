#include <libopencm3/stm32/rcc.h>     // Configuración del reloj del sistema
#include <libopencm3/stm32/gpio.h>    // Control de pines GPIO

// Función de retardo bloqueante mediante bucle ocupado (~20 ms aprox)
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

    // Activa resistencia de pull-up interna en PA0 (entrada botón)
    gpio_set(GPIOA, GPIO0); // Pull-up interno
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);

    int estado_anterior = 1; // Estado inicial: botón no presionado
    int led_on = 0;

    while (1) {
        int estado_actual = gpio_get(GPIOA, GPIO0);

        // Detecta flanco descendente (1 → 0)
        if (estado_anterior && !estado_actual) {
            delay(100000); // Retardo bloqueante (~20 ms)

            // Verifica que el botón siga presionado después del retardo
            if (gpio_get(GPIOA, GPIO0) == 0) {
                led_on = !led_on;

                if (led_on) {
                    gpio_clear(GPIOC, GPIO13); // Enciende LED
                } else {
                    gpio_set(GPIOC, GPIO13);   // Apaga LED
                }
            }
        }

        estado_anterior = estado_actual; // Actualiza para el próximo ciclo
    }
}
