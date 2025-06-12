#include "../include/fsm.h"
#include <libopencm3/stm32/usart.h>

static void send_line(const char *msg)
{
    while (*msg)
        usart_send_blocking(USART1, *msg++);
    usart_send_blocking(USART1, '\r');
    usart_send_blocking(USART1, '\n');
}

struct state;
typedef const struct state *state_ptr;

struct state {
    fsm_state_t id;
    void (*enter)(void);
    state_ptr (*on_event)(fsm_event_t e);
};

static state_ptr current_state;

/* Forward declarations */
static state_ptr off_on_event(fsm_event_t e);
static state_ptr blink_slow_on_event(fsm_event_t e);
static state_ptr blink_fast_on_event(fsm_event_t e);
static state_ptr error_on_event(fsm_event_t e);

static void enter_off(void) { send_line("LED OFF"); }
static void enter_blink_slow(void) { send_line("Blinking SLOW"); }
static void enter_blink_fast(void) { send_line("Blinking FAST"); }
static void enter_error(void) { send_line("ERROR state"); }

static const struct state s_off = {STATE_OFF, enter_off, off_on_event};
static const struct state s_blink_slow = {STATE_BLINK_SLOW, enter_blink_slow, blink_slow_on_event};
static const struct state s_blink_fast = {STATE_BLINK_FAST, enter_blink_fast, blink_fast_on_event};
static const struct state s_error = {STATE_ERROR, enter_error, error_on_event};

static state_ptr off_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &s_off;
    case EVENT_CMD_1: return &s_blink_slow;
    case EVENT_CMD_2: return &s_blink_fast;
    default: return &s_error;
    }
}

static state_ptr blink_slow_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &s_off;
    case EVENT_CMD_1: return &s_blink_slow;
    case EVENT_CMD_2: return &s_blink_fast;
    default: return &s_error;
    }
}

static state_ptr blink_fast_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &s_off;
    case EVENT_CMD_1: return &s_blink_slow;
    case EVENT_CMD_2: return &s_blink_fast;
    default: return &s_error;
    }
}

static state_ptr error_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &s_off;
    case EVENT_CMD_1: return &s_blink_slow;
    case EVENT_CMD_2: return &s_blink_fast;
    default: return &s_error;
    }
}

void fsm_reset(void)
{
    current_state = &s_off;
}

fsm_state_t fsm_get_state(void)
{
    return current_state->id;
}

void fsm_handle_event(fsm_event_t event)
{
    current_state = current_state->on_event(event);
    current_state->enter();
}
