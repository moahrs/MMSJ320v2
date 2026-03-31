CC = m68k-elf-gcc
OBJCOPY = m68k-elf-objcopy
OBJDUMP = m68k-elf-objdump

CFLAGS = -m68000 -Wall -Os -ffreestanding -nostdlib -fcommon -w -I../include -Iinclude -I.. -IuCOSII -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -flto
LDFLAGS = -T linker_mmsjos.ld -nostdlib -Wl,--just-symbols=../monitor.elf -Wl,--allow-multiple-definition -flto
LDLIBS = -lgcc

# Flags dedicated to source-to-assembly listings (closer to IDE68K experience).
LIST_CFLAGS = -m68000 -g -O0 -fverbose-asm -ffreestanding -nostdlib -fcommon -w -I../include -Iinclude -I.. -IuCOSII

SRC_C = \
	uCOSII/os_cpu_c.c \
	uCOSII/ucos_ii.c \
	../src/mmsj320mfp.c \
	src/mmsjos.c \
	src/mgui.c

SRC_S = \
	src/mmsjos_start.S \
	src/uCOSII/os_cpu_a.S \
	src/mgui_table.S \
	src/mmsjos_ram.S \
	src/mgui_sprite.S

OBJ = $(SRC_C:.c=.o) $(SRC_S:.S=.o)

# Explicit link order matching the historical IDE68K project order.
# IDE68K file "mguifunc.S" maps to current GCC source "src/mgui_table.S".
LINK_OBJ = \
	src/mmsjos_start.o \
	src/uCOSII/os_cpu_a.o \
	uCOSII/os_cpu_c.o \
	uCOSII/ucos_ii.o \
	src/mgui_table.o \
	../src/mmsj320mfp.o \
	src/mmsjos.o \
	src/mgui.o \
	src/mmsjos_ram.o \
	src/mgui_sprite.o

TARGET = mmsjos.elf
BIN = mmsjos.bin
MAP = mmsjos.map
LST = mmsjos.lst
MIX_LST = mmsjos_mix.lst

C_LST = $(SRC_C:.c=.c.lst)

all: $(TARGET) $(BIN) $(LST)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -Wl,-Map=$(MAP) -o $(TARGET) $(LINK_OBJ) $(LDLIBS)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

$(LST): $(TARGET)
	$(OBJDUMP) -x -d $(TARGET) > $(LST)

$(MIX_LST): $(TARGET)
	$(OBJDUMP) -d -S -l $(TARGET) > $(MIX_LST)

listings: $(C_LST)
	$(OBJDUMP) -d -S -l $(TARGET) > $(MIX_LST)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.c.lst: src/%.c
	$(CC) $(LIST_CFLAGS) -c $< -Wa,-adhln > $@

uCOSII/%.o: uCOSII/%.c
	$(CC) $(CFLAGS) -c $< -o $@

uCOSII/%.c.lst: uCOSII/%.c
	$(CC) $(LIST_CFLAGS) -c $< -Wa,-adhln > $@

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c $< -o $@

src/uCOSII/%.o: src/uCOSII/%.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f src/*.o src/uCOSII/*.o uCOSII/*.o *.o *.elf *.bin *.map *.lst src/*.c.lst uCOSII/*.c.lst ../src/*.o

.PHONY: all clean listings
