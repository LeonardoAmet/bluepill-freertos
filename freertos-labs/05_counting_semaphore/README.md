# Ejemplo de Semáforo de Conteo con FreeRTOS

Este proyecto muestra cómo utilizar un **semáforo no binario** (o de conteo) en la placa STM32 *Blue Pill* con la librería libopencm3 y FreeRTOS. Se hace uso únicamente del LED integrado en el puerto PC13 y la UART1.

## ¿Qué es un semáforo de conteo?

Un semáforo de conteo es un objeto de sincronización que, en lugar de limitarse a los valores 0/1 como el semáforo binario, permite almacenar una cantidad de "tokens" hasta un máximo fijado al crearlo. Cada `xSemaphoreGive()` incrementa el contador (mientras no se supere ese máximo) y cada `xSemaphoreTake()` lo decrementa. De esta manera es posible acumular múltiples eventos pendientes.

En sistemas embebidos se utiliza para señalar la disponibilidad de recursos repetibles (por ejemplo, posiciones libres en un búfer) o para registrar varios eventos ocurridos antes de que una tarea pueda procesarlos.

## Funcionamiento del ejemplo

La interrupción de recepción de la UART guarda cada carácter recibido en un pequeño búfer circular y luego entrega el semáforo con `xSemaphoreGiveFromISR()`. La tarea `echo_task` espera en bloque con `xSemaphoreTake()` hasta que haya un carácter por procesar. Al despertar, extrae el dato del búfer, lo reenvía por la misma UART y, si dicho carácter es `t` o `T`, conmuta el LED integrado.

Gracias a que se utiliza `xSemaphoreCreateCounting()` el semáforo puede acumular varios caracteres recibidos rápidamente, evitando que se pierdan aunque la tarea `echo_task` tarde en ejecutarse.

Además se incluye `heartbeat_task`, que hace parpadear el LED cada 500&nbsp;ms para indicar que el sistema sigue corriendo.

## Compilación y ejecución

```bash
make
make flash
```
