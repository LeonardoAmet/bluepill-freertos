# Antirrebote usando interrupción externa (EXTI) y temporizador

Este ejemplo combina el uso del periférico **EXTI** con un **temporizador** para implementar una técnica de antirrebote **eficiente, no bloqueante y precisa**. Ante un flanco descendente (presión del botón), se desactiva la interrupción externa y se habilita un temporizador que valida el estado pasado cierto tiempo. Si el botón sigue presionado luego del retardo, se considera válido el evento.

---

## 🔌 Conexión física

* **PA0** conectado a un botón pulsador.
* El otro terminal del botón a **GND**.
* Se usa **resistencia pull-up interna** (no requiere componentes externos).
* **PC13** controla el LED (activo en bajo en la Blue Pill).

---

## ⚙️ Funcionamiento general

1. El pin PA0 se configura como entrada con pull-up.
2. Se configura la interrupción EXTI0 para flanco descendente.
3. Al detectar una presión:

   * Se desactiva la interrupción EXTI0.
   * Se habilita el temporizador TIM2 para contar ms.
4. Cuando el temporizador alcanza el tiempo de espera (ej. 20 ms):

   * Si el botón **sigue presionado**, se alterna el estado del LED.
   * Se reactiva la interrupción externa.

---

## 🔢 Configuración del sistema

### EXTI (External Interrupt)

**EXTI** es un periférico del microcontrolador STM32 que permite detectar cambios de estado en pines de entrada y generar interrupciones automáticamente cuando ocurre un evento (por ejemplo, un flanco ascendente o descendente).

#### 🔧 Pasos para configurar EXTI:

1. **Habilitar el reloj del periférico AFIO**, necesario para enrutar EXTI:

   ```c
   rcc_periph_clock_enable(RCC_AFIO);
   ```

2. **Enrutar el pin deseado al canal EXTI correspondiente** usando `exti_select_source()`:

   ```c
   exti_select_source(EXTI0, GPIOA); // Ruta PA0 hacia EXTI0
   ```

3. **Elegir el tipo de flanco que disparará la interrupción**:

   ```c
   exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING); // Flanco descendente
   ```

4. **Habilitar la solicitud de interrupción externa**:

   ```c
   exti_enable_request(EXTI0);
   ```

5. **Habilitar la interrupción correspondiente en el NVIC**:

   ```c
   nvic_enable_irq(NVIC_EXTI0_IRQ);
   ```

### Tabla de vectores de interrupción

Cuando ocurre un evento EXTI, el microcontrolador consulta la **tabla de vectores**, una estructura que contiene las direcciones de las funciones ISR (Interrupt Service Routine). Por ejemplo:

* Para `EXTI0`, se ejecuta `void exti0_isr(void)`.

Este mecanismo permite que el programa "salte automáticamente" a ejecutar una función específica cuando un pin genera un evento externo.

### Temporizador TIM2

* Se configura para generar una interrupción cada 1 ms.
* Se ajustan `prescaler` y `period` usando `rcc_ahb_frequency`.
* En su ISR, lleva la cuenta del tiempo transcurrido desde que se activó el antirrebote.

---

## ✅ Ventajas de esta técnica

* ⚡️ **Respuesta rápida**: la detección ocurre por hardware.
* ⏱ **Validación robusta**: el rebote no activa falsos eventos.
* ❌ **No bloqueante**: no se interrumpe el flujo principal.

---

## 🎓 Recomendaciones&#x20;

* Probar con diferentes valores del retardo (5, 10, 20, 50 ms).
* Observar qué ocurre si no se desactiva EXTI durante el antirrebote.
* Agregar `printf` o toggles en la ISR para ver cuántas veces se activa EXTI.

---

## 🔌 Relación con otras técnicas

Esta técnica **combina**:

* ⚡ **EXTI (detección de flanco por hardware)**
* ⏱ **Validación por temporizador (software)**

Es superior a:

* **Delay bloqueante**: congela el sistema.
* **Verificación estable en loop**: más lenta o más propensa a errores si hay otras tareas.

Es una técnica profesional usada en aplicaciones embebidas críticas.

---

## 📚 Recursos

* ST RM0008 Reference Manual (sección de EXTI y TIM2).
* Jack Ganssle: "A Guide to Debouncing".
* [https://libopencm3.org](https://libopencm3.org)

---
