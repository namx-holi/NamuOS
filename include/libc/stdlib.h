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

#include <stddef.h> // size_t, wchar_t


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

// TODO: Documentation, Converts a byte string to a floating-point value
extern double atof(const char* str);

// TODO: Documentation, Converts a byte string to an integer value
extern int atoi(const char* str);
extern long atol(const char* str);
extern long long atoll(const char* str);

// TODO: Documentation, Converts a byte string to an integer value
extern long strtol(const char* restrict str, char** restrict str_end, int base);
extern long long strtoll(const char* restrict str, char** restrict str_end, int base);

// TODO: Documentation, Converts a byte string to an unsigned integer value
extern unsigned long strtoul(const char* restrict str, char** restrict str_end, int base);
extern unsigned long long strtoull(const char* restrict str, char** restrict str_end, int base);

// TODO: Documentation, Converts a byte string to a floating-point value
extern float strtof(const char* restrict str, char** restrict str_end);
extern double strtod(const char* restrict str, char** restrict str_end);
extern long double strtold(const char* restrict str, char** restrict str_end);

// TODO: Documentation, Converts a floating-point value to a byte string
extern int strfromf(char* restrict s, size_t n, const char* restrict format, float fp);
extern int strfromd(char* restrict s, size_t n, const char* restrict format, double fp);
extern int strfroml(char* restrict s, size_t n, const char* restrict format, long double fp);


/*********************************************************/
/* Null-Terminated Multibyte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/multibyte */
/*********************************************************/

// TODO: Documentation, Returns the number of bytes in the next multibyte character
extern int mblen(const char* s, size_t n);

// TODO: Documentation, Converts the next multibyte character to wide character
extern int mbtowc(wchar_t* restrict pwc, const char* restrict s, size_t n);

// TODO: Documentation, Converts a wide character to its multibyte representation
extern int wctomb(char* s, wchar_t wc);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
extern errno_t wctomb_s(int* restrict status, char* restrict s, rsize_t ssz, wchar_t wc);
#endif

// TODO: Documentation, Converts a narrow multibyte character string to wide string
extern size_t mbstowcs(wchar_t* restrict dst, const char* restrict src, size_t len);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
extern errno_t mbstowcs_s(size_t* restrict retval, wchar_t* restrict dst, rsize_t dstsz, const char* restrict src, rsize_t len);
#endif

// TODO: Documentation, Converts a wide string to narrow multibyte character string
extern size_t wcstombs(char* restrict dst, const wchar_t* restrict src, size_t len);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
extern errno_t wcstombs_s(size_t* restrict retval, char* restrict dst, rsize_t dstsz, const wchar_t* restrict src, rsize_t len);
#endif

/** @brief Maximum number of bytes in a multibyte character, in the current locale */
#define MB_CUR_MAX MB_LEN_MAX // Just use default max from <limits.h>


/****************************************************/
/* Null-Terminated Wide String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/wide */
/****************************************************/

// NOTE: Can be defined in <stddef.h>, <stdlib.h>, or <wchar.h>
#ifndef _WCHAR_T
#define _WCHAR_T 1
/** @brief Integer type that can hold any valid wide character */
typedef int wchar_t;
#endif


/*******************************************************/
/* Pseudo-Random Number Generation Functions           */
/* Ref: https://en.cppreference.com/w/c/numeric/random */
/*******************************************************/

/** @brief Maximum possible value generated by @ref rand()
 * 
 * Expands to an integer constant expression equal to the maximum value returned
 * by the function @ref rand(). This value is implementation dependent. It's
 * guaranteed that this value is at least `32767`.
 * 
 * @see @ref rand @copybrief rand
 * @see @ref srand @copybrief srand
*/
#define RAND_MAX INT_MAX // Just use maximum integer as RAND_MAX

/** @brief Generates a pseudo-random number
 * 
 * Returns a pseudo-random integer value between `0` and @ref RAND_MAX (`0` and
 * @ref RAND_MAX included).
 * 
 * @ref srand() seeds the pseudo-random number generator used by `rand()`. If
 * `rand()` is used before any calls to @ref srand(), `rand()` behaves as if it
 * was seeded with `srand(1)`. Each time `rand()` is seeded with @ref srand(),
 * it must produce the same sequence of values.
 * 
 * `rand()` is not guaranteed to be thread-safe.
 * 
 * @returns Pseudo-random integer value between `0` and @ref RAND_MAX,
 * inclusive.
 * 
 * @note There are no guarantees as to the quality of the random sequence
 * produced. In the past, some implementations of `rand()` have had serious
 * shortcomings in the randomness, distribution and period of the sequence
 * produced (in one well-known example, the low-order bit simply alternated
 * between 1 and 0 between calls). `rand()` is not recommended for serious
 * random-number generation needs, like cryptography.
 * @note POSIX requires that the period of the pseudo-random number generator
 * used by rand be at least \f$2^32\f$.
 * @note POSIX offered a thread-safe version of `rand` called `rand_r`, which is
 * obsolete in favour of the
 * [drand48](http://pubs.opengroup.org/onlinepubs/9699919799/functions/drand48.html)
 * family of functions.
 * 
 * @see @ref srand @copybrief srand
 * @see @ref RAND_MAX @copybrief RAND_MAX
*/
extern int rand();

