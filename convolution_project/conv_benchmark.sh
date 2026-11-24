#!/bin/bash
# ASC ASAX
set -euo pipefail

make

sizes=(256 512 1024 2048 4096 8192 16384 32768 65536)
for size in "${sizes[@]}"; do
    echo "Serial test: size = $size"
    ./conv_serial.out "$size" S
done

threads=(1 2 5 10 16 24 31)
for size in "${sizes[@]}"; do
    for thread_count in "${threads[@]}"; do
        echo "OpenMP test: size = $size, threads = $thread_count"
        OMP_NUM_THREADS="$thread_count" ./conv_OMP.out "$size" S
    done
done