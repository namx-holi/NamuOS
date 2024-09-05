/**
 * @file wchar.h
 * @defgroup libc_wchar <wchar.h>
 * @brief TODO
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/io)
 * for **File input/output**
 * 
 * @todo chrono ref
 * @todo multibyte ref
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/string/wide)
 * for **Null-terminated wide strings**
 * 
 * @{
*/

#ifndef _LIBC_WCHAR_H
#define _LIBC_WCHAR_H 1

#include <stdio.h> // FILE, va_list via <stdarg.h>
#include <time.h> // struct tm


/*******************************************/
/* File Input/Output Functions             */
/* Ref: https://en.cppreference.com/w/c/io */
/*******************************************/

// TODO: Documentation, Switches a file stream between wide character I/O and narrow character I/O
extern int fwide(FILE* stream, int mode);

// TODO: Documentation, Gets a wide character from a file stream
extern wint_t fgetwc(FILE* stream);
extern wint_t getwc(FILE* stream);

// TODO: Documentation, Gets a wide string from a file stream
extern wchar_t* fgetws(wchar_t* restrict str, int count, FILE* restrict stream);

// TODO: Documentation, Writes a wide character to a file stream
extern wint_t fputwc(wchar_t ch, FILE* stream);
extern wint_t putwc(wchar_t ch, FILE* stream);

// TODO: Documentation, Writes a wide string to a file stream
extern int fputws(const wchar_t* restrict str, FILE* restrict stream);

// TODO: Documentation, Reads a wide character from @ref stdin
extern wint_t getwchar(void);

// TODO: Documentation, Writes a wide character to @ref stdout
extern wint_t putwchar(wchar_t ch);

// TODO: Documentation, Puts a wide character back into a file stream
extern wint_t ungetwc(wint_t ch, FILE* stream);

// TODO: Documentation, Reads formatted wide character input from @ref stdin, a file stream or a buffer
extern int wscanf(const wchar_t* restrict format, ...);
extern int fwscanf(FILE* restrict stream, const wchar_t* restrict format, ...);
extern int swscanf(const wchar_t* restrict buffer, const wchar_t* restrict format, ...);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
extern int wscanf_s(const wchar_t* restrict format, ...);
extern int fwscanf_s(FILE* restrict stream, const wchar_t* restrict format, ...);
extern int swscanf_s(const wchar_t* restrict s, const wchar_t* restrict format, ...);
#endif

// TODO: Documentation, Reads formatted wide character input from @ref stdin, a file stream or a buffer using variable argument list
extern int vwscanf(const wchar_t* restrict format, va_list vlist);
extern int vfwscanf(FILE* restrict stream, const wchar_t* restrict format, va_list vlist);
extern int vswscanf(const wchar_t* restrict buffer, const wchar_t* restrict format, va_list vlist);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
extern int vwscanf_s(const wchar_t* restrict format, va_list vlist);
extern int vfwscanf_s(FILE* restrict stream, const wchar_t* restrict format, va_list vlist);
extern int vswscanf_s(const wchar_t* restrict buffer, const wchar_t* restrict format, va_list vlist);
#endif

// TODO: Documentation, Prints formatted wide character output to @ref stdout, a file stream or a buffer
extern int wprintf(const wchar_t* restrict format, ...);
extern int fwprintf(FILE* restrict stream, const wchar_t* restrict format, ...);
extern int swprintf(wchar_t* restrict buffer, size_t bufsz, const wchar_t* restrict format, ...);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
extern int wprintf_s(const wchar_t* restrict format, ...);
extern int fwprintf_s(FILE* restrict stream, const wchar_t* restrict format, ...);
extern int swprintf_s(wchar_t* restrict buffer, rsize_t bufsz, const wchar_t* restrict format, ...);
extern int snwprintf_s(wchar_t* restrict s, rsize_t n, const wchar_t* restrict format, ...);
#endif

// TODO: Documentation, Prints formatted wide character output to @ref stdout, a file stream or a buffer using variable argument list
extern int vwprintf(const wchar_t* restrict format, va_list vlist);
extern int vfwprintf(FILE* restrict stream, const wchar_t* restrict format, va_list vlist);
extern int vswprintf(wchar_t* restrict buffer, size_t bufsz, const wchar_t* restrict format, va_list vlist);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
extern int vwprintf_s(const wchar_t* restrict format, va_list vlist);
extern int vfwprintf_s(FILE* restrict stream, const wchar_t* restrict format, va_list vlist);
extern int vswprintf_s(wchar_t* restrict buffer, rsize_t bufsz, const wchar_t* restrict format, va_list vlist);
extern int vsnwprintf_s(wchar_t* restrict buffer, rsize_t bufsz, const wchar_t* restrict format, va_list vlist);
#endif


/***********************************************/
/* Date And Times Utility Functions            */
/* Ref: https://en.cppreference.com/w/c/chrono */
/***********************************************/

// TODO: Documentation, Converts a @ref tm object to custom wide string textual representation
extern size_t wcsftime(wchar_t* str, size_t count, const wchar_t* format, const struct tm* time);


