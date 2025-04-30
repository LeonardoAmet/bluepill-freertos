PROJ = blink
BUILD_DIR = bin

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -Wextra -Os -g \
  -I../../libopencm3/include \
  -mthumb -mcpu=cortex-m3 \
  -T../../common/linker.ld

LDFLAGS = -nostartfiles -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(PROJ).map \
  -L../../libopencm3/lib \
  -lopencm3_stm32f1

SRC = main.c

$(BUILD_DIR)/$(PROJ).elf: $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	$(OBJCOPY) -O ihex   $@ $(BUILD_DIR)/$(PROJ).hex
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(PROJ).bin

all: $(BUILD_DIR)/$(PROJ).elf

clean:
	rm -rf $(BUILD_DIR)
