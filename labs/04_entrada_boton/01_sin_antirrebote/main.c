#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(void) {
    // Configura el sistema a 72 MHz utilizando cristal externo de 8 MHz
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    // Habilita el reloj para los puertos GPIOC (LED) y GPIOA (botón)
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configura PC13 como salida push-pull (LED)
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    // Habilita resistencia de pull-up interna en PA0
    gpio_set(GPIOA, GPIO0); // Setea el bit para activar pull-up
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0); // Modo entrada con pull-up/pull-down

    int estado_anterior = 1; // Pull-up: el estado de reposo es alto (1)
    int led_on = 0;

    while (1) {
        int estado_actual = gpio_get(GPIOA, GPIO0);

        // Detecta flanco descendente (de 1 a 0): presión de botón
        if (estado_anterior && !estado_actual) {
            led_on = !led_on;

            if (led_on) {
                gpio_clear(GPIOC, GPIO13); // Enciende LED (activo en bajo)
            } else {
                gpio_set(GPIOC, GPIO13);   // Apaga LED
            }
        }

        estado_anterior = estado_actual; // Actualiza estado para próxima iteración
    }
}
// Fin del código