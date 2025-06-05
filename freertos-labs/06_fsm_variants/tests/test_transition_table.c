#include "../../tests/unity/unity.h"
#include "../include/fsm.h"

#include "../transition_table/fsm_transition_table.c"

void setUp(void) { fsm_reset(); }
void tearDown(void) {}

void test_transition_table(void) {
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
    RUN_TEST(test_transition_table);
    return UNITY_END();
}
