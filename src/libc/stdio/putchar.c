/// @file putchar.c

#include <stdio.h> // Implements

#if defined(__is_libk)
// #include <kernel/tty.h>
#endif


int putchar(int ic) {
	#if defined(__is_libk)
	// TODO: Replace this
	// char c = (char) ic;
	// terminal_write(&c, sizeof(c));
	#else
	// TODO: Implement stdio and the write system call.
	#endif
	return ic;
}
