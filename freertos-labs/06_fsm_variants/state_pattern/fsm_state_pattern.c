#include "../include/fsm.h"
#include <libopencm3/stm32/usart.h>

/* State Pattern implementation -------------------------------------------------
 * Each state provides two operations:
 *   - on_event:  returns the next state for a given event
 *   - enter:     action executed when entering the state
 */

typedef struct state state_t;
typedef state_t* state_ptr;

struct state {
    fsm_state_t id;
    state_ptr  (*on_event)(fsm_event_t);
    void       (*enter)(void);
};

static void send_line(const char *msg)
{
    while (*msg)
        usart_send_blocking(USART1, *msg++);
    usart_send_blocking(USART1, '\r');
    usart_send_blocking(USART1, '\n');
}

/* --- State actions --------------------------------------------------------- */

static void enter_off(void)       { send_line("LED OFF"); }
static void enter_blink_slow(void){ send_line("Blinking SLOW"); }
static void enter_blink_fast(void){ send_line("Blinking FAST"); }
static void enter_error(void)     { send_line("ERROR state"); }

/* Forward declarations of on_event handlers */
static state_ptr off_on_event(fsm_event_t e);
static state_ptr blink_slow_on_event(fsm_event_t e);
static state_ptr blink_fast_on_event(fsm_event_t e);
static state_ptr error_on_event(fsm_event_t e);

/* --- Concrete states ------------------------------------------------------ */

static state_t STATE_OFF_OBJ        = { STATE_OFF,        off_on_event,       enter_off       };
static state_t STATE_BLINK_SLOW_OBJ = { STATE_BLINK_SLOW, blink_slow_on_event, enter_blink_slow };
static state_t STATE_BLINK_FAST_OBJ = { STATE_BLINK_FAST, blink_fast_on_event, enter_blink_fast };
static state_t STATE_ERROR_OBJ      = { STATE_ERROR,      error_on_event,     enter_error     };

/* Current state pointer */
static state_ptr current_state = &STATE_OFF_OBJ;

/* --- Event handlers ------------------------------------------------------- */

static state_ptr off_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &STATE_OFF_OBJ;
    case EVENT_CMD_1: return &STATE_BLINK_SLOW_OBJ;
    case EVENT_CMD_2: return &STATE_BLINK_FAST_OBJ;
    default:          return &STATE_ERROR_OBJ;
    }
}

static state_ptr blink_slow_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &STATE_OFF_OBJ;
    case EVENT_CMD_1: return &STATE_BLINK_SLOW_OBJ;
    case EVENT_CMD_2: return &STATE_BLINK_FAST_OBJ;
    default:          return &STATE_ERROR_OBJ;
    }
}

static state_ptr blink_fast_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &STATE_OFF_OBJ;
    case EVENT_CMD_1: return &STATE_BLINK_SLOW_OBJ;
    case EVENT_CMD_2: return &STATE_BLINK_FAST_OBJ;
    default:          return &STATE_ERROR_OBJ;
    }
}

static state_ptr error_on_event(fsm_event_t e)
{
    switch (e.type) {
    case EVENT_CMD_0: return &STATE_OFF_OBJ;
    case EVENT_CMD_1: return &STATE_BLINK_SLOW_OBJ;
    case EVENT_CMD_2: return &STATE_BLINK_FAST_OBJ;
    default:          return &STATE_ERROR_OBJ;
    }
}

/* --- Public API ----------------------------------------------------------- */

void fsm_reset(void) { current_state = &STATE_OFF_OBJ; }

fsm_state_t fsm_get_state(void) { return current_state->id; }

void fsm_handle_event(fsm_event_t event)
{
    state_ptr new_state = current_state->on_event(event);
    if (new_state != current_state) {
        current_state = new_state;
        current_state->enter();
    }
}

