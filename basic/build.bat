@echo off
setlocal enabledelayedexpansion
set PATH=E:\MinGW64\bin;E:\MinGW-m68k-elf\bin;%PATH%

echo === Building BASIC with Q16 backend ===
cd /d d:\Projetos\MMSJ320v2\basic
make clean
make -B
if exist basic.elf (
    E:\MinGW-m68k-elf\bin\m68k-elf-objdump.exe -x -d basic.elf > basic.lst
)

echo.
if exist basic.elf (
    echo OK: basic.elf created
    dir basic.elf basic.bin basic.lst 2>&1 | findstr "basic"
) else (
    echo ERROR: basic.elf NOT created
)
