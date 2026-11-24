#ifndef CONV_COMPAT_H
#define CONV_COMPAT_H

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


#endif