/**
 * @file math.h
 * @defgroup libc_math <math.h>
 * @brief Common mathematics functions
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/numeric/math)
 * for **Common mathematical functions**
 * 
 * @{
*/

#ifndef _LIBC_MATH_H
#define _LIBC_MATH_H 1


/*****************************************************/
/* Common Mathematical Functions                     */
/* Ref: https://en.cppreference.com/w/c/numeric/math */
/*****************************************************/

// The <math.h> header provides for the following constants. The values
//  are of type double and are accurate within the precision of the
//  double type.
// TODO: Document with doxygen
#define M_E        2.7182818284590452354   ///< e
#define M_LOG2E    1.4426950408889634074   ///< log_2 e
#define M_LOG10E   0.43429448190325182765  ///< log_10 e
#define M_LN2      0.69314718055994530942  ///< log_e 2
#define M_LN10     2.30258509299404568402  ///< log_e 10
#define M_PI       3.14159265358979323846  ///< pi
#define M_PI_2     1.57079632679489661923  ///< pi/2
#define M_PI_4     0.78539816339744830962  ///< pi/4
#define M_1_PI     0.31830988618379067154  ///< 1/pi
#define M_2_PI     0.63661977236758134308  ///< 2/pi
#define M_2_SQRTPI 1.12837916709551257390  ///< 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880  ///< sqrt(2)
#define M_SQRT1_2  0.70710678118654752440  ///< 1/sqrt(2)

// TODO: fabs, fabsf, fabsl
// TODO: fmod, fmodf, fmodl
// TODO: remainder, remainderf, remainderl
// TODO: remquo, remquof, remquol
// TODO: fma, fmaf, fmal
// TODO: fmax, fmaxf, fmaxl
// TODO: fmin, fminf, fminl
// TODO: fdim, fdimf, fdiml
// TODO: nan, nanf, nanl
// TODO: exp, expf, expl
// TODO: exp2, exp2f, exp2l
// TODO: expm1, expm1f, expm1l
// TODO: log, logf, logl
// TODO: log10, log10f, log10l
// TODO: log2, log2f, log2l
// TODO: log1p, log1pf, log1pl
// TODO: pow, powf, powl
// TODO: sqrt, sqrtf, sqrtl
// TODO: cbrt, cbrtf, cbrtl
// TODO: hypot, hypotf, hypotl
// TODO: sin, sinf, sinl
// TODO: cos, cosf, cosl
// TODO: tan, tanf, tanl
// TODO: asin, asinf, asinl
// TODO: acos, acosf, acosl
// TODO: atan, atanf, atanl
// TODO: atan2, atan2f, atan2l
// TODO: sinh, sinhf, sinhl
// TODO: cosh, coshf, coshl
// TODO: tanh, tanhf, tanhl
// TODO: asinh, asinhf, asinhl
// TODO: acosh, acoshf, acoshl
// TODO: atanh, atanhf, atanhl
// TODO: erf, erff, erfl
// TODO: erfc, erfcf, erfcl
// TODO: tgamma, tgammaf, tgammal
// TODO: lgamma, lgammaf, lgammal

/** @brief Computes smallest integer not less than the given value
 * 
 * Computes the smallest integer value not less than `arg`.
 * 
 * Errors are reported as specified in @ref math_errhandling. If the
 * implementation supports IEEE floating-point arithmetic (IEC 60559):
 * - The current [rounding mode](https://en.cppreference.com/w/c/numeric/fenv/FE_round) has no effect.
 * - If `arg` is ±∞, it is returned, unmodified.
 * - If `arg` is ±0, it is returned, unmodified.
 * - If `arg` is NaN, NaN is returned.
 * 
 * @param arg Floating-point value
 * 
 * @returns If no errors occur, the smallest integer value not less than `arg`,
 * that is \f$\lceil arg \rceil\f$, is returned.
 * 
 * @note @ref FE_INEXACT may be (but isn't required to be) raised when rounding
 * a non-integer finite value.
 * @note THe largest representable floating-point values are exact integers in
 * all standard floating-point formats, so this function never overflows on its
 * own; however the result may overflow any integer type (including
 * @ref intmax_t), when stored in an integer variable.
 * @note This function (for `double` argument) behaves as if (except for the
 * freedom to not raise @ref FE_INEXACT) implemented by
 * ```c
 * #include <fenv.h>
 * #include <math.h>
 * #pragma STDC FENV_ACCESS ON
 * 
 * double ceil(double x) {
 *   double result;
 *   int save_round = fegetround();
 *   fesetround(FE_UPWARD);
 *   result = rint(x); // or nearbyint
 *   fesetround(save_round);
 *   return result;
 * }
 * ```
 * 
 * @see @ref floor, @ref floorf, @ref floorl, @copybrief floor
 * @see @ref trunc, @ref truncf, @ref truncl, @copybrief trunc
 * @see @ref round and variants, @copybrief round
 * @see @ref nearbyint, @ref nearbyintf, @ref nearbyintl, @copybrief nearbyint
 * @see @ref rint and variants, @copybrief rint
*/
extern double ceil(double arg);
/** @copydoc ceil */
extern float ceilf(float arg);
/** @copydoc ceil */
extern long double ceill(long double arg);

