/// @file kernel.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/multiboot.h>

#include <kernel/tty.h> // terminal_initialise
#include <kernel/memory.h> // memory_initialise

// TODO: Define `panic` instead of using abort


void kernel_main(multiboot_info_t* mb_info, uint32_t magic) {
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

	if (!memory_initialise(mb_info)) {
		printf("Failed to initialise memory.\n");
		abort();
	}

	if (mb_info->flags & MULTIBOOT_INFO_BOOTDEV)
		printf("boot device = 0x%x\n", mb_info->boot_device);
	
	if (mb_info->flags & MULTIBOOT_INFO_CMDLINE)
		printf("cmdline ptr = 0x%p\n", mb_info->cmdline);
	
	if (mb_info->flags & MULTIBOOT_INFO_MODS) {
		multiboot_module_t* mod;
		int i;

		printf("mods_count = %d, mods_addr = 0x$x\n", mb_info->mods_count, mb_info->mods_addr);
		for (i=0, mod = (multiboot_module_t*)mb_info->mods_addr; i < mb_info->mods_count; ++i, ++mod) {
			printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
				(unsigned)(mod->mod_start),
				(unsigned)(mod->mod_end),
				(char*)mod->cmdline);
		}
	} else {
		printf("No boot modules!\n");
	}
	printf("\n");

	if (mb_info->flags & MULTIBOOT_INFO_VBE_INFO) {
		printf("vbe_control_info is %x\n", mb_info->vbe_control_info);
		printf("vbe_mode_info is %x\n", mb_info->vbe_mode_info);

		
	} else {
		printf("No VBE info!\n");
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
