#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_PORT GPIOC
#define LED_PIN  GPIO13
#define USART    USART1

#define BUF_LEN 32

static SemaphoreHandle_t rx_sem;
static char rx_buf[BUF_LEN];
static volatile uint8_t head, tail;

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

    nvic_set_priority(NVIC_USART1_IRQ, 0x60); // compatible con FreeRTOS
    nvic_enable_irq(NVIC_USART1_IRQ);
    usart_enable_rx_interrupt(USART);
}

void usart1_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (usart_get_flag(USART, USART_SR_RXNE)) {
        char c = usart_recv(USART);
        uint8_t next = (head + 1) % BUF_LEN;
        if (next != tail) {
            rx_buf[head] = c;
            head = next;
            xSemaphoreGiveFromISR(rx_sem, &xHigherPriorityTaskWoken);
        }
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void echo_task(void *args) {
    (void)args;
    char c;
    while (1) {
        xSemaphoreTake(rx_sem, portMAX_DELAY);
        c = rx_buf[tail];
        tail = (tail + 1) % BUF_LEN;

        if (c == '\r' || c == '\n') {
            usart_send_blocking(USART, '\r');
            usart_send_blocking(USART, '\n');
        } else {
            usart_send_blocking(USART, c);
        }

        if (c == 't' || c == 'T')
            gpio_toggle(LED_PORT, LED_PIN);
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

    rx_sem = xSemaphoreCreateCounting(BUF_LEN, 0);
    configASSERT(rx_sem != NULL);

    xTaskCreate(echo_task, "Echo", 128, NULL, 1, NULL);
    xTaskCreate(heartbeat_task, "Blink", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    while (1);
}
