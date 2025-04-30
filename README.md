# bluepill-libopencm3

Este repositorio contiene ejemplos de uso de libopencm3 con la placa STM32F103C8T6 (Blue Pill).

## Requisitos

- Toolchain: arm-none-eabi-gcc
- Make
- Git

## Pasos

```bash
git clone --recurse-submodules ...
cd labs/01_blink_gpio
make
```

## Submódulo libopencm3

Asegurate de inicializar el submódulo:

```bash
git submodule update --init
cd libopencm3
make
```
