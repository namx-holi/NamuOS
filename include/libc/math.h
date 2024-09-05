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


// TODO: Documentation, Indicates value too big to be representable (infinity) by `double`
#define HUGE_VAL /* implementation defined */
// TODO: Define HUGE_VAL

// TODO: Documentation, Indicates value too big to be representable (infinity) by `float`
#define HUGE_VALF /* implementation defined */
// TODO: Define HUGE_VALF

// TODO: Documentation, Indicates value too big to be representable (infinity) by `long double`
#define HUGE_VALL /* implementation defined */
// TODO: Define HUGE_VALL

// TODO: Documentation, Evaluates to positive infinity or the value guaranteed to overflow a `float`
#define INFINITY /* implementation defined */
// TODO: Define INFINITY

// TODO: Documentation, Evaluates to a quiet NaN of type `float`
#define NAN /* implementation defined */
// TODO: Define NAN

// TODO: Documentation
// Ref: https://en.cppreference.com/w/c/numeric/math/math_errhandling
#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling /* implementation defined */

// TODO: Documentation, Indicates a floating-point category
#define FP_NORMAL /* implementation defined */
#define FP_SUBNORMAL /* implementation defined */
#define FP_ZERO /* implementation defined */
#define FP_INFINITE /* implementation defined */
#define FP_NAN /* implementation defined */
// TODO: Define FP_NORMAL through FP_NAN
// TODO: Individual docs per macro





/** @brief Most efficient floating-point type at least as wide as `float` or `double`
 * 
 * The types `float_t` and @ref double_t are floating types at least as wide as
 * `float` and `double`, respectively, and such that @ref double_t is at least
 * as wide as `float_t`. The value of @ref FLT_EVAL_METHOD determines the types
 * of `float_t` and `double_t`.
 * 
 * @see [FLT_EVAL_METHOD](https://en.cppreference.com/w/c/types/limits/FLT_EVAL_METHOD),
 * use of excended precision for intermediate results: 0 not used, 1 `double` is
 * used instead of `float`, 2: `long double` is used.
*/
#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
#error No implementation for float_t and double_t, see FLT_EVAL_METHOD in <float.h>
#endif







// TODO: Documentation, Computes absolute value of a floating-point value (|x|)
extern double fabs(double arg);
extern float fabsf(float arg);
extern long double fabsl(long double arg);
// extern _Decimal32 fabsd32(_Decimal32 arg);
// extern _Decimal64 fabsd64(_Decimal64 arg);
// extern _Decimal128 fabsd128(_Decimal128 arg);

// TODO: Documentation, Computes remainder of the floating-point division operation
extern double fmod(double x, double y);
extern float fmodf(float x, float y);
extern long double fmodl(long double x, long double y);

// TODO: Documentation, Computes signed remainder of the floating-point division operation
extern double remainder(double x, double y);
extern float remainderf(float x, float y);
extern long double remainderl(long double x, long double y);

// TODO: Documentation, Computes signed remainder as well as the three last bits of the division operation
extern double remquo(double x, double y, int* quo);
extern float remquof(float x, float y, int* quo);
extern long double remquol(long double x, long double y, int* quo);

// TODO: Documentation, Indicates that the @ref fma function generally executes about as fast as, or faster than, a multiply and an add of double operands
#define FP_FAST_FMA 0 // TODO: Check this is right
#define FP_FAST_FMAF 0 // TODO: Check this is right
#define FP_FAST_FMAL 0 // TODO: Check this is right

// TODO: Documentation, Computes fused multiply-add operation
extern double fma(double x, double y, double z);
extern float fmaf(float x, float y, float z);
extern long double fmal(long double x, long double y, long double z);

// TODO: Documentation, Determines larger of two floating-point values
extern double fmax(double x, double y);
extern float fmaxf(float x, float y);
extern long double fmaxl(long double x, long double y);

// TODO: Documentation, Determines smaller of two floating-point values
extern double fmin(double x, double y);
extern float fminf(float x, float y);
extern long double fminl(long double x, long double y);

