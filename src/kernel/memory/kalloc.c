
#include <kernel/memory.h> // Implements

#include <kernel/system.h> // DEBUG

// Boot allocation starts immediately after the end of kernel image.
//  `placement_pointer` is a physical address.
extern void* kernel_end_addr;
volatile uintptr_t placement_pointer = (uintptr_t)&kernel_end_addr;

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
	// If asked to align, move to the next page boundary
	if (align)
		placement_pointer = PAGE_ALIGN(placement_pointer);

	// The virtual address for the allocation is where the placement pointer
	//  currently sits, and the physical address is where in physical memory
	//  that allocated area sits. The physical address will be offset by
	//  PAGE_OFFSET if paging is enabled.
	uintptr_t vaddr = placement_pointer;
	uintptr_t paddr = placement_pointer;

	// If the end of the allocation isn't paged yet, we need to set up paging to
	//  at least the end of the allocation. Since we're still in kmalloc-space,
	//  the mapping is linear.
	if (pgd) {
		// First, because paging is enabled, the physical address needs to be
		//  shifted by the page offset
		paddr -= PAGE_OFFSET;

		// Extend the linear mapping to at least the end of allocation (plus any
		//  page tables to do this). If this did end up allocating space for
		//  page tables, we need to skip over those in our returned paddr and
		//  vaddr.
		uint32_t alloc_size = paging_extend_kmalloc_region(vaddr + size - 1);
		vaddr += alloc_size;
		paddr += alloc_size;
	}

	// If the memmap bitmap has been initialised, update frames as used.
	if (memmap_bitmap) {
		uint32_t start_pfn = paddr / PAGE_SIZE;
		uint32_t end_pfn = (paddr + size) / PAGE_SIZE;
		for (uint32_t pfn = start_pfn; pfn < end_pfn; ++pfn)
			memmap_mark_used(pfn);
	}

	// If we were given a pointer, store the physical allocation address in it
	if (paddr_ptr)
		*paddr_ptr = paddr;

	// Move to where the next allocation can take place, and return the virtual
	//  address of allocation!
	placement_pointer += size;
	return vaddr;
}

void update_boot_allocator_for_paging() {
	// Simply add PAGE_OFFSET to the allocator, so we can still access it using
	//  virtual address.
	placement_pointer += PAGE_OFFSET;
}