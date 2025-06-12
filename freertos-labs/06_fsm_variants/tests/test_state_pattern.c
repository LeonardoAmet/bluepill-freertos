#include "../../tests/unity/unity.h"
#include "../../tests/stubs/stubs.h"
#include "../include/fsm.h"

#include "../state_pattern/fsm_state_pattern.c"

void setUp(void) {}
void tearDown(void) {}

void test_state_pattern(void) {
    fsm_reset();
    fsm_handle_event((fsm_event_t){EVENT_CMD_1});
    TEST_ASSERT_EQUAL_INT(STATE_BLINK_SLOW, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_CMD_2});
    TEST_ASSERT_EQUAL_INT(STATE_BLINK_FAST, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_CMD_0});
    TEST_ASSERT_EQUAL_INT(STATE_OFF, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_INVALID});
    TEST_ASSERT_EQUAL_INT(STATE_ERROR, fsm_get_state());
}

void test_enter_called_on_state_change_only(void) {
    fsm_reset();
    usart_send_count = 0;
    fsm_handle_event((fsm_event_t){EVENT_CMD_0});
    TEST_ASSERT_EQUAL_INT(0, usart_send_count);

    usart_send_count = 0;
    fsm_handle_event((fsm_event_t){EVENT_CMD_1});
    if (usart_send_count == 0)
        TEST_FAIL_MESSAGE("enter() should be called on state change");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_state_pattern);
    RUN_TEST(test_enter_called_on_state_change_only);
    return UNITY_END();
}
