
#ifndef _KERNEL_MEMORY_STRUCTS_H
#define _KERNEL_MEMORY_STRUCTS_H 1

#include <stdint.h>


#define PAGE_SHIFT 12
#define PAGE_SIZE  (1UL << PAGE_SHIFT)
#define PAGE_MASK  (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
#define PTRS_PER_PTE 1024 // Entries per table

#define PGDIR_SHIFT 22
#define PGDIR_SIZE  (1UL << PGDIR_SHIFT)
#define PGDIR_MASK  (~(PGDIR_SIZE-1))
#define PTRS_PER_PDE 1024 // Entries per directory



/**
 * Page Table Entry
*/
union page_PTE {
	uint32_t raw;
	struct {
		// Bit 0, Present; Must be 1 to map to 4-KiB page
		uint32_t present:1;

		// Bit 1, Read/write; If 0, writes are not allowed to the 4 KiB region
		uint32_t rw:1;

		// Bit 2, User/supervisor; If 0, user-mode access not allowed to the
		//  4 KiB region
		uint32_t user:1;

		// Bit 3, Page-level write-through (PWT)
		uint32_t pwt:1;

		// Bit 4, Page-level cache disable (PCD)
		uint32_t pcd:1;

		// Bit 5, Accessed; Indicates whether software has accessed the 4 KiB
		//  page referenced by this entry
		uint32_t accessed:1;

		// Bit 6, Dirty; Indicates whether software has written to the 4 KiB
		//  page referenced by this entry
		uint32_t dirty:1;

		// Bit 7, PAT; If PAT not supported, must be 0
		uint32_t page_size:1;

		// Bit 8, Global; If CR4.PGE = 1, determines whether the translation is
		//  global. Ignored otherwise
		uint32_t global:1;

		// Bits 9 - 11, Ignored
		uint32_t ignored_9_11:3;

		// Bits 12 - 31, Physical address of referenced 4 KiB page frame
		uint32_t addr:20;
	};
} __attribute__((packed));
typedef union page_PTE PTE_t;


/**
 * Page Directory Entry
*/
union page_PDE {
	uint32_t raw;
	struct {
		// Bit 0, Present; Must be 1 to reference a page table
		uint32_t present:1;

		// Bit 1, Read/write; If 0, writes are not allowed to the 4 MiB region
		uint32_t rw:1;

		// Bit 2, User/supervisor; If 0, user-mode access not allowed to the
		//  4 MiB region
		uint32_t user:1;

		// Bit 3, Page-level write-through (PWT)
		uint32_t pwt:1;

		// Bit 4, Page-level cache disable (PCD)
		uint32_t pcd:1;

		// Bit 5, Accessed; Indicates whether this entry has been used for
		//  linear-address translation
		uint32_t accessed:1;

		// Bit 6, Ignored
		uint32_t ignored_6:1;

		// Bit 7, If PSE enabled, 1 maps to a 4 MiB page, 0 maps to a page
		//  table. If PSE disabled, ignored.
		uint32_t page_size:1;

		// Bits 8 - 11, Ignored
		uint32_t ignored_8_11:4;

		// Bits 12 - 31, Physical address of referenced page table
		uint32_t addr:20;
	};
} __attribute__((packed));
typedef union page_PDE PDE_t;


// TODO: 4 MiB Page Directory Entry (if PSE enabled)


#endif
