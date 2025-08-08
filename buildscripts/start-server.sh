#!/bin/bash

echo "Start nginx..."
nginx
ps -ax | grep nginx
echo "Start server..."
./cocktail-party
