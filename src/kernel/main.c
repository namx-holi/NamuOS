/// @file main.c

#include <stddef.h>
#include <stdint.h>

#include <namuos/boot_allocator.h>
#include <namuos/multiboot.h>
#include <namuos/paging.h>
#include <namuos/panic.h>
#include <namuos/terminal.h>


void kernel_main(multiboot_info_t* mb_info, uint32_t magic, uintptr_t mb_esp) {
	(void)mb_esp; // TODO: Set up stack properly

	// Fix any addresses in (and to) mb_info
	multiboot_fix_addresses(&mb_info);

	// Any setup needed for terminal display
	if (!terminal_initialise(mb_info)) panic(NULL);

	// Check if we have the correct multiboot header magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		panic("Invalid bootloader header magic number\n");
	
	// Set up boot allocator and paging
	bootmem_initialise(mb_info);
	paging_initialise();
	klog_info("bootmem allocator and paging initialised!\n");

	panic("Finished running kernel_main, aborting...\n");
}
