#ifndef LIBOPENCM3_STUB_H
#define LIBOPENCM3_STUB_H
#include <stdint.h>

#define GPIOC 0
#define GPIOA 1
#define GPIO9 9
#define GPIO10 10
#define GPIO13 13
#define GPIO_MODE_OUTPUT_2_MHZ 0
#define GPIO_MODE_OUTPUT_50_MHZ 0
#define GPIO_MODE_INPUT 0
#define GPIO_CNF_OUTPUT_PUSHPULL 0
#define GPIO_CNF_OUTPUT_ALTFN_PUSHPULL 0
#define GPIO_CNF_INPUT_PULL_UPDOWN 0

#define RCC_GPIOC 0
#define RCC_GPIOA 0
#define RCC_USART1 0

#define USART1 0
#define USART_STOPBITS_1 0
#define USART_MODE_TX_RX 0
#define USART_PARITY_NONE 0
#define USART_FLOWCONTROL_NONE 0

#define NVIC_USART1_IRQ 0
#define USART_SR_RXNE 0

struct rcc_clock_scale { int dummy; };
extern const struct rcc_clock_scale rcc_hse_configs[];
#define RCC_CLOCK_HSE8_72MHZ 0

void rcc_clock_setup_pll(const struct rcc_clock_scale *);
void rcc_periph_clock_enable(uint32_t periph);

void gpio_set_mode(uint32_t port, uint8_t mode, uint8_t cnf, uint16_t pin);
void gpio_toggle(uint32_t port, uint16_t pin);
void gpio_set(uint32_t port, uint16_t pin);
void gpio_clear(uint32_t port, uint16_t pin);

void usart_set_baudrate(uint32_t usart, uint32_t baud);
void usart_set_databits(uint32_t usart, uint32_t bits);
void usart_set_stopbits(uint32_t usart, uint32_t stopbits);
void usart_set_mode(uint32_t usart, uint32_t mode);
void usart_set_parity(uint32_t usart, uint32_t parity);
void usart_set_flow_control(uint32_t usart, uint32_t flow);
void usart_enable(uint32_t usart);
void usart_enable_rx_interrupt(uint32_t usart);
int usart_get_flag(uint32_t usart, uint32_t flag);
char usart_recv(uint32_t usart);
void usart_send_blocking(uint32_t usart, char data);

void nvic_set_priority(uint8_t irq, uint8_t prio);
void nvic_enable_irq(uint8_t irq);

extern int gpio_toggle_count;
extern char usart_send_buffer[256];
extern int usart_send_count;

#endif // LIBOPENCM3_STUB_H
