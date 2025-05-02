# Ejemplo de Timer con Interrupciones en STM32 (Blue Pill) usando libopencm3

Este ejemplo demuestra cómo usar el temporizador TIM2 junto con interrupciones (NVIC) para controlar el parpadeo de un LED conectado al pin PC13 de un microcontrolador STM32F103C8T6.

El objetivo es que sirva tanto como ejemplo funcional como material de apoyo para comprender el funcionamiento de los temporizadores, las interrupciones y el NVIC en microcontroladores ARM Cortex-M3.

---

## Tabla de contenidos

* [Objetivo del ejemplo](#objetivo-del-ejemplo)
* [Resumen del funcionamiento](#resumen-del-funcionamiento)
* [Temporizador TIM2](#temporizador-tim2)
* [NVIC y tabla de vectores](#nvic-y-tabla-de-vectores)
* [Relación con el 8051](#relación-con-el-8051)
* [Código y conceptos clave](#código-y-conceptos-clave)
* [Compilación y grabación](#compilación-y-grabación)
* [Apéndice conceptual](#apéndice-conceptual)

---

## Objetivo del ejemplo

* Configurar el **sistema de reloj** a 72 MHz
* Configurar un **temporizador** para generar una interrupción periódica (cada 500 ms)
* Atender esa interrupción para **alternar el estado del LED**
* Usar el **NVIC** (Nested Vectored Interrupt Controller) para habilitar la interrupción del timer

---

## Resumen del funcionamiento

* Se configura TIM2 para contar pulsos de reloj internos a 10 kHz
* Cuando el contador alcanza el valor definido (5000), se genera una interrupción
* El NVIC gestiona la solicitud de interrupción y transfiere el control al vector `tim2_isr()`
* En el ISR se cambia el estado del LED

---

## Temporizador TIM2

Los temporizadores en STM32 (como TIM2) permiten:

* Generar interrupciones periódicas
* Medir eventos externos
* Generar PWM, entre otras cosas

### Modos comunes de uso:

* **Contador ascendente:** cuenta de 0 hasta un valor (ARR - Auto Reload Register) y genera una interrupción

  > 🔍 **¿Qué es ARR?**
  >
  > Es el registro "Auto Reload Register" del timer. Define el valor máximo que alcanza el contador antes de reiniciarse a 0. Cuando el contador llega a ARR, se genera un evento de actualización (update event), que puede disparar una interrupción si está habilitada.
* **Contador descendente:** menos común
* **PWM / Input Capture / Output Compare:** no usados en este ejemplo

### Configuración mínima:

1. **Habilitar el reloj del periférico (RCC)**
2. **Configurar prescaler:** divide la frecuencia del sistema
3. **Configurar período (ARR):** cuándo se dispara la interrupción
4. **Habilitar la interrupción local en el timer**
5. **Habilitar la interrupción global en el NVIC**
6. **Activar el contador**

---

## NVIC y tabla de vectores

El **NVIC** es el componente del Cortex-M3 que se encarga de:

* Detectar y gestionar interrupciones de periféricos
* Priorizar y despachar interrupciones según su configuración

### Tabla de vectores

* Está ubicada al inicio de la memoria (generalmente en 0x00000000)
* Contiene punteros a funciones (ISR) para cada interrupción
* Cada periférico tiene una posición fija en la tabla

Por ejemplo:

| Vector | Periférico | Función ISR esperada  |
| ------ | ---------- | --------------------- |
| ...    | ...        | ...                   |
| 28     | TIM2       | `void tim2_isr(void)` |
| ...    | ...        | ...                   |

La startup o el linker script deben asegurar que estas funciones estén bien mapeadas.

---

## Relación con el 8051

Si trabajaste anteriormente con micro 8051, vale destacar algunas diferencias:

> 🧠 **Nota**: El 8051 permite interrupciones anidadas limitadas. Una ISR de prioridad baja puede ser interrumpida por otra de prioridad alta, pero no entre interrupciones del mismo nivel. Esto lo diferencia del Cortex-M3, donde las prioridades son más finas y permiten anidamiento generalizado.

| Concepto                 | 8051                            | Cortex-M3 / STM32                                |
| ------------------------ | ------------------------------- | ------------------------------------------------ |
| Número de interrupciones | 6 (fijas)                       | Decenas (NVIC configurable)                      |
| Prioridades              | Básicas (alta/baja)             | Configurables y permiten interrupciones anidadas |
| Vectorización            | Salto manual en código          | Vectorización automática vía tabla               |
| ISR nombre               | Predefinido (e.g. `TIMER0_ISR`) | Por convención (e.g. `tim2_isr`)                 |
| Activación global        | `EA = 1`                        | Implícita o mediante `cm_enable_interrupts()`    |

---

## Código y conceptos clave

A continuación se resumen las configuraciones más relevantes del timer y del NVIC, junto con la rutina de interrupción:

```c
// Configuración de TIM2

rcc_periph_clock_enable(RCC_TIM2);
timer_set_prescaler(TIM2, 7200 - 1); // Divide 72 MHz / 7200 = 10 kHz
timer_set_period(TIM2, 5000 - 1);    // 10 kHz / 5000 = 2 Hz = 500 ms
timer_enable_irq(TIM2, TIM_DIER_UIE); // Habilita interrupción de actualización (update)
timer_enable_counter(TIM2);
```

```c
// Habilitación del vector en NVIC

nvic_enable_irq(NVIC_TIM2_IRQ); // Habilita la interrupción global del TIM2 en el NVIC
```

```c
// ISR (rutina de servicio de interrupción)

void tim2_isr(void) {
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        timer_clear_flag(TIM2, TIM_SR_UIF);
        gpio_toggle(GPIOC, GPIO13);
    }
}
```

> 🔍 **¿Qué es `TIM_SR_UIF`?**
>
> Es el bit "Update Interrupt Flag" dentro del registro de estado del timer (`TIMx_SR`). Se activa automáticamente cuando el contador llega al valor del registro ARR. Para evitar múltiples disparos de la ISR, es obligatorio borrarlo manualmente dentro de la ISR llamando a `timer_clear_flag()`.

---

## Compilación y grabación

Para compilar y grabar el firmware se usan los siguientes comandos:

```bash
make         # Compila el proyecto
make flash   # Graba el binario en la Blue Pill usando OpenOCD
```

Asegurate de tener `arm-none-eabi-gcc`, `make` y `OpenOCD` correctamente instalados.

---

## Notas adicionales

* La llamada a `cm_enable_interrupts()` no es estrictamente necesaria en la mayoría de los casos, pero es buena práctica asegurar que las interrupciones estén habilitadas globalmente.

---

## Apéndice conceptual

### ¿Qué significa que las interrupciones sean "anidadas"?

En un Cortex-M3, el NVIC permite que una interrupción en curso sea interrumpida por otra de mayor prioridad. A esto se lo denomina interrupción anidada. El procesador guarda el contexto actual, atiende la nueva ISR, y luego retoma la anterior.

Esto es útil para atender eventos críticos sin demorar, incluso si otra ISR se está ejecutando. El 8051, por ejemplo, solo admite dos niveles de prioridad, y no todos los vectores pueden ser interrumpidos por otros.

### ¿Qué es un "vector de interrupción" y por qué se llama así?

Un "vector" en este contexto es una dirección de memoria que indica dónde empieza una rutina de atención a interrupciones (ISR). La "tabla de vectores" es un array ubicado en memoria que contiene punteros a las rutinas de interrupción.

Cuando ocurre una interrupción, el hardware consulta esa tabla y salta automáticamente a la dirección correspondiente. Este mecanismo evita tener que hacer verificaciones manuales, y se lo denomina vectorización automática.

El término "vector" proviene de la idea de apuntar o direccionar algo, como en matemática o física: un vector tiene dirección. Aquí, también: apunta a código ejecutable.

