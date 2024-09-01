/**
 * @file math.h
 * @defgroup clib_math <math.h>
 * @brief todo
 * @ingroup clib
 * @{
*/

#ifndef _STDLIB_H
#define _STDLIB_H 1


#ifdef __cplusplus
extern "C" {
#endif

// The <math.h> header provides for the following constants. The values
//  are of type double and are accurate within the precision of the
//  double type. 
#define M_E        2.7182818284590452354   /* e */
#define M_LOG2E    1.4426950408889634074   /* log_2 e */
#define M_LOG10E   0.43429448190325182765  /* log_10 e */
#define M_LN2      0.69314718055994530942  /* log_e 2 */
#define M_LN10     2.30258509299404568402  /* log_e 10 */
#define M_PI       3.14159265358979323846  /* pi */
#define M_PI_2     1.57079632679489661923  /* pi/2 */
#define M_PI_4     0.78539816339744830962  /* pi/4 */
#define M_1_PI     0.31830988618379067154  /* 1/pi */
#define M_2_PI     0.63661977236758134308  /* 2/pi */
#define M_2_SQRTPI 1.12837916709551257390  /* 2/sqrt(pi) */
#define M_SQRT2    1.41421356237309504880  /* sqrt(2) */
#define M_SQRT1_2  0.70710678118654752440  /* 1/sqrt(2) */

// The header defines the following symbolic constants:
// TODO: MAXFLOAT
# define HUGE_VAL 1e10000

// The following are declared as functions and may also be defined as
//  macros. Function prototypes must be provided for use with an ISO C
//  compiler.
double acos(double);          // TODO: Implement math/acos
double asin(double);          // TODO: Implement math/asin
double atan(double);          // TODO: Implement math/atan
double atan2(double, double); // TODO: Implement math/atan2
double ceil(double);          // TODO: Implement math/ceil
double cos(double);           // TODO: Implement math/cos
double cosh(double);          // TODO: Implement math/cosh
double exp(double);           // TODO: Implement math/exp
double fabs(double);          // TODO: Implement math/fabs
double floor(double);
double fmod(double, double);  // TODO: Implement math/fmod
double frexp(double, int *);  // TODO: Implement math/frexp
double ldexp(double, int);    // TODO: Implement math/ldexp
double log(double);           // TODO: Implement math/log
double log10(double);         // TODO: Implement math/log10
double modf(double, double *); // TODO: Implement math/modf
double pow(double, double); // TODO: Implement math/pow
double sin(double); // TODO: Implement math/sin
double sinh(double); // TODO: Implement math/sinh
double sqrt(double); // TODO: Implement math/sqrt
double tan(double); // TODO: Implement math/tan
double tanh(double); // TODO: Implement math/tanh
double erf(double); // TODO: Implement math/erf
double erfc(double); // TODO: Implement math/erfc
double gamma(double); // TODO: Implement math/gamma
double hypot(double, double); // TODO: Implement math/hypot
double j0(double); // TODO: Implement math/j0
double j1(double); // TODO: Implement math/j1
double jn(int, double); // TODO: Implement math/jn
double lgamma(double); // TODO: Implement math/lgamma
double y0(double); // TODO: Implement math/y0
double y1(double); // TODO: Implement math/y1
double yn(int, double); // TODO: Implement math/yn
int    isnan(double); // TODO: Implement math/isnan
double acosh(double); // TODO: Implement math/acosh
double asinh(double); // TODO: Implement math/asinh
double atanh(double); // TODO: Implement math/atanh
double cbrt(double); // TODO: Implement math/cbrt
double expm1(double); // TODO: Implement math/expm1
int    ilogb(double); // TODO: Implement math/ilogb
double log1p(double); // TODO: Implement math/log1p
double logb(double); // TODO: Implement math/logb
double nextafter(double, double); // TODO: Implement math/nextafter
double remainder(double, double); // TODO: Implement math/remainder
double rint(double); // TODO: Implement math/rint
double scalb(double, double); // TODO: Implement math/scalb

// The following external variable is defined:
extern int signgam; // TODO: Implement math/signgam


#ifdef __cplusplus
}
#endif

#endif

/** @} */
