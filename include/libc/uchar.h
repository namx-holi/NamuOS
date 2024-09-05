/**
 * @file uchar.h
 * @defgroup libc_uchar <uchar.h>
 * @brief UTF-16 and UTF-32 character utilities
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/string/multibyte)
 * for **Null-terminated multibyte strings**
 * 
 * @{
*/

#ifndef _LIBC_UCHAR_H
#define _LIBC_UCHAR_H 1

#include <stdint.h> // uint_least16_t, uint_least32_t
#include <stddef.h> // size_t
#include <wchar.h> // mbstate_t


/*********************************************************/
/* Null-Terminated Multibyte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/multibyte */
/*********************************************************/

/** @brief UTF-8 character type, an alias for `unsigned char`
 * 
 * `char8_t` is an unsigned integer type used for UTF-8 and is the same type as
 * `unsigned char`.
*/
typedef unsigned char char8_t;

/** @brief 16-bit wide character type
 * 
 * `char16_t` is an unsigned integer type used for 16-bit wide characters and is
 * the same type as `uint_least16_t`.
 * 
 * @note On any given platform, by the definition of `uint_least16_t`, the width
 * of type `char16_t` can be greater than 16 bits, but the actual values stored
 * in an object of type `char16_t` will always have a width of 16 bits.
*/
typedef uint_least16_t char16_t;

/** @brief 32-bit wide character type
 * 
 * `char32_t` is an unsigned integer type used for 32-bit wide characters and is
 * the same type as `uint_least32_t`.
 * 
 * @note On any given platform, by the definition of `uint_least32_t`, the width
 * of type `char32_t` can be greater than 32 bits, but the actual values stored
 * in an object of type `char32_t` will always have a width of 32 bits.
*/
typedef uint_least32_t char32_t;

// TODO: Documentation, Converts a narrow multibyte character to UTF-8 encoding
extern size_t mbrtoc8(char8_t* restrict pc8, const char* restrict s, size_t n, mbstate_t* restrict ps);

// TODO: Documentation, Converts UTF-8 string to narrow multibyte encoding
extern size_t c8rtomb(char* restrict s, char8_t c8, mbstate_t* restrict ps);

// TODO: Documentation, Generates the next 16-bit wide character from a narrow multibyte string
extern size_t mbrtoc16(char16_t* restrict pc16, const char* restrict s, size_t n, mbstate_t* restrict ps);

// TODO: Documentation, Converts a 16-bit wide character to narrow multibyte string
extern size_t c16rtomb(char* restrict s, char16_t c16, mbstate_t* restrict ps);

// TODO: Documentation, Generates the next 32-bit wide character from a narrow multibyte string
extern size_t mbrtoc32(char32_t* restrict pc32, const char* restrict s, size_t n, mbstate_t* restrict ps);

// TODO: Documentation, Converts a 32-bit wide character to narrow multibyte string
extern size_t c32rtomb(char* restrict s, char32_t c32, mbstate_t* restrict ps);

#endif

/** @} */
