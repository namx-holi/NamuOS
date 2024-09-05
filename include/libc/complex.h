/**
 * @file complex.h
 * @defgroup libc_complex <complex.h>
 * @brief Complex number arithmetic
 * @ingroup libc
 * 
 * If the macro constant `__STDC_NO_COMPLEX__` is defined by the
 * implementation, the complex types, the header @ref clib_complex and all the
 * names listed here are not provided.
 * 
 * The C porogramming language, as of C99, supports complex number math with the
 * three built-in types `double _Complex`, `float _Complex`, and `long double
 * _Complex` (see @ref _Complex). When the header @ref clib_complex is included,
 * the three complex number types are also accessible as `double complex`,
 * `float complex`, and `long double complex`.
 * 
 * In addition to the complex types, the three imaginary types may be supported:
 * `double _Imaginary`, `float _Imaginary`, and `long double _Imaginary` (see
 * @ref _Imaginary). When the header @ref clib_complex is included, the three
 * imaginary types are also accessible as `double imaginary`, `float imaginary`,
 * and `long double imaginary`.
 * 
 * Standard arithmetic operators `+`, `-`, `*`, `/` can be used with real,
 * complex, and imaginary types in any combination.
 * 
 * Imaginary numbers are supported if `__STDC_IEC_559_COMPLEX__` or
 * `__STDC_IEC_60559_COMPLEX__` is defined.
 * 
 * @note The following names are poteitially reserved for future addition to
 * @ref clib_complex and are not available for use in the programs that include
 * the header: `cerf`, `cerfc`, `cexp2`, `cexpm1`, `clog10`, `clog1p`, `clog2`,
 * `clgamma`, `ctgamma`, `csinpi`, `ccospi`, `ctanpi`, `casinpi`, `cacospi`,
 * `catanpi`, ccompoundn`, `cpown`, `cpowr`, `crootn`, `crsqrt`, `cexp10m1`,
 * `cexp10`, `cexp2m1`, `clog10p1`, `clog2p1`, `clogp1`, along with their `-f`
 * and `-l` suffixed variants.
 * @note Although the C standard names the inverse hyperbolics with "complex
 * arc hyperbolic sine" etc., the inverse functions of the hyperbolic functions
 * are the area functions. Their argument is the area of a hyperbolic sector,
 * not an arc. The correct names are "complex inverse hyperbolic sine" etc. Some
 * authors use "complex area hyperbolic sine" etc.
 * @note A complex or imaginary number is infinite if one of its parts is
 * infinite, even if the other part is NaN.
 * @note A complex or imaginary number is finite if both parts are neither
 * infinites nor NaNs.
 * @note A complex number or imaginary number is a zero if both parts are
 * positive or negative zeroes.
 * @note While MSVC does provide a @ref clib_complex header, it does not
 * implement complex numbers as native types, but as `struct`s, which are
 * incompatible with standard C complex types and do not support the `+`, `-`,
 * `*`, `/` operators.
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/numeric/complex)
 * for **Complex number arithmetic**
 * 
 * @{
*/

#ifndef _LIBC_COMPLEX_H
#define _LIBC_COMPLEX_H 1

#ifdef __STDC_IEC_559_COMPLEX__ /* Complex numbers enabled */


/********************************************************/
/* Complex Number Arithmetic Functions                  */
/* Ref: https://en.cppreference.com/w/c/numeric/complex */
/********************************************************/

/** @brief Imaginary type macro
 * 
 * This macro expands to the keyword @ref _Imaginary.
 * 
 * This is a convenience macro that makes it possible to use `float imaginary`,
 * `double imaginary`, and `long double imaginary` as an alternative way to
 * write the three pure imaginary C types `float _Imaginary`, `double
 * _Imaginary`, and `long double _Imaginary`
 * 
 * As with any pure imaginary number support in C, this macro is only defined if
 * the imaginary numbers are supported.
 * 
 * Imaginary numbers are supported if `__STDC_IEC_559_COMPLEX__` is
 * defined.
 * 
 * @note Programs are allowed to undefine and perhaps redefine the `imaginary`
 * macro.
 * @note To date, only Oracle C compiler is known to have implemented imaginary
 * types.
 * 
 * @see @ref complex @copybrief complex
*/
#define imaginary _Imaginary

