#include "unity/unity.h"
#include "stubs/stubs.h"

#include "../freertos-labs/02_uart_echo_queue/main.c"


void test_echo_cr_lf(void) {
    rx_queue = xQueueCreate(4, sizeof(char));
    char ch = '\r';
    xQueueSendFromISR(rx_queue, &ch, NULL);
    jump_after_receive = 2;
    usart_send_count = 0;
    if (setjmp(test_jmp_buf) == 0) {
        echo_task(NULL);
    }
    TEST_ASSERT_EQUAL_INT(2, usart_send_count);
    TEST_ASSERT_EQUAL_CHAR('\r', usart_send_buffer[0]);
    TEST_ASSERT_EQUAL_CHAR('\n', usart_send_buffer[1]);
}

void test_echo_regular_char(void) {
    rx_queue = xQueueCreate(4, sizeof(char));
    char ch = 'A';
    xQueueSendFromISR(rx_queue, &ch, NULL);
    jump_after_receive = 2;
    usart_send_count = 0;
    if (setjmp(test_jmp_buf) == 0) {
        echo_task(NULL);
    }
    TEST_ASSERT_EQUAL_INT(1, usart_send_count);
    TEST_ASSERT_EQUAL_CHAR('A', usart_send_buffer[0]);
}

