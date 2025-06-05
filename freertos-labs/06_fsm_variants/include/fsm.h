#ifndef FSM_H
#define FSM_H

#include <stdio.h>

typedef enum {
    STATE_OFF,
    STATE_BLINK_SLOW,
    STATE_BLINK_FAST,
    STATE_ERROR
} fsm_state_t;

typedef enum {
    EVENT_CMD_0,
    EVENT_CMD_1,
    EVENT_CMD_2,
    EVENT_INVALID
} fsm_event_type_t;

typedef struct {
    fsm_event_type_t type;
} fsm_event_t;

void fsm_handle_event(fsm_event_t event);
fsm_state_t fsm_get_state(void);
void fsm_reset(void);

#endif // FSM_H
