/// @file kernel.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/bios.h>
#include <kernel/bios_defines.h>
#include <kernel/tty.h>
#include "multiboot.h"

// TODO: Define `panic` instead of using abort

// TODO: Better name for this
#define KERNEL_BASE_PTR 0xC0000000



void kernel_main(multiboot_info_t* mbd, uint32_t magic) {
	terminal_initialise();

	// TODO: Move all the initialisation checks to a separate method
	// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format
	printf("Checking multiboot_info_t...\n");
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid bootloader magic number\n");
		abort();
	}

	// Check values of each flag for multiboot info
	// printf("Basic lower/upper memory info present: %d\n", mbd->flags & MULTIBOOT_INFO_MEMORY);
	// printf("Boot device set:                       %d\n", mbd->flags & MULTIBOOT_INFO_BOOTDEV);
	// printf("Command-line defined:                  %d\n", mbd->flags & MULTIBOOT_INFO_CMDLINE);
	// printf("Boot modules present:                  %d\n", mbd->flags & MULTIBOOT_INFO_MODS);
	// printf("AOUT symbol table present:             %d\n", mbd->flags & MULTIBOOT_INFO_AOUT_SYMS);
	// printf("ELF symbol table present:              %d\n", mbd->flags & MULTIBOOT_INFO_ELF_SHDR);
	// printf("Full memory map available:             %d\n", mbd->flags & MULTIBOOT_INFO_MEM_MAP);
	// printf("Drive info available:                  %d\n", mbd->flags & MULTIBOOT_INFO_DRIVE_INFO);
	// printf("Config table present:                  %d\n", mbd->flags & MULTIBOOT_INFO_CONFIG_TABLE);
	// printf("Boot loader name present:              %d\n", mbd->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME);
	// printf("AMP table present:                     %d\n", mbd->flags & MULTIBOOT_INFO_APM_TABLE);
	// printf("Video VBE information avaiable:        %d\n", mbd->flags & MULTIBOOT_INFO_VBE_INFO);
	// printf("Video framebuffer information present: %d\n", mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO);
	// printf("\n");

	// Make sure we have a valid memory map
	if (!(mbd->flags & MULTIBOOT_INFO_MEM_MAP)) {
		printf("Invalid memory map given by GRUB\n");
		abort();
	}
	// Loop through memory map and display values
	for (uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		// Fetch the memory map. Need to add kernel base to address to be valid
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i + KERNEL_BASE_PTR);

		printf("Start Addr: %lx | Length: %lx | Size: %x | Type: %d | ",
			mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
		switch (mmmt->type) {
			case MULTIBOOT_MEMORY_AVAILABLE: printf("Available\n"); break;
			case MULTIBOOT_MEMORY_RESERVED: printf("Reserved\n"); break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: printf("ACPI Reclaimable\n"); break;
			case MULTIBOOT_MEMORY_NVS: printf("NVS\n"); break;
			case MULTIBOOT_MEMORY_BADRAM: printf("Bad RAM\n"); break;
			default: printf("???\n");
		}
	}

	// printf("== Detected Hardware ==\n");
	// bios_log_equipment();
	// printf("\n");

	// uint16_t memsize = *BDA_MEMORY_SIZE_KB;
	// printf("Memory size in KB: %d\n", memsize);

	// Testing asserts
	printf("\nTesting assertions:\n");
	assert(0==1);
}
