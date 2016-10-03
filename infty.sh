#!/bin/bash

for param in params/infty*; do
    ./main.exe $param
    ./mainJack.exe $param
done
