/**
 * @file stdlib.h
 * @defgroup libc_stdlib <stdlib.h>
 * @brief General utilities: memory management, program utilities, string conversions, random numbers, algorithms
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/memory)
 * for **Dynamic memory management**
 * @see [C documentation](https://en.cppreference.com/w/c/program)
 * for **Program support utilities**
 * @see [C documentation](https://en.cppreference.com/w/c/string/byte)
 * for **Null-terminated byte strings**
 * @see [C documentation](https://en.cppreference.com/w/c/string/multibyte)
 * for **Null-terminated multibyte strings**
 * @see [C documentation](https://en.cppreference.com/w/c/string/wide)
 * for **Null-terminated wide strings**
 * @see [C documentation](https://en.cppreference.com/w/c/numeric/random)
 * for **Pseudo-random number generation**
 * @see [C documentation](https://en.cppreference.com/w/c/algorithm)
 * for **Algorithms library**
 *  @see [C documentation](https://en.cppreference.com/w/c/numeric/math)
 * for **Common mathematical functions**
 * 
 * @{
*/

// #undef _STDLIB_H // DEBUG

#ifndef _LIBC_STDLIB_H
#define _LIBC_STDLIB_H 1

// Ref: https://en.cppreference.com/w/c/memory
// TODO: malloc
// TODO: calloc
// TODO: realloc
// TODO: free
// TODO: free_sized
// TODO: free_aligned_sized
// TODO: aligned_alloc

/** @brief Causes abnormal program termination (without cleaning up)
 * 
 * Causes abnormal program termination unless @ref SIGABRT is being caught by a
 * signal handler passed to signal and the handler does not return.
 * 
 * Functions passed to @ref atexit are not called. Whether open resources such
 * as files are closed is implementation defined. An implementation defined
 * status is returned to the host environment that indicates unsuccessful
 * execution.
 * 
 * @note POSIX
 * [specifies](https://pubs.opengroup.org/onlinepubs/9699919799/functions/abort.html)
 * that the `abort()` function overrides blocking or ignoring the @ref SIGABRT
 * signal.
 * @note Some compiler intrinsics, e.g.
 * [`__builtin_trap`](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
 * (gcc, clang, and icc) or
 * [`__fastfail`](https://learn.microsoft.com/en-us/cpp/intrinsics/fastfail)/[`__debugbreak`](https://learn.microsoft.com/en-us/cpp/intrinsics/debugbreak)
 * (msvc), can be used to terminate the program as fast as possible.
 * 
 * @see @ref exit @copybrief exit
 * @see @ref atexit @copybrief atexit
 * @see @ref quick_exit @copybrief quick_exit
*/
extern void abort(void) __attribute__((__noreturn__));

// Ref: https://en.cppreference.com/w/c/program
// TODO: exit
// TODO: quick_exit
// TODO: _Exit
// TODO: atexit
// TODO: at_quick_exit
// TODO: EXIT_SUCCESS
// TODO: EXIT_FAILURE
// TODO: system
// TODO: getenv
// TODO: getenv_s

// Ref: https://en.cppreference.com/w/c/string/byte
// TODO: atof
// TODO: atoi
// TODO: atol
// TODO: atoll
// TODO: strtol
// TODO: strtoll
// TODO: strtoul
// TODO: strtoull
// TODO: strtof
// TODO: strtod
// TODO: strtold
// TODO: strfromf
// TODO: strfromd
// TODO: strfroml
// TODO: strtoimax
// TODO: strtoumax

// Ref: https://en.cppreference.com/w/cpp/string/multibyte
// TODO: mblen
// TODO: mbtowc
// TODO: wctomb, wctomb_s
// TODO: mbstowcs, mbstowcs_s
// TODO: wcstombs, wcstombs_s
// TODO: MB_CUR_MAX

// Ref: https://en.cppreference.com/w/c/string/wide
// TODO: wchar_t (in stdlib.h, wchar.h)

// Ref: https://en.cppreference.com/w/c/numeric/random
// TODO: rand
// TODO: srand
// TODO: RAND_MAX

// Ref: https://en.cppreference.com/w/c/algorithm
// TODO: qsort, qsort_s
// TODO: bsearch, bsearch_s

// Ref: https://en.cppreference.com/w/c/numeric/math
// TODO: abs
// TODO: labs
// TODO: llabs
// TODO: div
// TODO: ldiv
// TODO: lldiv
// TODO: div_t
// TODO: ldiv_t
// TODO: lldiv_t

#endif

/** @} */



// #undef _STDLIB_H // DEBUG
// #ifndef _STDLIB_H
// #define _STDLIB_H 1

// #include <sys/cdefs.h>

// #ifdef __cplusplus
// extern "C" {

