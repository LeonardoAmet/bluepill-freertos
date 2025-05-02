#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void usart_setup(void) {
    // Habilita reloj para USART1 y GPIOA
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configura PA9 (TX) como salida alternate function push-pull
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);

    // Configura PA10 (RX) como entrada flotante
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, GPIO10);

    // Configura USART1: 9600 baudios, 8N1, sin flow control
    usart_set_baudrate(USART1, 9600);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    usart_enable(USART1);
}

int main(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // Requiere que tengas HSE de 8 MHz

    usart_setup();

    // Enviar mensaje de bienvenida
    const char *msg = "Echo desde USART1 a 9600 baudios\r\n";
    for (const char *p = msg; *p != '\0'; p++) {
        usart_send_blocking(USART1, *p);
    }

    while (1) {
        if (usart_get_flag(USART1, USART_SR_RXNE)) {
            char c = usart_recv(USART1);
            // En caso de recibir un retorno de carro o un salto de línea, se envía el otro carácter
            // para que el terminal lo interprete correctamente.
            if (c == '\r') {
                usart_send_blocking(USART1, '\n'); // Salto de línea
            } else if (c == '\n') {
                usart_send_blocking(USART1, '\r'); // Retorno de carro
            }
            usart_send_blocking(USART1, c); // eco
        }
    }
}
