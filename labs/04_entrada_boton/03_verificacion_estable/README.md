# 🧪 Verificación de botón por estabilidad (antirrebote no bloqueante)

Este ejemplo implementa una técnica más avanzada de antirrebote, conocida como **verificación por estabilidad temporal**. En lugar de usar retardos o esperas activas, se valida un cambio de estado solo si el nuevo valor del botón se mantiene durante varias lecturas consecutivas.

Se utiliza **detección de flanco descendente** sobre el estado estable del botón. Esta técnica es muy efectiva y completamente **no bloqueante**, lo que la hace adecuada para sistemas embebidos que requieren eficiencia.

---

## 🔌 Conexión física

* **PA0** conectado a un botón.
* El otro terminal del botón va a **GND**.
* Se utiliza **pull-up interno**, no se requiere resistencia externa.
* **PC13** controla el LED (activo en bajo en las Blue Pill).

---

## ⚙️ Funcionamiento del código

* Se define un valor `estado_estable` que representa el último estado válido del botón.
* En cada iteración del bucle principal:

  * Se lee el estado actual del botón.
  * Si difiere del estado estable anterior, se incrementa un contador de estabilidad.
  * Si se alcanza un umbral predeterminado (`UMBRAL_ESTABILIDAD`), se acepta el nuevo estado.
* Si el nuevo estado estable corresponde a una **transición de 1 a 0** (flanco descendente), se alterna el estado del LED.

---

## ✅ Ventajas de la técnica

* 🔁 **No bloqueante**: no congela la ejecución del programa.
* 🎛️ **Configurable**: permite ajustar la sensibilidad ante rebotes.
* 💡 **Ligera y eficiente**: fácil de implementar, bajo uso de recursos.

---

## 🎓 Propuesta de prueba didáctica

Modificá el valor de `UMBRAL_ESTABILIDAD` en el código y observá el comportamiento del sistema:

* Con valores bajos como `1`, es probable que el sistema registre múltiples pulsaciones falsas debido al rebote.
* Con valores altos como `1000`, `2000` o incluso `100000`, observarás un comportamiento más robusto, aunque con mayor latencia perceptible al pulsar.

Estas pruebas permiten comprender el compromiso entre **sensibilidad** y **robustez** de la técnica, y cómo calibrarla según la aplicación concreta.

---

## 🧠 Objetivo didáctico

Este ejemplo representa un paso importante en el tratamiento del rebote de botones sin sacrificar rendimiento. A diferencia de las técnicas anteriores:

* No utiliza `delay()`.
* No espera a que el botón se libere.
* No congela la ejecución del sistema.

Es una solución profesional, inspirada en las recomendaciones de **Jack Ganssle**, ampliamente usada en sistemas embebidos reales.

---

## 🚀 Técnica siguiente sugerida

* ⏱️ **Temporizador con ISR**: uso de interrupciones periódicas para escaneo estable, sin depender del bucle principal.
