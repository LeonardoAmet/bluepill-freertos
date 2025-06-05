#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/*-----------------------------------------------------------
 * Configuración del kernel
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      (72000000UL)
#define configTICK_RATE_HZ                      (1000)
#define configMAX_PRIORITIES                    5
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (5 << 4)
#define configKERNEL_INTERRUPT_PRIORITY         (255)
#define configMINIMAL_STACK_SIZE                (128)
#define configTOTAL_HEAP_SIZE                   (8 * 1024)
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/*-----------------------------------------------------------
 * Configuración de sincronización y recursos
 *----------------------------------------------------------*/

#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               8

/*-----------------------------------------------------------
 * Inclusión de funciones de la API
 *----------------------------------------------------------*/

#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_xTaskGetSchedulerState          1

/*-----------------------------------------------------------
 * Seguridad y depuración
 *----------------------------------------------------------*/

#define configASSERT(x) if((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;); }

/* Redirect FreeRTOS port interrupts. */
#define vPortSVCHandler     sv_call_handler
#define xPortPendSVHandler  pend_sv_handler
#define xPortSysTickHandler sys_tick_handler

#endif /* FREERTOS_CONFIG_H */
