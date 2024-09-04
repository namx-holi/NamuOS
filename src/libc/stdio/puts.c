/// @file puts.c

#include <stdio.h> // Implements


int puts(const char* string) {
	return printf("%s\n", string);
}
