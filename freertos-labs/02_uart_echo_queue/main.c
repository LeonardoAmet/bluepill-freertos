#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define LED_PORT GPIOC
#define LED_PIN  GPIO13



static QueueHandle_t rx_queue;

static void usart_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    // PA9 = TX, PA10 = RX
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO10);

    gpio_set(GPIOA, GPIO10); // pull-up

    usart_set_baudrate(USART1, 9600);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable(USART1);

    /*
     * IMPORTANTE:
     * FreeRTOS solo permite usar funciones como xQueueSendFromISR()
     * desde interrupciones cuya prioridad sea numéricamente mayor o igual
     * a configMAX_SYSCALL_INTERRUPT_PRIORITY.
     *
     * En STM32F103, los valores de prioridad van de 0x00 (alta) a 0xFF (baja).
     * Si no configuramos explícitamente esta prioridad, se usa 0 (demasiado alta),
     * lo que hace que FreeRTOS se cuelgue con configASSERT().
     */
    nvic_set_priority(NVIC_USART1_IRQ, 0x60); // debe ser >= (5 << 4) = 0x50
    nvic_enable_irq(NVIC_USART1_IRQ);
    usart_enable_rx_interrupt(USART1);
}


void usart1_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (usart_get_flag(USART1, USART_SR_RXNE)) {
        char c = usart_recv(USART1);
        xQueueSendFromISR(rx_queue, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


static void echo_task(void *args) {
    char c;
    while (1) {
        if (xQueueReceive(rx_queue, &c, portMAX_DELAY) == pdPASS) {
            if (c == '\r' || c == '\n') {
                usart_send_blocking(USART1, '\r');
                usart_send_blocking(USART1, '\n');
            }
            else
                usart_send_blocking(USART1, c);             
        }
    }
}

static void heartbeat_task(void *args) {
    while (1) {
        gpio_toggle(GPIOC, GPIO13);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);
    
}

#ifndef UNIT_TEST
int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    rx_queue = xQueueCreate(32, sizeof(char));  
    xTaskCreate(echo_task, "Echo", 128, NULL, 1, NULL);
    xTaskCreate(heartbeat_task, "Blink", 128, NULL, 1, NULL);

    gpio_setup();
    usart_setup();

    usart_send_blocking(USART1, '>');
       
    vTaskStartScheduler();

    while (1); // no debería llegar nunca
}
#endif // UNIT_TEST
