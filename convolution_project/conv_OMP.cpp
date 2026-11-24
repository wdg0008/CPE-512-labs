/*!
 * \file conv_proj.cpp
 * \author William D. Goins V ("Quin") (116991920+wdg0008@users.noreply.github.com)
 * \brief Implements parallelized convolution algorithm with OpenMP
 * \version 0.1
 * \date 2026-11-22
 * 
 * \copyright Copyright (c) 2026
 * 
 */

#include <cstdio>
#include <cstdlib>
#include <charconv>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include "conv_compat.h"
#include "../ctimer_utils.h"

#define TEST_LEN 2048

std::optional<size_t> parse_length(const std::string_view text)
{
    if (text.empty())
    {
        return std::nullopt;
    }

    size_t value = 0;
    const auto [end, error] = std::from_chars(
        text.data(), text.data() + text.size(), value, 10);
    if (error != std::errc{} || end != text.data() + text.size() ||
        value == 0)
    {
        return std::nullopt;
    }

    return value;
}

template<typename T>
concept primitive = std::is_arithmetic_v<T>;

template<primitive T>
int convolve(
    const T * RESTRICT x, const size_t xlen,
    const T * RESTRICT h, const size_t hlen,
    T * RESTRICT y, const size_t ylen)
{
    // validate sizes of arrays
    if (xlen == 0 || hlen == 0 || xlen > SIZE_MAX - hlen + 1 ||
        ylen < xlen + hlen - 1)
    {
        return -1;
    }

    const size_t ny = xlen + hlen - 1;

    int r;
    #pragma omp parallel for schedule(guided)
    for (r = 0; r < ny; r++)
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

template <typename T>
void print_arr(const T* arr, const size_t len)
{
    std::cout << "{ ";
    for (size_t i = 0; i < len-1; i++)
    {
        std::cout << arr[i] << ", ";
    }
    std::cout << arr[len-1] << " }\n";
}

int main(int argc, char* argv[])
{
    size_t test_len = TEST_LEN;
    if (argc >= 2)
    {
        const auto parsed_len = parse_length(argv[1]);
        if (!parsed_len || *parsed_len > SIZE_MAX / sizeof(double))
        {
            std::cerr << "Invalid input length: " << argv[1] << '\n';
            return EXIT_FAILURE;
        }
        test_len = *parsed_len;
    }

    if (test_len > SIZE_MAX / 2 + 1)
    {
        std::cerr << "Input length is too large: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    size_t conv_len = 2 * test_len - 1;
    if (conv_len > SIZE_MAX / sizeof(double))
    {
        std::cerr << "Convolution output length is too large.\n";
        return EXIT_FAILURE;
    }

    bool do_print = true;
    bool is_outptut_csv = false;
    if (argc >= 3)
    {
        if (argv[2][0] == 'S')
        {
            do_print = false;
        }
        else if (argv[2][0] == 'C')
        {
            do_print = false;
            is_outptut_csv = true;
        }
    }

    std::unique_ptr<double[]> x;
    std::unique_ptr<double[]> y;

    try
    {
        x = std::make_unique<double[]>(test_len);
        y = std::make_unique<double[]>(conv_len);
    }
    catch (const std::bad_alloc& e)
    {
        std::cout << "Memory allocation failed: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < test_len; i++)
    {
        x[i] = 1;
    }

    if (do_print)
        print_arr(x.get(), test_len);

    TIMER_START;
    convolve(x.get(), test_len, x.get(), test_len, y.get(), conv_len);
    TIMER_STOP;

    if (do_print)
        print_arr(y.get(), conv_len);

    if (!is_outptut_csv)
        std::cout << "Time elapsed (seconds): ";
    std::cout << TIMER_ELAPSED << '\n';

    return EXIT_SUCCESS;
}