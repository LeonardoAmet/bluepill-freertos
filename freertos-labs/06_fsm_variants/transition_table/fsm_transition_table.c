#include "../include/fsm.h"

static fsm_state_t current_state = STATE_OFF;

static void enter_state(fsm_state_t s) {
    switch (s) {
    case STATE_OFF: printf("LED OFF\n"); break;
    case STATE_BLINK_SLOW: printf("Blinking SLOW\n"); break;
    case STATE_BLINK_FAST: printf("Blinking FAST\n"); break;
    case STATE_ERROR: printf("ERROR state\n"); break;
    }
}

static const fsm_state_t transition[4][4] = {
    /* from OFF */           {STATE_OFF, STATE_BLINK_SLOW, STATE_BLINK_FAST, STATE_ERROR},
    /* from BLINK_SLOW */    {STATE_OFF, STATE_BLINK_SLOW, STATE_BLINK_FAST, STATE_ERROR},
    /* from BLINK_FAST */    {STATE_OFF, STATE_BLINK_SLOW, STATE_BLINK_FAST, STATE_ERROR},
    /* from ERROR */         {STATE_OFF, STATE_BLINK_SLOW, STATE_BLINK_FAST, STATE_ERROR}
};

void fsm_reset(void) { current_state = STATE_OFF; }

fsm_state_t fsm_get_state(void) { return current_state; }

void fsm_handle_event(fsm_event_t event) {
    if (event.type > EVENT_INVALID) event.type = EVENT_INVALID;
    current_state = transition[current_state][event.type];
    enter_state(current_state);
}
