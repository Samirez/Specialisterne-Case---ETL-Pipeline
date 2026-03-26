#!/bin/bash
cd ..

set -e

mkdir -p bin

gcc -o bin/main src/*.c -lmicrohttpd $(pkg-config --cflags --libs libpq) -Wall -Wextra

if [[ "$1" == "--run" ]]; then
  echo "Executing the application..."
  ./bin/main
fi