/*!
 * \file conv_serial.c
 * \author William D. Goins V ("Quin") (116991920+wdg0008@users.noreply.github.com)
 * \brief Prototype convolution algorithm written in portable standard C17 code
 * \version 0.1
 * \date 2026-11-22
 *
 * \copyright Copyright (c) 2026
 *
 */

#include <stdio.h>
#include "../ctimer_utils.h"

#define TEST_LEN 32
#define CONV_LEN (TEST_LEN*2 - 1)

int convolve(
    const double * restrict x, const size_t xlen,
    const double * restrict h, const size_t hlen,
    double * restrict y, const size_t ylen)
{
    if (ylen < xlen + hlen - 1)
        return -1;

    int ny = xlen + hlen - 1;

    for (int r = 0; r < ny; r++)
    {
        // max(0, r - hlen + 1); // but check to avoid unsigned underflow
        const size_t lo = (r >= hlen) ? r - hlen + 1 : 0;

        // min(xlen-1, r); // but check to avoid unsigned underflow
        const size_t hi = (r < xlen) ? r : (xlen - 1);

        double sum = 0.0;

        for (size_t i = lo; i <= hi; i++) {
            sum += x[i] * h[r-i];
        }

        y[r] = sum;
    }
    return 0;
}

void print_arr(const double * arr, const size_t len)
{
    printf("{ ");
    for (size_t i = 0; i < len; i++)
    {
        printf("%.1lf, ", arr[i]);
    }
    printf(" }\n");
    return;
}

int main(int argc, char* argv[])
{
    double x[TEST_LEN];
    for (int i = 0; i < TEST_LEN; i++)
    {
        x[i] = 1;
    }

    print_arr(x, TEST_LEN);

    double y[CONV_LEN] = {0};

    TIMER_START;
    convolve(x, TEST_LEN, x, TEST_LEN, y, CONV_LEN);
    TIMER_STOP;
    print_arr(y, CONV_LEN);

    printf("Time elapsed (seconds): %lf", TIMER_ELAPSED); 

    return 0;
}
