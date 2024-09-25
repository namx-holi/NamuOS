#include <kernel/memory.h> // Implements

#include <stddef.h> // size_t
#include <string.h> // memset

#include <kernel/system.h> // Update kprintf pointers, and panic


// Physical addresses of regions of the kernel image. These are used to mark
//  regions as read-only.
extern void* kernel_addr;      // Start address of the kernel image
extern void* code_end_addr;    // End of read-only kernel image region
extern void* kernel_end_addr;  // End address of kernel image

// Used for boot memory allocator. We can allocate memory immediately after the
//  end of the kernel image.
uintptr_t placement_pointer = (uintptr_t)&kernel_end_addr;

// The global memory bitmap, and page directory
uint32_t* memmap_bitmap;
PDE_t* pgd;


// Boot allocators
uintptr_t boot_malloc_real(size_t size, int align, uintptr_t* paddr) {
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
uintptr_t boot_malloc(size_t size) {
	return boot_malloc_real(size, 0, NULL); // Not aligned, no phys address
}
uintptr_t boot_vmalloc(size_t size) {
	return boot_malloc_real(size, 1, NULL); // Aligned, no phys address
}
uintptr_t boot_malloc_p(size_t size, uintptr_t* paddr) {
	return boot_malloc_real(size, 0, paddr); // Not aligned, phys address
}
uintptr_t boot_vmalloc_p(size_t size, uintptr_t* paddr) {
	return boot_malloc_real(size, 1, paddr); // Aligned, phys address
}


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

// Helper to get the total number of frames in a system
uint32_t get_nb_frames(multiboot_memory_map_t* mmap, uint32_t nb_entries) {
	// NOTE: We'll assume the first address in mmap is 0x00000000.

	// We search for the largest mentioned address in the mmap. This should give
	//  us an idea of how large the memory space is. It may not work perfectly,
	//  but in the context of using qemu, this works.
	uint64_t largest_address = 0x00000000;
	for (uint32_t i = 0; i < nb_entries; ++i) {
		// Check if this entry spans past our currently largest-seen address
		uint64_t addr = mmap[i].base_addr;
		addr += mmap[i].length;
		if (addr > largest_address) largest_address = addr;
	}

	// Since we need to return the number of frames, divide by frame size (4KiB)
	return largest_address / PAGE_SIZE;
}






void memory_paging_setup() {
	kprintf("Setting up paging...\n");

	// Allocate space for the kernel page directory. Each page is one uint32
	uintptr_t pgd_paddr;
	pgd = (PDE_t*)boot_vmalloc_p(sizeof(uint32_t) * PTRS_PER_PDE, &pgd_paddr);
	kprintf("  PGD physical address is 0x%p\n", pgd);
	memset(pgd, 0, sizeof(uint32_t) * PTRS_PER_PDE); // Clear all pages
	for (uint32_t i = 0; i < PTRS_PER_PDE; ++i) {
		pgd[i].rw = 1; // Mark all pages as read/write
	}

	// Allocate space for 8 MiB worth of page tables to map the kernel image
	uintptr_t pg0_paddr;
	uintptr_t pg1_paddr;
	PTE_t* pg0 = (PTE_t*)boot_vmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg0_paddr);
	PTE_t* pg1 = (PTE_t*)boot_vmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg1_paddr);
	kprintf("  PG0 physical address is 0x%p\n", pg0);
	kprintf("  PG1 physical address is 0x%p\n", pg1);
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

	// Update the VGA memory address so we use the virtual address, and
	//  placement pointer so we can still boot-allocate
	kernel_kprintf_update_page_offset(PAGE_OFFSET);
	placement_pointer += PAGE_OFFSET;

	// Paging is now set up, rid outselves of the identity mapping and
	//  invalidate those addresses.
	pgd[0].present = 0;
	pgd[0].addr = 0;
	pgd[1].present = 0;
	pgd[1].addr = 0;
	invlpg(0 << PAGE_SHIFT);
	invlpg(1 << PAGE_SHIFT);
}

