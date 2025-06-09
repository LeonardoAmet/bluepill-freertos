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

static void enter_state(fsm_state_t state) {
    switch (state) {
    case STATE_OFF: send_line("LED OFF"); break;
    case STATE_BLINK_SLOW: send_line("Blinking SLOW"); break;
    case STATE_BLINK_FAST: send_line("Blinking FAST"); break;
    case STATE_ERROR: send_line("ERROR state"); break;
    }
}

static void handle_off(fsm_event_t e);
static void handle_blink_slow(fsm_event_t e);
static void handle_blink_fast(fsm_event_t e);
static void handle_error(fsm_event_t e);

static void (*handlers[])(fsm_event_t) = {
    handle_off,
    handle_blink_slow,
    handle_blink_fast,
    handle_error
};

void fsm_reset(void) { current_state = STATE_OFF; }

fsm_state_t fsm_get_state(void) { return current_state; }

static void handle_off(fsm_event_t e) {
    switch (e.type) {
    case EVENT_CMD_0:
        enter_state(STATE_OFF);
        break;
    case EVENT_CMD_1:
        current_state = STATE_BLINK_SLOW;
        enter_state(current_state);
        break;
    case EVENT_CMD_2:
        current_state = STATE_BLINK_FAST;
        enter_state(current_state);
        break;
    default:
        current_state = STATE_ERROR;
        enter_state(current_state);
        break;
    }
}

static void handle_blink_slow(fsm_event_t e) {
    switch (e.type) {
    case EVENT_CMD_0:
        current_state = STATE_OFF;
        enter_state(current_state);
        break;
    case EVENT_CMD_1:
        enter_state(STATE_BLINK_SLOW);
        break;
    case EVENT_CMD_2:
        current_state = STATE_BLINK_FAST;
        enter_state(current_state);
        break;
    default:
        current_state = STATE_ERROR;
        enter_state(current_state);
        break;
    }
}

static void handle_blink_fast(fsm_event_t e) {
    switch (e.type) {
    case EVENT_CMD_0:
        current_state = STATE_OFF;
        enter_state(current_state);
        break;
    case EVENT_CMD_1:
        current_state = STATE_BLINK_SLOW;
        enter_state(current_state);
        break;
    case EVENT_CMD_2:
        enter_state(STATE_BLINK_FAST);
        break;
    default:
        current_state = STATE_ERROR;
        enter_state(current_state);
        break;
    }
}

static void handle_error(fsm_event_t e) {
    switch (e.type) {
    case EVENT_CMD_0:
        current_state = STATE_OFF;
        enter_state(current_state);
        break;
    case EVENT_CMD_1:
        current_state = STATE_BLINK_SLOW;
        enter_state(current_state);
        break;
    case EVENT_CMD_2:
        current_state = STATE_BLINK_FAST;
        enter_state(current_state);
        break;
    default:
        enter_state(STATE_ERROR);
        break;
    }
}

void fsm_handle_event(fsm_event_t event) {
    handlers[current_state](event);
}
