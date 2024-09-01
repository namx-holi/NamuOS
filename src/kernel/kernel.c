/// @file kernel.c


// DEBUG: Testing imports. These should work without our own clib
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
// #include <stdckdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <tgmath.h>

// DEBUG: Testing imports. These are what we are currently implementing
#include <locale.h>
#include <ctype.h>


// Needed for kernel_main
#include <assert.h>
#include <stdio.h>
#include <kernel/tty.h>


void kernel_main(void) {
	terminal_initialize();

	// Testing printing
	printf("Hello, kernel World!\n");
	printf("test val is %d\n", 1234);
	int len = printf("what about negatives? %d\n", -4321);
	printf("length of last line? %d\n", len);
	printf("\n\n");

	// Testing tolower
	for (unsigned char u = 0; u < UCHAR_MAX; u++) {
		unsigned char l = tolower(u);
		if (l != u) printf("%c%c ", u, l);
	}
	printf("\n\n");

	// Testing asserts
	assert(0==1);
}
