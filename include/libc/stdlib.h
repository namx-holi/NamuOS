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

#include <stddef.h> // size_t


/***********************************************/
/* Dynamic Memory Management Functions         */
/* Ref: https://en.cppreference.com/w/c/memory */
/***********************************************/

/** @brief Allocates memory
 * 
 * Allocates `size` bytes of uninitialised storage.
 * 
 * If allocation succeeds, returns a pointer that is suitably aligned for any
 * object type with
 * [fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment).
 * 
 * If `size` is zero, the behaviour of `malloc` is implementation defined. For
 * example, a null pointer may be returned. Alternatively, a non-null pointer
 * may be returned; but such a pointer should not be dereferenced, and should be
 * passed to @ref free to avoid memory leaks.
 * 
 * `malloc` is thread-safe: It behaves as though only accessing the memory
 * locations visible through its argument, and not any static storage.
 * 
 * A previous call to @ref free, @ref free_sized, and @ref free_aligned_sized or
 * @ref realloc that deallocates a region of memory *synchronises-with* a call
 * to `malloc` that allocates the same or a part of the same region of memory.
 * This synchronisation occurs after any access to the memory by the
 * deallocating function and before any access to the memory by `malloc`. There
 * is a single total order of all allocation and deallocation functions
 * operating on each particular region of memory.
 * 
 * @param size Number of bytes to allocate
 * 
 * @returns On success, returns the pointer to the beginning of newly allocated
 * memory. To avoid a memory leak, the returned pointer must be deallocated with
 * @ref free or @ref realloc.
 * @returns On failure, returns a null pointer.
 * 
 * @see @ref free @copybrief free
*/
extern void* malloc(size_t size);

/** @brief Allocates and zeroes memory
 * 
 * Allocates memory for an array of `num` objects of `size` and initialises all
 * bytes in the allocated storage to zer.
 * 
 * If allocation succeeds, returns a pointer to the lowest (first) byte in the
 * allocated memory block that is suitably aligned for any object type with
 * [fundamental alignment](https://en.cppreference.com/w/c/language/object#Alignment).
 * 
 * If `size` is zero, the behaviour is implementation defined (null pointer may
 * be returned, or some non-null pointer may be returned that may not be used to
 * access storage).
 * 
 * `calloc` is thread-safe: It behaves as though only accessing the memory
 * locations visible through its argument, and not any static storage.
 * 
 * A previous call to @ref free, @ref free_sized, and @ref free_aligned_sized or
 * @ref realloc that deallocates a region of memory *synchronises-with* a call
 * to `calloc` that allocates the samemor a part of the same region of memory.
 * This synchronisation occurs after any access to the memory by the
 * deallocating function and before any access to the memory by `calloc`. There
 * is a single total order of all allocation and deallocation functions
 * operating on each particular region of memory.
 * 
 * @param num Number of objects
 * @param size Size of each object
 * 
 * @returns On success, returns the pointer to the beginning of newly allocated
 * memory. To avoid a memory leak, the returned pointer must be deallocated with
 * @ref free or @ref realloc.
 * @returns On failure, returns a null pointer.
 * 
 * @note Due to the alignment requirements, the number of allocated bytes is not
 * necessarily equal to `num * size`.
 * @note Initialisation to all bits zero does not guarantee that a
 * floating-point or a pointer would be initialised to `0.0` and the null
 * pointer respectively (although that is true on all common platforms).
 * @note Originally (in C89), support for the zero size was added to
 * accomodate code such as
 * ```c
 * OBJ* p = calloc(0, sizeof(OBJ)); // "zero-length" placeholder
 * ...
 * while (1) {
 *   p = realloc(p, c * sizeof(OBJ)); // reallocations until size settles
 *   ... // code that may change c or break out of loop
 * }
 * ```
*/
extern void* calloc(size_t num, size_t size);

/** @brief Expands previously allocated memory block
 * 
 * @todo Detailed description
*/
extern void* realloc(void* ptr, size_t new_size);

