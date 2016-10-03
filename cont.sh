#!/bin/bash

for param in params/cont*; do
    ./main.exe $param
    ./mainJack.exe $param
done
