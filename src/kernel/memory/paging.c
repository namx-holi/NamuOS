#include <kernel/memory.h> // Implements

#include <string.h> // memset

#include <kernel/system.h> // DEBUG


PTE_t* get_page(uintptr_t vaddr) {
	// This matches the method described in **4.3 32-BIT PAGING** in *Intel 64
	//  and IA-32 Architectures Software Developer's Manual Volume 3*. We could
	//  do this via indices, but I think it's better to keep it exactly the
	//  same as the method described.

	// NOTE: Essentially indexing PGD by first 10 bits of vaddr
	uintptr_t pde_paddr = (uintptr_t)pgd | (((uintptr_t)vaddr >> 22) << 2);
	PDE_t* pde = (PDE_t*)pde_paddr;
	if (!pde->present) {
		kprintf(" [!] get_page: Page fault for pde at 0x%p (not present)\n", pde);
		return NULL; // Page fault
	}

	// NOTE: Essentially indexing the found page table by 10 bits of vaddr
	uintptr_t pte_paddr = (pde->addr << 12) | ((((uintptr_t)vaddr >> 12) & 0x3ff) << 2);

	// Since paging is enabled, we need to shift the fetched PTE address by
	//  PAGE_OFFSET. We then return the PTE regardless of if present or not.
	PTE_t* pte = (PTE_t*)(pte_paddr + PAGE_OFFSET);
	return pte;
}

uint32_t paging_extend_kmalloc_region(uintptr_t vaddr) {
	// Find the last PDE that contains a set-up linear mapping (from 0xC...)
	uint32_t pde_start_index = PAGE_OFFSET >> PGDIR_SHIFT;
	uint32_t last_pde_index = pde_start_index;
	for (uint32_t i = pde_start_index; i < PTRS_PER_PDE; ++i) {
		if (pgd[i].present) last_pde_index = i;
		else break;
	}

	// TODO: Check if memory full (index == 1023)
	// TODO: Check if falls outside ZONE_NORMAL?

	// TODO: Make sure that the last PDE has pages up to vaddr present. If not,
	//  must mark them as present.

	// If this last PDE spans to at least vaddr, we don't need to do anything
	if (((last_pde_index + 1) << PGDIR_SHIFT) > vaddr)
		return 0;
	
	// Since we need to allocate new memory for page tables, we actually need
	//  more space than was let on as we'll allocate them before the desired
	//  block we're extending for. We calculate how many new page tables we
	//  need, and add the space needed for that to vaddr to know our new target.
	uint32_t nb_page_tables = (vaddr >> PGDIR_SHIFT) - last_pde_index;
	vaddr += nb_page_tables * PAGE_SIZE;

	// Create new page tables for the new linear mapping!
	uint32_t pde_index = last_pde_index + 1;
	while (vaddr > (pde_index << PGDIR_SHIFT) - 1) {
		// Allocate space for a new page table
		// FIXME: If we're super unfortunate, creating this page table may also
		//  call this method and then we're screwed? Not sure yet...
		uintptr_t pg_paddr;
		PTE_t* pg = (PTE_t*)kvmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg_paddr);
		memset(pg, 0, sizeof(uint32_t) * PTRS_PER_PTE); // Clear all entries
		kprintf(" [*] Allocated new page for PDE 0x%x at 0x%p\n", pde_index, pg);

		// Map 4 MiB to the new page table
		for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) {
			pg[i].present = 1; // This page points to a frame
			pg[i].global  = 1; // Accessible from anywhere, and shouldn't change
			pg[i].rw      = 1; // This region is writable, we just allocated it
			pg[i].addr    = i + (pde_index - pde_start_index) * PTRS_PER_PTE;
		}

		// Place this new page table right after the last one
		pgd[pde_index].present = 1;
		pgd[pde_index].addr    = (uint32_t)pg_paddr >> PAGE_SHIFT;
		
		// Invalidate the address range in this page in the TLB, and go next
		invlpg(pde_index << PGDIR_SHIFT);
		pde_index += 1;
	}

	// // Update the page table!
	// uint32_t cr3;
	// asm volatile ("mov %%cr3, %0" : "=r"(cr3));
	// kprintf("cr3 = 0x%x\n", cr3);
	// uint32_t new_cr3 = ((uint32_t)pgd + PAGE_OFFSET);
	// kprintf("New cr3 will be 0x%x\n", new_cr3);
	// asm volatile ("mov %0, %%cr3" : : "r"(new_cr3));
	// while(1){}

	kprintf(" [*] Paging now spans to 0x%p\n", (pde_index << PGDIR_SHIFT) - 1);

	// Return the extra space we allocated for page tables
	return nb_page_tables * PAGE_SIZE;
}
