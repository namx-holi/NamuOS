/**
 * @file string.h
 * @defgroup libc_string <string.h>
 * @brief String handling
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/string/byte)
 * for **Null-terminated byte strings**
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte)
 * for **Null-terminated byte strings**
 * 
 * @{
*/

#ifndef _LIBC_STRING_H
#define _LIBC_STRING_H 1

#include <stddef.h> // size_t

// Ref: https://en.cppreference.com/w/c/string/byte
// TODO: strcpy, strcpy_s
// TODO: strncpy, strncpy_s
// TODO: strcat, strcat_s
// TODO: strncat, strncat_s
// TODO: strxfrm
// TODO: strdup
// TODO: strndup

/** @brief Returns the length of a given string
 * 
 * Returns the length of the given null-terminated byte string, that is, the
 * number of characters in a character array whose first element is pointed to
 * by `str` up to and not including the first null character.
 * 
 * The behaviour is undefined if `str` is not a pointer to a null-terminated
 * byte string.
 * 
 * @param str Pointer to the null-terminated byte string to be examined
 * 
 * @returns The length of the null-terminated byte string `str`.
 * 
 * @see @ref wcslen and @ref wcsnlen_s, @copybrief wcslen
 * @see @ref mblen @copybrief mblen
*/
extern size_t strlen(const char* str);
/** @copybrief strlen
 * 
 * Returns the length of the given null-terminated byte string, that is, the
 * number of characters in a character array whose first element is pointed to
 * by `str` up to and not including the first null character. The function
 * returns zero if `str` is a null pointer, and returns `strsz` if the null
 * character was not found in the first `strsz` bytes of `str`.
 * 
 * The behaviour is undefined if `str` is not a pointer to a null-terminated
 * byte string and `strsz` is greater than the size of that character array.
 * 
 * @param str Pointer to the null-terminated byte string to be examined
 * @param strsz Maximum number of characters to examine
 * 
 * @returns The length of the null-terminated byte string `str` on success,
 * zero if `str` is a null pointer, `strsz` if the null character was not found.
 * 
 * @note @ref strnlen_s and @ref wcsnlen_s are the only
 * [bounds-checked functions](https://en.cppreference.com/w/c/error) that do not
 * invoke the runtime constaints handler. They are pure utility functions used
 * to provide limited support for non-null terminated strings.
*/
extern size_t strnlen_s(const char* str, size_t strsz);

// Ref: https://en.cppreference.com/w/c/string/byte
// TODO: strcmp
// TODO: strncmp
// TODO: strcoll
// TODO: strchr
// TODO: strrchr
// TODO: strspn
// TODO: strcpsn
// TODO: strpbrk
// TODO: strstr
// TODO: strtok, strtok_s

/** @brief Searches an array for the first occurrence of a character
 * 
 * Finds the first occurrence of `(unsigned char)ch` in the initial `count bytes
 * (each interpreted as `unsigned char`) of the object pointed to by `ptr`.
 * 
 * The behaviour is undefined if access occurs beyond the end of the array
 * searched. The behaviour is undefined if `ptr` is a null pointer.
 * 
 * This function behaves as if it reads the bytes sequentially and stops as soon
 * as a matching bytes is found: if the array pointed to by `ptr` is smaller
 * than `count`, but the match is found within the array, the behaviour is
 * well-defined
 * 
 * @param ptr Pointer to the object to be examined
 * @param ch Bytes to search for
 * @param count Max number of bytes to examine
 * 
 * @returns Pointer to the location of the byte, or a null pointer if no such
 * byte is found.
 * 
 * @see @ref strchr @copybrief strchr
*/
extern void* memchr(const void* ptr, int ch, size_t count);
// TODO: Type-generic version using QVoid

/** @brief Compares two buffers
 * 
 * Compares the first `count` bytes of the objects pointed to by `lhs` and
 * `rhs`. The comparison is done lexicographically.
 * 
 * The sign of the result is the sign of the difference between the values of
 * the first pair of bytes (both interpreted as `unsigned char`) that differ in
 * the objects being compared.
 * 
 * The behaviour is undefined if access occurs beyond the end of either object
 * pointed to by `lhs` and `rhs`. The behaviour is undefined if either `lhs` or
 * `rhs` is a null pointer.
 * 
 * @param lhs Pointer to one of the objects to compare
 * @param rhs Pointer to the other object to compare
 * @param count Number of bytes to examine
 * 
 * @returns Negative value if `lhs` appears before `rhs` in lexicographical
 * order.
 * @returns Zero if `lhs` and `rhs` compare equal, or if count is zero.
 * @returns Positive value if `lhs` appears after `rhs` in lexicographical
 * order.
 * 
 * @note This function reads
 * [object representations](https://en.cppreference.com/w/c/language/object),
 * not the object values, and is typically meaningful for byte arrays only:
 * structs may have padding bytes whose values are indeterminate, the values of
 * any bytes beyond the last stored member in a union are indeterminate, and a
 * type may have two or more representations for the same value (different
 * encodings for `+0` and `-0`, or for `+0.0` and `-0.0`, indeterminate padding
 * bits within the type).
 * 
 * @see strcmp @copybrief strcmp
 * @see strncmp @copybrief strncmp
*/
extern int memcmp(const void* lhs, const void* rhs, size_t count);

