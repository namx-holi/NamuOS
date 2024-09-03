/// @file memset.c

#include <string.h> // Implements

void* memset(void* dest, int ch, size_t count) {
	// Interpret object as unsigned char*
	unsigned char* d = (unsigned char*)dest;

	// Copy ch as unsigned char into each index of dest, up to count
	for (size_t i = 0; i < count; ++i)
		d[i] = (unsigned char)ch;
	
	// Return copy of `dest`
	return dest;
}

// TODO: memset_explicit
// void* memset_explicit(void* dest, int ch, size_t count);

#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: memset_s
// errno_t memset_s(void* dest, rsize_t destsz, int ch, rsize_t count);
#endif