/** @brief Complex type macro
 * 
 * This macro expands to a type specifier used to identify
 * [complex types](https://en.cppreference.com/w/c/language/arithmetic_types#Complex_floating_types).
 * 
 * A program may undefine and perhaps then redefine the complex macro.
 * 
 * @see @ref imaginary @copybrief imaginary
*/
#define complex _Complex

/** @brief The imaginary unit constant i
 * 
 * The `_Imaginary_I` macro expands to a value of type `const float _Imaginary`
 * with the value of the imaginary unit. As with any pure imaginary number
 * support in C, this macro is only defined if the imaginary numbers are
 * supported.
 * 
 * Imaginary numbers are supported if `__STDC_IEC_559_COMPLEX__` is defined.
 * 
 * @note This macro allows for the precise way to assemble a complex number from
 * its real and imaginary components, e.g. with `(double complex)((double)x +
 * _Imaginary_I * (double)y)`. This pattern was standardised in C11 as the macro
 * @ref CMPLX. Note that if @ref _Complex_I is used instead, this expression is
 * allowed to convert negative zero to positive zero in the imaginary position.
 * 
 * @see @ref _Complex_I @copybrief _Complex_I
 * @see @ref I @copybrief I
*/
#define _Imaginary_I /* unspecified */
// TODO: Declare _Imaginary_I type if needed

/** @brief The complex unit constant i
 * 
 * The `_Complex_I` macro expands to a value of type `const float _Complex` with
 * the value of the imaginary unit.
 * 
 * @note This macro may be used when @ref I is not available, such as when it
 * has been undefined by the application.
 * @note Unlike @ref _Imaginary_I and @ref CMPLX, use of this macro to construct
 * a complex number may lose the sign of zero on the imaginary part.
 * 
 * @see @ref _Imaginary_I @copybrief _Imaginary_I
 * @see @ref I @copybrief I
*/
#define _Complex_I /* unspecified */
// TODO: Declare _Complex_I type if needed

/** @brief The complex or imaginary unit constant i
 * 
 * The I macro expands to either @ref _Complex_I or @ref _Imaginary_I. If the
 * implementation does not support imaginary types, then the macro always
 * expands to @ref _Complex_I
 * 
 * A program may undefine and perhaps then redefine the macro `I`.
 * 
 * @note The macro is not named `i`, which is the name of the imaginary unit in
 * mathematics, because the name `i` was already used in many C programs, e.g.
 * as a loop counter variable.
 * @note The macro `I` is often used to form complex numbers, with expressions
 * such as `x + y*I`. If `I` is defined as @ref _Complex_I, then such expression
 * may create a value with imaginary component `+0.0` even when y is `-0.0`,
 * which is significant for complex number functions with branch cuts. The macro
 * @ref CMPLX provides a way to construct a complex number precisely.
 * @note GCC provides a non-portable extension that allows imaginary constants
 * to be specified with the suffix `i` on integer literals: `1.0fi`, `1.0i`, and
 * `1.0li` are imaginary units in GNU C. A similar approach is part of standard
 * C++ as of C++14 (`1.0if`, `1.0i`, and `1.0il` are the imaginary units in C++)
 * 
 * @see @ref _Imaginary_I @copybrief _Imaginary_I
 * @see @ref _Complex_I @copybrief _Complex_I
 * @see @ref CMPLX @copybrief CMPLX
 * @see @ref CMPLXF @copybrief CMPLXF
 * @see @ref CMPLXL @copybrief CMPLXL
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/operator%22%22i)
 * for `operator""i`
*/
#define I /* unspecified */
// TODO: Declare I type if needed

/** @brief Constructs a complex number from real and imaginary parts
 * 
 * Each of these macros expands to an expression that evaluates to the value of
 * the specified complex type, with the real part having the value of `real`
 * (converted to the specified argument type) and the imaginary part having the
 * value of `imag` (converted to the specified argument type).
 * 
 * The expressions are suitable for use as initialisers for objects with static
 * or thread storage duration, as long as the expressions `real` and `imag` are
 * also suitable.
 * 
 * @param real The real part of the complex number to return
 * @param imag The imaginary part of the complex number to return
 * 
 * @returns A complex number composed of `real` and `imag` as the real and
 * imaginary parts.
 * 
 * @note These macros are implemented as if the imaginary types are supported
 * (even if they are otherwise not supported and @ref _Imaginary_I is actually
 * undefined).
 *  
 * @see @ref _Imaginary_I @copybrief _Imaginary_I
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/complex)
 * for `complex`
*/
#define CMPLX(x, y) ((double complex)((double)(x) + _Imaginary_I * (double)(y)))
/// @copydoc CMPLX(x, y)
#define CMPLXF(x, y) ((float complex)((float)(x) + _Imaginary_I * (float)(y)))
/// @copydoc CMPLX(x, y)
#define CMPLXL(x, y) ((long double complex)((long double)(x) + _Imaginary_I * (long double)(y)))

