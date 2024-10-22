/// @file kmalloc.c

#include <namuos/memory.h> // Implements

#include <arch_i386/paging.h>


// Boot memory allocation takes place immediately after end of kernel code
extern void* _paddr_kernel_end;
static uintptr_t placement_pointer = (uintptr_t)&_paddr_kernel_end;

/** @brief Generic i386 kernel memory allocator
 * 
 * Generic i386 kernel memory allocator. Will allocate memory in the next
 * available space in the kernel-space linear mapped region. Handles updating
 * `memmap` if it's set up yet.
 * 
 * @param size Size of block to allocate
 * @param align Set to non-zero if needs to be page-aligned
 * @param paddr_ptr Optional pointer to store physical address of allocation
 * 
 * @returns Pointer to allocated block (virtual address)
*/
void* kmalloc_real(size_t size, int align, uintptr_t* paddr_ptr);



void* kmalloc(size_t size) {
	return kmalloc_real(size, 0, NULL); // Not aligned, no paddr
}

void* kvmalloc(size_t size) {
	return kmalloc_real(size, 1, NULL); // Aligned, no paddr
}

void* kmalloc_p(size_t size, uintptr_t* paddr_ptr) {
	return kmalloc_real(size, 0, paddr_ptr); // Not aligned, given paddr
}

void* kvmalloc_p(size_t size, uintptr_t* paddr_ptr) {
	return kmalloc_real(size, 1, paddr_ptr); // Aligned, given paddr
}



// Methods declared in this file:

void* kmalloc_real(size_t size, int align, uintptr_t* paddr_ptr) {
	// If asked to align, move the placement pointer to the next page boundary
	if (align)
		placement_pointer = PAGE_ALIGN(placement_pointer);

	uintptr_t paddr = placement_pointer;
	void* vaddr = __to_virt(placement_pointer);

	// // If operating system paging is set up, need to handle extending the paged
	// //  region if needed
	// if (pgd) {
	// 	// TODO: Handle extending region
	// }

	// If the memory map has been initialised, update frames as used
	// TODO: Handle marking frames as used

	// If we were given a pointer, store the physical allocation address in it
	if (paddr_ptr)
		*paddr_ptr = paddr;
	
	// Move to the next physical allocation address, and return virtual address
	//  of the allocation
	placement_pointer += size;
	return vaddr;
}
