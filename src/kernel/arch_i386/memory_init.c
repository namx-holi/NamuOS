/// @file memory_init.c

#include <namuos/memory.h> // Implements

#include <string.h> // memset
#include <arch_i386/paging.h>
#include <namuos/system.h>


// Addresses of linkages by `linker.ld`. Used to mark pages as read-only, and
//  flag frames as used.
extern void* _paddr_multiboot_header_start;
extern void* _paddr_multiboot_header_end;
extern void* _paddr_bootloader_start;
extern void* _paddr_bootloader_end;
extern void* _paddr_kernel_start;
extern void* _paddr_kernel_ro_start;
extern void* _paddr_kernel_ro_end;
extern void* _paddr_kernel_rw_start;
extern void* _paddr_kernel_rw_end;
extern void* _paddr_kernel_end;


// Our global page directory
PDE_t* pgd = NULL;

// Availability bitmap and array of frames
uint32_t* memmap_bitmap = NULL;
frame_t* memmap = NULL;


/** @brief Fetch the total number of frames on system given GRUB memory map
 * 
 * Finds the largest address span for the given memory map, and uses that to
 * estimate the total number of frames on the system.
 * 
 * @param mmap GRUB memory map
 * @param nb_entries Number of entries in memory map (not total size)
 * 
 * @returns Estimate number of frames
*/
uint32_t get_nb_frames(multiboot_memory_map_t* mmap, uint32_t nb_entries);

/// Helper that returns 0 if address falls in read-only region, 1 otherwise
int region_rw_permission(uintptr_t paddr);



void memory_paging_initialise(multiboot_info_t* mb_info) {
	// TODO: Create the entire linear mapping here? Or just keep to 8 MiB?

	(void)mb_info; // Multiboot info isn't needed for this architecture

	// Allocate space for the kernel page directory, and set all entries to
	//  read/write.
	uintptr_t pde_paddr;
	PDE_t* pde = (PDE_t*)kvmalloc_p(sizeof(PDE_t) * PTRS_PER_PDE, &pde_paddr);
	memset(pde, 0, sizeof(PDE_t) * PTRS_PER_PDE); // Clear all entries
	for (uint32_t i = 0; i < PTRS_PER_PDE; ++i)
		pde[i].rw = 1;

	// Allocate space for 8 MiB worth of page tables to map the kernel image.
	//  This should be enough to set up OS paging.
	uintptr_t pg0_paddr;
	uintptr_t pg1_paddr;
	PTE_t* pg0 = (PTE_t*)kvmalloc_p(sizeof(PTE_t) * PTRS_PER_PTE, &pg0_paddr);
	PTE_t* pg1 = (PTE_t*)kvmalloc_p(sizeof(PTE_t) * PTRS_PER_PTE, &pg1_paddr);
	memset(pg0, 0, sizeof(PTE_t) * PTRS_PER_PTE); // Clear all entries
	memset(pg1, 0, sizeof(PTE_t) * PTRS_PER_PTE); // Clear all entries

	// Map pg0 and pg1 to the first 8 MiB of physical memory, 4 MiB per table.
	// NOTE: BEcause pg1 is allocated immediately after pg0 using the boot
	//  allocator, we can overflow into pg1 from pg0.
	for (uint32_t i = 0; i < PTRS_PER_PTE * 2; ++i) {
		pg0[i].present = 1; // This page points to a frame
		pg0[i].global  = 1; // Accessible from anywhere, and shouldn't change
		pg0[i].addr    = i; // The PFN, physical addres / PAGE_SIZE

		// Check what region this page points to and set read-write accordingly
		pg0[i].rw = region_rw_permission((uintptr_t)i << PAGE_SHIFT);
	}

	// Make the first 8 MiB addressable from the start of kernel space, denoted
	//  by PAGE_OFFSET. We also need an identity mapping that we remove later
	//  just so everything doesn't immediately break when paging is enabled.
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

	// Set up the CR3 register to point to the physical address of our global
	//  page directory. We don't need any CR3 flags (bit 3,4 for PWT,PCD).
	uint32_t cr3 = (uint32_t)pde_paddr;
	asm volatile ("mov %0, %%cr3" : : "a"(cr3) : "memory");

	// Enable paging (bit 31) and write protect (bit 16) with CR0 register
	uint32_t cr0;
	asm volatile (
		"mov %%cr0, %0; or $0x80010000, %0; mov %0, %%cr0;" : "=r"(cr0) : : "memory");
	
	// Paging is now set up! Get rid of the identity mapping and invalidate
	//  those addresses.
	pde[0].present = 0;
	pde[0].addr    = 0;
	pde[1].present = 0;
	pde[1].addr    = 0;
	invalidate_page(0 << PAGE_SHIFT);
	invalidate_page(1 << PAGE_SHIFT);

	// Now that paging is set up, we can set our global page directory! We're
	//  free of the boot page directory!
	pgd = pde;
}

