AS=arm-none-eabi-as
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
BUILD_DIR=build
SRC_DIR=monitor_src

ASFLAGS=-g
CFLAGS=-mcpu=cortex-m4 -mthumb -ffreestanding -g -Og

LINKER_SCRIPT=link_sram.ld
OBJ_NAMES=startup.o hardware.o monitor.o
BUILD_OBJS=$(addprefix $(BUILD_DIR)/,$(OBJ_NAMES))

$(BUILD_DIR)/monitor: $(LINKER_SCRIPT) $(BUILD_OBJS)
	$(LD) -T $(LINKER_SCRIPT) -o $@ $(BUILD_OBJS)

$(BUILD_OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