/** @brief Computes the real part of a complex number
 * 
 * Returns the real part of z.
 * 
 * @param z Complex argument
 * 
 * @returns The real part of z.
 * @returns This function is fully specified for all possible inputs and is not
 * subject to any errors described in @ref math_errhandling
 * 
 * @note For any complex variable z, `z == creal(z) + I*cimag(z)`.
 * 
 * @see @ref creal (type-generic macro)
 * @see @ref cimag @copybrief cimag
 * @see @ref cimagf @copybrief cimagf
 * @see @ref cimagl @copybrief cimagl
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/real2)
 * for `real`
*/
extern double creal(double complex z);
/// @copydoc creal(double complex z)
extern float crealf(float complex z);
/// @copydoc creal(double complex z)
extern long double creall(long double complex z);

/** @brief Computes the imaginary part of a complex number
 * 
 * Returns the imaginary part of z.
 * 
 * @param z Complex argument
 * 
 * @returns The imaginary part of z.
 * @returns This function is fully specified for all possible inputs and is not
 * subject to any errors described in @ref math_errhandling
 * 
 * @note for any complex variable z, `z == creal(z) + I*cimag(z)`.
 * 
 * @see @ref cimag (type-generic macro)
 * @see @ref creal @copybrief creal
 * @see @ref crealf @copybrief crealf
 * @see @ref creall @copybrief creall
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/imag2)
 * for `imag`
*/
extern double cimag(double complex z);
/// @copydoc cimag(double complex z)
extern float cimagf(float complex z);
/// @copydoc cimag(double complex z)
extern long double cimagl(long double complex z);

/** @brief Computes the magnitude of a complex number
 * 
 * Computes the complex absolute value (also known as norm, modulus, or
 * magnitude) of z.
 * 
 * @param z Complex argument
 * 
 * @returns If no errors occur, returns the absolute value (norm, magnitude) of
 * z.
 * @returns Errors and special cases are handled as if the function is
 * implemented as `hypot(creal(z), cimag(z))
 * 
 * @see @ref fabs (type-generic macro)
 * @see @ref carg @copybrief carg
 * @see @link math.h::abs math.h abs @endlink Computes absolute value of an
 * integral value (\f$|x|\f$)
 * @see @link math.h::fabs math.h fabs @endlink Computes absolute value of a
 * floating-point value (\f$|x|\f$)
 * @see @link math.h::hypot math.h hypot @endlink Computes square root of the
 * sum of the squares of two given numbers (\f$\sqrt{x^2 + y^2}\f$)
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/abs)
 * for `abs`
*/
extern double cabs(double complex z);
/// @copydoc cabs(double complex z)
extern float cabsf(float complex z);
/// @copydoc cabs(double complex z)
extern long double cabsl(long double complex z);

/** @brief Computes the phase angle of a complex number
 * 
 * Computes the argument (also called phase angle) of z, with a branch cut along
 * the negative real axis.
 * 
 * @param z Complex argument
 * 
 * @returns If no errors occur, returns the phase angle of z in the interval
 * \f$[-\pi; \pi]\f$.
 * @returns Errors and special cases are handled as if the function is
 * implemented as `atan2(cimag(z), creal(z))`
 * 
 * @see @ref carg (type-generic macro)
 * @see @ref cabs @copybrief cabs
 * @see @link math.h::atan2 math.h atan2 @endlink Computes arc tangent, using
 * signs to determine quadrants
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/arg)
 * for `arg`
*/
extern double carg(double complex z);
/// @copydoc carg(double complex z)
extern float cargf(float complex z);
/// @copydoc carg(double complex z)
extern long double cargl(long double complex z);

/** @brief Computes the complex conjugate
 * 
 * Computes the [complex conjugate](https://en.wikipedia.org/wiki/Complex_conjugate)
 * of z by reversing the sign of the imaginary part.
 * 
 * @param z Complex argument
 * 
 * @returns The complex conjugate of z.
 * 
 * @see @ref conj (type-generic macro)
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/conj)
 * for `conj`
*/
extern double complex conj(double complex z);
/// @copydoc conj(double complex z)
extern float complex conjf(float complex z);
/// @copydoc conj(double complex z)
extern long double complex conjl(long double complex z);

