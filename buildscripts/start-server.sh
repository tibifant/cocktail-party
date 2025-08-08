#!/bin/bash

echo "Starting nginx..."
nginx
ps -ax | grep nginx
echo "Starting server..."
./cocktail-party