// TODO: Documentation, Determines positive difference of two floating-point values (max(0, x-y))
extern double fdim(double x, double y);
extern float fdimf(float x, float y);
extern long double fdiml(long double x, long double y);

// TODO: Documentation, Returns a NaN (not-a-number)
extern double nan(const char* arg);
extern float nanf(const char* arg);
extern long double nanl(const char* arg);
// extern _Decimal32 nand32(const char* arg);
// extern _Decimal64 nand64(const char* arg);
// extern _Decimal128 nand128(const char* arg);

// TODO: Documentation, Computes e raised to the given power (e^x)
extern double exp(double arg);
extern float expf(float arg);
extern long double expl(long double arg);

// TODO: Documentation, Computes 2 raised to the given power (2^x)
extern double exp2(double n);
extern float exp2f(float n);
extern long double exp2l(long double n);

// TODO: Documentation, Computes e raised to the given power, minus one (e^x - 1)
extern double expm1(double arg);
extern float expm1f(float arg);
extern long double expm1l(long double arg);

// TODO: Documentation, Computes natural (base-e) logarithm (ln x)
extern double log(double arg);
extern float logf(float arg);
extern long double logl(long double arg);

// TODO: Documentation, Computes common (base-10) logarithm (log_10 x)
extern double log10(double arg);
extern float log10f(float arg);
extern long double log10l(long double arg);

// TODO: Documentation, Computes base-2 logarithm (log_2 x)
extern double log2(double arg);
extern float log2f(float arg);
extern long double log2l(long double arg);

// TODO: Documentation, Computes natural (base-e) logarithm of 1 plus the given number (ln(1 + x))
extern double log1p(double arg);
extern float log1pf(float arg);
extern long double log1pl(long double arg);

// TODO: Documentation, Computes a number raised to the given power (x^y)
extern double pow(double base, double exponent);
extern float powf(float base, float exponent);
extern long double powl(long double base, long double exponent);

// TODO: Documentation, Computes square root (sqrt(x))
extern double sqrt(double arg);
extern float sqrtf(float arg);
extern long double sqrtl(long double arg);

// TODO: Documentation, Computes cube root (cbrt(x))
extern double cbrt(double arg);
extern float cbrtf(float arg);
extern long double cbrtl(long double arg);

// TODO: Documentation, Computes square root of the sum of the squares of two given numbers (sqrt(x^2 + y^2))
extern double hypot(double x, double y);
extern float hypotf(float x, float y);
extern long double hypotl(long double x, long double y);

// TODO: Documentation, Computes sine (sin x)
extern double sin(double arg);
extern float sinf(float arg);
extern long double sinl(long double arg);
// extern _Decimal32 sind32(_Decimal32 arg);
// extern _Decimal64 sind64(_Decimal64 arg);
// extern _Decimal128 sind128(_Decimal128 arg);

// TODO: Documentation, Computes cosine (sin x)
extern double cos(double arg);
extern float cosf(float arg);
extern long double cosl(long double arg);
// extern _Decimal32 cosd32(_Decimal32 arg);
// extern _Decimal64 cosd64(_Decimal64 arg);
// extern _Decimal128 cosd128(_Decimal128 arg);

// TODO: Documentation, Computes tangent (tan x)
extern double tan(double arg);
extern float tanf(float arg);
extern long double tanl(long double arg);
// extern _Decimal32 tand32(_Decimal32 arg);
// extern _Decimal64 tand64(_Decimal64 arg);
// extern _Decimal128 tand128(_Decimal128 arg);

// TODO: Documentation, Computes arc sine (arcsin x)
extern double asin(double arg);
extern float asinf(float arg);
extern long double asinl(long double arg);
// extern _Decimal32 asind32(_Decimal32 arg);
// extern _Decimal64 asind64(_Decimal64 arg);
// extern _Decimal128 asind128(_Decimal128 arg);

