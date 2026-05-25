AS=arm-none-eabi-as
CC=arm-none-eabi-gcc
HOST_CC=cc
HOST_AR=ar
HOST_OBJCOPY=objcopy
LD=arm-none-eabi-ld
BUILD_DIR=build
B_HOST_DIR=$(BUILD_DIR)/host
SRC_DIR=monitor_src

ASFLAGS=-g
CFLAGS=-mcpu=cortex-m4 -mthumb -ffreestanding -g -Og

LINKER_SCRIPT=link_sram.ld
C_OBJ_NAMES=monitor.o vt.o assert.o
ASM_OBJ_NAMES=startup.o hardware.o
OBJ_NAMES=$(ASM_OBJ_NAMES) $(C_OBJ_NAMES)
BUILD_OBJS=$(addprefix $(BUILD_DIR)/,$(OBJ_NAMES))
HOST_BUILD_OBJS=$(addprefix $(B_HOST_DIR)/,$(C_OBJ_NAMES))
DEMO_DEPS=$(BUILD_DIR)/demo_monitor_redefined.a $(B_HOST_DIR)/demo/fakes.o $(B_HOST_DIR)/demo/demo_support.o

all: $(BUILD_DIR)/monitor $(BUILD_DIR)/demo_gethexword $(BUILD_DIR)/demo_test

$(BUILD_DIR)/monitor: $(LINKER_SCRIPT) $(BUILD_OBJS)
	$(LD) -T $(LINKER_SCRIPT) -o $@ $(BUILD_OBJS)

$(BUILD_DIR)/demo_gethexword: demos/gethexword.c $(DEMO_DEPS)
	$(HOST_CC) -g -o $@ $^

$(BUILD_DIR)/demo_test: demos/test.c $(DEMO_DEPS)
	$(HOST_CC) -g -o $@ $^

$(BUILD_DIR)/demo_monitor.a: $(HOST_BUILD_OBJS)
	$(HOST_AR) rcs $@ $^

$(BUILD_DIR)/demo_monitor_redefined.a: $(BUILD_DIR)/demo_monitor.a demos/symbol_rename_map
	$(HOST_OBJCOPY) --redefine-syms=demos/symbol_rename_map $< $@

$(BUILD_OBJS): | $(BUILD_DIR)
$(HOST_BUILD_OBJS): | $(B_HOST_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(B_HOST_DIR):
	mkdir -p $(B_HOST_DIR)/demo

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(B_HOST_DIR)/%.o: $(SRC_DIR)/%.c
	$(HOST_CC) -g -c -DHOST -ffreestanding -o $@ $<

$(B_HOST_DIR)/demo/%.o: demos/%.c
	$(HOST_CC) -g -c -o $@ $<

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
