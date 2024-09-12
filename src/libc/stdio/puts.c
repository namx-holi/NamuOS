/// @file puts.c

#include <stdio.h> // Implements

#include <string.h> // strlen

#if defined(__is_libk)
// #include <kernel/tty.h> // terminal_write
#endif


int puts(const char* string) {
	#if defined(__is_libk)
	// TODO: Replace this
	// terminal_writestring(string);
	#else
	// TODO: Implement write system call
	#endif

	// TODO: Handle this better
	return strlen(string);
}
