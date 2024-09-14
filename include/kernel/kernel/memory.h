/// @file memory.h

#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <stdint.h>
#include <stddef.h>
#include <kernel/multiboot.h> // multiboot_info_t
#include <kernel/system.h>

// Representation of the CR3 register
union CR3_register {
	struct {
		// Ignored
		uint32_t ignored_1:3; // Bits 0-2

		// Page-level write-through; indirectly determines the memory type used to
		//  access the page directory during linear-address translation.
		uint32_t pwt:1; // Bit 3

		// Page-level cache-disable; indirectly determines the memory type used to
		//  access the page directory during linear-address translation.
		uint32_t pcd:1; // Bit 4

		// Ignored
		uint32_t ignored_2:7; // Bits 5-11

		// Physical address of the 4-KiB aligned page directory used for linear-
		//  address translation
		uint32_t addr:20; // Bits 12-31
	};
	uint32_t raw;
} __attribute__((packed));
typedef union CR3_register CR3_register_t;

// Page Directory Entry
union page_PDE {
	struct {
		// Present; must be 1 to reference a page table
		uint32_t present:1; // Bit 0

		// Read/write; if 0, writes may not be allowed to the 4-MiB region
		//  controlled by this entry
		uint32_t rw:1; // Bit 1

		// User/supervisor; if 0, user-mode accesses are not allowed to the 4-MiB
		//  region controlled by this entry
		uint32_t user:1; // Bit 2

		// Page-level write-through; indirectly determines the memory type used to
		//  access the page table referenced by this entry.
		uint32_t pwt:1; // Bit 3

		// Page-level cache disable; indirectly determines the memory type used to
		//  access the page table referenced by this entry
		uint32_t pcd:1; // Bit 4

		// Accessed; indicates whether this entry has been used for linear-address
		//  translation
		uint32_t accessed:1; // Bit 5

		// Ignored
		uint32_t ignored_1:1; // Bit 6

		// If CR4.PSE = 1, must be 0 (otherwise, this entry maps a 4-MiB page);
		//  otherwise, ignored.
		uint32_t page_size:1; // Bit 7

		// Ignored
		uint32_t ignored_2:4; // Bits 8 - 11

		// Physical address of the 4-KiB aligned page table referenced by this entry
		uint32_t addr:20; // Bits 12 - 31
	};
	uint32_t raw;
} __attribute__((packed));
typedef union page_PDE PDE_t;

// 4 MiB Page Directory Entry (currently unused)
struct page_PDE_4MB {
	// Present; must be 1 to map to a 4-MiB page
	uint32_t present:1; // Bit 0

	// Read/write; if 0, writes may not be allowed to the 4-MiB page referenced
	//  by this entry.
	uint32_t rw:1; // Bit 1

	// User/supervisor; if 0, user-mode accesses are not allowed to the 4-MiB
	//  page referenced by this entry.
	uint32_t user:1; // Bit 2

	// Page-level write-through; indirectly determines the memory type used to
	//  access the 4-MiB page referenced by this entry.
	uint32_t pwt:1; // Bit 3

	// Page-level cache disable; indirectly determines the memory type used to
	//  access the 4-MiB page referenced by this entry.
	uint32_t pcd:1; // Bit 4

	// Accessed; indicates whether software has accessed the 4-MiB page
	//  referenced by this entry
	uint32_t accessed:1; // Bit 5

	// Dirty; indicates whether software has written to the 4-MiB page
	//  referenced by this entry
	uint32_t dirty:1; // Bit 6

	// Page size; must be 1 (otherwise, this entry references a page table)
	uint32_t page_size:1; // Bit 7

	// Global; if CR4.PGE = 1, determines whether the translation is global;
	//  ignored otherwise.
	uint32_t global:1; // Bit 8

	// Ignored
	uint32_t ignored_1:3; // Bits 9 - 11

	// If the PAT is supported, indirectly determines the memory type used to
	//  access the 4-MiB page referenced by this entry; otherwise, reserved
	//  (must be 0).
	uint32_t pat:1; // Bit 12

	// Bits 39:32 of physical address of the 4-MiB page referenced by this entry
	uint32_t addr_upper:8; // Bits 13-20
	// NOTE: Assuming MAXPHYSADDR = 40

	// Reserved (must be 0)
	uint32_t reserved:1; // Bit 14
	// NOTE: Assuming MAXPHYSADDR = 40

	// Bits 31:22 of physical address of the 4-MiB page referenced by this entry
	uint32_t addr_lower:10; // Bits 22-31
} __attribute__((packed));
// TODO: Typedef for PDE_4MB

// Page Table Entry
union page_PTE {
	struct {
		// Present; must be 1 to map to a 4-KiB page
		uint32_t present:1; // Bit 0

		// Read/write; if 0, writes may not be allowed to the 4-KiB page referenced
		//  by this entry
		uint32_t rw:1; // Bit 1

		// User/supervisor; if 0, user-mode accesses are not allowed to the 4-KiB
		//  page referenced by this entry.
		uint32_t user:1; // Bit 2

		// Page-level write-through; indirectly determines the memory type used to
		//  access the 4-KiB page referenced by this entry.
		uint32_t pwt:1; // Bit 3

		// Page-level cache disable; indirectly determines the memory type used to
		//  access the 4-KiB page referenced by this entry.
		uint32_t pcd:1; // Bit 4

		// Accessed; indicates whether software has accessed the 4-KiB page
		//  referenced by this entry
		uint32_t accessed:1; // Bit 5

		// Dirty; indicates whether software has written to the 4-KiB page
		//  referenced by this entry
		uint32_t dirty:1; // Bit 6

		// If the PAT is supported, indirectly determines the memory type used to
		//  access the 4-KiB page referenced by this section; otherwise, reserved
		//  (must be 0)
		uint32_t pat:1; // Bit 7

		// Global; if CR4.PGE = 1, determines whether the translation is global;
		//  ignored otherwise
		uint32_t global:1; // Bit 8

		// Ignored
		uint32_t ignored:3; // Bits 9 - 11

		// Physical address of the 4-KiB page referenced by this entry
		uint32_t addr:20; // Bits 12 - 31
	};
	uint32_t raw;
} __attribute__((packed));
typedef union page_PTE PTE_t;


// Used to keep track of where to place new allocations
extern uintptr_t placement_pointer;

// TODO: Doc, currently unused
extern uintptr_t heap_end;

// Dummy CR3 register. The first paging structure used for any translation.
extern CR3_register_t cr3;


// Enables paging
extern void setup_paging(multiboot_info_t* mb_info);

// Sets `placement_pointer`
extern void kmalloc_startat(physical_addr_t address);

// Allocates a block at physical address if heap not set up
extern physical_addr_t kmalloc_real(size_t size, int align, physical_addr_t* phys);

// Allocates a block
extern physical_addr_t kmalloc(size_t size);

// Allocates an aligned block
extern physical_addr_t kvmalloc(size_t size);

// Allocates a block at physical address
extern physical_addr_t kmalloc_p(size_t size, physical_addr_t* phys);

// Allocates an aligned block at physical address
extern physical_addr_t kvmalloc_p(size_t size, physical_addr_t* phys);


// Gets a page given the virtual address
extern PTE_t* get_page(void* address);

// Translates virtual address to physical address
extern physical_addr_t get_physical_address(void* address);





#endif
