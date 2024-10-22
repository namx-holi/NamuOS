/// @file bootmem.c

#include <arch_i386/bootmem.h> // Implements

#include <string.h> // memset
#include <arch_i386/paging.h>
#include <namuos/memory.h>
#include <namuos/system.h>


bootmem_data_t bootmem_data;

// Our boot memory allocation can start at the earliest after the end of kernel
//  code.
extern void* _paddr_kernel_end;


// Helpers to set, clear, and test bitmap bits
void bitmap_set(uint32_t pfn);
void bitmap_clear(uint32_t pfn);
char bitmap_test(uint32_t pfn);

// Actual allocation method, wrapped by bootmem methods
void* __bootmem_alloc(size_t size, uint32_t align, uintptr_t goal);


// TODO: Extern these, and have them set in setup?
uint32_t min_low_pfn; // Lowest PFN that is available in the system
uint32_t max_low_pfn; // Highest PFN that may be addressed by low memory (ZONE_NORMAL)
uint32_t highstart_pfn; // PFN of the beginning of high memory (ZONE_HIGHMEM)
uint32_t highend_pfn; // Last PFN in high memory
uint32_t max_pfn; // Last PFN available to the system


void bootmem_initialise(multiboot_info_t* mb_info) {
	// TODO: Use multiboot info to make sure we've got enough actual RAM to span
	//  to ZONE_NORMAL, and set the bitmap for reserved regions.
	(void)mb_info;

	// Set up the boot allocator to span from start of mem to end of ZONE_NORMAL
	bootmem_data.pfn_start = 0;
	bootmem_data.pfn_end = ZONE_HIGHMEM_OFFSET / PAGE_SIZE;
	bootmem_data.last_pfn = bootmem_data.pfn_start;
	bootmem_data.last_offset = 0;

	// Round up the number of bytes needed to map all the PFNs in bitmap, and
	//  find the address of the first frame after kernel image for allocation.
	uint32_t bitmap_size = (bootmem_data.pfn_end - bootmem_data.pfn_start + 7) / 8;
	uint32_t bitmap_alloc_pfn = ((uintptr_t)&_paddr_kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;

	// Make a naive allocation for the bitmap, and clear all bits to free the
	//  frames. We'll say the last offset is 0 to indicate we should dedicate
	//  all used pages as just for the bitmap so freeing is easier.
	bootmem_data.bitmap = (char*)__to_virt(bitmap_alloc_pfn << PAGE_SHIFT);
	bootmem_data.last_pfn = bitmap_alloc_pfn + (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
	bootmem_data.last_offset = 0;
	memset(bootmem_data.bitmap, 0, bitmap_size);
	klog_debug("bitmap address = 0x%p\n", bootmem_data.bitmap);
	klog_debug("bitmap size = 0x%x\n", bitmap_size);
	klog_debug("bootmem last_pfn = %d\n", bootmem_data.last_pfn);

	// Reserve all the frames in the kernel image up to the end of bitmap
	bootmem_reserve(0x00000000, __to_phys(bootmem_data.bitmap) + bitmap_size);
}

void bootmem_reserve(uintptr_t paddr, size_t size) {
	// We need to mark any frames that are spanned by the address range as used.
	//  Round the start PFN down, and round the end PFN up.
	uint32_t pfn_start = paddr / PAGE_SIZE;
	uint32_t pfn_end = (paddr + size + PAGE_SIZE - 1) / PAGE_SIZE;

	// Set all page frames from pfn_start up to pfn_end as reserved
	for (uint32_t pfn = pfn_start; pfn < pfn_end; ++pfn)
		bitmap_set(pfn);
	// NOTE: Could possibly do this in bulk, setting whole bytes
}

void bootmem_free(uintptr_t paddr, size_t size) {
	// We can only mark frames as free if they are *fully* freed by the given
	//  address range. Round the start PFN up, and round the end PFN down.
	uint32_t pfn_start = (paddr + PAGE_SIZE - 1) / PAGE_SIZE;
	uint32_t pfn_end = (paddr + size) / PAGE_SIZE;

	// Clear all pages from pfn_start to pfn_end. If any are already cleared,
	//  panic for double free.
	for (uint32_t pfn = pfn_start; pfn < pfn_end; ++pfn) {
		if (bitmap_test(pfn) == 0)
			panic("Bootmem double free for PFN %d\n", pfn);
		bitmap_clear(pfn);
	}
	// NOTE: Could possibly do this in bulk, clearing whole bytes
}

void* bootmem_alloc(size_t size) {
	// Call actual allocator with the goal of allocating within ZONE_NORMAL
	// TODO: Align with L1 cache size
	return __bootmem_alloc(size, 1, ZONE_NORMAL_OFFSET);
}

void* bootmem_alloc_low(size_t size) {
	// Call actual allocator with the goal of allocating within ZONE_DMA
	// TODO: Align with L1 cache size
	return __bootmem_alloc(size, 1, ZONE_DMA_OFFSET);
}

void* bootmem_aligned_alloc(size_t size) {
	// Call actual allocator with the goal of allocating within ZONE_NORMAL and
	//  page aligned
	return __bootmem_alloc(size, PAGE_SIZE, ZONE_NORMAL_OFFSET);
}

void bootmem_free_all() {
	panic("bootmem_free_all() not implemented!\n");

	// TODO: For all unallocated pages known to the allocator:
	//  clear PG_reserved flag
	//  set count to 1
	//  call __free_pages() so buddy allocator can build its free lists
	// TODO: Free all pages used for bitmap and give them to buddy allocator
}

/*
// TODO: Use as reference
void* old_aligned_alloc(size_t size) {
	// TODO: Search bitmap for available space

	// We need to start on a new frame. If the next allocation space is offset,
	//  move to the next frame.
	if (bootmem_data.next_offset) {
		bootmem_data.next_offset = 0;
		bootmem_data.next_pfn += 1;
	}

	// If we don't have enough free frames for the allocation, panic!
	uint32_t needed_frames = (size + PAGE_SIZE - 1) / PAGE_SIZE; // Rounded up
	if ((bootmem_data.next_pfn + needed_frames) >= bootmem_data.pfn_end)
		panic("Not enough space for bootmem allocation\n");
	
	// Create an allocation!
	void* alloc = (void*)__to_virt(bootmem_data.next_pfn << PAGE_SHIFT);
	bootmem_reserve(__to_phys(alloc), size);
	bootmem_data.next_pfn += size / PAGE_SIZE;
	bootmem_data.next_offset = size % PAGE_SIZE;
	return alloc;
}
*/



// Methods declared in this file:

void bitmap_set(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Mark that bit as 1
	bootmem_data.bitmap[index] |= (char)1 << bit_index;
}

void bitmap_clear(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Mark that bit as 0
	bootmem_data.bitmap[index] &= ~((char)1 << bit_index);
}

char bitmap_test(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Check if bit is 1
	return bootmem_data.bitmap[index] & ((char)1 << bit_index);
}

void* __bootmem_alloc(size_t size, uint32_t align, uintptr_t goal) {
	panic_new("__bootmem_alloc(%d, %d, 0x%p) not implemented!\n", size, align, goal);

	// TODO: Linearly scan bitmap from `goal` for a block of memory large enough
	//  to satisfy the allocation.

	// TODO: Check if allocation can be merged:
	//  - If last allocation is adjacent to found page (bootmem_data->last_pfn)
	//  - If last page has space in it (bootmem_data->offset != 0)
	//  - If alignment is less than PAGE_SIZE

	// TODO: Regardless of whether allocations are merged or not, pos and offset
	//  are updated to show last page used for allocating and how much of page
	//  was used (0 for fully used).
}
