#include <kernel/memory.h> // Implements

#include <stddef.h> // size_t
#include <string.h> // memset

#include <kernel/system.h> // Update kprintf pointers, and panic


// Physical addresses of regions of the kernel image. These are used to mark
//  regions as read-only.
extern void* kernel_addr;      // Start address of the kernel image
extern void* code_end_addr;    // End of read-only kernel image region
extern void* kernel_end_addr;  // End address of kernel image

// Global memory map, and page directory
uint32_t* memmap_bitmap = NULL;
// TODO: memmap
PDE_t* pgd = NULL;


// Test method to check if we are setting the correct addresses in page table
uintptr_t test_translate(void* addr) {
	kprintf("test_translate: PGD paddr = 0x%p\n", pgd);

	uintptr_t pde_paddr = (uintptr_t)(pgd) | (((uintptr_t)addr >> 22) << 2);
	kprintf("test_translate: pde_paddr = 0x%p\n", pde_paddr);
	PDE_t* pde = (PDE_t*)pde_paddr;
	if (!pde->present) {
		kprintf("test_translate: Page fault for that PDE\n");
		return 0;
	}

	kprintf("test_translate: pde->addr = 0x%p\n", pde->addr);
	uintptr_t pte_paddr = (pde->addr << 12) | ((((uintptr_t)addr >> 12) & 0x3ff) << 2);
	kprintf("test_translate: pte_paddr = 0x%p\n", pte_paddr);
	PTE_t* pte = (PTE_t*)pte_paddr;
	if (!pte->present) {
		kprintf("test_translate: Page fault for that PTE\n");
		return 0;
	}

	kprintf("test_translate: pte->addr = 0x%p\n", pte->addr);
	uintptr_t paddr = (pte->addr << 12) | ((uintptr_t)addr & 0x3ff);
	kprintf("test_translate: paddr = 0x%p\n", paddr);

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
	klog_debug("Kernel ro-data spans 0x%p - 0x%p\n",
		(uintptr_t)&kernel_addr, (uintptr_t)&code_end_addr-1);
	klog_debug("Kernel rw-data spans 0x%p - 0x%p\n",
		(uintptr_t)&code_end_addr, (uintptr_t)&kernel_end_addr-1);

	// Allocate space for the kernel page directory. Each entry is one uint32.
	uintptr_t pde_paddr;
	PDE_t* pde = (PDE_t*)kvmalloc_p(sizeof(uint32_t) * PTRS_PER_PDE, &pde_paddr);
	memset(pde, 0, sizeof(uint32_t) * PTRS_PER_PDE); // Clear all entries
	for (uint32_t i = 0; i < PTRS_PER_PDE; ++i) {
		pde[i].rw = 1; // Mark all pages as read/write
	}

	// Allocate space for 8 MiB worth of page tables to map the kernel image.
	//  This should be enough for just setting up paging. Each entry is one
	//  uint32.
	uintptr_t pg0_paddr;
	uintptr_t pg1_paddr;
	PTE_t* pg0 = (PTE_t*)kvmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg0_paddr);
	PTE_t* pg1 = (PTE_t*)kvmalloc_p(sizeof(uint32_t) * PTRS_PER_PTE, &pg1_paddr);
	memset(pg0, 0, sizeof(uint32_t) * PTRS_PER_PTE); // Clear all entries
	memset(pg1, 0, sizeof(uint32_t) * PTRS_PER_PTE); // Clear all entries

	// Map pg0 and pg1 to the first 8 MiB of physical memory, 4 MiB per table.
	// NOTE: Because pg1 is allocated immediately after pg0 using the boot
	//  allocator, and both have the size of one page, we can overflow into pg1
	//  from pg0. This is why we iterate `PTRS_PER_PTE*2` times.
	for (uint32_t i = 0; i < PTRS_PER_PTE; ++i) {
		pg0[i].present = 1; // This page points to a frame
		pg0[i].global  = 1; // Accessible from anywhere, and shouldn't change
		pg0[i].addr    = i; // The PFN, physical address / PAGE_SIZE

		// Check where this address falls in kernel image for read-write perms
		uintptr_t paddr = i << PAGE_SHIFT;
		if (paddr < (uintptr_t)&kernel_addr) {
			// Real-mode address space before kernel image. Contains some BIOS
			//  data that we need to write to for VGA
			pg0[i].rw = 1;
		} else if (paddr < (uintptr_t)&code_end_addr) {
			// This is the multiboot header, .text region, and .rodata region.
			pg0[i].rw = 0;
		} else if (paddr < (uintptr_t)&kernel_end_addr) {
			// This is the .data and .bss regions. Need to be able to write.
			pg0[i].rw = 1;
		} else {
			// Anything else after the kernel image. This includes the page
			//  tables, so mark as writable.
			pg0[i].rw = 1;
		}
	}

	// Map pg0 and pg1 to the beginning of kernel-space so the first 8 MiB of
	//  virtual addresses starting with 0xC... map to first 8 MiB of physical
	//  memory. We also need to set up an identity mapping so we don't break
	//  everything immediately after swapping over to paging. The identity
	//  mapping is removed later.
	// Kernel-space mapping (vaddr = paddr + PAGE_OFFSET)
	uint32_t start_pfn = PAGE_OFFSET >> PGDIR_SHIFT;
	pde[start_pfn].present   = 1;
	pde[start_pfn].addr      = (uint32_t)pg0_paddr >> PAGE_SHIFT;
	pde[start_pfn+1].present = 1;
	pde[start_pfn+1].addr    = (uint32_t)pg1_paddr >> PAGE_SHIFT;
	// Identity mapping (vaddr = paddr)
	pde[0].present = 1;
	pde[0].addr    = (uint32_t)pg0_paddr >> PAGE_SHIFT;
	pde[1].present = 1;
	pde[1].addr    = (uint32_t)pg1_paddr >> PAGE_SHIFT;
	kprintf(" [*] Mapping first 8 MiB of Kernel Image... (0x%p - 0x%p)\n",
		0, 0 + 8*1024*1024);

	// Set up the CR3 register to point to the physical address of our global
	//  page directory. We don't need any CR3 flags (bit 3,4 for PWT,PCD).
	uint32_t cr3 = (uint32_t)pde_paddr;
	asm volatile ("mov %0, %%cr3" : : "r"(cr3));

	// Enable paging (bit 31) and write protect (bit 16) with CR0 register
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0)); // Copy current flags
	cr0 |= 0x80000000; //0x80010000; // Set bits for paging and write protect
	asm volatile ("mov %0, %%cr0" : : "r"(cr0));

	// Update the VGA memory address so we use the new virtual address instead
	//  of virtual address, and update boot allocator so we can still use it
	//  with virtual addresses!
	kernel_kprintf_update_page_offset(PAGE_OFFSET);
	update_boot_allocator_for_paging();

	// Paging is now set up, rid ourselves of the identity mapping and
	//  invalidate those addresses.
	pde[0].present = 0;
	pde[0].addr    = 0;
	pde[1].present = 0;
	pde[1].addr    = 0;
	invlpg(0 << PAGE_SHIFT);
	invlpg(1 << PAGE_SHIFT);

	// Now that paging is set up, we can set our global page directory!
	pgd = pde;
	kprintf(" [*] Paging enabled!\n");
}

