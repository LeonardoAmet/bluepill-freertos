#include "../include/fsm.h"
#include <libopencm3/stm32/usart.h>

static void send_line(const char *msg)
{
    while (*msg)
        usart_send_blocking(USART1, *msg++);
    usart_send_blocking(USART1, '\r');
    usart_send_blocking(USART1, '\n');
}

static fsm_state_t current_state = STATE_OFF;

static void enter_state(fsm_state_t s) {
    switch (s) {
    case STATE_OFF: send_line("LED OFF"); break;
    case STATE_BLINK_SLOW: send_line("Blinking SLOW"); break;
    case STATE_BLINK_FAST: send_line("Blinking FAST"); break;
    case STATE_ERROR: send_line("ERROR state"); break;
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
