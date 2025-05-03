# Lectura de entrada digital

## Técnicas de Antirrebote con STM32 (Blue Pill) y libopencm3

Este conjunto de ejemplos tiene como objetivo explorar distintas técnicas para gestionar el rebote mecánico de botones (debounce) en sistemas embebidos, utilizando una placa STM32F103C8T6 ("Blue Pill") y la biblioteca [libopencm3](https://github.com/libopencm3/libopencm3).

Este recorrido forma parte del estudio de periféricos en sistemas embebidos, a partir de la necesidad de realizar una lectura confiable de un pin GPIO conectado a un botón. Esta operación, que parece trivial, se vuelve problemática cuando no se tiene en cuenta el fenómeno del rebote de los contactos, ya sea de un pulsador como de otros dispositivos de entrada que conmutan un "contacto seco". El rebote puede provocar múltiples activaciones por una única pulsación, resultando en un comportamiento errático.

Los ejemplos permiten comparar distintas estrategias, entender  ventajas y limitaciones, y aprender a usar periféricos como temporizadores, interrupciones externas y técnicas mixtas.

## 📁 Estructura

```text
04_entrada_boton/
├── 01_sin_antirrebote
├── 02_con_retardo_bloqueante
├── 03_verificacion_estable
├── 04_temporizado_por_timer
├── 05_interrupcion_exti
└── README.md (este archivo)
```

## 🔍 Técnicas implementadas

| Técnica               | Descripción breve                                                          | Periféricos usados        |
| --------------------- | -------------------------------------------------------------------------- | ------------------------- |
| Sin antirrebote       | El botón se lee directamente, sin filtrar rebotes                          | GPIO                      |
| Retardo bloqueante    | Agrega un `delay` luego de detectar una pulsación                          | GPIO, ciclo ocupado       |
| Verificación estable  | Verifica que el botón se mantenga presionado cierto tiempo antes de actuar | GPIO, bucle no bloqueante |
| Temporizado por timer | Usa un temporizador que verifica estabilidad tras interrupción             | GPIO, Timer, NVIC         |
| Interrupción EXTI     | Usa interrupciones externas combinadas con timer para antirrebote preciso  | EXTI, GPIO, Timer, NVIC   |

## 🔧 Requisitos para compilar y grabar

* toolchain `arm-none-eabi-gcc`
* `make`
* biblioteca **libopencm3** incluida como submódulo del repositorio
* herramienta `openocd` para flashear la placa

Para compilar y grabar en la placa:

```bash
make
make flash
```

## 📚 Lectura recomendada

* Jack Ganssle: *A Guide to Debouncing* (artículo de referencia con fundamentos físicos y prácticos sobre el rebote de contactos)

## 💡 Sugerencia

* Visualizar el rebote con un osciloscopio para generar conciencia del problema
