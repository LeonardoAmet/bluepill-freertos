# Ejemplo de Mutex con FreeRTOS

Este proyecto muestra el uso básico de un **mutex** para proteger el acceso al
puerto serie en la placa **Blue Pill**. Dos tareas intentan enviar mensajes por
UART; el mutex evita que los textos se mezclen. Además se parpadea el LED
onboard como indicación de que el sistema está activo.

## Compilación y ejecución

```bash
make
make flash
```
