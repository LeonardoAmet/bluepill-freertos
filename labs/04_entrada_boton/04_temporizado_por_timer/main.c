#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

// === Parámetros configurables ===
#define TIMER_FREQUENCY_HZ     1000     // Frecuencia deseada del timer: 1 kHz → interrupción cada 1 ms
#define TIMER_PRESCALER        7200     // Prescaler para reducir el clock: 72 MHz / 7200 = 10 kHz
#define UMBRAL_ESTABILIDAD     20       // 20 lecturas estables → se requieren 20 ms de estabilidad

// ⚠️ Nota sobre el cálculo del período del timer:
// En lugar de asumir una frecuencia fija como 72 MHz, utilizamos la variable
// global `rcc_ahb_frequency`, provista por libopencm3. Esta variable se
// actualiza automáticamente cuando se llama a `rcc_clock_setup_pll(...)`.
// Equivale conceptualmente a `SystemCoreClock` en CMSIS.
//
// De este modo, el valor de TIMER_PERIOD se adapta a la configuración activa
// del reloj del sistema, sin necesidad de hardcodear valores.
#define TIMER_PERIOD  (rcc_ahb_frequency / TIMER_PRESCALER / TIMER_FREQUENCY_HZ)

// Variables para gestionar la lógica del botón y el LED
volatile int contador_estabilidad   = 0;
volatile int estado_estable         = 1;
volatile int estado_anterior        = 1;
volatile int led_on                 = 0;

// === Interrupción del Timer TIM2 ===
// Se ejecuta automáticamente cada 1 ms
void tim2_isr(void) {
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        timer_clear_flag(TIM2, TIM_SR_UIF);  // Limpia la bandera de interrupción

        int lectura = gpio_get(GPIOA, GPIO0);  // Lee el botón (activo en bajo)

        if (lectura != estado_estable) {
            contador_estabilidad++;
            if (contador_estabilidad >= UMBRAL_ESTABILIDAD) {
                estado_estable = lectura;
                contador_estabilidad = 0;

                // Si hubo flanco descendente (1 → 0), togglear el LED
                if (estado_anterior && !estado_estable) {
                    led_on = !led_on;
                    if (led_on)
                        gpio_clear(GPIOC, GPIO13);  // LED ON
                    else
                        gpio_set(GPIOC, GPIO13);    // LED OFF
                }

                estado_anterior = estado_estable;
            }
        } else {
            contador_estabilidad = 0;
        }
    }
}

// === Configuración de GPIOs ===
// - PC13: salida push-pull para LED
// - PA0: entrada con pull-up interno para botón
static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);  // Apaga el LED (activo en bajo)

    gpio_set(GPIOA, GPIO0);  // Pull-up interno
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
}

// === Configuración del Timer TIM2 ===
// Se configura para interrumpir cada 1 ms, usando un prescaler y período calculados.
static void timer2_setup(void) {
    rcc_periph_clock_enable(RCC_TIM2);     // Habilita el reloj del periférico TIM2
    timer_disable_counter(TIM2);           // Detiene el contador durante la configuración

    // Configura el prescaler y el período:
    // - Prescaler: divide el clock de 72 MHz → 10 kHz
    // - Período: 10 kHz / 10 = 1 kHz → interrupción cada 1 ms
    timer_set_prescaler(TIM2, TIMER_PRESCALER - 1);
    timer_set_period(TIM2, TIMER_PERIOD - 1);

    timer_clear_flag(TIM2, TIM_SR_UIF);           // Limpia la bandera de interrupción
    timer_enable_irq(TIM2, TIM_DIER_UIE);         // Habilita interrupción por overflow (update)
    nvic_enable_irq(NVIC_TIM2_IRQ);               // Habilita TIM2 en el NVIC (controlador global de interrupciones)

    timer_enable_counter(TIM2);                   // Inicia el temporizador
}

// === Programa principal ===
int main(void) {
    // Configura el sistema a 72 MHz a partir de un cristal externo de 8 MHz
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    gpio_setup();
    timer2_setup();

    while (1) {
        // El LED se controla desde la ISR, no es necesario hacer nada aquí
    }
}
