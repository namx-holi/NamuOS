/// @file memx.c

#include <string.h> // Implements


void* memchr(const void* ptr, int ch, size_t count) {
	// Interpret object as unsigned char*, and `ch` as unsigned char
	const unsigned char* p = (unsigned char*)ptr;
	const unsigned char  c = (unsigned char)ch;

	// Search up to `count` bytes for `c`. If found, return address
	for (size_t i = 0; i < count; ++i) {
		if (p[i] == c)
			return (void*)ptr + i; // Found! Return address
	}
	
	// Could not find `ch`, return NULL
	return NULL;
}

int memcmp(const void* lhs, const void* rhs, size_t count) {
	// Interpret both pointers as unsigned char*
	const unsigned char* a = (const unsigned char*)lhs;
	const unsigned char* b = (const unsigned char*)rhs;

	// Step through the two strings until we hit `count`, or there's a differing
	//  character
	for (size_t i = 0; i < count; ++i) {
		if (a[i] < b[i])
			return -1; // lhs (a) appears before rhs (b) in lexicographical order
		else if (b[i] < a[i])
			return 1; // rhs (b) appears before lhs (a) in lexicographical order
	}
	return 0; // No difference (or count was zero)
}

void* memset(void* dest, int ch, size_t count) {
	// Interpret object as unsigned char*
	unsigned char* d = (unsigned char*)dest;

	// Copy `ch` as an unsigned char into each index of `dest` up to `count`
	for (size_t i = 0; i < count; ++i)
		d[i] = (unsigned char)ch;
	
	// Return a copy of `dest`
	return dest;
}

// TODO: Implement memset_explicit. Not sure how to guarentee and make safe.
// void* memset_explicit(void* dest, int ch, size_t count);

void* memcpy(void* restrict dest, const void* restrict src, size_t count) {
	// Interpret both objects as unsigned char*
	unsigned char* d = (unsigned char*)dest;
	const unsigned char* s = (unsigned char*)src;

	// Copy each char up to `count`
	for (size_t i = 0; i < count; ++i)
		d[i] = s[i];
	
	// Return a copy of `dest`
	return dest;
}

void* memmove(void* dest, const void* src, size_t count) {
	// If pointers are the same, do nothing and just return dest
	if (dest == src)
		return dest;

	// If destination starts before source, we can fall back to memcpy
	if (dest < src)
		return memcpy(dest, src, count);
	
	// Otherwise, we need to copy backwards so we don't overlap in copy before
	//  we actually get to the bytes we need to copy. Interpret both objects as
	//  unsigned char*, and work backwards through source.
	unsigned char* d = (unsigned char*)dest;
	const unsigned char* s = (unsigned char*)src;
	for (size_t i = count; i != 0; --i)
		d[i-1] = s[i-1];
	
	// Return a copy of `dest`
	return dest;
}

void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count) {
	// TODO: Test this using the example in docstring for method

	// Interpret both objects as unsigned char*
	unsigned char* d = (unsigned char*)dest;
	const unsigned char* s = (unsigned char*)src;

	// Copy each char up to `count`, or until we find the char `c`
	unsigned char terminal = (unsigned char)c;
	for (size_t i = 0; i < count; ++i) {
		if (s[i] == terminal) {
			// Return pointer to end of written data in dest
			return &d[i];
		}

		d[i] = s[i];
	}

	// Return NULL if terminal character not found
	return NULL;
}

#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
errno_t memset_s(void* dest, rsize_t destsz, int ch, rsize_t count);
errno_t memcpy_s(void* restrict dest, rsize_t destsz, const void* restrict src, rsize_t count);
errno_t memmove_s(void* dest, rsize_t destsz, const void* src, rsize_t count);
#endif
