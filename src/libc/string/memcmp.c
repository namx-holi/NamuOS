/// @file memcmp.c

#include <string.h> // Implements

int memcmp(const void* lhs, const void* rhs, size_t count) {
	// NOTE: Undefined if lhs/rhs are null. Ignore that case.

	// Interpret both pointers as unsigner char*
	const unsigned char* a = (const unsigned char*)lhs;
	const unsigned char* b = (const unsigned char*)rhs;

	// Step through the two strings until we hit `count` or there's a
	//  differing character
	for (size_t i = 0; i < count; ++i) {
		if (a[i] < b[i])
			return -1; // lhs (a) appears before rhs (b) in lexicographical order
		else if (b[i] < a[i])
			return 1;  // rhs (b) appears before lhs (a) in lexicotraphical order
	}
	return 0; // No difference (or count was zero)
}
