# Ejemplo de Semáforo con FreeRTOS

En este ejemplo se utiliza un **semáforo binario** para sincronizar la llegada de
caracteres por UART con una tarea de procesamiento. El ISR de la UART entrega el
semáforo cada vez que se recibe un byte. La tarea que espera el semáforo reenvía
el carácter recibido y, si éste es `t`, conmuta el LED integrado de la placa
**Blue Pill**.

## Compilación y ejecución

```bash
make
make flash
```
