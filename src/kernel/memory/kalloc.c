/// @file kalloc.c

#include <kernel/memory.h> // Implements

#include <stdlib.h> // malloc, valloc

#include <kernel/system.h> // DEBUG: kprintf


uintptr_t kmalloc_real(size_t size, int align, uintptr_t* phys) {
	// If heap already set up, pass to malloc instead
	if (heap_end) {
		uintptr_t address;

		// Depending on if asked to align, pass to valloc or malloc
		// NOTE: valloc is an extension
		if (align) address = (uintptr_t)aligned_alloc(0x1000, size);
		else address = (uintptr_t)malloc(size);

		// If given a physical address pointer, translate the virtual address
		//  we use to physical address and set `phys`
		if (phys) {
			// TODO: Handle page fault
			*phys = get_physical_address(address);
		}
		
		// Return the virtual address from allocation
		return address;
	}

	// If asked to align (and not already aligned), move to the next 4 KiB page
	//  boundary
	if (align && (placement_pointer & 0x00000FFF)) {
		placement_pointer &= 0xFFFFF000;
		placement_pointer += 0x1000;
	}

	// If given a physical address pointer, return the physical location that
	//  allocation took place
	if (phys)
		*phys = placement_pointer;
	
	// Return the address of the start of allocation, and update placement
	//  pointer for next allocation
	uintptr_t address = placement_pointer;
	placement_pointer += size;
	return address;
}

uintptr_t kmalloc(size_t size) {
	return kmalloc_real(size, 0, NULL);
}

uintptr_t kvmalloc(size_t size) {
	return kmalloc_real(size, 1, NULL);
}

uintptr_t kmalloc_p(size_t size, uintptr_t* phys) {
	return kmalloc_real(size, 0, phys);
}

uintptr_t kvmalloc_p(size_t size, uintptr_t* phys) {
	return kmalloc_real(size, 1, phys);
}
