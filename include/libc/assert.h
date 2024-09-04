/** 
 * @file assert.h
 * @defgroup libc_assert <assert.h>
 * @brief Conditionally compiled macro that compares its argument to zero
 * @ingroup libc
 * 
 * `assert.h` defines one macro function that can be used as a standard
 * debugging tool.
 * 
 * @note Since C23, `static_assert` is itself a keyword, which may also be a
 * predefined macro, so @ref libc_assert no longer provides it.
 * 
 * @see @ref math_errhandling @copybrief math_errhandling
 * @see @ref MATH_ERRNO @copybrief MATH_ERRNO
 * @see @ref MATH_ERREXCEPT @copybrief MATH_ERREXCEPT
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/error)
 * for **Error handling**
 * 
 * @{
*/

#ifndef _LIBC_ASSERT_H
#define _LIBC_ASSERT_H 1


/**********************************************/
/* Error Handling Functions                   */
/* Ref: https://en.cppreference.com/w/c/error */
/**********************************************/

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
/** @brief Reports a failed assertion
 * 
 * Reports a failed assertion
 * 
 * @param condition Condition that was checked and failed
 * @param file What file this assertion was in
 * @param func Function this assertion was called in
 * @param line What line of the file this assertion was on
*/
extern void _assert(const char* condition, const char* file, const char* func, int line);

/** @brief Aborts the program if the user-specified condition is not true.
 * 
 * The definition of the macro assert depends on another macro, `NDEBUG`, which
 * is not defined by the standard library.
 * 
 * If `NDEBUG` is defined as a macro name at the point in the source code where
 * @ref clib_assert is included, then assert does nothing.
 * 
 * If `NDEBUG` is not defined, then assert checks if the expression synthesised
 * from `__VA_ARGS__` (which must have a scalar type, otherwise, the behaviour
 * is undefined) compares equal to zero. If it does, assert outputs
 * implementation-specific diagnostic information on the standard error output
 * and calls @ref abort. The diagnostic information is required to include the
 * text of expression, as well as the values of the
 * [predefined variable](https://en.cppreference.com/w/c/language/function_definition)
 * `__func__` and of the [predefined macros](https://en.cppreference.com/w/c/preprocessor/replace)
 * `__FILE__` and `__LINE__`.
 * 
 * @param condition Expression of scalar type
 * 
 * @note There is no standardised interface to add an additional message to
 * assert errors. A portable way to include one is to use a
 * [comma operator](https://en.cppreference.com/w/c/language/operator_other#Comma_operator),
 * or use `&&` with a string literal:
 * ```c
 * assert(("There are five lights", 2 + 2 == 5));
 * assert(2 + 2 == 5 && "There are five lights");
 * ```
 * @note The implementation of assert in
 * [Microsoft CRT](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-macro-assert-wassert)
 * does not conform to C99 and later revisions, because its underlying function
 * (`_wassert`) takes neither `__func__` nor an equivalent replacement.
 * @note Even though the change of assert in C23 is not a formal defect report,
 * the C committee recommends implementations to backport the change to old
 * modes.
 * 
 * @todo Handle __VA_ARGS__
*/
#define assert(condition)                                  \
{                                                          \
	if (condition) {}                                      \
	else {                                                 \
		_assert(#condition, __FILE__, __func__, __LINE__); \
	}                                                      \
}
#endif /* NDEBUG */

#endif

/** @} */
