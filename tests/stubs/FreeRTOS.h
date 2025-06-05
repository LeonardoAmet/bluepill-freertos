#ifndef FREERTOS_STUB_H
#define FREERTOS_STUB_H
#include <stdint.h>
#include <setjmp.h>

typedef void* QueueHandle_t;
typedef int32_t BaseType_t;
typedef uint32_t UBaseType_t;
typedef uint32_t TickType_t;

#define pdPASS 1
#define pdFALSE 0
#define pdMS_TO_TICKS(x) (x)
#define portMAX_DELAY 0xffffffff

extern jmp_buf test_jmp_buf;
extern int jump_after_delay;
extern int jump_after_receive;

QueueHandle_t xQueueCreate(UBaseType_t length, UBaseType_t item_size);
BaseType_t xQueueReceive(QueueHandle_t q, void *item, TickType_t timeout);
BaseType_t xQueueSendFromISR(QueueHandle_t q, const void *item, BaseType_t *hpw);
BaseType_t xTaskCreate(void (*task)(void*), const char *name, uint16_t stack,
                       void *params, UBaseType_t prio, void *handle);
void vTaskDelay(TickType_t ticks);
void vTaskStartScheduler(void);
void portYIELD_FROM_ISR(BaseType_t x);

#endif // FREERTOS_STUB_H
