#include "freertos.h"
#include <stdlib.h>
#include <string.h>

jmp_buf test_jmp_buf;
int jump_after_delay = 0;
int jump_after_receive = 0;

typedef struct {
    char *buffer;
    UBaseType_t size;
    UBaseType_t head;
    UBaseType_t tail;
    UBaseType_t count;
} StubQueue;

QueueHandle_t xQueueCreate(UBaseType_t length, UBaseType_t item_size) {
    (void)item_size;
    StubQueue *q = calloc(1, sizeof(StubQueue));
    q->buffer = calloc(length, sizeof(char));
    q->size = length;
    return (QueueHandle_t)q;
}

BaseType_t xQueueReceive(QueueHandle_t handle, void *item, TickType_t timeout) {
    (void)timeout;
    StubQueue *q = (StubQueue*)handle;
    if (q->count == 0) {
        if (jump_after_receive > 0) {
            jump_after_receive--;
            if (jump_after_receive == 0) longjmp(test_jmp_buf, 1);
        }
        return pdFALSE;
    }
    *((char*)item) = q->buffer[q->tail];
    q->tail = (q->tail + 1) % q->size;
    q->count--;
    if (jump_after_receive > 0) {
        jump_after_receive--;
        if (jump_after_receive == 0) longjmp(test_jmp_buf, 1);
    }
    return pdPASS;
}

BaseType_t xQueueSendFromISR(QueueHandle_t handle, const void *item, BaseType_t *hpw) {
    (void)hpw;
    StubQueue *q = (StubQueue*)handle;
    if (q->count >= q->size) return !pdPASS;
    q->buffer[q->head] = *(const char*)item;
    q->head = (q->head + 1) % q->size;
    q->count++;
    return pdPASS;
}

BaseType_t xTaskCreate(void (*task)(void*), const char *name, uint16_t stack,
                       void *params, UBaseType_t prio, void *handle) {
    (void)task; (void)name; (void)stack; (void)params; (void)prio; (void)handle;
    return pdPASS;
}

void vTaskDelay(TickType_t ticks) {
    (void)ticks;
    if (jump_after_delay) { jump_after_delay = 0; longjmp(test_jmp_buf, 1); }
}

void vTaskStartScheduler(void) {}
void portYIELD_FROM_ISR(BaseType_t x) { (void)x; }
