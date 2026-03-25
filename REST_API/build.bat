@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0"
echo Compiling REST_API...
echo.

REM Run bash from MSYS64 with the compilation command
C:\msys64\ucrt64.exe bash -lc "gcc -v -o REST_API.exe main.c utils.c postgres_handler.c response_builder.c -I./headers -I/ucrt64/include -L/ucrt64/lib -lws2_32 2>&1"

echo.
echo Compilation finished with exit code: %ERRORLEVEL%
echo.

if exist REST_API.exe (
    echo SUCCESS: REST_API.exe created!
    dir REST_API.exe
    pause
) else (
    echo FAILED: REST_API.exe was not created
    pause
)
