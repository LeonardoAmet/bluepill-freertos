#include "../include/fsm.h"
#include <stdio.h>

static fsm_event_t event_from_char(char c) {
    switch (c) {
    case '0':
        return (fsm_event_t){EVENT_CMD_0};
    case '1':
        return (fsm_event_t){EVENT_CMD_1};
    case '2':
        return (fsm_event_t){EVENT_CMD_2};
    default:
        return (fsm_event_t){EVENT_INVALID};
    }
}

int main(void) {
    const char seq[] = {'1','2','0','x','\0'};
    fsm_reset();
    for (int i = 0; seq[i] != '\0'; ++i) {
        printf("Input: %c\n", seq[i]);
        fsm_handle_event(event_from_char(seq[i]));
    }
    return 0;
}
