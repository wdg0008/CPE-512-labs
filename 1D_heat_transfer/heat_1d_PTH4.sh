#!/bin/bash

echo "Original Serial Case" > heat_1d_PTH4.txt
./heat_1d_serial.out 71663616 20 S >> heat_1d_PTH4.txt

echo "1-Worker Thread pthread Case" >> heat_1d_PTH4.txt
./heat_1d_PTH4.out 71663616 20 1 S >> heat_1d_PTH4.txt

echo "2-Worker Thread pthread Case" >> heat_1d_PTH4.txt
./heat_1d_PTH4.out 71663616 20 2 S >> heat_1d_PTH4.txt

echo "3-Worker Thread pthread Case" >> heat_1d_PTH4.txt
./heat_1d_PTH4.out 71663616 20 3 S >> heat_1d_PTH4.txt

echo "4-Worker Thread pthread Case" >> heat_1d_PTH4.txt
./heat_1d_PTH4.out 71663616 20 4 S >> heat_1d_PTH4.txt