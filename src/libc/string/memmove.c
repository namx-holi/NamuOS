/// @file memmove.c

#include <string.h> // Implements

void* memmove(void* dest, const void* src, size_t count) {
	// If pointers are the same, do nothing and just return dest
	if (dest == src)
		return dest;

	// Interpret both objects as unsigned char*
	unsigned char* d = (unsigned char*)dest;
	const unsigned char* s = (const unsigned char*)src;

	// TODO: Fall back to memcpy if no overlap

	// If destination starts before source, copy forward to not overlap
	if (d < s) {
		for (size_t i = 0; i < count; ++i)
			d[i] = s[i];
	}

	// If destination starts after source, copy backwards to not overlap
	else {
		for (size_t i = count; i != 0; --i)
			d[i-1] = s[i-1];
	}

	// Return copy of `dest`
	return dest;
}

// TODO: memmove_s
// errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count);
