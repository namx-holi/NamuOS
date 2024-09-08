/// @file memory.c

#include <kernel/memory.h> // Implements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



bool memory_initialise(multiboot_info_t* mb_info) {
	// Make sure we've got info about lower and upper memory
	if (!(mb_info->flags & MULTIBOOT_INFO_MEMORY)) {
		printf("No information about lower/upper memory from GRUB\n");
		return false;
	}

	// Make sure we've gotten a valid memory map from GRUB
	if (!(mb_info->flags & MULTIBOOT_INFO_MEM_MAP)) {
		printf("Invalid memory map given by GRUB\n");
		return false;
	}

	// Fetch lower/upper memory sizes.
	uint32_t mem_lower = mb_info->mem_lower;
	uint32_t mem_upper = mb_info->mem_upper;
	printf("mem_lower = %u KB, mem_upper = %u KB\n", mem_lower, mem_upper);
	printf("\n");

	printf("Size of mmap is %d\n", mb_info->mmap_length);

	// Loop through the memory map
	multiboot_memory_map_t* entry;
	// NOTE: entry->size can be used to skip to next pair instead of sizeof
	for (uint32_t i = 0; i < mb_info->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		entry = (multiboot_memory_map_t*)(mb_info->mmap_addr + i);

		// printf("Entry address: %p\n", entry);

		// TODO: Handle each entry
		switch (entry->type) {
			// Type 1: Usable (normal) RAM
			case MULTIBOOT_MEMORY_AVAILABLE:
				printf("Available entry at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
			
			// Type 2: Reserved - unusable
			case MULTIBOOT_MEMORY_RESERVED:
				printf("Reserved entry at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
			
			// Type 3: ACPI reclaimable memory
			// NOTE: Can be used as long as finished with ACPI tables
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				printf("ACPI Reclaimable entry at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
			
			// Type 4: Non-volatile memory - unusable
			// NOTE: Indicates reserved memory which needs to be preserved on hibernation
			case MULTIBOOT_MEMORY_NVS:
				printf("NVS entry at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
			
			// Type 5: Area containing bad memory - unusable
			case MULTIBOOT_MEMORY_BADRAM:
				printf("Bad memory at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
			
			// Unlisted - treat as Type 2
			default:
				printf("Unlisted entry at %lx - %lx (Len: %lx)\n", entry->addr, entry->addr + entry->len, entry->len);
				break;
		}
	}
	printf("\n");

	return true;
}