/** @brief Fills a buffer with a character
 * 
 * Copies the value `(unsigned char)ch` into each of the first `count`
 * characters of the object pointed to by `dest`.
 * 
 * The behaviour is undefined if access occurs beyond the end of the `dest`
 * array. The behaviour is undefined if `dest` is a null pointer.
 * 
 * @param dest Pointer to the object to fill
 * @param ch Fill byte
 * @param count Number of bytes to fill
 * 
 * @returns Returns a copy of `dest`
 * 
 * @note `memset` may be optimised away (under the
 * [as-if](https://en.cppreference.com/w/c/language/as_if) rules) if the object
 * modified by this function is not accessed again for the rest of its lifetime.
 * For that reason, this function cannot be used to scrub memory (e.g., to fill
 * an array that stored a password with zeroes).
 * @note This optimisation is prohibited for @ref memset_explicit and
 * @ref memset_s: they are guaranteed to perform the memory write.
 * 
 * @see @ref memcpy and @ref memcpy_s, @copybrief memcpy
 * @see @ref wmemset @ref wmemset
*/
extern void* memset(void* dest, int ch, size_t count);
/** @copybrief memset
 * 
 * Copies the value `(unsigned char)ch` into each of the first `count`
 * characters of the object pointed to by `dest`.
 * 
 * Writing is guaranteed and safe for sensitive information.
 * 
 * The behaviour is undefined if access occurs beyond the end of the `dest`
 * array. The behaviour is undefined if `dest` is a null pointer.
 * 
 * @param dest Pointer to the object to fill
 * @param ch Fill byte
 * @param count Number of bytes to fill
 * 
 * @returns Returns a copy of `dest`
 * 
 * @see @ref memcpy and @ref memcpy_s, @copybrief memcpy
 * @see @ref wmemset @ref wmemset
*/
extern void* memset_explicit(void* dest, int ch, size_t count);
/** @copybrief memset
 * 
 * Copies the value `(unsigned char)ch` into each of the first `count`
 * characters of the object pointed to by `dest`.
 * 
 * The following errors are detected at runtime and call the currently installed
 * [constraint handler](https://en.cppreference.com/w/c/error/set_constraint_handler_s)
 * function after storing `ch` in every location of the destination range
 * `[dest, dest+destsz]` if `dest` and `destsz` are themselves valid:
 * - `dest` is a null pointer
 * - `destsz` or `count` is greater than @ref RSIZE_MAX
 * - `count` is greater than `destsz` (buffer overflow would occur)
 * 
 * The behaviour is undefined if the size of the character array pointed to by
 * `dest < count <= destsz`; in other words, an erronous value of `destsz` does
 * not expose the impending buffer overflow.
 * 
 * @param dest Pointer to the object to fill
 * @param ch Fill byte
 * @param count Number of bytes to fill
 * @param destsz Size of the destination array
 * 
 * @returns Zero on success, non-zero on error. Also on error, if `dest` is not
 * a null pointer and `destsz` is valid, writes `destsz` fill bytes `ch` to the
 * destination array.
 * 
 * @see @ref memcpy and @ref memcpy_s, @copybrief memcpy
 * @see @ref wmemset @ref wmemset
*/
// extern errno_t memset_s(void* dest, rsize_t destsz, int ch, rsize_t count);

/** @brief Copies one buffer to another
 * 
 * Copies `count` characters from the object pointed to by `src` to the object
 * pointed to by `dest`. Both objects are interpreted as arrays of `unsigned
 * char`.
 * 
 * The behaviour is undefined if access occurs beyond the end of the `dest`
 * array. If the objects overlap (which is a violation of the `restrict`
 * contract), the behaviour is undefined. The behaviour is undefined if either
 * `dest` or `src` is an invalid or null pointer.
 * 
 * @param dest Pointer to the object to copy to
 * @param src Pointer to the object to copy from
 * @param count Number of bytes to copy
 * 
 * @returns Returns a copy of `dest`
 * 
 * @note `memcpy` may be used to set the
 * [effective type](https://en.cppreference.com/w/c/language/object#Effective_type)
 * of an object obtained by an allocation function.
 * @note `memcpy` is the fastest library routine for memory-to-memory copy. It
 * is usually more efficient than @ref strcpy, which must scan the data it
 * copies, or @ref memmove, which must take precautions to handle overlapping
 * inputs.
 * @note Several C compilers transform suitable memory-copying loops to `memcpy`
 * calls.
 * @note Where
 * [strict aliasing](https://en.cppreference.com/w/c/language/object#Strict_aliasing)
 * prohibits examining the same memory as values of two different types,
 * `memcpy` may be used to convert the values.
 * 
 * @see @ref memccpy @copybrief memccpy
 * @see @ref memmove and @ref memmove_s, @copybrief memmove
 * @see @ref wmemcpy and @ref wmemcpy_s, @copybrief wmemcpy
*/
extern void* memcpy(void* restrict dest, const void* restrict src, size_t count);
// TODO: Doc for memcpy_s
// extern errno_t memcpy_s(void* restrict dest, rsize_t destsz, const void* restrict src, rsize_t count);

