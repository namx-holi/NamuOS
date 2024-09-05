/**
 * @file fenv.h
 * @defgroup libc_fenv <fenv.h>
 * @brief Floating-point environment
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/numeric/fenv)
 * for **Floating-point environment**
 * 
 * @{
*/

#ifndef _LIBC_FENV_H
#define _LIBC_FENV_H 1

/*****************************************************/
/* Floating-Point Environment Functions              */
/* Ref: https://en.cppreference.com/w/c/numeric/fenv */
/*****************************************************/

// TODO: Group floating-point exceptions with documentation
// TODO: Double check values
#define FE_DIVBYZERO 1 /**< Pole error occured in an earlier floating-point operation */
#define FE_INEXACT 2 /**< Inexact result: rounding was necessary to store the result of an earlier floating-point operation */
#define FE_INVALID 4 /**< Domain error occured in an earlier floating-point operation */
#define FE_OVERFLOW 8 /**< The result of an earlier floating-point operation was too large to be representable */
#define FE_UNDERFLOW 16 /**< The result of an earlier floating-point operation was subnormal with a loss of precision */

/** Bitwise OR of all supported floating-point exceptions */
#define FE_ALL_EXCEPT FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW


// TODO: Group floating-point rounding directions with documentation
// TODO: Double check values
#define FE_DOWNWARD   0 /**< Rounding towards negative infinity */
#define FE_TONEAREST  1 /**< Rounding towards nearest representable value */
#define FE_TOWARDZERO 2 /**< Rounding towards zero */
#define FE_UPWARD     3 /**< Rounding towards positive infinity */

/** @brief Default floating-point environment
 * 
 * The macro constant `FE_DFL_ENV` expands to an expression of type
 * `const fenv_t*`, which points to a full copy of the default floating-point
 * environment, that is, the environment as loaded at program startup.
 * 
 * Additional macros that begin with `FE_` followed by uppercase letters, and
 * have the type `const fenv_t*`, may be supported by an implementation.
 * 
 * @see @ref fegetenv @copybrief fegetenv
 * @see @ref fesetenv @copybrief fesetenv
 * @see @ref feupdateenv @copybrief feupdateenv
*/


// Default floating-point environment
// TODO: Implement
#define FE_DFL_ENV /* implementation defined */


// TODO: Documentation, The type representing the entire floating-point environment
typedef struct {} fenv_t;
// TODO: fenv_t

// TODO: Documentation, The type representing all floating-point status flags collectively
typedef struct {} fexcept_t;
// TODO: fexcept_t

// TODO: Documentation, Clears the specified floating-point status flags
extern int feclearexcept(int excepts);

// TODO: Documentation, Determines which of the specified floating-point flags are set
extern int fetestexcept(int excepts);

// TODO: Documentation, Raises the specified floating-point exceptions
extern int feraiseexcept(int excepts);

// TODO: Documentation, Copies the state of the specified floating-point status flags from or to the floating-point environment
extern int fegetexceptflag(fexcept_t* flagp, int excepts);
extern int fesetexceptflag(const fexcept_t* flagp, int excepts);

// TODO: Documentation, Gets or sets rounding direction
extern int fegetround(int round);
extern int fesetround();

// TODO: Documentation, Saves or restores the current floating-point environment
int fegetenv(fenv_t* envp);
int fesetenv(const fenv_t* envp);

// TODO: Documentation, Saves the environment, clears all status flags and ignores all future errors
extern int feholdexcept(fenv_t* envp);

// TODO: Documentation, Restores the floating-point environment and raises the previously raised exceptions
extern int feupdateenv(const fenv_t* envp);

#endif

/** @} */
