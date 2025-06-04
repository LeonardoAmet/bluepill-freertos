#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_PORT GPIOC
#define LED_PIN  GPIO13
#define USART    USART1

static SemaphoreHandle_t serial_mutex;

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
    gpio_clear(LED_PORT, LED_PIN);
}

static void usart_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);  // TX
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO10);    // RX
    gpio_set(GPIOA, GPIO10); // pull-up

    usart_set_baudrate(USART, 9600);
    usart_set_databits(USART, 8);
    usart_set_stopbits(USART, USART_STOPBITS_1);
    usart_set_mode(USART, USART_MODE_TX_RX);
    usart_set_parity(USART, USART_PARITY_NONE);
    usart_set_flow_control(USART, USART_FLOWCONTROL_NONE);
    usart_enable(USART);
}

static void send_string(const char *s) {
    while (*s)
        usart_send_blocking(USART, *s++);
}

static void writer_task(void *args) {
    const char *msg = (const char *)args;
    while (1) {
        if (xSemaphoreTake(serial_mutex, portMAX_DELAY) == pdPASS) {
            send_string(msg);
            xSemaphoreGive(serial_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void heartbeat_task(void *args) {
    (void)args;
    while (1) {
        gpio_toggle(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    gpio_setup();
    usart_setup();

    serial_mutex = xSemaphoreCreateMutex();
    configASSERT(serial_mutex != NULL);

    xTaskCreate(writer_task, "A", 128, "Task A -> uso de mutex\r\n", 1, NULL);
    xTaskCreate(writer_task, "B", 128, "Task B -> recurso compartido\r\n", 1, NULL);
    xTaskCreate(heartbeat_task, "LED", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    while (1);
}
