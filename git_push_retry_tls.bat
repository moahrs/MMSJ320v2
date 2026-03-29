@echo off
setlocal enabledelayedexpansion
cd /d d:\Projetos\MMSJ320v2

echo [git] Applying transport overrides for this push session...
set GIT_TRACE_CURL=0
set GIT_CURL_VERBOSE=0

echo [git] Trying push with retries...
set MAX_RETRIES=5
set WAIT_SECS=8
set N=1

:retry
echo.
echo Attempt !N! of %MAX_RETRIES%...
git push -u origin main
if %ERRORLEVEL% EQU 0 (
  echo.
  echo Push succeeded.
  exit /b 0
)

if !N! GEQ %MAX_RETRIES% (
  echo.
  echo Push failed after %MAX_RETRIES% attempts.
  echo Tip: if HTTPS keeps failing, switch remote to SSH and try again.
  exit /b 1
)

echo Push failed, waiting %WAIT_SECS%s before retry...
timeout /t %WAIT_SECS% /nobreak >nul
set /a N+=1
goto retry
