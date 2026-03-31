@echo off
set PATH=E:\MinGW64\bin;E:\MinGW-m68k-elf\bin;%PATH%
cd /d %~dp0
echo === CLEAN === > build_test.log 2>&1
make clean >> build_test.log 2>&1
echo. >> build_test.log
echo === BUILD === >> build_test.log
make -B >> build_test.log 2>&1
echo. >> build_test.log
echo === FILES === >> build_test.log
for %%I in (mmsjos.bin mmsjos.elf mmsjos.map) do @if exist %%I echo %%~nxI %%~zI bytes>> build_test.log
echo. >> build_test.log
echo === LISTINGS === >> build_test.log
make listings >> build_test.log 2>&1
if errorlevel 1 (
    echo LISTINGS_FAIL >> build_test.log
    goto :end
)
echo ALL_OK >> build_test.log
:end
echo Done. See build_test.log
