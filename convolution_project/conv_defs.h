#ifndef CONV_DEFS_H
#define CONV_DEFS_H

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
* \def PROTOSTR(x)
* \brief Stringitizes a value
*/
#define PROTOSTR(x) #x

/*!
* \def STR(x)
* \brief Expands a macro before stringitizing it
*/
#define STR(x) PROTOSTR(x)

/*!
* \def COMPILER_STYLE_GCC
* \brief Denotes a GCC-style compiler
*/

/*!
* \def COMPILER_STYLE_MSVC
* \brief Denotes a MSVC-style compiler
*/

#if defined(__GNUC__) || defined(__clang__) || defined(__ti__)
    #define COMPILER_STYLE_GCC 1
#elif defined(_MSC_VER)
    #define COMPILER_STYLE_MSVC 1
#endif

/*!
* \def RESTRICT
* \brief Expands into C99 keyword or compiler-specific equivalent
*/

#ifdef __cplusplus
    #if COMPILER_STYLE_GCC
        #define RESTRICT __restrict__
    #elif COMPILER_STYLE_MSVC
        #define RESTRICT __restrict
    #endif
#else
    #define RESTRICT restrict /* standard keyword */
#endif

/*!
* \def NO_ALIAS
* \brief Return type modifier indicating the pointer is not aliased in any sense
*/

#ifdef COMPILER_STYLE_GCC
    #define NO_ALIAS __attribute__((malloc))
#elif defined COMPILER_STYLE_MSVC
    #define NO_ALIAS __declspec(restrict)
#endif

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