/*********************************************************/
/* Null-Terminated Multibyte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/multibyte */
/*********************************************************/

/** @brief Conversion state information necessary to iterate multibyte character strings
 * 
 * The type `mbstate_t` is a trivial non-array type that can represent any of
 * the conversion states that can occur in an implementation-defined set of
 * supported multibyte character encoding rules. Zero-initialised value of
 * `mbstate_t` represents the initial conversion state, although other values of
 * `mbstate_t` may exist that also represent the initial conversion state.
 * 
 * Possible implementation of `mbstate_t` is a struct type holding an array
 * representing the incomplete multibyte character, an integer counter
 * indicating the number of bytes in the array that have been processed, and a
 * representation of the current shift state.
 * 
 * The following functions should not be called from multiple threads without
 * synchronisation with the `mbstate_t*` argument of a null pointer due to
 * possible data races: @ref mbrlen, @ref mbrtowc, @ref mbsrtowcs, @ref mbtowc,
 * @ref wcrtomb, @ref wcsrtombs, @ref wctomb.
 * 
 * @see @ref mbsinit @copybrief mbsinit
*/
typedef struct {} mbstate_t;
// TODO: Implement mbstate_t

// TODO: Documentation, Checks if the @ref mbstate_t object represents initial shift state
extern int mbsinit(const mbstate_t* ps);

// TODO: Documentation, Widens a single-byte narrow character to wide character, if possible
extern wint_t btowc(int c);

// TODO: Documentation, Narrows a wide character to a single-byte narrow character, if possible
extern int wctob(wint_t c);

// TODO: Documentation, Returns the number of bytes in the next multibyte character, given state
extern size_t mbrlen(const char* restrict s, size_t n, mbstate_t* restrict ps);

// TODO: Documentation, Converts the next multibyte character to wide character, given state
extern size_t mbrtowc(wchar_t* restrict pwc, const char* restrict s, size_t n, mbstate_t* restrict ps);

// TODO: Documentation, Converts a wide character to its multibyte representation, given state
extern size_t wcrtomb(char* restrict s, wchar_t wc, mbstate_t* restrict ps);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcrtomb_s
extern errno_t wcrtomb_s(size_t* restrict retval, char* restrict s, rsize_t ssz, wchar_t wc, mbstate_t* restrict ps);
#endif

// TODO: Documentation, Converts a narrow multibyte character string to wide string, given state 
extern size_t mbsrtowcs(wchar_t* restrict dst, const char** restrict src, size_t len, mbstate_t* restrict ps);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for mbsrtowcs_s
extern errno_t mbsrtowcs_s(size_t* restrict retval, wchar_t* restrict dst, rsize_t dstsz, const char** restrict src, rsize_t len, mbstate_t* restrict ps);
#endif

// TODO: Documentation, Converts a wide string to narrow multibyte character string, given state
extern size_t wcsrtombs(char* restrict dst, const wchar_t** restrict src, size_t len, mbstate_t* restrict ps);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcsrtombs_s
extern errno_t wcsrtombs_s(size_t* restrict retval, char* restrict dst, rsize_t dstsz, const wchar_t** restrict src, rsize_t len, mbstate_t* restrict ps);
#endif


/****************************************************/
/* Null-Terminated Wide String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/wide */
/****************************************************/

// NOTE: Can be defined in <wchar.h> or <wctype.h>
#ifndef WEOF
/** @brief A non-character value of type @ref wint_t used to indicate errors */
#define WEOF (0xffffffffu)
#endif

// NOTE: May be defined in <stdint.h>
#ifndef WCHAR_MIN
/** @brief The smallest valid value of @ref wchar_t */
#define WCHAR_MIN (-__WCHAR_MAX__ - 1)

/** @brief The largest valid value of @ref wchar_t */
#define WCHAR_MAX __WCHAR_MAX__
#endif

// NOTE: Can be defined in <stddef.h>, <stdlib.h>, or <wchar.h>
#ifndef _WCHAR_T
#define _WCHAR_T 1
/** @brief Integer type that can hold any valid wide character */
typedef int wchar_t;
#endif

// NOTE: Can be defined in <wchar.h> or <wctype.h>
#ifndef _WINT_T
#define _WINT_T 1
/** @brief Integer type that can hold any valid wide character and at least one more value */
typedef unsigned int wint_t;
#endif

// TODO: Documentation, Converts a wide string to an integer value
extern long wcstol(const wchar_t* restrict str, wchar_t** restrict str_end, int base);
extern long long wcstoll(const wchar_t* restrict str, wchar_t** restrict str_end, int base);

// TODO: Documentation, Converts a wide string to an unsigned integer value
extern unsigned long wcstoul(const wchar_t* restrict str, wchar_t** restrict str_end, int base);
extern unsigned long long wcstoull(const wchar_t* restrict str, wchar_t** restrict str_end, int base);

// TODO: Documentation, Converts a wide string to a floating-point value
extern float wcstof(const wchar_t* restrict str, wchar_t** restrict str_end);
extern double wcstod(const wchar_t* restrict str, wchar_t** restrict str_end);
extern long double wcstold(const wchar_t* restrict str, wchar_t** restrict str_end);