void memory_node_setup(multiboot_info_t* mb_info) {
	kprintf("Setting up memory map...\n");
	// NOTE: Sets up a very basic bitmap of available frames

	// Since paging is enabled, we need to shift any address we have from
	//  mb_info by PAGE_OFFSET
	mb_info = (multiboot_info_t*)((uintptr_t)mb_info + PAGE_OFFSET);

	// If a memory map wasn't provided by the bootloader, we can't set up nodes
	if (!(mb_info->flags & MULTIBOOT_FLAG_MMAP)) panic("MMAP not provided by boot loader\n");

	// Similar to mb_info, we need to shift the address of mmap_addr by
	//  PAGE_OFFSET to access it.
	multiboot_memory_map_t* mmap = \
		(multiboot_memory_map_t*)((uintptr_t)mb_info->mmap_addr + PAGE_OFFSET);

	// Fetch the number of frames present on the system. `mmap_length` is the
	//  *total* size of the array, so we need to divide by size of elements to
	//  use it as an index rane.
	uint32_t mmap_length = mb_info->mmap_length / sizeof(multiboot_memory_map_t);
	uint32_t nb_frames = get_nb_frames(mmap, mmap_length);
	kprintf("  Total number of frames on system: %d (%d MiB)\n", 
		nb_frames, nb_frames / 256);

	// Allocate space for a bitmap for every possible frame on the system. We
	//  use uint32s for the bitmap, so we need nb_frames/4 bytes of space.
	uintptr_t memmap_bitmap_paddr;
	memmap_bitmap = (uint32_t*)boot_vmalloc_p(nb_frames / 4, &memmap_bitmap_paddr);
	memset(memmap_bitmap, UINT32_MAX, nb_frames/4); // Mark every frame as free
	kprintf("  Bitmap addr = 0x%p, placement pointer now 0x%p\n", memmap_bitmap, placement_pointer);

	// Mark everything up to the current placement pointer as used. This will
	//  prevent us from accidentally allocating BIOS regions or video memory.
	uintptr_t placement_paddr = placement_pointer - PAGE_OFFSET;
	for (uint32_t pfn = 0; pfn < placement_paddr/PAGE_SIZE; ++pfn)
		memmap_mark_used(pfn);
	kprintf("  Marked PFN 0x%x to 0x%x as used (%d frames)\n",
		0, placement_paddr/PAGE_SIZE-1, placement_paddr/PAGE_SIZE-1);
	
	// Just as a test, check if first kernel image pfn is used
	// kprintf("testing frame 0x100 (start of kernel image): %d\n", memmap_test(0x100));
	// kprintf("testing end of placement pointer: %d\n", memmap_test(placement_paddr / PAGE_SIZE));
	// kprintf("test just before placement pointer: %d\n", memmap_test(placement_paddr / PAGE_SIZE - 1));

	// Step through the memory map to flag any reserved regions as used
	for (uint32_t i = 0; i < mmap_length; ++i) {
		// Only if a region is marked explicitly as *AVAILABLE* can we use it.
		//  We could refine this by using ACPI reclaimable or non-volatile
		//  regions, but that's hard...
		if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) continue;

		// Turn the start and end address into page frame numbers, and iterate
		//  through those frames to mark as reserved
		uint32_t start_pfn = mmap[i].base_addr / PAGE_SIZE;
		uint32_t end_pfn = (mmap[i].base_addr + mmap[i].length) / PAGE_SIZE;
		for (uint32_t pfn = start_pfn; pfn < end_pfn; ++pfn)
			memmap_mark_used(pfn);
		
		kprintf("  Marked PFN 0x%x to 0x%x as used (%d frames)\n",
			start_pfn, end_pfn-1, end_pfn-start_pfn);
	}

	// TODO: Anything else we need to do here?
}

void memmap_mark_used(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Mark that bit as 0
	memmap_bitmap[index] &= ~((uint32_t)1 << bit_index);
}

void memmap_mark_free(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Mark that bit as 1
	memmap_bitmap[index] |= (uint32_t)1 << bit_index;
}

uint32_t memmap_test(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Check if bit is 1
	return memmap_bitmap[index] & ((uint32_t)1 << bit_index);
}
