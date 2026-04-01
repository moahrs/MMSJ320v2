
CC = m68k-elf-gcc
OBJDUMP = m68k-elf-objdump

CFLAGS = -m68000 -Wall -O2 -ffreestanding -nostdlib -Iinclude -w
LDFLAGS = -T linker.ld -nostdlib
LDLIBS = -lgcc

# Keep explicit source lists for deterministic build/link behavior.
SRC_C = \
	src/libc_stub.c \
	src/mmsj320vdp.c \
	src/mmsj320mfp.c \
	src/monitor.c

SRC_S = \
	src/os_boot.S \
	src/monitor_ram.S \
	src/monitor_fontes.S

OBJ = $(SRC_C:.c=.o) $(SRC_S:.S=.o)

# Link order mapped from monitor.prj:
#   os_boot.asm -> os_boot.S
#   mmsj320vdp.c
#   mmsj320mfp.c
#   monitor.c
#   monitor_ram.a68 -> monitor_ram.S
#   monitor_fontes.a68 -> monitor_fontes.S
# std68k.lib replacement in GCC flow: libc_stub.c.
LINK_OBJ = \
	src/os_boot.o \
	src/mmsj320vdp.o \
	src/mmsj320mfp.o \
	src/monitor.o \
	src/monitor_ram.o \
	src/monitor_fontes.o \
	src/libc_stub.o

all: $(OBJ)
	$(CC) $(LDFLAGS) -Wl,-Map=monitor.map -o monitor.elf $(LINK_OBJ) $(LDLIBS)
	$(OBJDUMP) -x -d monitor.elf > monitor.lst

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@cmd /c "del /q src\*.o *.o monitor.elf monitor.bin monitor.map monitor.lst >nul 2>nul" || exit 0