// TODO: Documentation, Copies one wide string to another
extern wchar_t* wcscpy(wchar_t* dest, const wchar_t* src);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcscpy_s
extern errno_t wcscpy_s(wchar_t* restrict dest, rsize_t destsz, const wchar_t* restrict src);
#endif

// TODO: Documentation, Copies a certain amount of wide characters from one string to another
extern wchar_t* wcsncpy(wchar_t* restrict dest, const wchar_t* restrict src, size_t count);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcsncpy_s
extern errno_t wcsncpy_s(wchar_t* restrict dest, rsize_t destsz, const wchar_t* restrict src, rsize_t count);
#endif

// TODO: Documentation, Appends a copy of one wide string to another
extern wchar_t* wcscat(wchar_t* restrict dest, const wchar_t* restrict src);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcscat_s
extern errno_t wcscat_s(wchar_t* restrict dest, rsize_t destsz, const wchar_t* restrict src);
#endif

// TODO: Documentation, Appends a certain amount of wide characters from one wide string to another
extern wchar_t* wcsncat(wchar_t* restrict dest, const wchar_t* restrict src, size_t count);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcsncat_s
extern errno_t wcsncat_s(wchar_t* restrict dest, rsize_t destsz, const wchar_t* restrict src, rsize_t count);
#endif

// TODO: Documentation, Transform a wide string so that @ref wcscmp would produce the same result as @ref wcsoll
extern size_t wcsxfrm(wchar_t* restrict dest, const wchar_t* restrict src, size_t count);

// TODO: Documentation, Returns the length of a wide string
extern size_t wcslen(const wchar_t* str);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcsnlen_s
extern size_t wcsnlen_s(const wchar_t* str, size_t strsz);
#endif

// TODO: Documentation, Compares two wide strings
extern int wcscmp(const wchar_t* lhs, const wchar_t* rhs);

// TODO: Documentation, Compares a certain amount of characters from two wide strings
extern int wcsncmp(const wchar_t* lhs, const wchar_t* rhs, size_t count);

// TODO: Documentation, Compares two wide strings in accordance to the current locale
extern int wcscoll(const wchar_t* lhs, const wchar_t* rhs);

// TODO: Documentation, Finds the first occurrence of a wide character in a wide string
extern wchar_t* wcschr(const wchar_t* str, wchar_t ch);
// TODO: QWchar_t version

// TODO: Documentation, Finds the last occurrence of a wide character in a wide string
extern wchar_t* wcsrchr(const wchar_t* str, wchar_t ch);
// TODO: QWchar_t version

// TODO: Documentation, Returns the length of the maximum initial segment that consists of only the wide characters found in another wide string
extern size_t wcsspn(const wchar_t* dest, const wchar_t* src);

// TODO: Documentation, Returns the length of the maximum initial segment that consists of only the wide chars *not* found in another wide string
extern size_t wcscspn(const wchar_t* dest, const wchar_t* src);

// TODO: Documentation, Finds the first location of any wide character in one wide string, in another wide string
extern wchar_t* wcspbrk(const wchar_t* dest, const wchar_t* str);
// TODO: QWchar_t version

// TODO: Documentation, Finds the first occurrence of a wide string within another wide string
extern wchar_t* wcsstr(const wchar_t* dest, const wchar_t* src);
// TODO: QWchar_t version

// TODO: Documentation, Finds the next token in a wide string
extern wchar_t* wcstok(wchar_t* restrict str, const wchar_t* restrict delim, wchar_t** restrict ptr);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wcstok_s
extern wchar_t* wcstok_s(wchar_t* restrict str, rsize_t* restrict strmax, const wchar_t* restrict delim, wchar_t** restrict ptr);
#endif

// TODO: Documentation, Copies a certain amount of wide characters between two non-overlapping arrays
extern wchar_t* wmemcpy(wchar_t* restrict dest, const wchar_t* restrict src, size_t count);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wmemcpy_s
extern errno_t wmemcpy_s(wchar_t* restrict dest, rsize_t destsz, const wchar_t* restrict src, rsize_t count);
#endif

// TODO: Documentation, Copies a certain amount of wide characters between two, possibly overlapping, arrays
extern wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, size_t count);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for wmwmmove_s
extern errno_t wmemmove_s(wchar_t* dest, rsize_t destsz, const wchar_t* src, rsize_t count);
#endif

// TODO: Documentation, Compares a certain amount of wide characters from two arrays
extern int wmemcmp(const wchar_t* lhs, const wchar_t* rhs, size_t count);

// TODO: Documentation, Finds the first occurrence of a wide character in a wide character array
extern wchar_t* wmemchr(const wchar_t* ptr, wchar_t ch, size_t count);
// TODO: QWchar_t version

// TODO: Documentation, Copies the given wide character to every position in a wide character array
extern wchar_t* wmemset(wchar_t* dest, wchar_t ch, size_t count);

#endif

/** @} */