/** @brief Computes the projection on Riemann sphere
 * 
 * Computes the projection of z on the Riemann sphere.
 * 
 * For most z, `cproj(z)==z`, but all complex infinities, even the numbers where
 * one component is infinite and the other is NaN, become positive real
 * infinity, `INFINITY+0.0*I` or `INFINITY-0.0*I`. The sign of the imaginary
 * (zero) component is the sign of `cimag(z)`.
 * 
 * @param z Complex argument
 * 
 * @returns The projection of z on the Riemann sphere.
 * @returns This function is fully specified for all possible inputs and is not
 * subject to any errors described in @ref math_errhandling
 * 
 * @note The `cproj` function helps model the Riemann sphere by mapping all
 * infinities to one (give or take the sign of the imaginary zero), and should
 * be used just before any operation, especially comparisons, that might give
 * spurious results for any of the other infinities.
 * 
 * @see @ref cproj (type-generic macro)
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/numeric/complex/proj)
 * for `proj`
*/
double complex cproj(double complex z);
/// @copydoc cproj(double complex z)
float complex cprojf(float complex z);
/// @copydoc cproj(double complex z)
long double complex cprojl(long double complex z);

// TODO: Documentation, Computes the complex base-e exponential
extern double complex cexp(double complex z);
extern float complex cexpf(float complex z);
extern long double complex cexpl(long double complex z);

// TODO: Documentation, Computes the complex natural logarithm
extern double complex clog(double complex z);
extern float complex clogf(float complex z);
extern long double complex clongl(long double complex z);

// TOOD: Documentation, Computes the complex power function
extern double complex cpow(double complex x, double complex y);
extern float complex cpowf(float complex x, float complex y);
extern long double complex cpowl(long double complex x, long double complex y);

// TODO: Documentation, Computes the complex square root
extern double complex csqrt(double complex z);
extern float complex csqrtf(float complex z);
extern long double complex csqrtl(long double complex z);

// TODO: Documentation, Computes the complex sine
extern double complex csin(double complex z);
extern float complex csinf(float complex z);
extern long double complex csinl(long double complex z);

// TODO: Documentation, Computes the complex cosine
extern double complex ccos(double complex z);
extern float complex ccosf(float complex z);
extern long double complex ccosl(long double complex z);

// TODO: Documentation, Computes the complex tangent
extern double complex ctan(double complex z);
extern float complex ctanf(float complex z);
extern long double complex ctanl(long double complex z);

// TODO: Documentation, Computes the complex arc sine
extern double complex casin(double complex z);
extern float complex casinf(float complex z);
extern long double complex casinl(long double complex z);

// TODO: Documentation, Computes the complex arc cosine
extern double complex cacos(double complex z);
extern float complex cacosf(float complex z);
extern long double complex cacosl(long double complex z);

// TODO: Documentation, Computes the complex arc tangent
extern double complex catan(double complex z);
extern float complex catanf(float complex z);
extern long double complex catanl(long double complex z);

// TODO: Documentation, Computes the complex hyperbolic sine
extern double complex csinh(double complex z);
extern float complex csinhf(float complex z);
extern long double complex csinhl(long double complex z);

// TODO: Documentation, Computes the complex hyperblic cosine
extern double complex ccosh(double complex z);
extern float complex ccoshf(float complex z);
extern long double complex ccoshl(long double complex z);

// TODO: Documentation, Computes the complex hyperbolic tangent
extern double complex ctanh(double complex z);
extern float complex ctanhf(float complex z);
extern long double complex ctanhl(long double complex z);

// TODO: Documentation, Computes the complex arc hyperbolic sine
extern double complex casinh(double complex z);
extern float complex casinhf(float complex z);
extern long double complex casinhl(long double complex z);

// TODO: Documentation, Computes the complex arc hyperbolic cosine
extern double complex cacosh(double complex z);
extern float complex cacoshf(float complex z);
extern long double complex cacoshl(long double complex z);

// TODO: Documentation, Computes the complex arc hyperbolic tangent
extern double complex catanh(double complex z);
extern float complex catanhf(float complex z);
extern long double complex catanhl(long double complex z);

#endif /* Complex numbers enabled */

#endif

/** @} */
