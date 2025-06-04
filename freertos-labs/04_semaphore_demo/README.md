# Ejemplo de Semáforo con FreeRTOS

Este proyecto demuestra el uso de un **semáforo binario** para coordinar una tarea con la interrupción de la UART en la placa STM32 "Blue Pill".

## ¿Qué es un semáforo en FreeRTOS?

Un semáforo es un objeto especial utilizado para sincronizar tareas y eventos. Permite que una tarea quede bloqueada hasta que otro contexto "dé" el semáforo (por ejemplo, desde una interrupción). En FreeRTOS existen dos variedades principales:
- **Semáforo binario**: solo puede tomar los valores 0 o 1. Se usa para avisar que ocurrió un evento único.
- **Semáforo de conteo**: admite contar hasta un valor configurado. Permite acumular múltiples eventos antes de que una tarea los procese.

En este ejemplo se emplea un *semáforo binario*, creado con `xSemaphoreCreateBinary()`.

## Propósito dentro del código

La interrupción de recepción de la UART almacena el carácter recibido en `rx_char` y da el semáforo. La tarea `echo_task` permanece bloqueada con `xSemaphoreTake()` hasta que dicho semáforo es entregado. Al despertar, reenvía el mismo carácter y, si este es `t`, conmuta el LED. Se incluye una segunda tarea llamada `heartbeat_task` que hace parpadear el LED cada 500 ms para mostrar que el sistema sigue corriendo.

```c
void usart1_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (usart_get_flag(USART, USART_SR_RXNE)) {
        rx_char = usart_recv(USART);
        xSemaphoreGiveFromISR(rx_sem, &xHigherPriorityTaskWoken); // avisa a la tarea
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void echo_task(void *args) {
    while (1) {
        xSemaphoreTake(rx_sem, portMAX_DELAY); // espera hasta recibir un byte
        usart_send_blocking(USART, rx_char);
        usart_send_blocking(USART, '\r');
        usart_send_blocking(USART, '\n');
        if (rx_char == 't' || rx_char == 'T')
            gpio_toggle(LED_PORT, LED_PIN);
    }
}
```

## Compilación y ejecución

```bash
make
make flash
```
