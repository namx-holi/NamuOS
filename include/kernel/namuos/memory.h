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


// Macros to translate to-from virtual/physical address in kernel space

/// Translates physical address to virtual address
#define __to_virt(x) (void*)((uintptr_t)(x) + PAGE_OFFSET)
/// Translates virtual address to physical address
#define __to_phys(x) (uintptr_t)((uintptr_t)(x) - PAGE_OFFSET)


// Page Frame
// TODO: Doxygen comment
struct page_frame {
	uint32_t ref_count;
	uint32_t flags; // TODO: Set up flag bits

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
