@echo off
cd /d %~dp0

echo ============================================
echo  MMSJ320v2 BIOS - Build + Flash Preparation
echo ============================================

echo.
echo [1/2] Building...
make clean
make -B
if errorlevel 1 (
    echo.
    echo BUILD FAILED!
    pause
    exit /b 1
)

echo.
echo [2/2] Converting ELF to binary...
m68k-elf-objcopy -O binary monitor.elf monitor.bin
if errorlevel 1 (
    echo.
    echo OBJCOPY FAILED!
    pause
    exit /b 1
)

echo.
echo Done. monitor.bin ready for flash.
pause
