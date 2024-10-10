/**
 * @file memory.h
 * @defgroup namuos_memory <namuos/memory.h>
 * @brief Architecture-independent methods for interfacing with memory
 * @ingroup namuos
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _NAMUOS_MEMORY_H
#define _NAMUOS_MEMORY_H 1

#include <stddef.h>
#include <stdint.h>
#include <namuos/arch.h>
#include <namuos/multiboot.h>


/// Virtual address for the beginning of kernel-space
#define PAGE_OFFSET 0xC0000000

// Offsets of special regions in physical address space
#define ZONE_DMA_OFFSET     0x00000000
#define ZONE_DMA_SIZE       0x01000000 // First 16 MiB
#define ZONE_NORMAL_OFFSET  0x01000000
#define ZONE_NORMAL_SIZE    0x37000000 // 16 MiB to 896 MiB
#define ZONE_HIGHMEM_OFFSET 0x38000000
#define ZONE_HIGHMEM_SIZE   0xc8000000 // 896 MiB to end
// TODO: Eventually remove bitmap, and use free/used lists. Also move to nodes.


// Macros to translate to-from virtual/physical address in kernel space

/// Translates physical address to virtual address
#define __to_virt(x) (void*)((uintptr_t)(x) + PAGE_OFFSET)
/// Translates virtual address to physical address
#define __to_phys(x) (uintptr_t)((uintptr_t)(x) - PAGE_OFFSET)


// Page Frame
// TODO: Doxygen comment
struct page_frame {
	uint32_t ref_count;

	// Flags for the frame
	union {
		uint32_t flags;
		struct {
			// Bit 0, if page must be locked in memory for disk I/O. When I/O
			//  starts this is set, and released when it completes.
			uint32_t locked:1;

			// Bit 1, if an error occurs during disk I/O, this bit is set.
			uint32_t error:1;

			// Bit 2, if a page is mapped and it is referenced through the
			//  mapping, index hash table, this bit is set. It is used during
			//  page replacement for moving the page around LRU lists.
			uint32_t referenced:1;

			// Bit 3, when a page is read from disk without error, this bit will
			//  be set.
			uint32_t uptodate:1;

			// Bit 4, this indicates if a page needs to be flushed to disk. When
			//  a page is written to that is backed by disk, it is not flushed
			//  immediately. This bit is needed to ensure a dirty page is not
			//  freed before it is written out.
			uint32_t dirty:1;

			// Bit 5, ???
			// TODO: Document
			uint32_t decr_after:1;

			// Bit 6, this bit is set if a page is on the `active_list` LRU and
			//  cleared when it is removed. It marks a page as being hot.
			uint32_t active:1;

			// Bit 7, ???
			// TODO: Document
			uint32_t inactive_dirty:1;

			// Bit 8, this will flag a page as being used by the slab allocator.
			uint32_t slab:1;

			// Bit 9, ???
			// TODO: Document
			uint32_t swap_cache:1;

			// Bit 10, used by some architectures to skip over parts of the
			//  address space with no backing physical memory.
			uint32_t skip:1;

			// Bit 11, ???
			// TODO: Document
			uint32_t inactive_clean:1;

			// Bit 12, pages in high memory cannot be mapped permanently by the
			//  kernel. Pages that are in high memory are flagged with this bit
			//  during memory setup.
			uint32_t highmem:1;

			// Bits 13 - 29, ignored (in two chunks?)
			uint32_t ignored_13_20:8;
			uint32_t ignored_21_29:9;

			// Bit 30, architecture specific page state bit. This allows an
			//  architecture to defer the flushing of the D-Cache until the page
			//  is mapped by a process.
			uint32_t arch_1:1;

			// Bit 31, this is set for pages that can never be swapped out. It
			//  is set by the boot memory allocator for pages allocated during
			//  system startup. Later it is used to flag empty pages or ones
			//  that do not even exist.
			uint32_t reserved:1;
		};
	};

	// If this frame is mapped by `kmap()`, this is the virtual address
	void* virtual;
} __attribute__((packed));
typedef struct page_frame frame_t;


/// Bitmap of free frames. A bit of 1 indicates a free frame.
extern uint32_t* memmap_bitmap;

/// Pointer to the beginning of memmap, an array of `frame_t` for each frame.
extern frame_t* memmap;


/** @brief Initialises memory for the operating system
 * 
 * Initialises memory for the operating system by calling all other setup
 * methods.
 * 
 * @param mb_info Multiboot info passed from GRUB
*/
void memory_initialise(multiboot_info_t* mb_info);

/** @brief Sets up operating-system paging for given architecture
 * 
 * Sets up any architecture-specific structures and calls any required
 * instructions to set up kernel-space linear mapping.
 * 
 * @param mb_info Multiboot info passed from GRUB
*/
ARCH_METHOD void memory_paging_initialise(multiboot_info_t* mb_info);

/** @brief Sets up frame memory map for given architecture
 * 
 * @todo Description
 * 
 * @param mb_info Multiboot info passed from GRUB
*/
ARCH_METHOD void memory_memmap_initialise(multiboot_info_t* mb_info);
// NOTE: Memory map is initialised per-architecture as it initialises frames
//  based on regions in the linker script for the architecture.


// TODO: Doxygen comments
void memmap_bitmap_flag_used(uint32_t pfn);
void memmap_bitmap_flag_free(uint32_t pfn);
uint32_t memmap_bitmap_test(uint32_t pfn);




// Kernel-level memory allocators

/** @brief Allocate kernel memory
 * 
 * Allocates memory in kernel-space.
 * 
 * @param size Size of the requested memory block
 * 
 * @returns Pointer to the memory block.
*/
ARCH_METHOD void* kmalloc(size_t size);

/** @brief Allocate page-aligned kernel memory
 * 
 * Allocates memory in kernel-space. The allocated block will be page-aligned.
 * 
 * @param size Size of the requested memory block
 * 
 * @returns Pointer to the memory block.
*/
ARCH_METHOD void* kvmalloc(size_t size);

/** @brief Allocate kernel memory, returning the physical address
 * 
 * Allocates memory in kernel-space. The physical address of this allocation
 * will be returned in @ref paddr_ptr.
 * 
 * @param size Size of the requested memory block
 * @param paddr_ptr Pointer to where to store physical address of allocation
 * 
 * @returns Pointer to the memory block.
*/
ARCH_METHOD void* kmalloc_p(size_t size, uintptr_t* paddr_ptr);

/** @brief Allocate page-aligned kernel memory, returning the physical address
 * 
 * Allocates memory in kernel-space. The allocated block will be page-aligned.
 * The physical address of this allocation will be returned in @ref paddr_ptr.
 * 
 * @param size Size of the requested memory block
 * @param paddr_ptr Pointer to where to store physical address of allocation
 * 
 * @returns Pointer to the memory block.
*/
ARCH_METHOD void* kvmalloc_p(size_t size, uintptr_t* paddr_ptr);

#endif

/** @} */