/** @brief Deallocates previously allocated memory block
 * 
 * @todo Detailed description
*/
extern void free(void* ptr);

/** @brief Deallocates previously allocated sized memory
 * 
 * @todo Detailed description
*/
extern void free_sized(void* ptr, size_t size);

/** @brief Deallocates previously allocated sized and aligned memory
 * 
 * @todo Detailed description
*/
extern void free_aligned_sized(void* ptr, size_t alignment, size_t size);

/** @brief Allocates aligned memory
 * 
 * @todo Detailed description
*/
extern void* aligned_alloc(size_t alignment, size_t size);


/************************************************/
/* Program Support Functions                    */
/* Ref: https://en.cppreference.com/w/c/program */
/************************************************/

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

/** @brief Causes normal program termination with cleaning up
 * 
 * @todo Detailed description
*/
extern void exit(int exit_code) __attribute__((__noreturn__));

/** @brief Causes normal program termination without completely cleaning up
 * 
 * @todo Detailed description
*/
extern void quick_exit(int exit_code) __attribute__((__noreturn__));

/** @brief Causes normal program termination without cleaning up
 * 
 * @todo Detailed description
*/
extern void _Exit(int exit_code) __attribute__((__noreturn__));

/** @brief Registers a function to be called on @ref exit invocation
 * 
 * @todo Detailed description
*/
extern int atexit(void (*func)(void));

/** @brief Registers a function to be called on @ref quick_exit invocation
 * 
 * @todo Detailed description
*/
extern int at_quick_exit(void (*func)(void));

/** @brief Indicates successful execution of a program
 * 
 * The `EXIT_SUCCESS` and @ref EXIT_FAILURE macros expand into integral
 * expressions that can be used as arguments to the @ref exit function (and,
 * therefore, as the values to return from the main function), and indicate
 * program execution status (`EXIT_SUCCESS` for successful execution, and
 * @ref EXIT_FAILURE for unsuccessful execution).
 * 
 * @note Both `EXIT_SUCCESS` and the value zero indicate successful program
 * execution status (see @ref exit), although it is not required that
 * `EXIT_STATUS` equals zero.
*/
#define EXIT_SUCCESS 0

/** @brief Indicates unsuccessful execution of a program
 * 
 * The @ref EXIT_SUCCESS and `EXIT_FAILURE` macros expand into integral
 * expressions that can be used as arguments to the @ref exit function (and,
 * therefore, as the values to return from the main function), and indicate
 * program execution status (@ref EXIT_SUCCESS for successful execution, and
 * `EXIT_FAILURE` for unsuccessful execution).
*/
#define EXIT_FAILURE 1

/** @brief Calls the host environment's command processor
 * 
 * Calls the host environment's command processor with the parameter `command`.
 * Returns an implementation-defined value (usually the value that the invoked
 * program returns).
 * 
 * If `command` is a null pointer, checks if the host environment has a command
 * processor and returns a nonzero value if and only if the command processor
 * exists.
 * 
 * @param command Character string identifying the command to be run in the
 * command processor. If a null pointer is given, command processor is checked
 * for existence.
 * 
 * @returns Implementation-defined value. If `command` is a null pointer,
 * returns a nonzero value if and only if the command processor exists.
 * 
 * @note On POSIX systems, the return value can be decomposed using
 * [`WEXITSTATUS` and `WSTOPSIG`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/wait.html).
 * @note The related POSIX function
 * [`popen`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/popen.html)
 * makes the output generated by `command` available to the caller.
*/
extern int system(const char* command);

/** @brief Access to the list of environment variables
 * 
 * @todo Detailed documentation
*/
extern char* getenv(const char* name);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
/** @copybrief getenv
 * 
 * @todo Detailed doc
*/
extern errno_t getenv_s(size_t* restrict len, char* restrict value, rsize_t valuesz, const char* restrict name);
#endif


/****************************************************/
/* Null-Terminated Byte Functions                   */
/* Ref: https://en.cppreference.com/w/c/string/byte */
/****************************************************/
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


