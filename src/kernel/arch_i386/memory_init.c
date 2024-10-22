/// @file memory_init.c

#include <namuos/memory.h> // Implements

#include <arch_i386/bootmem.h>
#include <arch_i386/paging.h>


void memory_initialise(multiboot_info_t* mb_info) {
	// Set up the boot allocator
	bootmem_initialise(mb_info);

	// Set up the full linear mapping of kernel-space
	paging_initialise(mb_info);
}
