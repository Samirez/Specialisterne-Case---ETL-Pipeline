#!/bin/sh
set -e

echo "Compiling REST API..."
mkdir -p bin

gcc -o bin/REST_API \
    $(find /app -maxdepth 1 -name '*.c') \
    -I./headers \
    $(pkg-config --cflags libpq) \
    -lmicrohttpd \
    $(pkg-config --libs libpq) \
    -lssl -lcrypto \
    -Wall -Wextra

echo "Compilation successful. Starting REST API on port 8080..."
exec ./bin/REST_API
