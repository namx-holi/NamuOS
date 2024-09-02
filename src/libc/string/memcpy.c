/// @file memcpy.c

#include <string.h> // Implements

void* memcpy(void* restrict dest, const void* restrict src, size_t count) {
	// Interpret both objects as unsigned char*
	unsigned char* d = (unsigned char*)dest;
	const unsigned char* s = (unsigned char*)src;

	// Copy each char up to `count`!
	for (size_t i = 0; i < count; ++i) {
		d[i] = s[i];
	}

	// Returns a copy of `dest`
	return dest;
}

// TODO: memcpy_s
// errno_t memcpy_s(void* restrict dest, rsize_t destsz, const void* restrict src, rsize_t count);