// TODO: Documentation, Computes arc cosine (arccos x)
extern double acos(double arg);
extern float acosf(float arg);
extern long double acosl(long double arg);
// extern _Decimal32 acosd32(_Decimal32 arg);
// extern _Decimal64 acosd64(_Decimal64 arg);
// extern _Decimal128 acosd128(_Decimal128 arg);

// TODO: Documentation, Computes arc tangent (arctan x)
extern double atan(double arg);
extern float atanf(float arg);
extern long double atanl(long double arg);
// extern _Decimal32 atand32(_Decimal32 arg);
// extern _Decimal64 atand64(_Decimal64 arg);
// extern _Decimal128 atand128(_Decimal128 arg);

// TODO: Documentation, Computes arc tangent, using signs to determine quadrants
extern double atan2(double y, double x);
extern float atan2f(float y, float x);
extern long double atan2l(long double y, long double x);
// extern _Decimal32 atan2d32(_Decimal32 y, _Decimal32 x);
// extern _Decimal64 atan2d64(_Decimal64 y, _Decimal64 x);
// extern _Decimal128 atan2d128(_Decimal128 y, _Decimal128 x);

// TODO: Documentation, Computes hyperbolic sine (sinh x)
extern double sinh(double arg);
extern float sinhf(float arg);
extern long double sinhl(long double arg);

// TODO: Documentation, Computes hyperbolic cosine (cosh x)
extern double cosh(double arg);
extern float coshf(float arg);
extern long double coshl(long double arg);

// TODO: Documentation, Computes hyperbolic tangent (tanh x)
extern double tanh(double arg);
extern float tanhf(float arg);
extern long double tanhl(long double arg);

// TODO: Documentation, Computes inverse hyperbolic sine (arcsinh x)
extern double asinh(double arg);
extern float asinhf(float arg);
extern long double asinhl(long double arg);

// TODO: Documentation, Computes inverse hyperbolic cosine (arccosh x)
extern double acosh(double arg);
extern float acoshf(float arg);
extern long double acoshl(long double arg);

// TODO: Documentation, Computes inverse hyperbolic tangent (arctanh x)
extern double atanh(double arg);
extern float atanhf(float arg);
extern long double atanhl(long double arg);

// TODO: Documentation, Computes error function
extern double erf(double arg);
extern float erff(float arg);
extern long double erfl(long double arg);

// TODO: Documentation, Computes complementary error function
extern double erfc(double arg);
extern float erfcf(float arg);
extern long double erfcl(long double arg);

// TODO: Documentation, Computes gamma function
extern double tgamma(double arg);
extern float tgammaf(float arg);
extern long double tgammal(long double arg);

// TODO: Documentation, Computes natural (base-e) logarithm of the gamma function
extern double lgamma(double arg);
extern float lgammaf(float arg);
extern long double lgammal(long double arg);

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

// TODO: Documentation, Rounds to nearest integer not greater in magnitude than the given value
extern double trunc(double arg);
extern float truncf(float arg);
extern long double truncl(long double arg);

// TODO: Documentation, Rounds to the nearest integer, rounding away from zero in halfway cases
extern double round(double arg);
extern float roundf(float arg);
extern long double roundl(long double arg);
extern long lround(double arg);
extern long lroundf(float arg);
extern long lroundl(long double arg);
extern long long llround(double arg);
extern long long llroundf(float arg);
extern long long llroundl(long double arg);

// TODO: Documentation, Rounds to an integer using current rounding mode
extern double nearbyint(double arg);
extern float nearbyintf(float arg);
extern long double nearbyintl(long double arg);

// TODO: Documentation, Rounds to an integer using current rounding mode with exception if the result differs
extern double rint(double arg);
extern float rintf(float arg);
extern long double rintl(long double arg);
extern long lrint(double arg);
extern long lrintf(float arg);
extern long lrintl(long double arg);
extern long long llrint(double arg);
extern long long llrintf(float arg);
extern long long llrintl(long double arg);

