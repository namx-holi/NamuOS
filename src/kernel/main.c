/// @file main.c

#include <stdint.h>
#include <stdlib.h> // abort

#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/system.h>


static uintptr_t esp; // Stack pointer


extern void* code;
extern void* data;
extern void* bss;
extern void* end;


void kernel_main(multiboot_info_t* mb_info, uint32_t magic, uintptr_t mb_esp) {
	esp = mb_esp; // Save the stack pointer

	// Want to use kprintf
	if (kernel_setup_kprintf(mb_info) < 0) abort();

	// Check if we have the correct multiboot header magic
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) panic("Invalid bootloader header magic number\n");

	// kprintf("code pointer is 0x%p\n", &code);
	// kprintf("data pointer is 0x%p\n", &data);
	// kprintf("bss pointer is  0x%p\n", &bss);
	// kprintf("end pointer is  0x%p\n", &end);
	// kprintf("esp is 0x%p\n", esp);

	kprintf("mem_lower = %d KiB\n", mb_info->mem_lower);
	kprintf("mem_upper = %d KiB\n", mb_info->mem_upper);

	setup_paging(mb_info);
	while(1) {}
	kprintf("Test printing post-paging enable! I'm printing while paging enabled!\n");

	abort();
}
