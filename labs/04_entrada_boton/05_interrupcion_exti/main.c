#include <libopencm3/stm32/rcc.h>      // Relojes del sistema
#include <libopencm3/stm32/gpio.h>     // Configuración de puertos GPIO
#include <libopencm3/stm32/exti.h>     // Interrupciones externas (EXTI)
#include <libopencm3/stm32/timer.h>    // Timers (TIM2)
#include <libopencm3/cm3/nvic.h>       // NVIC: controlador de interrupciones
#include <stdbool.h>                   // Para usar tipo bool

// === Parámetros configurables ===
#define UMBRAL_ESTABILIDAD     20        // 20 lecturas estables → se requieren 20 ms de estabilidad
#define TIMER_PRESCALER        7200      // 72 MHz / 7200 = 10 kHz
#define TIMER_FREQ_HZ          1000      // 10 kHz / 10 = 1 kHz (1 ms entre interrupciones)
#define TIMER_PERIOD           (rcc_ahb_frequency / TIMER_PRESCALER / TIMER_FREQ_HZ)

// === Variables globales (compartidas entre main e ISRs) ===
volatile int contador_estabilidad   = 0;
volatile int estado_estable         = 1;
volatile int lectura_anterior       = 1;
volatile int led_on                 = 0;

// === Interrupción del Timer TIM2 ===
// Se ejecuta automáticamente cada 1 ms
void tim2_isr(void) {
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        timer_clear_flag(TIM2, TIM_SR_UIF);  // Limpia la bandera de interrupción

        int lectura = gpio_get(GPIOA, GPIO0);  // Lee el botón (activo en bajo)

        if (lectura == lectura_anterior) {
            contador_estabilidad++;
            if (contador_estabilidad >= UMBRAL_ESTABILIDAD) {
                estado_estable = lectura;
                contador_estabilidad = 0;

                // Si es estado estable es 0, entonces efectivamente detectamos una pulsación
                if (estado_estable == 0) {
                    led_on = !led_on;
                    if (led_on)
                        gpio_clear(GPIOC, GPIO13);  // LED ON
                    else
                        gpio_set(GPIOC, GPIO13);    // LED OFF
                }

                // Fin del proceso de antirrebote
                timer_disable_irq(TIM2, TIM_DIER_UIE);
                timer_disable_counter(TIM2);
                exti_reset_request(EXTI0);
                exti_enable_request(EXTI0);
            }
        } else {
            contador_estabilidad = 0;
        }
        // Actualiza la lectura anterior
        lectura_anterior = lectura;
    }
}

// === ISR de EXTI0 ===
// Se ejecuta cuando se detecta un flanco descendente en PA0.
void exti0_isr(void) {
    exti_disable_request(EXTI0);
    exti_reset_request(EXTI0);

    contador_estabilidad = 0;

    timer_set_counter(TIM2, 0);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}


// === Configuración de pines GPIO ===
static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);  // PA0 (botón)
    rcc_periph_clock_enable(RCC_GPIOC);  // PC13 (LED)

    // PC13 como salida push-pull
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);  // LED apagado (activo en bajo)

    // PA0 como entrada con resistencia de pull-up interna
    gpio_set(GPIOA, GPIO0);  // Para activar el pull-up
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
}

// === Configuración de EXTI0 ===
// Ruta PA0 → EXTI0, flanco descendente, interrupción activada
static void exti_setup(void) {
    rcc_periph_clock_enable(RCC_AFIO);                       // AFIO necesario para enrutar EXTI
    exti_select_source(EXTI0, GPIOA);                        // PA0 → EXTI0
    exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);           // Flanco descendente
    exti_enable_request(EXTI0);                              // Habilita EXTI0
    nvic_enable_irq(NVIC_EXTI0_IRQ);                         // Habilita en NVIC
}

// === Configuración del Timer 2 ===
// Temporizador para generar interrupciones cada 1 ms
static void timer_setup(void) {
    rcc_periph_clock_enable(RCC_TIM2);
    timer_set_prescaler(TIM2, TIMER_PRESCALER - 1);
    timer_set_period(TIM2, TIMER_PERIOD - 1);
    nvic_enable_irq(NVIC_TIM2_IRQ);
}

// === Función principal ===
int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]); // Sistema a 72 MHz

    gpio_setup();
    exti_setup();
    timer_setup();

    while (1) {
        // Acá no se hace nada, el trabajo real ocurre en las ISR
    }
}
// === Fin del programa ===
