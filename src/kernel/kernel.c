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

	// // Testing fetching BIOS info
	// uint16_t e = fetch_bios_area_data(BDA_DETECTED_HARDWARE);
	// printf("IPL diskette installed? %d\n", (e & 0x0001) >> 0);
	// printf("Math coprocessor? %d\n", (e & 0x0002) >> 1);
	// printf("Pointing device installed (PS/2)? %d\n", (e & 0x0004) >> 2);
	// printf("Not used on PS/2? %d\n", (e & 0x0008) >> 3);
	// printf("Initial video mode? %d\n", (e & 0x0030) >> 4);
	// printf("# of diskette drives, less 1: %d\n", (e & 0x00c0) >> 6);
	// printf("0 if DMA installed: %d\n", (e & 0x0100) >> 8);
	// printf("Number of serial ports: %d\n", (e & 0x0e00) >> 9);
	// printf("Game adapter? %d\n", (e & 0x1000) >> 12);
	// printf("Internal modem (PS/2)? %d\n", (e & 0x2000) >> 13);
	// printf("Number of printer ports: %d\n", (e & 0xc000) >> 14);
	// printf("\n");

	// Testing printing
	printf("Hello, kernel world!\n");
	printf("Printing number 1234 using format: %d\n", 1234);
	printf("Printing number -1234 using format: %d\n", -1234);
	int len = printf("Testing counting line length...\n");
	printf("Length of last line was %d (should be 32)\n", len);
	printf("\tTesting indentation (should be indented by 4 characters)\n");
	printf("Testing tab alignment:\t1\t12\t123\t1234\t12341\t1234\n");
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
