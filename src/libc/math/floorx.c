/// @file floorx.c

#include <math.h> // Implements

#include <stdint.h>


// TODO: Rewrite this a bit, and add floorf and floorl

// Ref: https://stackoverflow.com/a/64409333/9330968

// INTMAX_MAX is not exact as a double, yet INTMAX_MAX + 1 is an exact
//  double
#define INTMAX_MAX_P1 ((INTMAX_MAX/2 + 1)*2.0)

double floor(double x) {
	if (x >= 0.0) {
		if (x < INTMAX_MAX_P1) {
			return (double)(intmax_t) x;
		}
		return x;
	}

	else if (x < 0.0) {
		if (x >= INTMAX_MIN) {
			intmax_t ix = (intmax_t) x;
			return (ix == x) ? x : (double)(ix - 1);
		}
		return x;
	}

	return x; // NaN
}

// TODO: floorf
// TODO: floorl