/*********************************************************/
/* Null-Terminated Multibyte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/multibyte */
/*********************************************************/
// TODO: mblen
// TODO: mbtowc
// TODO: wctomb, wctomb_s
// TODO: mbstowcs, mbstowcs_s
// TODO: wcstombs, wcstombs_s
// TODO: MB_CUR_MAX


/****************************************************/
/* Null-Terminated Wide String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/wide */
/****************************************************/
// TODO: wchar_t (in stdlib.h, wchar.h)


/*******************************************************/
/* Pseudo-Random Number Generation Functions           */
/* Ref: https://en.cppreference.com/w/c/numeric/random */
/*******************************************************/
// TODO: rand
// TODO: srand
// TODO: RAND_MAX, 2147483647, same as INT_MAX


/**************************************************/
/* Algorithm Functions                            */
/* Ref: https://en.cppreference.com/w/c/algorithm */
/**************************************************/
// TODO: qsort, qsort_s
// TODO: bsearch, bsearch_s


/*****************************************************/
/* Common Mathematical Functions                     */
/* Ref: https://en.cppreference.com/w/c/numeric/math */
/*****************************************************/

/** @brief Structure type, return of the @ref div function */
typedef struct {
    int quot; ///< Quotient
    int rem;  ///< Remainder
} div_t;

/** @brief Structure type, return of the @ref ldiv function */
typedef struct {
    long quot; ///< Quotient
    long rem;  ///< Remainder
} ldiv_t;

/** @brief Structure type, return of the @ref lldiv function */
typedef struct {
    long long quot; ///< Quotient
    long long rem;  ///< Remainder
} lldiv_t;

/** @brief Computes absolute value of an integral value (\f$|n|\f$)
 * 
 * Computes the absolute value of an integer number. The behaviour is undefined
 * if the result cannot bre represented by the return type.
 * 
 * @param n Integer value
 * 
 * @returns The absolute value of `n` (i.e. \f$|n|\f$), if it is representable.
 * 
 * @note In 2's complement systems, the absolute value of the most-negative
 * value is out of range, e.e. for 32-bit 2's complement type `int`,
 * @ref INT_MIN is `-2147483648`, but the would-be result `2147483648` is
 * greater than @ref INT_MAX, which is `2147483647`.
 * 
 * @see @ref fabs, @ref fabsf, @ref fabsl, @copybrief fabs
 * @see @ref cabs, @ref cabsf, @ref cabsl, @copybrief cabs
*/
extern int abs(int n);
/** @copydoc abs */
extern long labs(long n);
/** @copydoc abs */
extern long long llabs(long long n);

/** @brief Computes quotient and remainder of integer division
 * 
 * Computes both the quotient and the remainder of the division of the numerator
 * `x` by the denominator `y`.
 * 
 * Computes the quotient and remainder simultaneously.
 * 
 * If either the remainder or the quotient cannot be represented, the behaviour
 * is undefined.
 * 
 * @param x Integer numerator
 * @param y Integer denominator
 * 
 * @returns If both the remainder and the quotient can be represented as objects
 * of the corresponding type (`int, `long`, `long long`, `intmax_t`), returns
 * both quotient and remainder as an object of type @ref div_t, @ref ldiv_t,
 * @ref lldiv_t, @ref imaxdiv_t respectively.
 * 
 * @note On many platforms, a single CPU instruction obtains both the quotient
 * and the remainder, and this function may leverage that, although compilers
 * are generally able to merge nearby `/` and `%` where suitable.
 * 
 * @see @ref fmod, @ref fmodf, @ref fmodl, @copybrief fmod
 * @see @ref remainder, @ref remainderf, @ref remainderl, @copybrief remainder
 * @see @ref remquo, @ref remquof, @ref remquol, @copybrief remquo
*/
extern div_t div(int x, int y);
/** @copydoc div */
extern ldiv_t ldiv(long x, long y);
/** @copydoc div */
extern lldiv_t lldiv(long long x, long long y);

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
