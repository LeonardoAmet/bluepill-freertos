# Lectura de botón con detección de flanco (sin antirrebote)

Este ejemplo muestra cómo leer un botón conectado a PA0 para alternar el estado de un LED conectado a PC13, sin aplicar ninguna técnica de antirrebote.

Se utiliza la detección de flanco descendente (cuando el botón pasa de no presionado a presionado) para evitar múltiples cambios mientras se mantiene presionado. Sin embargo, **no se aplica ningún filtrado** sobre la señal de entrada, por lo que es esperable que el LED se comporte de forma errática al presionar el botón, debido al rebote mecánico.

---

## Conexión física

* **PA0** conectado a un botón.
* El otro terminal del botón va a **GND**.
* **PC13** está conectado al LED integrado en la Blue Pill (activo en bajo).
* **No se necesita resistencia externa**, ya que se activa la **resistencia de pull-up interna** en el código.

---

## Funcionamiento del código

* Se configura PA0 como entrada con pull-up interno.
* Se compara el estado actual del botón con el anterior para detectar un **flanco descendente**.
* Al detectar dicho flanco, se alterna el estado del LED.

Este mecanismo evita múltiples toggles durante una pulsación mantenida, pero **no evita múltiples toggles provocados por los rebotes eléctricos** del botón.

---

## ¿Por qué no usar espera por liberación?

Una alternativa común es esperar a que el botón sea soltado después de registrar una pulsación. Esta técnica se conoce como **espera por liberación**, y su implementación típica consiste en un bucle que no permite avanzar hasta que el botón vuelva a su estado inactivo:

```c
if (gpio_get(GPIOA, GPIO0)) {
    toggle_led();
    while (gpio_get(GPIOA, GPIO0)); // espera a que el botón se suelte
}
```

Esta forma de control evita múltiples activaciones mientras el botón está presionado, pero es una solución **bloqueante**, que congela la ejecución del programa y limita su escalabilidad.

La técnica implementada en este ejemplo (detección de flanco) es **no bloqueante** y sirve como base para aplicar más adelante técnicas reales de antirrebote por software.

---