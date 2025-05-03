#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define UMBRAL_ESTABILIDAD 2000  // cantidad de lecturas consecutivas necesarias

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13); // Apaga LED

    gpio_set(GPIOA, GPIO0);  // pull-up
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);

    int estado_anterior = 1;
    int contador_estabilidad = 0;
    int estado_estable = 1;
    int led_on = 0;

    while (1) {
        int lectura_actual = gpio_get(GPIOA, GPIO0);

        // Si cambia respecto al estado estable, resetea contador
        if (lectura_actual != estado_estable) {
            contador_estabilidad++;
            if (contador_estabilidad >= UMBRAL_ESTABILIDAD) {
                estado_estable = lectura_actual;
                contador_estabilidad = 0;

                // Si el cambio fue flanco descendente (1 → 0)
                if (estado_anterior && !estado_estable) {
                    led_on = !led_on;
                    if (led_on)
                        gpio_clear(GPIOC, GPIO13);
                    else
                        gpio_set(GPIOC, GPIO13);
                }
                estado_anterior = estado_estable;
            }
        } else {
            contador_estabilidad = 0; // reinicia si no hay cambio
        }
    }
}
// Fin del programa