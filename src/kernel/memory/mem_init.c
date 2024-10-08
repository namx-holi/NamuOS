/// @file mem_init.c

#include <namuos/memory.h> // Implements


void memory_initialise(multiboot_info_t* mb_info) {
	// Set up paging
	memory_paging_initialise(mb_info);

	// Set up memory map
	memory_memmap_initialise(mb_info);
}
