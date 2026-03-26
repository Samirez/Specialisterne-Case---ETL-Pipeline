@echo off
setlocal EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
set "PS_SCRIPT=%SCRIPT_DIR%docker_run.ps1"

if not exist "%PS_SCRIPT%" (
  echo ERROR: Missing script: %PS_SCRIPT%
  exit /b 1
)

set "ARGS="

:parse_args
if "%~1"=="" goto run_ps

if /I "%~1"=="Build" (
  set "ARGS=!ARGS! -Build"
) else if /I "%~1"=="-Build" (
  set "ARGS=!ARGS! -Build"
) else if /I "%~1"=="Down" (
  set "ARGS=!ARGS! -Down"
) else if /I "%~1"=="-Down" (
  set "ARGS=!ARGS! -Down"
) else if /I "%~1"=="Logs" (
  set "ARGS=!ARGS! -Logs"
) else if /I "%~1"=="-Logs" (
  set "ARGS=!ARGS! -Logs"
) else (
  echo ERROR: Unsupported argument: %~1
  echo Supported args: Build ^| -Build ^| Down ^| -Down ^| Logs ^| -Logs
  exit /b 1
)

shift
goto parse_args

:run_ps
powershell -NoProfile -ExecutionPolicy Bypass -File "%PS_SCRIPT%" %ARGS%
exit /b %ERRORLEVEL%