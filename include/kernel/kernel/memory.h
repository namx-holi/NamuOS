

#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <stdint.h>

#include <kernel/atomic.h>
// #include <kernel/list.h>
#include <kernel/multiboot.h>


// NOTE: When caching becomes an issue for speed, the order of a lot of the
//  struct attributes will have to be shuffled around.


// Where kernel-space begins in memory
#define PAGE_OFFSET 0xC0000000

// Method for invalidating TLB for a physical address
static inline void invlpg(uintptr_t paddr) {
	asm volatile ("invlpg (%0)" : : "r"(paddr) : "memory");
}



// This represents a physical page frame in memory.
struct phys_page {
	// Backpointer to the zone this frame belongs to
	struct mem_zone* zone;

	// Reference count. If this drops to 0, this page may be freed.
	atomic_t count;

	// Flags for this page
	union flags {
		uint32_t raw;
		// TODO: Struct for each flag
	};

	// If this frame is mapped into the kernel, this is the virtual address
	void* virtual;

	// TODO: Attributes for when we set up slab allocator
	// TODO: Attributes for when we set up swapping
};
typedef struct phys_page phys_page_t;

// Global memory map, sits after kernel image. Pointers to all physical pages in
//  the system.
extern phys_page_t* mem_map;



// A zone represents a partition of a node. It's a range within memory.
//  This can be of three types: ZONE_DMA, ZONE_NORMAL, ZONE_HIGHMEM
struct mem_zone {
	// Backpointer to the node this zone is for
	struct mem_node* node;

	// C string for the name of the zone: "DMA", "Normal", "HighMem"
	char* name;

	// Pointer to the array of physical frames in this zone. It should be stored
	//  somewhere in the global `mem_map`.
	phys_page_t* phys_pages;
	uint32_t nb_phys_pages; // And total number of page frames in the node
	uint32_t free_pages; // Number of free pages in the zone

	// `start_pfn` is the first page frame number for this zone, and
	//  `start_map_pfn` is the page offset within the global `mem_map`.
	uint32_t start_pfn;
	uint32_t start_map_pfn;

	// TODO: Zone watermarks, for swapping.
};
typedef struct mem_zone mem_zone_t;

// Zones available on this OS
#define ZONE_DMA     0
#define ZONE_NORMAL  1
#define ZONE_HIGHMEM 2
#define MAX_NB_ZONES 3

// Null delimited list of zones
struct mem_zone_list {
	mem_zone_t* zones[MAX_NB_ZONES + 1];
};
typedef struct mem_zone_list mem_zone_list_t;



// A node represents a set of memory available on the machine.
struct mem_node {
	// NID of this node. First node should be 0.
	int32_t node_id;

	// `zones` is a list of all zones that *could* be present on a node. Not all
	//  of them m ay be present, but they're all defined.
	// `zonelist` is a list of *actual* zones present for this node, in order of
	//  preferred allocations.
	mem_zone_t zones[MAX_NB_ZONES];
	mem_zone_list_t zonelist;

	// Pointer to the array of physical frames in this node. It should be stored
	//  somewhere in the global `mem_map`.
	phys_page_t* phys_pages;
	uint32_t nb_phys_pages; // And total number of page frames in the node

	// `start_pfn` is the first page frame number for this node (ideally 0).
	// `start_map_pfn` is the page offset within the global `mem_map`. This may
	//  be different from `start_pfn` for nodes other than the first.
	uint32_t start_pfn;
	uint32_t start_map_pfn;

	// Bitmap describing any holes in memory. May be useful for ISA memhole.
	uint32_t* valid_addr_bitmap;

	// Pointer to the next node on the system. Will be NULL if this is the last
	//  one.
	struct mem_node* next;
};
typedef struct mem_node mem_node_t;

// Global list of all nodes on the system
#define MAX_NB_MEM_NODES 1
extern mem_node_t mem_nodes[MAX_NB_MEM_NODES];



// Page Table Entry
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

#define PAGE_SHIFT 12
#define PAGE_SIZE  (1UL << PAGE_SHIFT)
#define PAGE_MASK  (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
#define PTRS_PER_PTE 1024 // Entries per table

// TODO: 4 MiB Page Directory Entry (if PSE enabled)

// Page Directory Entry
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

#define PGDIR_SHIFT 22
#define PGDIR_SIZE  (1UL << PGDIR_SHIFT)
#define PGDIR_MASK  (~(PGDIR_SIZE-1))
#define PTRS_PER_PDE 1024 // Entries per directory

// Pointer to the current Page Global Directory (PGD)
extern PDE_t* pgd;



// Sets up nodes on the system
void memory_node_setup(multiboot_info_t* mb_info);

// Sets up paging on the system
void memory_paging_setup();




#endif
