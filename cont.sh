#!/bin/bash

for param in params/cont*.par; do
    ./main.exe $param
done
