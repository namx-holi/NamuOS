/// @file paging.c

#include <arch_i386/paging.h> // Implements

#include <string.h> // memset
#include <arch_i386/bootmem.h>
#include <namuos/memory.h>
#include <namuos/system.h>


// Kernel page directory, and first two pages. Set up in `boot.S`
extern void* _kernel_pgd;
extern void* _kernel_pg0;
extern void* _kernel_pg1;
PDE_t* kernel_pgd = (PDE_t*)&_kernel_pgd;

// Our current directory we're currently using
PDE_t* current_pgd = NULL;

// Addresses of regions of kernel image. See `linker.ld`. Used to mark pages as
//  read-only. Also method that returns 0 if a given physical address falls in a
//  read-only region, and returns 1 if writable.
extern void* _paddr_kernel_start;
extern void* _paddr_kernel_ro_end;
extern void* _paddr_kernel_rw_end;
int kernel_image_rw_permission(uintptr_t paddr);



void paging_initialise(multiboot_info_t* mb_info) {
	// Set the current global page directory to the kernel directory set up in
	//  `boot.S`.
	current_pgd = kernel_pgd;

	// Set the permissions for the first two pages of the kernel page directory.
	//  The `present` bit has already been set for each entry, we just need to
	//  set `global`, and the correct read/write permissions absed on kernel
	//  image regions.
	PTE_t* pg0 = (PTE_t*)&_kernel_pg0;
	PTE_t* pg1 = (PTE_t*)&_kernel_pg1;
	for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) { // pg0
		pg0[i].global = 1;
		pg0[i].rw = kernel_image_rw_permission((uintptr_t)i << PAGE_SHIFT);	
	}
	for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) { // pg1
		pg1[i].global = 1;
		pg1[i].rw = kernel_image_rw_permission((uintptr_t)(i + PTRS_PER_PTE) << PAGE_SHIFT);
	}

	// Total number of page table entries we need to cover the entire ZONE_DMA
	//  and ZONE_NORMAL regions, and the number of new page tables we need. We
	//  round up table count and subtract 2 from the total as we already have
	//  pg0 and pg1 allocated in boot.S.
	uint32_t total_entries = ZONE_HIGHMEM_OFFSET / PAGE_SIZE;
	uint32_t new_tables = (total_entries + PTRS_PER_PTE - 1) / PTRS_PER_PTE - 2;

	// Allocate space for the remaining page tables, and clear all entries. All
	//  page tables are allocated contiguously in one block, and we can overflow
	//  to fill entries for the linear mapping.
	PTE_t* entries = (PTE_t*)bootmem_aligned_alloc(sizeof(PTE_t) * PTRS_PER_PTE * new_tables);
	memset(entries, 0, sizeof(PTE_t) * PTRS_PER_PTE * new_tables);

	// Set `global` and `present` for entries we need, and set as writable as we
	//  are outside of the kernel image and don't need to worry about hitting
	//  read-only code regions.
	for (uint32_t i = 2*PTRS_PER_PTE; i < total_entries; ++i) {
		entries[i-2*PTRS_PER_PTE].present = 1;
		entries[i-2*PTRS_PER_PTE].global = 1;
		entries[i-2*PTRS_PER_PTE].rw = 1;
		// This ends up being physical address >> PAGE_SHIFT
		entries[i-2*PTRS_PER_PTE].addr = i;
	}

	// Add these new page tables to the kernel page directory
	uint32_t offset = PAGE_OFFSET >> PGDIR_SHIFT; // PDE kernelspace offset
	for (uint32_t i = 0; i < new_tables; ++i) {
		kernel_pgd[offset+i+2].present = 1;
		kernel_pgd[offset+i+2].addr = (__to_phys(entries) >> PAGE_SHIFT) + i;
	}

	// Update the current paging directory for CPU, and we've got paging set up!
	paging_update_current_pgd();
	klog_debug("Kernel linear mapping set up\n");
}

void invalidate_page(uintptr_t vaddr) {
	asm volatile ("invlpg (%0)" : : "r"(vaddr) : "memory");
}

void paging_update_current_pgd() {
	// Don't need flags for PWT or PCD (bit 3 and 4 respectively). By setting
	//  CR3 to the address of current page directory, we set the 20 most sig
	//  bits to (address >> PAGE_SHIFT)
	uint32_t cr3 = (uint32_t)__to_phys(current_pgd);
	asm volatile ("mov %0, %%cr3" : : "a"(cr3) : "memory");
}



// Methods declared in this file:

int kernel_image_rw_permission(uintptr_t paddr) {
	// The real mode address space (up to 1 MiB) contains a number of buffers we
	//  can write to, such as video display memory. We mark this as writable.
	if (paddr < (uintptr_t)&_paddr_kernel_start)
		return 1;
	
	// Addresses in the kernel image that are read-only are marked appropriately
	//  (e.g. .text and .rodata).
	else if (paddr < (uintptr_t)&_paddr_kernel_ro_end)
		return 0;
	
	// Similarly, mark writable regions appropriately (e.g. .data and .bss)
	else if (paddr < (uintptr_t)&_paddr_kernel_rw_end)
		return 1;
	
	// Any address outside of the kernel image is free for writing.
	return 1;
}
