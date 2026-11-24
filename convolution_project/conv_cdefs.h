#ifndef CONV_CDEFS_H
#define CONV_CDEFS_H

#include "conv_compat.h"

/*!
* \def TEST_LEN
* \brief Length of the convolution input array
*/
#define TEST_LEN 2048

/*!
* \def CONV_LEN
* \brief Length of the convolution output array
*/
#define CONV_LEN (TEST_LEN*2 - 1)

/*!
 * \brief Discrete convolution function
 * \remarks y must not overlap at all with x or h, although those two may.
 * 
 * \param x Input array to convolve
 * \param xlen Number of elements in input array
 * \param h Filter array to convolve
 * \param hlen Number of elements in filter array
 * \param y Output array for convolution
 * \param ylen Nuber of elements in output array
 * \return int Success or failure
 * \version 0.1
 * \author William D. Goins V ("Quin") (116991920+wdg0008@users.noreply.github.com)
 * \date 2026-11-24
 * \copyright Copyright (c) 2026
 */
int convolve(
    const double * RESTRICT x, const size_t xlen,
    const double * RESTRICT h, const size_t hlen,
    double * RESTRICT y, const size_t ylen);


/*!
 * \brief Prints the contents of an array
 * 
 * \param arr Array to print
 * \param len Number of elements in arr
 * \version 0.1
 * \author William D. Goins V ("Quin") (116991920+wdg0008@users.noreply.github.com)
 * \date 2026-11-24
 * \copyright Copyright (c) 2026
 */
void print_double_arr(const double * arr, const size_t len);

#endif