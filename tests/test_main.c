#include "unity/unity.h"

void test_led_task_toggles_once(void);
void test_echo_cr_lf(void);
void test_echo_regular_char(void);

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_led_task_toggles_once);
    RUN_TEST(test_echo_cr_lf);
    RUN_TEST(test_echo_regular_char);
    return UNITY_END();
}
