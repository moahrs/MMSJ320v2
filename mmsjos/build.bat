@echo off
setlocal

cd /d %~dp0

set MAKE_EXE=E:\MinGW64\bin\make.exe
if not exist "%MAKE_EXE%" set MAKE_EXE=make

echo [MMSJOS] Cleaning...
%MAKE_EXE% clean
if errorlevel 1 goto :err

echo [MMSJOS] Building...
%MAKE_EXE% -B
if errorlevel 1 goto :err

echo.
echo [MMSJOS] Build OK
if exist mmsjos.elf echo   mmsjos.elf
if exist mmsjos.bin echo   mmsjos.bin
if exist mmsjos.map echo   mmsjos.map
if exist mmsjos.lst echo   mmsjos.lst

exit /b 0

:err
echo.
echo [MMSJOS] Build FAILED
exit /b 1