/** @brief Seeds pseudo-random number generator
 * 
 * Seeds the pseudo-random number generator used by @ref rand() with the value
 * `seed`.
 * 
 * If @ref rand() is used before any calls to `srand()`, @ref rand() behaves as
 * if it was seeded with `srand(1)`. Each time @ref rand() is seeded with the
 * same seed, it must produce the same sequence of values.
 * 
 * `srand()` is not guaranteed to be thread-safe.
 * 
 * @param seed The seed value
 * 
 * @note Generally speaking, the pseudo-random number generator should only be
 * seeded once, before any calls to @ref rand(), and the start of the program.
 * It should not be repeatedly seeded, or reseeded every time you wish to
 * generate a new batch of pseudo-random numbers.
 * @note Standard practice is to use the result of a call to @ref time(0) as the
 * seed. However, @ref time() returns a @ref time_t value, and @ref time_t is
 * not guaranteed to be an integral type. In practice, though, every major
 * implementation defines @ref time_t to be an integral type, and this is also
 * what POSIX requires.
 * 
 * @see @ref rand @copybrief rand
 * @see @ref RAND_MAX @copybrief RAND_MAX
*/
extern void srand(unsigned seed);


/**************************************************/
/* Algorithm Functions                            */
/* Ref: https://en.cppreference.com/w/c/algorithm */
/**************************************************/

/** @brief Signature of the comparison function used by @ref qsort and @ref bsearch */
typedef int (*__compar_fn_t)(const void*, const void*);

/** @brief Sorts a range of elements with unspecified type
 * 
 * Sorts the given array pointed to by `ptr` in ascending order. The array
 * contains `count` elements of `size` bytes. Function pointed to by `comp` is
 * used for object comparison.
 * 
 * If `comp` indicates two elements as equivalent, their order in the resulting
 * sorted array is unspecified.
 * 
 * @param ptr Pointer to the array to sort
 * @param count Number of elements in the array
 * @param size Size of each element in the array in bytes
 * @param comp Comparison function which returns a negative integer value if the
 * first argument is *less* than the second, a positive integer value if the
 * first argument is *greater* than the second, and zero if the arguments are
 * equivalent. The function must not modify the objects passed to it and must
 * return consistent results when called for the same objects, regardless of
 * their positions in the array.
 * 
 * @note Despite the name, neither C nor POSIX standards require this function
 * to be implemented using [quicksort](https://en.wikipedia.org/wiki/Quicksort)
 * or make any complexity or stability guarantees.
 * 
 * @see @ref bsearch and @ref bsearch_s, @copybrief bsearch
*/
extern void qsort(void* ptr, size_t count, size_t size, __compar_fn_t comp);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Documentation for qsort_s
extern errno_t qsort_s(void* ptr, rsize_t count, rsize_t size, __compar_fn_t comp, void* context);
#endif

/** @brief Searches an array for an element of unspecified type
 * 
 * Finds an element equal to `key` in an array pointed to by `ptr`. The array
 * contains `count` elements of `size` bytes and must be partitioned with
 * respect to `key`, that is, all the elements that compare less than must
 * appear before all the elements that compare equal to, and those must appear
 * before all the elements that compare greater than the key object. A fully
 * sorted array satisfies these requirements. The elements are compared using
 * function pointed to by `comp`. The behaviour is undefined if the array is not
 * already partitioned with respect to `*key` in ascending order according to the
 * same criterion that `comp` uses.
 * 
 * @param key Pointer to the element to search for
 * @param ptr Pointer to the array to examine
 * @param count Number of elements in the array
 * @param size Size of each element in the array in bytes
 * @param comp Comparison function which returns a negative integer value if the
 * first argument is *less* than the second, a positive integer value if the
 * first argument is *greater* than the second, and zero if the arguments are
 * equivalent. `key` is passed as the first argument, and an element from the
 * array as the second. The function must not modify the objects passed to it
 * and must return consistent results when called for the same objects,
 * regardless of their positions in the array.
 * 
 * @returns Pointer to an element in the array that compares equal to `*key`, or
 * null pointer if such element has not been found.
 * 
 * @note Despite the name, neither C nor POSIX standards require this function
 * to be implemented using binary search or make any complexity guarantees.
*/
extern void* bsearch(const void* key, const void* ptr, size_t count, size_t size, __compar_fn_t comp);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Documentation for bsearch_s
extern void* bsearch_s(const void* key, const void* ptr, rsize_t count, rsize_t size, __compar_fn_t comp, void* context);
#endif
// TODO: QVoid version of bsearch
// TODO: QVoid version of bsearch_s


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