void memory_memmap_initialise(multiboot_info_t* mb_info) {
	// If memory map wasn't produced by the bootloader, we can't set up memmap
	if (!(mb_info->flags & MULTIBOOT_FLAG_MMAP))
		panic("Memory map not provided by bootloader!\n");
	
	// `mmap_length` is the **total** size of the array, rather than the number
	//  of indices. Divide by size of entry to get number of actual entries.
	uint32_t mmap_length = mb_info->mmap_length / sizeof(multiboot_memory_map_t);

	// Find the total number of frames on the system
	uint32_t nb_frames = get_nb_frames(mb_info->mmap_addr, mmap_length);
	klog_debug("Total number of frames on system: %d (%d MiB)\n", nb_frames, nb_frames/256);

	// Allocate space for the frame usage bitmap. Each byte can represent 8
	//  frames, so we need (nb_frames/8) bytes to represent every frame
	uintptr_t bitmap_paddr;
	memmap_bitmap = (uint32_t*)kvmalloc_p(nb_frames/8, &bitmap_paddr);
	memset(memmap_bitmap, UINT32_MAX, nb_frames/8); // Mark every frame as free

	// As memmap_bitmap has been allocated in kernel-space, the end of this
	//  array is the end of any allocations so far (just allocates linearly).
	//  We can mark all frames up to the physical address of the end of
	//  memmap_bitmap as used. This includes any BIOS stuff that appears before
	//  the kernel image.
	uintptr_t bitmap_end_paddr = bitmap_paddr + nb_frames/8;
	klog_debug("End physical address of memmap_bitmap is 0x%p\n", bitmap_end_paddr-1);
	for (uint32_t pfn = 0; pfn < bitmap_end_paddr/PAGE_SIZE; ++pfn)
		memmap_bitmap_flag_used(pfn);
	
	// Step through the memory map from GRUB to flag reserved regions as used
	for (uint32_t i = 0; i < mmap_length; ++i) {
		multiboot_memory_map_t entry = mb_info->mmap_addr[i];

		// Only if a region is marked explicitly as *AVAILABLE* can we use it.
		// NOTE: We could refine this by using ACPI reclaimable or non-volatile,
		//  but that's hard...
		if (entry.type == MULTIBOOT_MEMORY_AVAILABLE) continue;

		// Get the page frame numbers of the start and end addresses of this
		//  range, and mark all frames as used
		uint32_t start_pfn = entry.base_addr / PAGE_SIZE;
		uint32_t end_pfn = (entry.base_addr + entry.length) / PAGE_SIZE;
		for (uint32_t pfn = start_pfn; pfn < end_pfn; ++pfn)
			memmap_bitmap_flag_used(pfn);
	}

	// TODO: Continue this
	klog_warning("TODO: Finish memory_memmap_initialise\n");
}



// Methods declared in this file:

uint32_t get_nb_frames(multiboot_memory_map_t* mmap, uint32_t nb_entries) {
	// FIXME: We'll assume the first address in mmap is 0x0

	// We search for the largest mentioned address in the mmap. This should give
	//  us an idea of how large the memory space is. It may not work perfectly,
	//  but in the context of using qemu, this works.
	uint64_t largest_address = 0x00000000;
	for (uint32_t i = 0; i < nb_entries; ++i) {
		// Check if this entry spans past our currently largest-seen address
		uint64_t addr = mmap[i].base_addr + mmap[i].length;
		if (addr > largest_address)
			largest_address = addr;
	}

	// Since we want to return the number of frames, divide by frame size
	return largest_address / PAGE_SIZE;
}

int region_rw_permission(uintptr_t paddr) {
	// The real mode address space (up to 1 MiB) contains a number of buffers
	//  we can write to (e.g. video display memory). We'll mark up to the start
	//  of kernel image as read-write
	if (paddr < (uintptr_t)&_paddr_kernel_start)
		return 1;
	
	// Addresses in the kernel image that are read-only are marked appropriately
	//  (e.g. .text and .rodata)
	else if (paddr < (uintptr_t)&_paddr_kernel_ro_end)
		return 0;
	
	// Similarly, mark read-write regions appropriately (e.g. .data and .bss)
	else if (paddr < (uintptr_t)&_paddr_kernel_rw_end)
		return 1;

	// We'll mark else past the end of the kernel image as read-write. We could
	//  mark as read-only and then update that when we allocate memory, but
	//  there's really no need.
	return 1;
}
