#include "unity/unity.h"
#include "stubs/stubs.h"

// Include the code under test
#include "../freertos-labs/01_blink_task/main.c"

void test_led_task_toggles_once(void) {
    gpio_toggle_count = 0;
    jump_after_delay = 1;
    if (setjmp(test_jmp_buf) == 0) {
        led_task(NULL);
    }
    TEST_ASSERT_EQUAL_INT(1, gpio_toggle_count);
}
