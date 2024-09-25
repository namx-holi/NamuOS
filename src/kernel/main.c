/// @file main.c

#include <stdint.h>
#include <stdlib.h> // abort

#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/system.h>


static uintptr_t esp; // Stack pointer


void kernel_main(multiboot_info_t* mb_info, uint32_t magic, uintptr_t mb_esp) {
	esp = mb_esp; // Save the stack pointer

	// Set up kprintf for VGA output
	if (kernel_setup_kprintf(mb_info) < 0) abort();

	// Check if we have the correct multiboot header magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) panic("Invalid bootloader header magic number\n");

	// kprintf("esp is 0x%p\n", esp);

	kprintf("mem_lower = %d KiB\n", mb_info->mem_lower);
	kprintf("mem_upper = %d KiB\n", mb_info->mem_upper);

	memory_paging_setup();
	kprintf("Paging successfully initialised!\n");
	memory_node_setup(mb_info);
	kprintf("Memory nodes successfully initialised!\n");

	kprintf("Finished running kernel_main, aborting...\n");
	abort();
}
