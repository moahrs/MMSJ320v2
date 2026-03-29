@echo off
setlocal enabledelayedexpansion
cd /d d:\Projetos\MMSJ320v2
set PATH=E:\MinGW64\bin;E:\MinGW-m68k-elf\bin;%PATH%
echo === PATH includes ===
echo E:\MinGW64\bin
echo E:\MinGW-m68k-elf\bin
echo.
echo === Cleaning ===
make clean
echo.
echo === Building ===
make -B
if exist monitor.elf (
	E:\MinGW-m68k-elf\bin\m68k-elf-objdump.exe -x -d monitor.elf > monitor.lst
)
echo.
echo === Done ===
if exist monitor.lst dir monitor.elf monitor.bin monitor.lst 2>nul
