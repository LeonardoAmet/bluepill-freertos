# Antirrebote con verificación estable usando temporizador (no bloqueante)

Este ejemplo demuestra una técnica robusta para filtrar el rebote de un botón usando **verificación por estabilidad** en una ISR generada por un **temporizador de 1 ms**. Es una estrategia **no bloqueante** y altamente eficiente para sistemas embebidos.

---

## 🛠️ Descripción general

El sistema se basa en el uso del **timer TIM2** para generar una interrupción cada 1 ms. En cada interrupción, se realiza una lectura del pin del botón y se compara con el estado estable anterior. Si el nuevo valor se mantiene durante una cantidad de ciclos consecutivos (“umbral”), se considera una transición válida.

Si dicha transición corresponde a un **flanco descendente** (botón presionado), se alterna el estado de un LED conectado al pin **PC13**.

---

## ⚖️ Ventajas de la técnica

* ✅ **No bloqueante**: el programa principal sigue ejecutándose normalmente.
* ✅ **Robusta y configurable**: se puede ajustar el umbral de estabilidad para adaptarse a distintos tipos de botones.
* ✅ **Escalable**: ideal si querés escanear varios botones.

---

## 🔌 Conexión física

* **PA0**: conectado a un botón, con el otro extremo a **GND**.
* Se utiliza la **resistencia de pull-up interna**, no requiere resistencias externas.
* **PC13**: salida para controlar un LED (activo en bajo en placas Blue Pill).

---

## 🔧 Configuración del temporizador

Se configura **TIM2** para interrumpir cada 1 ms:

* Se usa un **prescaler de 7200**, que reduce el reloj de sistema (72 MHz) a 10 kHz.
* Luego se ajusta el **período** para obtener 1 ms: 10 kHz / 10 = 1 kHz.

Para obtener el valor correcto del período, se utiliza:

```c
#define TIMER_CLOCK_HZ ((rcc_apb1_frequency == rcc_ahb_frequency) ? \
                        rcc_apb1_frequency : rcc_apb1_frequency * 2)
#define TIMER_PERIOD   (TIMER_CLOCK_HZ / TIMER_PRESCALER / TIMER_FREQUENCY_HZ)
```

Esto permite adaptar el código a distintas configuraciones de reloj sin asumir una frecuencia fija.

---

## 🎓 Propuesta de prueba didáctica

Modificá el valor del macro `UMBRAL_ESTABILIDAD` y observá:

* Con valores bajos (`1`, `2`, `5`): vas a notar que el sistema responde rápido, pero también puede registrar rebotes.
* Con valores altos (`50`, `100`, `1000`): se vuelve más robusto, pero puede haber una latencia perceptible.

Esto te permite entender el **compromiso entre velocidad de respuesta y robustez**.

---

## 🧠 Comentarios finales

Esta técnica es ampliamente usada en la industria cuando se necesita filtrado de entradas sin bloquear la ejecución del sistema. Es una excelente introducción al uso de timers para tareas de escaneo periódico.

Este ejemplo también muestra buenas prácticas:

* Lectura desde ISR con lógica simple
* Separación clara entre hardware y comportamiento
* Uso de `rcc_ahb_frequency` en lugar de valores fijos

---

🚀 **Siguiente paso sugerido:** implementar el mismo filtrado, pero para múltiples botones, o con temporizadores más precisos y menor consumo.
