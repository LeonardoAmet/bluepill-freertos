# Lectura de botón con retardo bloqueante

Este ejemplo implementa una técnica básica de antirrebote mediante un **retardo bloqueante**, activado solamente cuando se detecta una pulsación válida. Se utiliza además **detección de flanco descendente** para iniciar el proceso de validación, lo cual lo vuelve coherente con el enfoque del proyecto anterior.

El botón está conectado a PA0 (con pull-up interno activado) y controla el encendido y apagado de un LED conectado a PC13.

---

## Conexión física

* **PA0** conectado a un botón.
* El otro terminal del botón va a **GND**.
* Se utiliza **pull-up interno**, no se requiere resistencia externa.
* **PC13** controla el LED (activo en bajo en las Blue Pill).

---

## Funcionamiento del código

* Se mantiene el estado anterior del botón.
* Se detecta un **flanco descendente**: transición de no presionado (1) a presionado (0).
* Se introduce un retardo de \~20 ms para filtrar rebotes.
* Se verifica que el botón **siga presionado** después del retardo.
* Si se confirma, se alterna el estado del LED.

---

## Consideraciones

* Esta técnica **evita el rebote** en la mayoría de los casos.
* No hay espera de liberación (no se congela el programa hasta que el botón se suelte).
* Aún así, el uso de `delay()` bloquea temporalmente la ejecución.
* Es una solución válida para sistemas simples o como paso intermedio hacia técnicas más avanzadas.

---

## Objetivo didáctico

Este ejemplo permite comparar directamente con el proyecto anterior:

* En el proyecto anterior se usaba flanco descendente **sin antirrebote**, y era evidente el efecto del rebote.
* En este, al aplicar un retardo breve tras detectar el flanco, se mitiga significativamente el efecto del rebote.

Esta progresión ayuda a visualizar el problema y valorar la solución aplicada.
