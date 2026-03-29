@echo off
setlocal enabledelayedexpansion
cd /d d:\Projetos\MMSJ320v2\basic
set PATH=E:\MinGW64\bin;E:\MinGW-m68k-elf\bin;%PATH%
echo === Building BASIC with Q16 backend ===
echo.
echo === Cleaning old artifacts ===
make clean
echo.
echo === Building BASIC.elf ===
make -B
if exist basic.elf (
	E:\MinGW-m68k-elf\bin\m68k-elf-objdump.exe -x -d basic.elf > basic.lst
)
echo.
echo === BASIC build complete ===
dir basic.elf basic.bin basic.map basic.lst 2>&1 | findstr "basic\."
