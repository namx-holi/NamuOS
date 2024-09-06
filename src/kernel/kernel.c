/// @file kernel.c

#include <assert.h>
#include <stdio.h>

#include <kernel/bios.h>
#include <kernel/bios_defines.h>
#include <kernel/tty.h>


void kernel_main(void) {
	terminal_initialise();

	bios_log_equipment();

	uint16_t memsize = *BDA_MEMORY_SIZE_KB;
	printf("Memory size in KB: %d\n", memsize);

	// Testing asserts
	printf("\nTesting assertions:\n");
	assert(0==1);
}
