/// @file assert.c

#include <assert.h> // Implements

#include <stdio.h> // printf
#include <stdlib.h> // abort


void _assert(const char* condition, const char* file, const char* func, int line) {
	printf("Assertion failed: %s, file %s, line %d in %s\n", condition, file, line, func);
	abort();
}