/** @brief Moves one buffer to another
 * 
 * Copies `count` characters from the object pointed to by `src` to the object
 * pointed to by `dest`. Both objects are interpreted as arrays of `unsigned
 * char`. The objects may overlap: copying takes place as if the characters were
 * copied to a temporary character array and then the characters were copied
 * from the array to `dest`.
 * 
 * The behaviour is undefined if access occurs beyond the end of the `dest`
 * array. The behaviour is undefined if either `dest` or `src` is an invalid or
 * null pointer.
 * 
 * @param dest Pointer to the object to copy to
 * @param src Pointer to the object to copy from
 * @param count Number of bytes to copy
 * 
 * @returns Returns a copy of `dest`
 * 
 * @note `memmove` may be used to set the
 * [effective type](https://en.cppreference.com/w/c/language/object#Effective_type)
 * of an object obtained by an allocation function.
 * @note Despite being specified "as if" a temporary buffer is used, actual
 * implementations of this function do not incur the overhead or double copying
 * or extra memory. A common approach (glibc and bsd libc) is to copy bytes
 * forward from the beginning of the buffer if the destination starts before the
 * source, and backwards from the end otherwise, with a fall back to the more
 * efficient @ref memcpy when there is no overlap at all.
 * @note Where
 * [strict aliasing](https://en.cppreference.com/w/c/language/object#Strict_aliasing)
 * prohibits examining the same memory as values of two different types,
 * `memmove` may be used to convert the values.
 * 
 * @see @ref memcpy and @ref memcpy_s, @copybrief memcpy
 * @see @ref wmemmove and @ref wmemmove_s, @copybrief wmemmove
*/
extern void* memmove(void* dest, const void* src, size_t count);
// TODO: Doc for memmove_s
// extern errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count);

/** @brief Copies one buffer to another, stopping after the specified delimiter
 * 
 * Copies bytes from the object pointed to by `src` to the object pointed to by
 * `dest`, stopping after *any* of the next two conditions are satisfied:
 * - `count` bytes are copied
 * - the byte `(unsigned char)c` is found (and copied).
 * 
 * The `src` and `dest` objects are interpreted as array of `unsigned char`.
 * 
 * The behaviour is undefined if *any* condition is met:
 * - access occurs beyond the end of the `dest` array
 * - the objects overlap (which is a violation of the `restrict` contract)
 * - either `dest` or `src` is an invalid or null pointer
 * 
 * @param dest Pointer to the object to copy to
 * @param src Pointer to the object to copy from
 * @param c Terminating byte, converted to `unsigned char` at first
 * @param count Number of bytes to copy
 * 
 * @returns If the byte `(unsigned char)c` was found, `memccpy` returns a
 * pointer to the next byte in `dest` after `(unsigned char)c`. Otherwise it
 * returns a null pointer.
 * 
 * @note The function is identical to the
 * [POSIX memccpy](https://pubs.opengroup.org/onlinepubs/9699919799/functions/memccpy.html).
 * @note `memccpy(dest, src, 0, count)` behaves similar to
 * `strncpy(dest, src, count)`, except that the former returns a pointer to the
 * *end* of the buffer written, and does not zero-pad the destination array.
 * Thus, `memccpy` is useful for efficiently concatenating multiple strings.
 * ```c
 * char bigString[100];
 * char*  end = bigString + sizeof(bigString);
 * 
 * char* p = memccpy(bigString, "John, ", 0, sizeof(bigString) - 1);
 * if (p)
 *   p = memccpy(p - 1, "Paul, ", 0, end - p);
 * if (p)
 *   p = memccpy(p - 1, "George, ", 0, end - p);
 * if (p)
 *   p = memccpy(p - 1, "Joel ", 0, end - p);
 * if (!p)
 *   end[-1] = 0;
 * 
 * puts(bigString); // John, Paul, George, Joel
 * ```
 * 
 * @see @ref memcpy and @ref memcpy_s, @copybrief memcpy
 * @see @ref wmemcpy and @ref wmemcpy_s, @copybrief wmemcpy
 * @see @ref memmove and @ref memmove_s, @copybrief memmove
 * @see @ref strcpy and @ref strcpy_s, @copybrief strcpy
 * @see @ref strcat and @ref strcat_s, @copybrief strcat
*/
extern void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count);

// Ref: https://en.cppreference.com/w/c/string/byte
// TODO: strerror, strerror_s, strerrorlen_s

#endif

/** @} */
