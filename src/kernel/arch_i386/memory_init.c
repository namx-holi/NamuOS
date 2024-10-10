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
	(void)mb_info; // Multiboot info isn't needed for this architecture

	// Allocate space for the kernel page directory, and set all entries to
	//  read/write.
	uintptr_t pde_paddr;
	PDE_t* pde = (PDE_t*)kvmalloc_p(sizeof(PDE_t) * PTRS_PER_PDE, &pde_paddr);
	memset(pde, 0, sizeof(PDE_t) * PTRS_PER_PDE); // Clear all entries
	for (uint32_t i = 0; i < PTRS_PER_PDE; ++i)
		pde[i].rw = 1;
	
	// Allocate enough page tables for the entire linear mapping, and clear all
	//  entries for each table.
	// NOTE: This takes 4 MiB, make sure boot page tables are large enough!
	uintptr_t page_tables_paddr;
	PTE_t* page_tables = (PTE_t*)kvmalloc_p(
		sizeof(PDE_t)
		* PTRS_PER_PTE * PTRS_PER_PDE,
		&page_tables_paddr);
	memset(page_tables, 0, sizeof(PTE_t) * PTRS_PER_PTE * PTRS_PER_PDE);
	klog_debug(
		"paging - Allocated %d KiB for page tables\n",
		sizeof(PDE_t) * PTRS_PER_PTE * PTRS_PER_PDE / 1024);

	// Set up mapping for the kernel linear address space. Since the page tables
	//  are allocated contiguously, we can overflow the index into each table.
	for (uint32_t pfn = 0; pfn < (ZONE_HIGHMEM_OFFSET >> PAGE_SHIFT); ++pfn) {
		page_tables[pfn].present = 1; // This page points to a frame
		page_tables[pfn].global  = 1; // Accessible from anywhere, and shouldn't change
		page_tables[pfn].addr    = pfn; // Physical address / PAGE_SIZE
		
		// Check what region this frame points to and set read-write accordingly
		page_tables[pfn].rw = region_rw_permission((uintptr_t)pfn << PAGE_SHIFT);
	}
	klog_debug("paging - Page tables for kernel linear mapping set up\n");

	// Make this linear mapping addressable starting at PAGE_OFFSET.
	uint32_t start_pfn = PAGE_OFFSET >> PGDIR_SHIFT;
	for (uint32_t i = 0; i < ZONE_HIGHMEM_OFFSET/PGDIR_SIZE; ++i) {
		pde[start_pfn + i].present = 1;
		pde[start_pfn + i].addr    = ((uint32_t)page_tables_paddr >> PAGE_SHIFT) + i;
	}
	klog_debug("paging - Page directory for kernel linear mapping set up\n");

	// Set up the CR3 register to point to the physical address of the page
	//  directory. We don't need any CR3 flags for this (bit 3,4 for PWT, PCD).
	uint32_t cr3 = (uint32_t)pde_paddr;
	asm volatile ("mov %0, %%cr3" : : "a"(cr3) : "memory");

	// Enable paging (bit 31) and write protect (bit 16) with CR0 register
	// TODO: Do we need to still do this if paging already enabled?
	// uint32_t cr0;
	// asm volatile (
	// 	"mov %%cr0, %0; or $0x80010000, %0; mov %0, %%cr0;" : "=r"(cr0) : : "memory");

	// Now that paging is set up, we can set our global page directory! We're
	//  free of the boot page directory!
	pgd = pde;
	klog_debug("paging - Paging enabled for linear mapping\n");
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
	klog_debug(
		"memmap - Total number of frames on system: %d (%d MiB)\n",
		nb_frames, nb_frames * (PAGE_SIZE / 1024) / 1024);

	// Allocate space for the usage bitmap, and memmap itself. We'll set all
	//  frames as currently free and cleared so we can set them up from clean.
	// bitmap
	uintptr_t bitmap_paddr;
	uint32_t bitmap_size = nb_frames / 8; // 8 bits per byte
	uint32_t* new_bitmap = (uint32_t*)kvmalloc_p(bitmap_size, &bitmap_paddr);
	memset(new_bitmap, UINT32_MAX, bitmap_size);
	klog_debug("memmap - Allocated %d KiB for memmap bitmap\n", bitmap_size/1024);
	// memmap
	uintptr_t memmap_paddr;
	uint32_t memmap_size = nb_frames * sizeof(frame_t);
	frame_t* new_memmap = (frame_t*)kvmalloc_p(memmap_size, &memmap_paddr);
	memset(new_memmap, 0, memmap_size);
	klog_debug("memmap - Allocated %d KiB for memmap\n", memmap_size/1024);

	// We can now set the global bitmap and memmap, as we don't need any more
	//  allocations until they're set up
	memmap_bitmap = new_bitmap;
	memmap = new_memmap;

	// Everything allocated by the kernel so far must be reserved so that it
	//  isn't swapped out. We'll also mark those frames as used in the bitmap.
	uintptr_t memmap_end_paddr = (uintptr_t)memmap_paddr + memmap_size;
	for (uint32_t pfn = 0; pfn < (memmap_end_paddr >> PAGE_SHIFT); ++pfn) {
		memmap_bitmap_flag_used(pfn);
		memmap[pfn].reserved = 1;
	}
	klog_debug("memmap - Address range 0x0 to 0x%p reserved for kernel\n", memmap_end_paddr-1);

	// Step through the memory map from GRUB to flag reserved regions as such
	for (uint32_t i = 0; i < mmap_length; ++i) {
		multiboot_memory_map_t entry = mb_info->mmap_addr[i];

		// We'll only use regions explicitly marked as *AVAILABLE*. We *could*
		//  use ACPI reclaimable or non-volatile, but that's hard...
		if (entry.type == MULTIBOOT_MEMORY_AVAILABLE) continue;

		// Get the page frame numbers of the start and end of this entry, and
		//  mark as used, and to skip.
		uint32_t pfn_start = entry.base_addr >> PAGE_SHIFT;
		uint32_t pfn_end = (entry.base_addr + entry.length) >> PAGE_SHIFT;
		for (uint32_t pfn = pfn_start; pfn < pfn_end; ++pfn) {
			memmap_bitmap_flag_used(pfn);
			memmap[pfn].skip = 1;
			memmap[pfn].reserved = 1;
		}
		klog_debug(
			"memmap - Address range 0x%p to 0x%p skipped\n",
			pfn_start<<PAGE_SHIFT, (pfn_end<<PAGE_SHIFT) - 1);
	}

	// Finally, mark frames in ZONE_HIGHMEM as such
	for (uint32_t pfn = (ZONE_HIGHMEM_OFFSET >> PAGE_SHIFT); pfn < nb_frames; ++pfn) {
		memmap[pfn].highmem = 1;
	}
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
