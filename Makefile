AS=arm-none-eabi-as
CC=arm-none-eabi-gcc
HOST_CC=cc
HOST_OBJCOPY=objcopy
LD=arm-none-eabi-ld
BUILD_DIR=build
SRC_DIR=monitor_src

ASFLAGS=-g
CFLAGS=-mcpu=cortex-m4 -mthumb -ffreestanding -g -Og

LINKER_SCRIPT=link_sram.ld
OBJ_NAMES=startup.o hardware.o monitor.o
BUILD_OBJS=$(addprefix $(BUILD_DIR)/,$(OBJ_NAMES))

all: $(BUILD_DIR)/monitor $(BUILD_DIR)/demo_gethexword

$(BUILD_DIR)/monitor: $(LINKER_SCRIPT) $(BUILD_OBJS)
	$(LD) -T $(LINKER_SCRIPT) -o $@ $(BUILD_OBJS)

$(BUILD_DIR)/demo_monitor.o: $(SRC_DIR)/monitor.c
	$(HOST_CC) -c -DHOST -ffreestanding -o $@ $^

$(BUILD_DIR)/demo_monitor_redefined.o: $(BUILD_DIR)/demo_monitor.o demos/symbol_rename_map
	$(HOST_OBJCOPY) --redefine-syms=demos/symbol_rename_map $< $@

$(BUILD_DIR)/demo_gethexword: demos/gethexword.c demos/fakes.c $(BUILD_DIR)/demo_monitor_redefined.o
	$(HOST_CC) -o $@ $^

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
