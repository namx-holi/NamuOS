/// @file inttypes.c

#include <inttypes.h> // Implements


imaxdiv_t imaxdiv(intmax_t x, intmax_t y) {
	imaxdiv_t val = {x / y, x % y};
	return val;
}
