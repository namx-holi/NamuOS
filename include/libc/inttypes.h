/**
 * @file inttypes.h
 * @defgroup libc_inttypes <inttypes.h>
 * @brief Format conversion of integer types
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @todo See Common mathematical functions
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/types/integer)
 * for **Fixed width integer types**
 * @see [C documentation](https://en.cppreference.com/w/c/numeric/math)
 * for **Common mathematical functions**
 * @see [C documentation](https://en.cppreference.com/w/c/string/byte)
 * for **Null-terminated byte strings**
 * 
 * @{
*/

#ifndef _LIBC_INTTYPES_H
#define _LIBC_INTTYPES_H 1

#include <stdint.h> // intmax_t


/******************************************************/
/* Fixed Width Integer Types                          */
/* Ref: https://en.cppreference.com/w/c/types/integer */
/******************************************************/
// TODO: Format macro constants. See https://en.cppreference.com/w/c/types/integer


/*****************************************************/
/* Common Mathematical Functions                     */
/* Ref: https://en.cppreference.com/w/c/numeric/math */
/*****************************************************/

/** @copydoc stdlib.h::abs */
extern intmax_t imaxabs(intmax_t n);

/** @brief Structure type, return of the @ref imaxdiv function */
typedef struct {
    intmax_t quot; ///< Quotient
    intmax_t rem;  ///< Remainder
} imaxdiv_t;

/** @copydoc stdlib.h::div */
extern imaxdiv_t imaxdiv(intmax_t x, intmax_t y);



/****************************************************/
/* Null-Terminated byte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/byte */
/************************************************** */

// TODO: Documentation, Converts a byte string to @ref int_max or @ref uintmax_t
extern intmax_t strtoimax(const char* restrict nptr, char** restrict endptr, int base);
extern uintmax_t strtoumax(const char* restrict nptr, char** restrict endptr, int base);

#endif

/** @} */
