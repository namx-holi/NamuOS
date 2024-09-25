
#include <kernel/memory.h> // Implements

#include <kernel/system.h> // DEBUG

// Boot allocation starts immediately after the end of kernel image.
//  `placement_pointer` is a physical address.
extern void* kernel_end_addr;
uintptr_t placement_pointer = (uintptr_t)&kernel_end_addr;

/**
 * Boot allocator.
 * 
 * Allocates memory without setting frame bitmap or paging. If this needs to be
 * done, must be done manually after allocation.
*/
uintptr_t boot_malloc(size_t size, int align, uintptr_t* paddr_ptr);



uintptr_t kmalloc(size_t size) {
	return kmalloc_real(size, 0, NULL); // Not aligned, no paddr
}

uintptr_t kvmalloc(size_t size) {
	return kmalloc_real(size, 1, NULL); // Aligned, no paddr
}

uintptr_t kmalloc_p(size_t size, uintptr_t* paddr_ptr) {
	return kmalloc_real(size, 0, paddr_ptr); // Not aligned, given paddr
}

uintptr_t kvmalloc_p(size_t size, uintptr_t* paddr_ptr) {
	return kmalloc_real(size, 1, paddr_ptr); // Aligned, given paddr
}

uintptr_t kmalloc_real(size_t size, int align, uintptr_t* paddr_ptr) {
	// We may need the physical address anyway, so we keep our own copy
	uintptr_t paddr;
	uintptr_t vaddr = boot_malloc(size, align, &paddr);
	
	// If paging is set up, paddr is PAGE_OFFSET behind virtual address
	if (pgd != NULL)
		paddr -= PAGE_OFFSET;

	// TODO: Handle page creation
	// TODO: Handle bitmap updating

	// If we need to return paddr, set the given pointer
	if (paddr_ptr)
		*paddr_ptr = paddr;

	// Return the virtual address of the allocation
	return vaddr;
}

uintptr_t boot_malloc(size_t size, int align, uintptr_t* paddr_ptr) {
	// If asked to align, move to the next page boundary
	if (align)
		placement_pointer = PAGE_ALIGN(placement_pointer);
	
	// If given a physical address pointer, set it to where this allocation
	//  takes place
	if (paddr_ptr)
		*paddr_ptr = placement_pointer;
	
	// Return the address of the start of allocation, and update placement
	//  pointer for the next boot allocation
	uintptr_t addr = placement_pointer;
	placement_pointer += size;
	return addr;
}

void update_boot_allocator_for_paging() {
	// Simply add PAGE_OFFSET to the allocator, so we can still access it using
	//  virtual address.
	placement_pointer += PAGE_OFFSET;
}