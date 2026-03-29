
CC = m68k-elf-gcc
OBJDUMP = m68k-elf-objdump

CFLAGS = -m68000 -Wall -O2 -ffreestanding -nostdlib -Iinclude -w
LDFLAGS = -T linker.ld -nostdlib
LDLIBS = -lgcc

SRC = $(wildcard src/*.c) $(wildcard src/*.S)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.S=.o)

all: $(OBJ)
	$(CC) $(LDFLAGS) -Wl,-Map=monitor.map -o monitor.elf $(OBJ) $(LDLIBS)
	$(OBJDUMP) -x -d monitor.elf > monitor.lst

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@cmd /c "del /q src\*.o *.o monitor.elf monitor.bin monitor.map monitor.lst >nul 2>nul" || exit 0