// TODO: Documentation, Breaks a number into significand and a power of 2
extern double frexp(double arg, int* exp);
extern float frexpf(float arg, int* exp);
extern long double frexpl(long double arg, int* exp);

// TODO: Documentation, Multiplies a number by 2 raised to a power
extern double ldexp(double arg, int exp);
extern float ldexpf(float arg, int exp);
extern long double ldexpl(long double arg, int exp);

// TODO: Documentation, Breaks a number into integer and fractional parts
extern double modf(double arg, double* iptr);
extern float modff(float arg, float* iptr);
extern long double modfl(long double arg, long double* iptr);

// TODO: Documentation, Computes efficiently a number times @ref FLT_RADIX raised to a power
extern double scalbn(double arg, int exp);
extern float scalbnf(float arg, int exp);
extern long double scalbnl(long double arg, int exp);
extern double scalbln(double arg, long exp);
extern float scalblnf(float arg, long exp);
extern long double scalblnl(long double arg, long exp);

// TODO: Documentation, Evaluates to `llogb(x)` if x is zero or NaN, respectively
// TODO: Definitions for FP_ILOGB0 and FP_ILOGBNAN
#define FP_ILOGB0 /* implementation defined */
#define FP_ILOGBNAN /* implementation defined */

// TODO: Documentation, Extracts exponent of the given number
extern int ilogb(double arg);
extern int ilogbf(float arg);
extern int ilogbl(long double arg);

// TODO: Documentation, Extracts exponent of the given number
extern double logb(double arg);
extern float logbf(float arg);
extern long double logbl(long double arg);

// TODO: Documentation, Determines next representable floating-point value towards the given value
extern double nextafter(double from, double to);
extern float nextafterf(float from, float to);
extern long double nextafterl(long double from, long double to);
extern double nexttoward(double from, long double to);
extern float nexttowardf(float from, long double to);
extern long double nexttowardl(long double from, long double to);

// TODO: Documentation, Produces a value with the magnitude of a given value and the sign of another given value
extern double copysign(double x, double y);
extern float copysignf(float x, float y);
extern long double copysignl(long double x, long double y);

// TODO: Documentation, Classifies the given floating-point value
#define fpclassify(arg) /* implementation defined */
// TODO: Define fpclassify

// TODO: Documentation, Checks if the given number has finite value
#define isfinite(arg) /* implementation defined */
// TODO: Define isfinite

// TODO: Documentation, Checks if the given number is infinite
#define isinf(arg) /* implementation defined */
// TODO: Define isinf

// TODO: Documentation, Checks if the given number is NaN
#define isnan(arg) /* implementation defined */
// TODO: Define isnan

// TODO: Documentation, Checks if the given number is normal
#define isnormal(arg) /* implementation defined */
// TODO: Define isnormal

// TODO: Documentation, Checks if the given number is negative
#define signbit(arg) /* implementation defined */
// TODO: Define signbit

// TODO: Documentation, Checks if the first floating-point argument is greater than the second
#define isgreater(x, y) /* implementation defined */
// TODO: Define isgreater

// TODO: Documentation, Checks if the first floating-point argument is greater or equal than the second
#define isgreaterequal(x, y) /* implementation defined */
// TODO: Define isgreaterequal

// TODO: Documentation, Checks if the first floating-point argument is less than the second
#define isless(x, y) /* implementation defined */
// TODO: Define isless

// TODO: Documentation, Checks if the first floating-point argument is less or equal than the second
#define islessequal(x, y) /* implementation defined */
// TODO: Define islessequal

// TODO: Documentation, Checks if the first floating-point argument is less or greater than the second
#define islessgreater(x, y) /* implementation defined */
// TODO: Define islessgreater

// TODO: Documentation, Checks if two floating-point values are unordered
#define isunordered(x, y) /* implementation defined */
// TODO: Define isunordered

// TODO: Other methods present in other math.h libraries. Should these be
//  implemented?
// MAXFLOAT
// gamma
// j0, j1, jn
// y0, y1, yn
// scalb
// signgam

#endif

/** @} */
