/// @file main.c

#include <stdint.h>
#include <stdlib.h> // abort

#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/system.h>


// TODO: Why do we need this?
static uintptr_t esp; // Stack pointer


void kernel_main(multiboot_info_t* mb_info, uint32_t magic, uintptr_t mb_esp) {
	// Save the stack pointer
	esp = mb_esp;

	// Set up VGA output for kprintf
	if (kernel_setup_kprintf(mb_info) < 0) abort();

	// Check if we have the correct multiboot header magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) panic("Invalid bootloader header magic number\n");

	// Set up memory management
	memory_paging_setup();
	memory_memmap_setup(mb_info);

	klog_warning("Finished running kernel_main, aborting...\n");
	abort();
}