// /* Macros */
// #define EXIT_FAILURE 1 ///< Failure termination code
// #define EXIT_SUCCESS 0 ///< Success termination code
// // TODO: Implement MB_CUR_MAX macro // Maximum size of multibyte characters
// #define RAND_MAX 2147483647 ///< Maximum value returned by rand, same as INT_MAX


// // Needed for:
// // - NULL, null pointer
// // - size_t, unsigned integral type
// // - wchar_t
// #include <stddef.h>

// /* Types */
// /// Structure returned by @ref div
// typedef struct {
// 	int quot; ///< Quotient
// 	int rem;  ///< Remainder
// } div_t;

// /// Structure returned by @ref ldiv
// typedef struct {
// 	long int quot; ///< Quotient
// 	long int rem;  ///< Remainder
// } ldiv_t;

// /// Structure returned by @ref lldiv
// typedef struct {
// 	long long int quot; ///< Quotient
// 	long long int rem;  ///< Remainder
// } lldiv_t;


// /* String conversion */
// double atof(const char* str); // Convert string to double
// int atoi(const char* str); // Convert string to integer
// long int atol(const char* str); // Convert string to long integer
// long long int atoll(const char* str); // Convert string to long long integer
// double strtod(const char* __restrict str, char** __restrict endptr); // Convert string to double
// float strtof(const char* __restrict str, char** __restrict endptr); // Convert string to float
// long int strtol(const char* __restrict str, char** __restrict endptr, int base); // Convert string to long integer
// long double strtold(const char* __restrict str, char** __restrict endptr); // Convert string to long double
// long long int strtoll(const char* __restrict str, char** __restrict endptr, int base); // Convert string to long long integer
// unsigned long int strtoul(const char* __restrict str, char** __restrict endptr, int base); // Convert string to unsigned long integer
// unsigned long long int strtoull(const char* __restrict str, char** __restrict endptr, int base); // Convert string to unsigned long long integer

// /* Pseudo-random sequence generation */
// int  rand(void); // Generate random number
// void srand(unsigned int seed); // Initialise random number generator

// /* Dynamic memory management */
// void* calloc(size_t num, size_t size); // Allocate and zero-initialise array
// void  free(void* ptr); // Deallocate memory block
// void* malloc(size_t size); // Allocate memory block
// void* realloc(void* ptr, size_t size); // Reallocate memory block

// /* Environment */
// void  abort(void) __attribute__((__noreturn__)); // Abort current process // TODO: noexcept
// int   atexit(void (*func)(void)); // Set function to be executed on exit // TODO: noexcept, handle C++?
// int   at_quick_exit(void (*func)(void)); // Set function to be executed on quick exit // TODO: noexcept, handle C++?
// void  exit(int status) __attribute__((__noreturn__)); // Terminate calling process
// char* getenv(const char* name); // Get environment string
// void  quick_exit(int status) __attribute__((__noreturn__)); // Terminate calling process quick // TODO: noexcept
// int   system(const char* command); // Execute system command
// void  _Exit(int status) __attribute__((__noreturn__)); // Terminate calling process // TODO: noexcept

// /* Searching and sorting */
// typedef int (*__compar_fn_t)(const void*, const void*); ///< Shorthand for type of comparison functions
// void* bsearch(const void* key, const void* base, size_t num, size_t size, __compar_fn_t compar); // Binary search in array
// void  qsort(void* base, size_t num, size_t size, __compar_fn_t compar); // Sort elements of array

// /* Integer arithmetics */
// int           abs(int n) __attribute__((__const__)); // Absolute value
// div_t         div(int numer, int denom) __attribute__((__const__)); // Integral division
// long int      labs(long int n) __attribute__((__const__)); // Absolute value
// ldiv_t        ldiv(long int numer, long int denom) __attribute__((__const__)); // Integral division
// long long int llabs(long long int n) __attribute__((__const__)); // Absolute value
// lldiv_t       lldiv(long long int numer, long long int denom) __attribute__((__const__)); // Integral division

// /* Multibyte characters */
// int mblen(const char* pmb, size_t max); // Get length of multibyte character
// int mbtowc(wchar_t* __restrict pwc, const char* __restrict pmb, size_t max); // Convert multibyte sequence to wide character
// int wctomb(char* pmb, wchar_t wc); // Convert wide character to multibyte sequence

// /* Multibyte strings */
// size_t mbstowcs(wchar_t* __restrict dest, const char* __restrict src, size_t max); // Convert multibyte string to wide-character string
// size_t wcstombs(char* __restrict dest, const wchar_t* __restrict src, size_t max); // Convert wide-character string to multibyte string

// #ifdef __cplusplus
// }
// #endif

// #endif
