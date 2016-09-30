#!/bin/bash

for param in params/infty*.par; do
    ./main.exe $param
done
