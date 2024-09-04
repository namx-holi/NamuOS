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
#include <stdlib.h>

// Needed for kernel_main
#include <assert.h>
#include <stdio.h>
#include <kernel/tty.h>


void kernel_main(void) {
	terminal_initialise();

	// Testing printing
	printf("Hello, kernel world!\n");
	printf("Printing number 1234 using format: %d\n", 1234);
	printf("Printing number -1234 using format: %d\n", -1234);
	int len = printf("Testing counting line length...\n");
	printf("Length of last line was %d (should be 33)\n", len);
	printf("\tTesting indentation (should be indented by 4 characters)\n");
	printf("Testing vertical tab, \vshould be under the last line\n");
	printf("******* testing carriage return, should be no *s\rTesting\n");

	// Testing tolower
	printf("\nTesting tolower, printing alphabet:\n");
	for (unsigned char u = 0; u < UCHAR_MAX; ++u) {
		unsigned char l = tolower(u);
		if (l != u) printf("%c%c ", u, l);
	}
	printf("\n\n");

	// Testing div
	div_t d = div(20, 7);
	printf("Testing div(), 20 / 7 is %d remainder %d\n\n", d.quot, d.rem);

	// Testing asserts
	printf("Testing assertions:\n");
	assert(0==1);
}
