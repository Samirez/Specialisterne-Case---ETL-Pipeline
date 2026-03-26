@echo off
setlocal enabledelayedexpansion

set "CHERE_INVOKING=1"
cd /d "%~dp0"
echo Compiling REST_API...
echo.

REM Run gcc in non-interactive MSYS2 UCRT64 shell and preserve gcc exit code
set "MSYSTEM=UCRT64"
C:\msys64\usr\bin\bash.exe --login -c "gcc -v -o REST_API.exe main.c utils.c postgres_handler.c response_builder.c -I./headers -I/ucrt64/include -L/ucrt64/lib -lws2_32 2>&1"
set "BUILD_EXIT=%ERRORLEVEL%"

echo.
echo Compilation finished with exit code: %BUILD_EXIT%
echo.

if exist "%~dp0REST_API.exe" (
    echo SUCCESS: REST_API.exe created!
    dir "%~dp0REST_API.exe"
    pause
) else (
    echo FAILED: REST_API.exe was not created
    pause
)
