#include "../../tests/unity/unity.h"
#include "../include/fsm.h"

#include "../state_pattern/fsm_state_pattern.c"

void setUp(void) { fsm_reset(); }
void tearDown(void) {}

void test_state_pattern(void) {
    fsm_handle_event((fsm_event_t){EVENT_CMD_1});
    TEST_ASSERT_EQUAL_INT(STATE_BLINK_SLOW, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_CMD_2});
    TEST_ASSERT_EQUAL_INT(STATE_BLINK_FAST, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_CMD_0});
    TEST_ASSERT_EQUAL_INT(STATE_OFF, fsm_get_state());
    fsm_handle_event((fsm_event_t){EVENT_INVALID});
    TEST_ASSERT_EQUAL_INT(STATE_ERROR, fsm_get_state());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_state_pattern);
    return UNITY_END();
}
