CC = m68k-elf-gcc
OBJCOPY = m68k-elf-objcopy
OBJDUMP = m68k-elf-objdump

CFLAGS = -m68000 -Wall -Os -ffreestanding -nostdlib -w -Iinclude -I.. -I../include -ffunction-sections -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -flto
LDFLAGS = -T ../linker.ld -nostdlib -Wl,--just-symbols=../monitor.elf -flto
LDLIBS = -lgcc

SRC_C = src/basic.c src/float_alt_q16.c
OBJ = src/basic.o src/float_alt_q16.o
# Startup assembly only - FFP stubs not needed (Q16 backend replaces all FFP calls)
STARTUP_OBJ = src/basic_inic.o

# Monitor code is NOT embedded in BASIC.
# Symbol addresses are resolved from monitor.elf (already resident in memory).

TARGET = basic.elf
BIN = basic.bin
MAP = basic.map
LST = basic.lst

all: $(TARGET) $(BIN) $(LST)

$(TARGET): $(OBJ) $(STARTUP_OBJ)
	$(CC) $(LDFLAGS) -Wl,-Map=$(MAP) -o $(TARGET) $(STARTUP_OBJ) $(OBJ) $(LDLIBS)

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)

$(LST): $(TARGET)
	$(OBJDUMP) -x -d $(TARGET) > $(LST)

src/basic_inic.o: src/basic_inic.S
	$(CC) $(CFLAGS) -c src/basic_inic.S -o src/basic_inic.o

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.a68
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-del /q src\*.o *.o *.elf *.bin *.map *.lst 2>nul

.PHONY: all clean
