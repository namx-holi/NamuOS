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
 * 
 * @{
*/

#ifndef _LIBC_INTTYPES_H
#define _LIBC_INTTYPES_H 1

// TODO: Include <stdint.h>
// TODO: Format macro constants. See https://en.cppreference.com/w/c/types/integer

/** @copydoc stdlib.h::abs */
intmax_t imaxabs(intmax_t n);

// Ref: https://en.cppreference.com/w/c/numeric/math
// TODO: imaxdiv
// TODO: imaxdiv_t

#endif

/** @} */
