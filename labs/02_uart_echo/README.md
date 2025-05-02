# Ejemplo UART Echo con STM32 (Blue Pill) usando libopencm3

Este ejemplo muestra cómo configurar el periférico USART1 del STM32F103C8T6 para enviar y recibir datos por puerto serie. Implementa un sencillo "echo": todo carácter recibido es reenviado inmediatamente, incluyendo el manejo de retorno de carro (`\r`) y salto de línea (`\n`) para facilitar la visualización en un terminal serie.

---

## Tabla de contenidos

* [Objetivo del ejemplo](#objetivo-del-ejemplo)
* [Configuración de hardware](#configuración-de-hardware)
* [Configuración del USART](#configuración-del-usart)
* [Funcionamiento del programa](#funcionamiento-del-programa)
* [Compilación y grabación](#compilación-y-grabación)
* [Notas adicionales](#notas-adicionales)
* [Próximos pasos sugeridos](#próximos-pasos-sugeridos)

---

## Objetivo del ejemplo

* Configurar USART1 para trabajar a 9600 baudios con 8 bits de datos, sin paridad, 1 bit de stop (8N1)
* Enviar un mensaje inicial de bienvenida
* Leer caracteres desde el terminal serie
* Reenviar (eco) el carácter recibido, con ajuste de `\r` y `\n` para correcta visualización

---

## Configuración de hardware

* El pin **PA9** se utiliza como **TX** (transmisión)
* El pin **PA10** se utiliza como **RX** (recepción)
* Se requiere un conversor USB–Serial (TTL 3.3V) conectado a estos pines y al puerto USB de la PC

---

## Configuración del USART

La función `usart_setup()` realiza:

1. Habilitación de los relojes para GPIOA y USART1
2. Configuración de PA9 como salida alternativa push-pull (Tx)
3. Configuración de PA10 como entrada flotante (Rx)
4. Configuración del periférico USART1:

   * Baudrate: 9600
   * Datos: 8 bits
   * Paridad: ninguna
   * Stop bits: 1
   * Modo: transmisión y recepción habilitados
   * Flow control: deshabilitado

---

## Funcionamiento del programa

Al iniciar:

* Se configura el sistema de reloj a 72 MHz usando cristal externo de 8 MHz
* Se inicializa el USART
* Se envía un mensaje de bienvenida
* Luego, en el bucle principal:

  * Si hay un carácter recibido (`USART_SR_RXNE` activo), se lee
  * Si es `\r` se envía `\n`, y viceversa, para compatibilidad con terminales
  * Finalmente, se reenvía el carácter original

---

## Compilación y grabación

```bash
make         # Compila el proyecto
make flash   # Graba el firmware usando OpenOCD
```

Asegurate de tener instalados: `arm-none-eabi-gcc`, `make` y `OpenOCD`.

---

## Notas adicionales

* Esta implementación no utiliza interrupciones ni DMA. Todo el manejo es **bloqueante** (polling)
* El uso de `usart_get_flag(USART1, USART_SR_RXNE)` permite verificar si hay un carácter disponible sin interrumpir el flujo del programa

---

## Fragmento de código del eco

```c
if (usart_get_flag(USART1, USART_SR_RXNE))
{
    char c = usart_recv(USART1);
    // En caso de recibir un retorno de carro o un salto de línea, se envía el otro carácter
    // para que el terminal lo interprete correctamente.
    if (c == '\r')
    {
        usart_send_blocking(USART1, '\n'); // Salto de línea
    }
    else if (c == '\n')
    {
        usart_send_blocking(USART1, '\r'); // Retorno de carro
    }
    usart_send_blocking(USART1, c); // eco
}
```

> 🔍 Este bloque realiza el eco de un carácter recibido. También adapta el salto de línea según el carácter recibido para facilitar la visualización en terminales como PuTTY.

---

## Glosario de registros y funciones clave

* `USART_SR_RXNE`: bandera que indica que hay un dato disponible para leer
* `USART_SR_TXE`: bandera que indica que el transmisor está listo para enviar otro byte
* `usart_recv()`: lee un byte desde el buffer del periférico
* `usart_send_blocking()`: envía un byte y espera hasta que el transmisor esté disponible
* `USART1`: periférico UART del STM32F103C8T6 que se utiliza en este ejemplo

---

## Interacción con la Blue Pill usando PuTTY

Para comunicarse con la Blue Pill vía UART desde una PC:

1. Conectá un conversor USB–Serial (TTL 3.3V) a los pines:

   * **PA9 → RX del conversor**  *(PA9 actúa como TX en la Blue Pill)*
   * **PA10 → TX del conversor** *(PA10 actúa como RX en la Blue Pill)*
   * GND ↔ GND

> 🔁 Se requiere conexión cruzada: el TX de la Blue Pill debe ir al RX del conversor, y viceversa.

2. Abrí **PuTTY** (o similar) en modo **Serial**, configurando:

   * Velocidad: **9600 baudios**
   * Bits de datos: **8**
   * Paridad: **None**
   * Bits de stop: **1**
   * Control de flujo: **None**

3. Al resetear la placa, deberías ver el mensaje de bienvenida y luego recibir eco de cada tecla que presiones.

---

## Próximos pasos sugeridos

* Usar interrupciones para recibir datos de forma no bloqueante
* Implementar un buffer circular para almacenar múltiples caracteres
* Agregar comandos simples (modo interactivo)
* Probar a distintas velocidades (e.g. 115200 baudios)

