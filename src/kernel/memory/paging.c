/// @file kalloc.c

#include <kernel/memory.h> // Implements

#include <kernel/system.h> // kprintf


PTE_t* get_page(void* address) {
	// PDE is selected using the physical address defined as follows:
	// - Bits 39:32 are 0
	// - Bits 31:12 are from CR3
	// - Bits 11:2 are bits 31:22 of the linear address
	// - Bits 1:0 are 0
	uintptr_t pde_phys = (kernel_page_directory << 12) | (((uintptr_t)address >> 22) << 2);
	PDE_t* pde = (PDE_t*)pde_phys;
	if (!pde->present) {
		// TODO: Page fault
		kprintf("TODO: Page fault for pde_phys = 0x%p\n", pde_phys);
		return NULL;
	}

	// TODO: Handle case for CR4.PSE and 4-MiB page mapping

	// If CR4.PSE = 0 or the PDE's PS flag is 0, PTE is selected using the
	// physical address defined as follows:
	// - Bits 39:32 are 0
	// - Bits 31:12 are from the PDE
	// - Bits 11:2 are bits 21:12 of the linear address
	// - Bits 1:0 are 0
	uintptr_t pte_phys = (pde->addr << 12) | ((((uintptr_t)address >> 12) & 0x3ff) << 2);
	PTE_t* pte = (PTE_t*)pte_phys;
	if (!pte->present) {
		// TODO: Page fault
		kprintf("TODO: Page fault for pte_phys = 0x%p\n", pte_phys);
		return NULL;
	}

	return pte;
}

uintptr_t translate_virtual_address(void* address) {
	// Get the page this address sits in
	PTE_t* pte = get_page(address);
	if (pte == NULL) {
		// TODO: Pass up page fault
		kprintf("TODO: Pass on page fault in get_physical_address\n");
		return 0;
	}
	// The final physical address is computed as follows:
	// - Bits 39:32 are 0
	// - Bits 31:12 are from the PTE
	// - Bits 11:0 are from the original linear address
	uintptr_t addr = (pte->addr << 12) | ((uintptr_t)address & 0xfff);
	return addr;
}
