/// @file inttypes.c

#include <inttypes.h> // Implements


// TODO: Implement imaxabs
// intmax_t imaxabs(intmax_t n);

imaxdiv_t imaxdiv(intmax_t x, intmax_t y) {
	imaxdiv_t val = {x/y, x%y};
	return val;
}

// TODO: Implement strtoimax
// intmax_t strtoimax(const char* restrict nptr, char** restrict endptr, int base);

// TODO: Implement strtoumax
// uintmax_t strtoumax(const char* restrict nptr, char** restrict endptr, int base);
