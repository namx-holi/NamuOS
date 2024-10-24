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

	// Reserve all the frames in the kernel image up to the end of bitmap
	bootmem_reserve(0x00000000, __to_phys(bootmem_data.bitmap) + bitmap_size);

	klog_debug(
		"Initialised boot allocator up to physical address 0x%p\n",
		bootmem_data.pfn_end << PAGE_SHIFT);
}

#define _LOG_BOOTMEM_RESERVE 0
void bootmem_reserve(uintptr_t paddr, size_t size) {
	// We need to mark any frames that are spanned by the address range as used.
	//  Round the start PFN down, and round the end PFN up.
	uint32_t pfn_start = paddr / PAGE_SIZE;
	uint32_t pfn_end = (paddr + size + PAGE_SIZE - 1) / PAGE_SIZE;

	#if _LOG_BOOTMEM_RESERVE
	klog_debug("Reserving PFN %d to %d inclusive\n", pfn_start, pfn_end-1);
	#endif

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

void* bootmem_aligned_alloc_low(size_t size) {
	// Call actual allocator with the goal of allocating within ZONE_DMA and
	//  page aligned. Should only really be used by paging initialisation.
	return __bootmem_alloc(size, PAGE_SIZE, ZONE_DMA_OFFSET);
}

void bootmem_free_all() {
	panic("bootmem_free_all() not implemented!\n");

	// TODO: For all unallocated pages known to the allocator:
	//  clear PG_reserved flag
	//  set count to 1
	//  call __free_pages() so buddy allocator can build its free lists
	// TODO: Free all pages used for bitmap and give them to buddy allocator
}



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

#define _LOG_BOOTMEM_ALLOC 0
void* __bootmem_alloc(size_t size, uint32_t align, uintptr_t goal) {
	// Find what PFN the goal lands in, and how many PFNs we need for allocation
	uint32_t goal_pfn = goal / PAGE_SIZE;
	uint32_t needed_pfns = (size + PAGE_SIZE - 1) / PAGE_SIZE; // Rounded up
	// NOTE: Ideally, `goal` will be on a page boundary
	#if _LOG_BOOTMEM_ALLOC
	klog_debug("__bootmem_alloc: Given goal is 0x%p, goal PFN is %d\n", goal, goal_pfn);
	klog_debug("__bootmem_alloc: Requested allocation size %d bytes, %d PFNs\n", size, needed_pfns);
	#endif

	// Starting from the goal, search for a linear string of pages that fit the
	//  allocation.
	uint32_t block_pfn = goal_pfn; // PFN of the start of the block
	uint32_t found_pfns = 0; // Number of pages found in a row
	for (uint32_t pfn = goal_pfn; pfn < bootmem_data.pfn_end; ++pfn) {
		// If `pfn` is an occupied page, reset our count and block start.
		if (bitmap_test(pfn)) {
			#if _LOG_BOOTMEM_ALLOC
			klog_debug("__bootmem_alloc: PFN %d is reserved, skipping to next PFN for block.\n", pfn);
			#endif

			block_pfn = pfn + 1; // This is reserved, next we'll check next block
			found_pfns = 0;
			continue;
		}

		// Otherwise, add one PFN to the block and check if we've got a large
		//  enough region for the allocation
		found_pfns += 1;
		#if _LOG_BOOTMEM_ALLOC
		klog_debug("__bootmem_alloc: %d PFNs starting from %d free\r", found_pfns, block_pfn);
		#endif
		if (found_pfns >= needed_pfns) {
			#if _LOG_BOOTMEM_ALLOC
			klog_debug("\n");
			klog_debug("__bootmem_alloc: Found block large enough (%d >= %d)\n", found_pfns, needed_pfns);
			#endif
			break; // Found a large enough block, stop searching!
		}
	}

	// Check if we successfully found a large enough block. If not, log a
	//  warning and return a null pointer.
	if (found_pfns < needed_pfns) {
		#if _LOG_BOOTMEM_ALLOC
		klog_debug("__bootmem_alloc: Could not find a block large enough for %d PFNs\n", needed_pfns);
		#endif

		klog_warning("__bootmem_alloc: Not enough memory for allocation of size %d\n", size);
		return NULL;
	}
	uintptr_t alloc_paddr = block_pfn << PAGE_SHIFT;
	#if _LOG_BOOTMEM_ALLOC
	klog_debug("__bootmem_alloc: paddr of allocation is 0x%p\n", alloc_paddr);
	#endif

	// Merging allocations: If the start of the new block is adjacent to the
	//  last allocation, and there's enough space remaining on that last page,
	//  we can merge the allocation onto that last page (given alignment).
	if (
		(block_pfn == bootmem_data.last_pfn - 1) &&
		(bootmem_data.last_offset != 0) &&
		(align != PAGE_SIZE) // TODO: Handle alignment other than PAGE_SIZE
	) {
		#if _LOG_BOOTMEM_ALLOC
		klog_debug("__bootmem_alloc: Can merge allocation with last page!\n");
		#endif

		// Can merge the allocation! Shift our paddr back to start at the last
		//  offset.
		alloc_paddr -= (PAGE_SIZE - bootmem_data.last_offset);

		#if _LOG_BOOTMEM_ALLOC
		klog_debug("__bootmem_alloc: Alignment not PAGE_SIZE, adjusted paddr to 0x%p\n", alloc_paddr);
		#endif
	}

	// Update the last PFN to the PFN where allocation *ends*. We subtract one
	//  so we get the PFN of the last byte.
	bootmem_data.last_pfn = (alloc_paddr + size - 1) / PAGE_SIZE;

	// Update the last offset to the current offset of alloc_paddr in a page.
	bootmem_data.last_offset = (alloc_paddr + size) % PAGE_SIZE;

	// Finally, reserve this address space and return a virtual address to the
	//  allocation!
	bootmem_reserve(alloc_paddr, size);

	#if _LOG_BOOTMEM_ALLOC
	klog_debug("__bootmem_alloc: bootmem_data.last_pfn updated to %d\n", bootmem_data.last_pfn);
	klog_debug("__bootmem_alloc: bootmem_data.last_offset updated to %d\n", bootmem_data.last_offset);
	klog_debug("__bootmem_alloc: Returning vaddr 0x%p\n", __to_virt(alloc_paddr));
	#endif

	return __to_virt(alloc_paddr);
}
