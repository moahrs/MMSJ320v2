@echo off
cd /d %~dp0

echo ============================================
echo  MMSJ320v2 BIOS - Build + Flash Preparation
echo ============================================

echo.
echo [1/3] Building...
make clean
make -B
if errorlevel 1 (
    echo.
    echo BUILD FAILED!
    pause
    exit /b 1
)

echo.
echo [2/3] Converting ELF to binary...
E:\MinGW-m68k-elf\bin\m68k-elf-objcopy.exe -O binary monitor.elf monitor.bin
if errorlevel 1 (
    echo.
    echo OBJCOPY FAILED!
    pause
    exit /b 1
)

echo.
echo Done. monitor.bin ready for flash.
pause
