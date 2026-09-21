#!/bin/bash

echo "Original Serial Case" > heat_1d_PTH5.txt
./heat_1d_serial.out 71663615 50 S >> heat_1d_PTH5.txt

echo "1-Worker Thread pthread Case" >> heat_1d_PTH5.txt
./heat_1d_PTH5.out 71663615 50 1 S >> heat_1d_PTH5.txt

echo "2-Worker Thread pthread Case" >> heat_1d_PTH5.txt
./heat_1d_PTH5.out 71663615 50 2 S >> heat_1d_PTH5.txt

echo "3-Worker Thread pthread Case" >> heat_1d_PTH5.txt
./heat_1d_PTH5.out 71663615 50 3 S >> heat_1d_PTH5.txt

echo "4-Worker Thread pthread Case" >> heat_1d_PTH5.txt
./heat_1d_PTH5.out 71663615 50 4 S >> heat_1d_PTH5.txt