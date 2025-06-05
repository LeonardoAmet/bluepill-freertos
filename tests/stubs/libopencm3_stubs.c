#include "libopencm3.h"
#include "freertos.h"
#include <stdio.h>

const struct rcc_clock_scale rcc_hse_configs[] = { {0} };

int gpio_toggle_count = 0;
char usart_send_buffer[256];
int usart_send_count = 0;

void rcc_clock_setup_pll(const struct rcc_clock_scale *cfg) { (void)cfg; }
void rcc_periph_clock_enable(uint32_t periph) { (void)periph; }

void gpio_set_mode(uint32_t port, uint8_t mode, uint8_t cnf, uint16_t pin) {
    (void)port; (void)mode; (void)cnf; (void)pin;
}

void gpio_toggle(uint32_t port, uint16_t pin) {
    (void)port; (void)pin;
    gpio_toggle_count++;
}

void gpio_set(uint32_t port, uint16_t pin) { (void)port; (void)pin; }
void gpio_clear(uint32_t port, uint16_t pin) { (void)port; (void)pin; }

void usart_set_baudrate(uint32_t usart, uint32_t baud) { (void)usart; (void)baud; }
void usart_set_databits(uint32_t usart, uint32_t bits) { (void)usart; (void)bits; }
void usart_set_stopbits(uint32_t usart, uint32_t stopbits) { (void)usart; (void)stopbits; }
void usart_set_mode(uint32_t usart, uint32_t mode) { (void)usart; (void)mode; }
void usart_set_parity(uint32_t usart, uint32_t parity) { (void)usart; (void)parity; }
void usart_set_flow_control(uint32_t usart, uint32_t flow) { (void)usart; (void)flow; }
void usart_enable(uint32_t usart) { (void)usart; }
void usart_enable_rx_interrupt(uint32_t usart) { (void)usart; }
int usart_get_flag(uint32_t usart, uint32_t flag) { (void)usart; (void)flag; return 0; }
char usart_recv(uint32_t usart) { (void)usart; return 0; }
void usart_send_blocking(uint32_t usart, char data) {
    (void)usart;
    usart_send_buffer[usart_send_count++] = data;
}

void nvic_set_priority(uint8_t irq, uint8_t prio) { (void)irq; (void)prio; }
void nvic_enable_irq(uint8_t irq) { (void)irq; }
