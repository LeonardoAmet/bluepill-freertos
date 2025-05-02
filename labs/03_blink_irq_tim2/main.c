#include <libopencm3/stm32/rcc.h>     // Reloj del sistema y periféricos
#include <libopencm3/stm32/gpio.h>    // Control de pines GPIO
#include <libopencm3/stm32/timer.h>   // Temporizadores (TIM2)
#include <libopencm3/cm3/nvic.h>      // Control de interrupciones NVIC
#include <libopencm3/cm3/cortex.h>    // Funciones del núcleo Cortex-M

// --- Constantes para cálculo del timer ---
#define SYS_FREQ_HZ     72000000U      // Frecuencia del sistema (72 MHz)
#define TIMER_FREQ_HZ   10000U         // Frecuencia base del timer (10 kHz)
#define BLINK_FREQ_HZ   2U             // Frecuencia de parpadeo del LED (2 Hz)

#define TIMER_PRESCALER ((SYS_FREQ_HZ / TIMER_FREQ_HZ) - 1)
#define TIMER_PERIOD    ((TIMER_FREQ_HZ / BLINK_FREQ_HZ) - 1)

// --- Variable global para depuración (opcional) ---
volatile uint32_t blink_count = 0;

// --- ISR de TIM2 ---
// Se ejecuta al vencer el período configurado.
void tim2_isr(void) {
    if (timer_get_flag(TIM2, TIM_SR_UIF)) {
        timer_clear_flag(TIM2, TIM_SR_UIF);        // Limpia la bandera de interrupción
        gpio_toggle(GPIOC, GPIO13);                // Cambia el estado del LED
        blink_count++;                             // Incrementa el contador
    }
}

// --- Configuración del LED (PC13 como salida push-pull) ---
void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);            // Habilita reloj para GPIOC
    gpio_set_mode(GPIOC,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO13);
    gpio_set(GPIOC, GPIO13);                       // Apaga el LED (activo en bajo)
}

// --- Configuración del temporizador TIM2 con interrupción ---
void timer2_setup(void) {
    rcc_periph_clock_enable(RCC_TIM2);             // Habilita reloj para TIM2

    timer_disable_counter(TIM2);                   // Detiene el contador (por seguridad)
    timer_set_prescaler(TIM2, TIMER_PRESCALER);    // Configura prescaler
    timer_set_period(TIM2, TIMER_PERIOD);          // Configura período

    timer_set_counter(TIM2, 0);                    // Reinicia el contador
    timer_clear_flag(TIM2, TIM_SR_UIF);            // Limpia bandera de interrupción

    timer_enable_irq(TIM2, TIM_DIER_UIE);          // Habilita interrupción por "update"
    nvic_enable_irq(NVIC_TIM2_IRQ);                // Habilita TIM2 en NVIC

    timer_enable_counter(TIM2);                    // Inicia el contador
}

// --- Configuración del reloj del sistema a 72 MHz desde HSE ---
void system_clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

// --- Función principal ---
int main(void) {
    system_clock_setup();         // Configura reloj del sistema
    gpio_setup();                 // Configura LED en PC13
    timer2_setup();               // Configura TIM2 e interrupciones
    // Habilita interrupciones globales.
    // No siempre es necesario porque suelen estar activadas por defecto,
    // pero es buena práctica dejarlo explícito.
    cm_enable_interrupts();

    while (1) {
        // El LED parpadea en la ISR. Aquí podrían realizarse otras tareas.
    }
}
