/// @file paging.c

#include <namuos/paging.h> // Implements

#include <string.h> // memset
#include <namuos/boot_allocator.h>
#include <namuos/terminal.h>


// Kernel page directory from `boot.S`, and the first two pages.
extern void* _kernel_pgd;
extern void* _kernel_pg0;
extern void* _kernel_pg1;
PDE_t* kernel_pgd = (PDE_t*)&_kernel_pgd;

// The current directory we're using
PDE_t* current_pgd = NULL;

// Addresses of regions of the kernel image. See `linker.ld`. Used to mark pages
//  as read-only. The `_kernel_image_rw_permission()` method returns 1 if the
//  page frame is writable, otherwise 0.
extern void* _paddr_kernel_start;
extern void* _paddr_kernel_ro_end;
extern void* _paddr_kernel_rw_end;
int _kernel_image_pfn_rw_permission(uint32_t pfn);


void paging_initialise() {
	// Set the current global page directory to the kernel directory set up in
	//  `boot.S`.
	current_pgd = kernel_pgd;

	// Set the permissions for the first two pages of the kernel page directory.
	//  The `present` bit has already been set for each entry, we just need to
	//  set `global` and the correct read/write permissions.
	PTE_t* pg0 = (PTE_t*)&_kernel_pg0;
	PTE_t* pg1 = (PTE_t*)&_kernel_pg1;
	for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) { // pg0
		pg0[i].global = 1;
		pg0[i].rw = _kernel_image_pfn_rw_permission(i);
	}
	for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) { // pg1
		pg1[i].global = 1;
		pg1[i].rw = _kernel_image_pfn_rw_permission(i + PTRS_PER_PTE);
	}

	// Calculate how many page table entries we need to cover the entire linear
	//  mappin of low memory. We subtract 2 from the rounded up number as we've
	//  already got pg0 and pg1.
	uint32_t total_entries = ZONE_HIGHMEM_OFFSET / PAGE_SIZE;
	uint32_t new_tables = (total_entries + PTRS_PER_PTE - 1) / PTRS_PER_PTE - 2;

	// Allocate space for the remaining page tables and clear all the entries.
	//  All page tables are allocated contiguously in one block in ZONE_DMA as
	//  otherwise it's not mapped in the 8 MiB initial mapping.
	PTE_t* entries = (PTE_t*)bootmem_aligned_alloc_low(sizeof(PTE_t) * PTRS_PER_PTE * new_tables);
	memset(entries, 0, sizeof(PTE_t) * PTRS_PER_PTE * new_tables);

	// Set `global` and `present` for all the new entries, and set as writable
	//  as they are outside the kernel image.
	for (uint32_t i = 2*PTRS_PER_PTE; i < total_entries; ++i) {
		entries[i - 2*PTRS_PER_PTE].present = 1;
		entries[i - 2*PTRS_PER_PTE].global = 1;
		entries[i - 2*PTRS_PER_PTE].rw = 1;
		// This ends up being physical address >> PAGE_SHIFT
		entries[i - 2*PTRS_PER_PTE].addr = i;
	}

	// Add these new page tables to the kernel page directory
	uint32_t pde_offset = PAGE_OFFSET >> PGDIR_SHIFT; // PDE kernel-space offset
	for (uint32_t i = 0; i < new_tables; ++i) {
		kernel_pgd[pde_offset+i+2].present = 1;
		kernel_pgd[pde_offset+i+2].addr = (__to_phys(entries) >> PAGE_SHIFT) + i;
	}

	// Update the current paging directory to the kernel PGD, and we've got
	//  paging set up!
	paging_change_pgd(kernel_pgd);
	klog_debug(
		"Initialised linear mapping 0x%p to 0x%p\n",
		__to_virt(0), __to_virt(ZONE_HIGHMEM_OFFSET));
}

void invalidate_page(void* vaddr) {
	// Invalidates the page given `vaddr` falls on in TLB
	asm volatile ("invlpg (%0)" : : "r"(vaddr) : "memory");
}

void paging_change_pgd(PDE_t* pgd) {
	// We don't use the PWT or PCD bits. By setting CR3 to the address of the
	//  given directory, we set the 20 most sig bits to (address >> PAGE_SHIFT).
	uint32_t cr3 = (uint32_t)__to_phys(pgd);
	asm volatile ("mov %0, %%cr3" : : "a"(cr3) : "memory");
}

int _kernel_image_pfn_rw_permission(uint32_t pfn) {
	uintptr_t paddr = pfn << PAGE_SHIFT;

	// The real mode address space (up to 1 MiB) contains a number of buffers we
	//  can write to, such as video display memory. We mark this as writable.
	if (paddr < (uintptr_t)&_paddr_kernel_start)
		return 1;
	
	// Addresses in the kernel iamge that are read-only are marked appropriately
	//  (e.g. .text and .rodata)
	else if (paddr < (uintptr_t)&_paddr_kernel_ro_end)
		return 0;
	
	// Simiarly, mark writable regions appropriately (e.g. .data and .bss)
	else if (paddr < (uintptr_t)&_paddr_kernel_rw_end)
		return 1;
	
	// Any address outside of the kernel image is free for writing
	return 1;
}
