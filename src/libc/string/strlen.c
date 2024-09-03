/// @file strlen.c

#include <string.h> // Implements

size_t strlen(const char* str) {
	// Count characters up to null term
	size_t len = 0;
	while (str[len] != '\0')
		len++;
	return len;
}

#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: strnlen_s
// size_t strnlen_s(const char* str, size_t strsz);
#endif
