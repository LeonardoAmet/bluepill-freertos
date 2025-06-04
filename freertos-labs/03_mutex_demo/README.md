# Ejemplo de Mutex con FreeRTOS

Este directorio contiene un ejemplo didáctico de cómo utilizar un **mutex** en FreeRTOS sobre la placa **Blue Pill** (STM32F103C8T6). El objetivo es proteger el acceso al puerto serie para que dos tareas puedan enviar mensajes sin interferirse.

## ¿Qué es un mutex?

Un **mutex** (del inglés *Mutual Exclusion*) es un mecanismo de sincronización que garantiza que solo una tarea a la vez pueda acceder a un recurso compartido. En FreeRTOS los mutexes se implementan como un tipo especial de semáforo que:

1. Soporta *priority inheritance* para evitar la *inversión de prioridades* cuando una tarea de baja prioridad bloquea a una de mayor prioridad.
2. Solo puede ser liberado por la misma tarea que lo tomó.

Su uso típico es proteger regiones críticas (por ejemplo una UART, archivos, variables compartidas, etc.). Se crean con `xSemaphoreCreateMutex()` y se manejan con `xSemaphoreTake()` y `xSemaphoreGive()`.

## Funcionamiento del ejemplo

Al iniciar `main()` se configura el sistema de reloj, el GPIO del LED y la UART1. Luego se crea el mutex `serial_mutex`:

```c
serial_mutex = xSemaphoreCreateMutex();
configASSERT(serial_mutex != NULL);
```

Se crean dos instancias de `writer_task` que intentan transmitir cadenas diferentes por la misma UART. Cada tarea espera indefinidamente hasta obtener el mutex, envía su mensaje y luego lo libera:

```c
if (xSemaphoreTake(serial_mutex, portMAX_DELAY) == pdPASS) {
    send_string(msg);
    xSemaphoreGive(serial_mutex);
}
```

Mientras tanto, `heartbeat_task` conmuta el LED cada 500&nbsp;ms para indicar que el sistema está en ejecución.

El uso del mutex asegura que los textos de ambas tareas se envíen completos y en orden sin mezclarse en la salida por el puerto serie.

## Compilación y ejecución

```bash
make
make flash
```
