#!/bin/bash

cd ..

CONTAINER_NAME="REST-API-C"

docker rm -f "$CONTAINER_NAME" >/dev/null 2>&1

docker build -t REST-API-C .

docker run -d -p 8080:80 --name "$CONTAINER_NAME" REST-API-C