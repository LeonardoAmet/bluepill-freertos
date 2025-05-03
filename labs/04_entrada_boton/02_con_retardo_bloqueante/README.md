# Lectura de botón con retardo bloqueante

Este ejemplo implementa una técnica básica de antirrebote mediante un **retardo bloqueante**. El botón está conectado a PA0 con pull-up interno activado, y controla el encendido y apagado de un LED conectado a PC13.

Al presionar el botón, el programa introduce un retardo de aproximadamente 20 ms. Si el botón sigue presionado luego del retardo, se considera una pulsación válida y se alterna el estado del LED.

---

## Conexión física

* **PA0** conectado a un botón.
* El otro terminal del botón va a **GND**.
* Se utiliza **pull-up interno**, no se requiere resistencia externa.
* **PC13** controla el LED (activo en bajo en las Blue Pill).

---

## Funcionamiento del código

* Se detecta un nivel bajo en PA0 (flanco descendente → presión de botón).
* Se espera un retardo fijo (`delay(100000)`), equivalente a \~20 ms.
* Se vuelve a leer el pin para verificar que el botón siga presionado.
* Si se confirma, se cambia el estado del LED.
* Luego, se espera a que el botón sea soltado (vuelva a nivel alto).

---

## Consideraciones

* Esta técnica es efectiva y muy fácil de implementar.
* Sin embargo, el uso de retardo bloqueante **congela la ejecución** del programa durante el tiempo de espera.
* No es adecuada en sistemas con múltiples tareas o donde se necesite buena capacidad de respuesta.

---

## Comparación con otras técnicas

Más adelante se explorarán otras estrategias más eficientes y no bloqueantes, como:

* Verificación por tiempo estable
* Contador de estabilidad (integrador)
* Uso de timer
* Uso de interrupciones externas (EXTI)

Este ejemplo sirve como base introductoria y de contraste para esas soluciones.