void memory_memmap_setup(multiboot_info_t* mb_info) {
	// Since paging is enabled, we need to shift any address we have from the
	//  multiboot structure by PAGE_OFFSET to access it
	mb_info = (multiboot_info_t*)((uintptr_t)mb_info + PAGE_OFFSET);

	// If memory map wasn't provided by bootloader, we can't set up memmap.
	if (!(mb_info->flags & MULTIBOOT_FLAG_MMAP))
		panic("Memory map not provided by bootloader!\n");
	
	// Similar to mb_info, the address given by mmap_addr needs to be shifted.
	//  The given mmap_length is also the total size of the array rather than
	//  number of indices, so adjust this so we can index easily
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)((uintptr_t)mb_info->mmap_addr + PAGE_OFFSET);
	uint32_t mmap_length = mb_info->mmap_length / sizeof(multiboot_memory_map_t);

	// Using the helper function, find the total number of frames present on the
	//  system.
	uint32_t nb_frames = get_nb_frames(mmap, mmap_length);
	klog_debug("Total number of frames on system: %d (%d MiB)\n", nb_frames, nb_frames/256);

	// Allocate space for the frame usage bitmap. Each byte can represent 8
	//  frames, so we need nb_frames/8 bytes to represent every frame.
	uintptr_t bitmap_paddr;
	memmap_bitmap = (uint32_t*)kvmalloc_p(nb_frames / 8, &bitmap_paddr);
	memset(memmap_bitmap, UINT32_MAX, nb_frames/8); // Mark every frame as free

	// Mark all the frames up to the end of memmap_bitmap as currently used.
	//  This includes the whole kernel image and any BIOS data, so we actually
	//  end up marking that as used twice when we iterate through memory map.
	uintptr_t bitmap_end_paddr = bitmap_paddr + nb_frames/8;
	for (uint32_t pfn = 0; pfn < bitmap_end_paddr/PAGE_SIZE; ++pfn)
		memmap_mark_used(pfn);
	
	// TODO: Test if we've got the right frames here

	// Step through the mempry map to flag any reserved regions as used
	for (uint32_t i = 0; i < mmap_length; ++i) {
		// Only if a region is marked explicitly as *AVAILABLE* can we use it.
		// NOTE: We could refine this by using ACPI reclaimable or non-volatile,
		//  but that's hard...
		if (mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) continue;

		// Get the page frame numbers of the start and end address of this
		//  range, and mark all frames as reserved
		uint32_t start_pfn =  mmap[i].base_addr / PAGE_SIZE;
		uint32_t end_pfn   = (mmap[i].base_addr + mmap[i].length) / PAGE_SIZE;
		for (uint32_t pfn = start_pfn; pfn < end_pfn; ++pfn)
			memmap_mark_used(pfn);
	}


	// TODO: Anything else?
	// DEBUG: Try allocate 16 bytes per frame. This could be space for type (1),
	//  flags (4), index (4), some other stuff (7)
	kprintf("TEST: Allocating a test memmap...\n");
	uintptr_t memmap_paddr;
	char* memmap = (char*)kvmalloc_p(nb_frames * 16, &memmap_paddr);
	kprintf("TEST: memmap paddr = 0x%p\n", memmap_paddr);
	kprintf("TEST: memmap vaddr = 0x%p\n", memmap);
	kprintf("TEST: memmap size  = %d B\n", nb_frames * 16);

	// See if we can set a value in this range after alloc - this confirms
	//  paging was successfully set up.
	uintptr_t memmap_end_vaddr = (uintptr_t)memmap + nb_frames * 16 - 1;
	PTE_t* page = get_page(memmap_end_vaddr);
	uintptr_t frame_paddr = page->addr << PAGE_SHIFT;
	kprintf("TEST: memmap last page frame address is 0x%p\n", frame_paddr);
	kprintf("TEST: memmap translated address is 0x%p\n", frame_paddr + (memmap_end_vaddr%PAGE_SIZE));
	memmap[nb_frames*16 - 1] = 1;
	kprintf("TEST: Set value at index %d to 1\n", 1);
	kprintf("TEST: Trying to print nb_frames: %d\n", nb_frames);
	kprintf("TEST: Set value at index %d to 1\n", nb_frames*16-1);

	// // TODO: Continue this

	kprintf("TEST: memmap successfully initialised!\n");
	// while(1){}
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
