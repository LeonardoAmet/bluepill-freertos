#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "FreeRTOS.h"
#include "task.h"

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

void led_task(void *args) {
    while (1) {
        gpio_toggle(GPIOC, GPIO13);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

#ifndef UNIT_TEST
int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);    

    gpio_setup();

    BaseType_t ok = xTaskCreate(led_task, "LED", 128, NULL, 1, NULL);
    configASSERT(ok == pdPASS);

    vTaskStartScheduler();

    // Si llega acá, el scheduler no arrancó
    gpio_clear(GPIOC, GPIO13);  // LED ON
    while (1);  // Esperá congelado
}
#endif // UNIT_TEST

