/// @file xdiv.c

#include <stdlib.h> // Implements

div_t div(int x, int y) {
	div_t val = {x / y, x % y};
	return val;
}

ldiv_t ldiv(long x, long y) {
	ldiv_t val = {x / y, x % y};
	return val;
}

lldiv_t lldiv(long long x, long long y) {
	lldiv_t val = {x / y, x % y};
	return val;
}
