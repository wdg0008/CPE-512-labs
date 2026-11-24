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
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "../ctimer_utils.h"
#include "conv_cdefs.h"
#include <stdbool.h>

int convolve(
    const double * RESTRICT x, const size_t xlen,
    const double * RESTRICT h, const size_t hlen,
    double * RESTRICT y, const size_t ylen)
{
    // validate sizes of arrays
    if (xlen == 0 || hlen == 0 || xlen > SIZE_MAX - hlen + 1 ||
        ylen < xlen + hlen - 1)
    {
        return -1;
    }

    const size_t ny = xlen + hlen - 1;

    #pragma omp parallel for schedule(static)
    for (size_t r = 0; r < ny; r++)
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
    return EXIT_SUCCESS;
}

void print_double_arr(const double * arr, const size_t len)
{
    printf("{ ");
    for (size_t i = 0; i < len-1; i++)
    {
        printf("%.1lf, ", arr[i]);
    }
    printf("%.1lf }\n", arr[len-1]);
    return;
}

/*!
 * \brief Main function for program execution
 * 
 * \param argc Number of arguments passed
 * \param argv arguments to program: <arr_size> [S]
 * The first specifies how many elements are in the input array,
 * and the second specifies whether to suppress printing their contents.
 * \return int 
 * \version 0.1
 * \author William D. Goins V ("Quin") (116991920+wdg0008@users.noreply.github.com)
 * \date 2026-11-24
 * \copyright Copyright (c) 2026
 */
int main(int argc, char* argv[])
{
    size_t test_len = TEST_LEN;
    if (argc >= 2)
    {
        char *end = NULL;
        errno = 0;
        const uintmax_t parsed_len = strtoumax(argv[1], &end, 10);
        if (errno == ERANGE || end == argv[1] || *end != '\0' ||
            parsed_len == 0 || parsed_len > SIZE_MAX / sizeof(double)
            || argv[1][0] == '-')
        {
            fprintf(stderr, "Invalid input length: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        test_len = (size_t)parsed_len;
    }

    if (test_len > SIZE_MAX / 2 + 1)
    {
        fprintf(stderr, "Input length is too large.\n");
        return EXIT_FAILURE;
    }

    size_t conv_len = 2 * test_len - 1;
    if (conv_len > SIZE_MAX / sizeof(double))
    {
        fprintf(stderr, "Convolution output length is too large.\n");
        return EXIT_FAILURE;
    }

    bool do_print = true;
    if (argc >= 3)
    {
        if (argv[2][0] == 'S')
        {
            do_print = false;
        }
    }

    double* x = malloc(test_len * sizeof(double));
    double* y = malloc(conv_len * sizeof(double));
    if (x == NULL || y == NULL)
    {
        fprintf(stderr, "Unable to allocate arrays.\n");
        free(x);
        free(y);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < test_len; i++)
    {
        x[i] = 1;
    }

    if (do_print)
        print_double_arr(x, test_len);

    TIMER_START;
    if (convolve(x, test_len, x, test_len, y, conv_len) != 0)
    {
        fprintf(stderr, "Convolution failed.\n");
        free(x);
        free(y);
        return EXIT_FAILURE;
    }
    TIMER_STOP;

    if (do_print)
        print_double_arr(y, conv_len);

    printf("Time elapsed (seconds): %lf", TIMER_ELAPSED); 

    free(x);
    free(y);
    return EXIT_SUCCESS;
}
