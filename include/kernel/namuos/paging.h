/// @file paging.h
// TODO: Doxygen comments

#ifndef _PAGING_H
#define _PAGING_H 1

#include <stddef.h>
#include <stdint.h>


/// Virtual address for the beginning of kernel space
#define PAGE_OFFSET 0xC0000000

// Macros to translate to-from virtual/physical address in kernel space
#define __to_virt(x) (void*)((uintptr_t)(x) + PAGE_OFFSET)
#define __to_phys(x) (uintptr_t)((uintptr_t)(x) - PAGE_OFFSET)

// Locations of special regions in physical address space
#define ZONE_DMA_OFFSET     0x00000000
#define ZONE_DMA_SIZE       0x01000000 // First 16 MiB
#define ZONE_NORMAL_OFFSET  0x01000000
#define ZONE_NORMAL_SIZE    0x37000000 // 16 MiB to 896 MiB
#define ZONE_HIGHMEM_OFFSET 0x38000000
#define ZONE_HIGHMEM_SIZE   0xc8000000 // 896 MiB to end

// PTE bits
#define PAGE_SHIFT 12
#define PAGE_SIZE  (1UL << PAGE_SHIFT)
#define PAGE_MASK  (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
#define PTRS_PER_PTE 1024

// PDE bits
#define PGDIR_SHIFT 22
#define PGDIR_SIZE  (1UL << PGDIR_SHIFT)
#define PGDIR_MASK  (~(PGDIR_SIZE-1))
#define PTRS_PER_PDE 1024


/// Structure used for page table entries (PTEs)
union page_PTE {
	uint32_t raw;
	struct {
		// Bit 0, Present; Must be 1 to map to 4-KiB page
		#define PTE_PRESENT 1<<0
		uint32_t present:1;

		// Bit 1, Read/write; If 0, writes are not allowed to the 4 KiB region
		#define PTE_RW 1<<1
		uint32_t rw:1;

		// Bit 2, User/supervisor; If 0, user-mode access not allowed to the
		//  4 KiB region
		#define PTE_USER 1<<2
		uint32_t user:1;

		// Bit 3, Page-level write-through (PWT)
		#define PTE_PWT 1<<3
		uint32_t pwt:1;

		// Bit 4, Page-level cache disable (PCD)
		#define PTE_PCD 1<<4
		uint32_t pcd:1;

		// Bit 5, Accessed; Indicates whether software has accessed the 4 KiB
		//  page referenced by this entry
		#define PTE_ACCESSED 1<<5
		uint32_t accessed:1;

		// Bit 6, Dirty; Indicates whether software has written to the 4 KiB
		//  page referenced by this entry
		#define PTE_DIRTY 1<<6
		uint32_t dirty:1;

		// Bit 7, PAT; If PAT not supported, must be 0
		#define PTE_PAGE_SIZE 1<<7
		uint32_t page_size:1;

		// Bit 8, Global; If CR4.PGE = 1, determines whether the translation is
		//  global. Ignored otherwise
		#define PTE_GLOBAL 1<<8
		uint32_t global:1;

		// Bits 9 - 11, Ignored
		uint32_t ignored_9_11:3;

		// Bits 12 - 31, Physical address of referenced 4 KiB page frame
		uint32_t addr:20;
	};
} __attribute__((packed));
typedef union page_PTE PTE_t;

/// Structure used for page directory entries (PDEs)
union page_PDE {
	uint32_t raw;
	struct {
		// Bit 0, Present; Must be 1 to reference a page table
		#define PDE_PRESENT 1<<0
		uint32_t present:1;

		// Bit 1, Read/write; If 0, writes are not allowed to the 4 MiB region
		#define PDE_RW 1<<1
		uint32_t rw:1;

		// Bit 2, User/supervisor; If 0, user-mode access not allowed to the
		//  4 MiB region
		#define PDE_USER 1<<2
		uint32_t user:1;

		// Bit 3, Page-level write-through (PWT)
		#define PDE_PWT 1<<3
		uint32_t pwt:1;

		// Bit 4, Page-level cache disable (PCD)
		#define PDE_PCD 1<<4
		uint32_t pcd:1;

		// Bit 5, Accessed; Indicates whether this entry has been used for
		//  linear-address translation
		#define PDE_ACCESSED 1<<5
		uint32_t accessed:1;

		// Bit 6, Ignored
		uint32_t ignored_6:1;

		// Bit 7, If PSE enabled, 1 maps to a 4 MiB page, 0 maps to a page
		//  table. If PSE disabled, ignored.
		#define PDE_PAGE_SIZE 1<<7
		uint32_t page_size:1;

		// Bits 8 - 11, Ignored
		uint32_t ignored_8_11:4;

		// Bits 12 - 31, Physical address of referenced page table
		uint32_t addr:20;
	};
} __attribute__((packed));
typedef union page_PDE PDE_t;

// TODO: 4 MiB PDE if PSE bit enabled


// Pointers to current page directory, and kernel page directory
extern PDE_t* kernel_pgd;  ///< Kernel page directory
extern PDE_t* current_pgd; ///< Current page directory

// TODO: Doxygen comment
void paging_initialise();

// TODO: Doxygen comment
void invalidate_page(void* vaddr);

// TODO: Doxygen comment
void paging_change_pgd(PDE_t* pgd);

#endif
