#!/bin/bash
# ASC ASAX
set -euo pipefail

module load gcc
make

dimension=10
iterations=100

echo "Serial test: n=${dimension}, iterations=${iterations}"
./heat_2d_serial.out "$dimension" "$iterations" S

threads=(1 2 5 10)
echo "OpenMP tests: n=${dimension}, iterations=${iterations}"
for thread_count in "${threads[@]}"; do
    echo "Threads: $thread_count"
    OMP_NUM_THREADS="$thread_count" ./heat_2d_OMP.out "$dimension" "$iterations" S
done