/** @brief Computes largest integer not greater than the given value
 * 
 * Computes the largest integer value not greater than `arg`.
 * 
 * Errors are reported as specified in @ref math_errhandling. If the
 * implementation supports IEEE floating-point arithmetic (IEC 60559):
 * - The current [rounding mode](https://en.cppreference.com/w/c/numeric/fenv/FE_round) has no effect.
 * - If `arg` is ±∞, it is returned, unmodified.
 * - If `arg` is ±0, it is returned, unmodified.
 * - If `arg` is NaN, NaN is returned.
 * 
 * @param arg Floating-point value
 * 
 * @returns If no errors occur, the largest integer value not greater than
 * `arg`, that is \f$\lfloor arg \rfloor\f$, is returned.
 * 
 * @note @ref FE_INEXACT may be (but isn't required to be) raised when rounding
 * a non-integer finite value.
 * @note The largest representable floating-point values are exact integers in
 * all standard floating-point formats, so this function never overflows on its
 * own; however the result may overflow any integer type (including
 * @ref intmax_t), when stored in an integer variable.
*/
extern double floor(double arg);
/** @copydoc floor */
extern float floorf(float arg);
/** @copydoc floor */
extern long double floorl(long double arg);

// TODO: trunc, truncf, trunfl
// TODO: round, roundf, roundl
// TODO: lround, lroundf, lroundl
// TODO: llround, llroundf, llroundl
// TODO: nearbyint, nearbyintf, nearbyintl
// TODO: rint, rintf, rintl
// TODO: lrint, lrintf, lrintl
// TODO: llrint, llrintf, llrintl
// TODO: frexp, frexpf, frexpl
// TODO: ldexp, ldexpf, ldexpl
// TODO: modf, modff, modfl
// TODO: scalbn, scalbnf, scalbnl
// TODO: scalbln, scalblnf, scalblnl
// TODO: ilogb, ilogbf, ilogbl
// TODO: logb, logbf, logbl
// TODO: nextafter, nextafterf nextafterl
// TODO: nexttoward, nexttowardf, nexttowardl
// TODO: copysign, copysignf, copysignl
// TODO: fpclassify
// TODO: isfinite
// TODO: isinf
// TODO: isnan
// TODO: isnormal
// TODO: signbit
// TODO: isgreater
// TODO: isgreaterequal
// TODO: isless
// TODO: islessequal
// TODO: islessgreater
// TODO: isunordered
// TODO: float_t
// TODO: double_t
// TODO: HUGE_VAL, HUGE_VALF, HUGEVALL (first could be 1e10000)
// TODO: INFINITY
// TODO: NAN
// TODO: FP_FAST_FMA, FP_FAST_FMAF, FP_FAST_FMAL
// TODO: FP_ILOGB0, FP_ILOGBNAN

// TODO: Documentation
// Ref: https://en.cppreference.com/w/c/numeric/math/math_errhandling
#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling /* implementation defined */

// TODO: FP_NORMAL
// TODO: FP_SUBNORMAL
// TODO: FP_ZERO
// TODO: FP_INFINITE
// TODO: FP_NAN

// TODO: MAXFLOAT?
// TODO: gamma?
// TODO: j0, j1, jn?
// TODO: y0, y1, yn?
// TODO: scalb?
// TODO: signgam?

#endif

/** @} */
