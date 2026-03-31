CC = m68k-elf-gcc
OBJCOPY = m68k-elf-objcopy
OBJDUMP = m68k-elf-objdump

CFLAGS = -m68000 -Wall -Os -ffreestanding -nostdlib -w -I../include -Iinclude -I.. -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -flto
LDFLAGS = -T ../linker.ld -nostdlib -Wl,--just-symbols=../monitor.elf -flto
LDLIBS = -lgcc

SRC_C = \
	../src/mmsj320mfp.c \
	src/basic.c \
	src/float_alt_q16.c

SRC_S = \
	src/basic_inic.S

OBJ = $(SRC_C:.c=.o) $(SRC_S:.S=.o)

# Explicit link order matching historical IDE68K project sequence.
# IDE68K "basic_fpp.S" maps to the new Q16 floating-point implementation in src/float_alt_q16.c.
LINK_OBJ = \
	src/basic_inic.o \
	../src/mmsj320mfp.o \
	src/basic.o \
	src/float_alt_q16.o

# Monitor code is NOT embedded in BASIC.
# Symbol addresses are resolved from monitor.elf (already resident in memory).

TARGET = basic.elf
BIN = basic.bin
MAP = basic.map
LST = basic.lst

all: $(TARGET) $(BIN) $(LST)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -Wl,-Map=$(MAP) -o $(TARGET) $(LINK_OBJ) $(LDLIBS)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

$(LST): $(TARGET)
	$(OBJDUMP) -x -d $(TARGET) > $(LST)

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.a68
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f src/*.o *.o *.elf *.bin *.map *.lst ../src/*.o

.PHONY: all clean
