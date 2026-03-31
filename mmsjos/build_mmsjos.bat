@echo off
setlocal enabledelayedexpansion
set PATH=E:\MinGW64\bin;E:\MinGW-m68k-elf\bin;%PATH%

echo === Building MMSJOS ===
cd /d d:\Projetos\MMSJ320v2\mmsjos
make clean
make -B
if exist mmsjos.elf (
    E:\MinGW-m68k-elf\bin\m68k-elf-objdump.exe -x -d mmsjos.elf > mmsjos.lst
)

echo.
if exist mmsjos.elf (
    echo OK: mmsjos.elf created
    dir mmsjos.elf mmsjos.bin mmsjos.lst 2>&1 | findstr "mmsjos"
) else (
    echo ERROR: mmsjos.elf NOT created
)
