#!/bin/bash
# ASC ASAX
set -euo pipefail

make

csv_file="conv_benchmark.csv"
printf 'thread_count,size,execution_time\n' > "$csv_file"

sizes=(256 512 1024 2048 4096 8192 16384 32768 65536)
for size in "${sizes[@]}"; do
    echo "Serial test: size = $size"
    execution_time=$(./conv_serial.out "$size" C)
    printf '0,%s,%s\n' "$size" "$execution_time" >> "$csv_file"
done

threads=(1 2 5 10 16 24 31)
for thread_count in "${threads[@]}"; do
    for size in "${sizes[@]}"; do
        echo "OpenMP test: size = $size, threads = $thread_count"
        execution_time=$(OMP_NUM_THREADS="$thread_count" ./conv_OMP.out "$size" C)
        printf '%s,%s,%s\n' "$thread_count" "$size" "$execution_time" >> "$csv_file"
    done
done