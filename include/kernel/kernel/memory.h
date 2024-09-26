

#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <stddef.h>
#include <stdint.h>
#include <kernel/memory_structs.h>
#include <kernel/multiboot.h>

// Where kernel-space begins in memory
#define PAGE_OFFSET 0xC0000000

// Bitmap of free frames. 1 marks a free frame.
extern uint32_t* memmap_bitmap;

// The current Page Global Directory
extern PDE_t* pgd;

// Helper method for invalidating TLB for a physical address
static inline void invlpg(uintptr_t paddr) {
	asm volatile ("invlpg (%0)" : : "r"(paddr) : "memory");
}

// Kernel-level allocation
extern uintptr_t kmalloc_real(size_t size, int align, uintptr_t* paddr_ptr);
extern uintptr_t kmalloc(size_t size);
extern uintptr_t kvmalloc(size_t size);
extern uintptr_t kmalloc_p(size_t size, uintptr_t* paddr_ptr);
extern uintptr_t kvmalloc_p(size_t size, uintptr_t* paddr_ptr);
extern void update_boot_allocator_for_paging();




// Sets up paging on the system
extern void memory_paging_setup();

// Gets a page if exists, or returns NULL if doesn't exist
extern PTE_t* get_page(uintptr_t vaddr);

// Extends the kmalloc region to at least given vaddr, in 4 MiB chunks. vaddr
//  should be the last byte needed. Returns how much memory was allocated for
//  paging.
extern uint32_t paging_extend_kmalloc_region(uintptr_t vaddr);




// Sets up frame bitmap
extern void memory_memmap_setup(multiboot_info_t* mb_info);

// Marks frames as free/used, and tests if frames are free or not
// TODO: Move to the .c file so they aren't seen by header include
extern void memmap_mark_used(uint32_t pfn);
extern void memmap_mark_free(uint32_t pfn);
extern uint32_t memmap_test(uint32_t pfn);

#endif
