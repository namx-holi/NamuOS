#include <kernel/memory.h> // Implements

#include <stddef.h> // size_t
#include <string.h> // memset

#include <kernel/system.h> // Update kprintf pointers


// Physical addresses of regions of the kernel image
extern void* kernel_addr;      // Start address of the kernel image
extern void* code_end_addr;    // End of read-only kernel image region
extern void* kernel_end_addr;  // End address of kernel image


// This acts as a boot memory allocator. We can allocate memory immediately
//  after the end of the kernel image.
uintptr_t placement_pointer = (uintptr_t)&kernel_end_addr;




// Boot allocators
uintptr_t bmalloc_real(size_t size, int align, uintptr_t* paddr) {
	// If asked to align, move to next page boundary
	if (align)
		placement_pointer = PAGE_ALIGN(placement_pointer);
	
	// If given a physical address pointer, set to the location this allocation
	//  takes place
	if (paddr)
		*paddr = placement_pointer;
	
	// Return the address of the start of allocation, and update placement
	//  pointer for next allocation
	uintptr_t addr = placement_pointer;
	placement_pointer += size;
	return addr;
}
uintptr_t bmalloc(size_t size) {
	return bmalloc_real(size, 0, NULL); // Not aligned, no phys address
}
uintptr_t bvmalloc(size_t size) {
	return bmalloc_real(size, 1, NULL); // Aligned, no phys address
}
uintptr_t bmalloc_p(size_t size, uintptr_t* paddr) {
	return bmalloc_real(size, 0, paddr); // Not aligned, phys address
}
uintptr_t bvmalloc_p(size_t size, uintptr_t* paddr) {
	return bmalloc_real(size, 1, paddr); // Aligned, phys address
}


// Global Page Directory
PDE_t* pgd;


// Test method to check if we are setting the correct addresses in page table
uintptr_t test_translate(void* addr) {
	kprintf("PGD paddr = 0x%p\n", pgd);

	uintptr_t pde_paddr = (uintptr_t)(pgd) | (((uintptr_t)addr >> 22) << 2);
	kprintf("pde_paddr = 0x%p\n", pde_paddr);
	PDE_t* pde = (PDE_t*)pde_paddr;
	if (!pde->present) {
		kprintf("Page fault for that PDE\n");
		return 0;
	}

	uintptr_t pte_paddr = (pde->addr << 12) | ((((uintptr_t)addr >> 12) & 0x3ff) << 2);
	kprintf("pte_paddr = 0x%p\n", pte_paddr);
	PTE_t* pte = (PTE_t*)pte_paddr;
	if (!pte->present) {
		kprintf("Page fault for that PTE\n");
		return 0;
	}

	uintptr_t paddr = (pte->addr << 12) | ((uintptr_t)addr & 0x3ff);
	kprintf("paddr = 0x%p\n", paddr);

	return paddr;
}



void memory_node_setup(multiboot_info_t* mb_info) {
	// TODO
}

void memory_paging_setup() {
	// Allocate space for the kernel page directory
	uintptr_t pgd_phys;
	pgd = (PDE_t*)bvmalloc_p(sizeof(uint32_t) * PTRS_PER_PDE, &pgd_phys);
	memset(pgd, 0, sizeof(uint32_t) * PTRS_PER_PDE);
	// Enable writing for all entries
	for (int i = 0; i < PTRS_PER_PDE; ++i) {
		pgd[i].rw = 1;
	}

	// Allocate space for 8 MiB worth of page tables to map the kernel image
	uintptr_t pg0_phys;
	uintptr_t pg1_phys;
	PTE_t* pg0 = (PTE_t*)bvmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg0_phys);
	PTE_t* pg1 = (PTE_t*)bvmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg1_phys);
	memset(pg0, 0, sizeof(uint32_t) * PTRS_PER_PTE);
	memset(pg1, 0, sizeof(uint32_t) * PTRS_PER_PTE);

	// Create a linear mapping of 0-8 MiB, 4 MiB per table.
	// NOTE: Because pg1 is allocated immediately after pg0 using the boot
	//  allocator, and both have the size of one page, we can overflow into pg1
	//  from pg0. This is why we iterate `PTRS_PER_PTE*2` times.
	for (int i = 0; i < PTRS_PER_PTE * 2; ++i) {
		pg0[i].present = 1; // This page points to a frame
		pg0[i].global  = 1; // Accessible anywhere, and shouldn't change
		pg0[i].addr    = i; // The frame physical address / PAGE_SIZE

		// Check where this address falls in kernel image for read-write perms
		uintptr_t paddr = i << PAGE_SHIFT;
		if (paddr < (uintptr_t)&kernel_addr) {
			// Real-mode address space before kernel image. Contains some BIOS
			//  data that we need to write to for VGA.
			pg0[i].rw = 1;
		} else if (paddr < (uintptr_t)&code_end_addr) {
			// This is the multiboot header, .text region, and .rodata region.
			pg0[i].rw = 0;
			kprintf("Frame starting at 0x%p set as read-only.\n", paddr);
		} else if (paddr < (uintptr_t)&kernel_end_addr) {
			// This is the .data and .bss regions. Need to be able to write.
			pg0[i].rw = 1;
		} else {
			// Anything else after the kernel image. This includes the page
			//  tables, so mark as read-write.
			pg0[i].rw = 1;
		}
	}

	// Map pg0 and pg1 to the beginning of kernel-space, as well as set up an
	//  identity mapping so we can enable paging without breaking code pointer
	// Kernel-space mapping (vaddr = paddr + PAGE_OFFSET)
	uint32_t start_pfn = PAGE_OFFSET >> PGDIR_SHIFT;
	pgd[start_pfn].present   = 1;
	pgd[start_pfn].addr      = (uint32_t)pg0 >> PAGE_SHIFT;
	pgd[start_pfn+1].present = 1;
	pgd[start_pfn+1].addr    = (uint32_t)pg1 >> PAGE_SHIFT;
	// Identity mapping (vaddr = paddr)
	pgd[0].present = 1;
	pgd[0].addr    = (uint32_t)pg0 >> PAGE_SHIFT;
	pgd[1].present = 1;
	pgd[1].addr    = (uint32_t)pg1 >> PAGE_SHIFT;

	// Set the CR3 page directory address to our global page directory. Then
	//  enable paging using CR0 register with write protect.
	// NOTE: Don't need any CR3 flags (bit 3 for PWT, bit 4 for PCD).
	uint32_t cr3 = (uint32_t)pgd;
	asm volatile ("mov %0, %%cr3" : : "r"(cr3)); // Set page directory address
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80010000; // Bit 31 for paging, bit 16 for write protect
	asm volatile ("mov %0, %%cr0" : : "r"(cr0)); // Enable paging/write protect

	// Update the VGA memory address so we use the virtual address
	kernel_kprintf_update_page_offset(PAGE_OFFSET);

	// Paging is now set up, rid outselves of the identity mapping and
	//  invalidate those addresses.
	pgd[0].present = 0;
	pgd[0].addr = 0;
	pgd[1].present = 0;
	pgd[1].addr = 0;
	invlpg(0 << PAGE_SHIFT);
	invlpg(1 << PAGE_SHIFT);
}
