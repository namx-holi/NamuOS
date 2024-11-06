/// @file boot_allocator.c

#include <namuos/boot_allocator.h> // Implements

#include <string.h> // memset
#include <namuos/paging.h>
#include <namuos/panic.h>
#include <namuos/terminal.h>


// Boot allocator information
bootmem_data_t bootmem_data;

// Our boot memory allocator needs to know where to start allocations, i.e. the
//  end of the kernel image.
extern void* _paddr_kernel_end;

// Helpers to set, clear, and test bitmap bits
void _bitmap_set(uint32_t pfn);
void _bitmap_clear(uint32_t pfn);
char _bitmap_test(uint32_t pfn);

// Actual allocation method, wrapped by bootmem methods
void* __bootmem_alloc(size_t size, uint32_t align, uintptr_t goal);


void bootmem_initialise(multiboot_info_t* mb_info) {
	// TODO: Check mb_info to see if we've got enough RAM to span ZONE_NORMAL
	(void)mb_info; // Currently unused.

	// Set up the boot allocator to span from the very start of physical memory
	//  to the end of ZONE_NORMAL
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
		"Initialsied boot allocator up to physical address 0x%p\n",
		bootmem_data.pfn_end << PAGE_SHIFT);
}

void bootmem_reserve(uintptr_t paddr, size_t size) {
	// We need to mark any frames that are spanned by the address range as used.
	//  Round the start PFN down, and round the end PFN up.
	uint32_t pfn_start = paddr / PAGE_SIZE;
	uint32_t pfn_end = (paddr + size + PAGE_SIZE - 1) / PAGE_SIZE;
	for (uint32_t pfn = pfn_start; pfn < pfn_end; ++pfn)
		_bitmap_set(pfn);
	// NOTE: Could possibly do this in bulk, setting whole bytes
}

void bootmem_free(uintptr_t paddr, size_t size) {
	// We can only mark frames as free if they are *fully* spanned by the given
	//  address raneg. Round the start PFN up, and round the end PFN down.
	uint32_t pfn_start = (paddr + PAGE_SIZE - 1) / PAGE_SIZE;
	uint32_t pfn_end = (paddr + size) / PAGE_SIZE;

	for (uint32_t pfn = pfn_start; pfn < pfn_end; ++pfn) {
		// If already cleared, panic for double free
		if (_bitmap_test(pfn) == 0)
			panic("Bootmem double free for PFN %d\n", pfn);
		_bitmap_clear(pfn);
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

void _bitmap_set(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Mark that bit as 1
	bootmem_data.bitmap[index] |= (char)1 << bit_index;
}

void _bitmap_clear(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Mark that bit as 0
	bootmem_data.bitmap[index] &= ~((char)1 << bit_index);
}

char _bitmap_test(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = (pfn - bootmem_data.pfn_start) / 8; // 8 bits per value
	uint32_t bit_index = (pfn - bootmem_data.pfn_start) % 8;

	// Check if bit is 1
	return bootmem_data.bitmap[index] & ((char)1 << bit_index);
}

void* __bootmem_alloc(size_t size, uint32_t align, uintptr_t goal) {
	// Find what PFN the goal lands in, and how many PFNs we need for allocation
	uint32_t goal_pfn = goal / PAGE_SIZE;
	uint32_t needed_pfns = (size + PAGE_SIZE - 1) / PAGE_SIZE; // Rounded up
	// NOTE: Ideally, `goal` will be on a page boundary

	// Starting from the goal, search for a linear string of pages that fit the
	//  allocation.
	uint32_t block_pfn = goal_pfn; // PFN of the start of the block
	uint32_t found_pfns = 0; // Number of pages found in a row
	for (uint32_t pfn = goal_pfn; pfn < bootmem_data.pfn_end; ++pfn) {
		// If `pfn` is an occupied page, reset our count and block start
		if (_bitmap_test(pfn)) {
			block_pfn = pfn + 1; // Current PFN is reserved, we'll check next block
			found_pfns = 0;
			continue;
		}

		// Otherwise, add one PFN to the block and check if we've got a large
		//  enough region for the allocation
		found_pfns += 1;
		if (found_pfns >= needed_pfns)
			break;
	}

	// Check if we successfully found a large enough block. If not, log a
	//  warning and return null pointer
	if (found_pfns < needed_pfns) {
		klog_warning("__bootmem_alloc: Not enough memory for allocation of size %d\n", size);
		return NULL;
	}

	// This will be the physical address of allocation. May be adjusted by
	//  merging allocations.
	uintptr_t alloc_paddr = block_pfn << PAGE_SHIFT;

	// Merging allocations: If the start of the new block is adjacent to the
	//  last allocation, and there's enough space remaining on that last page,
	//  we can merge the allocation onto that last page (given alignment)
	if (
		(block_pfn == bootmem_data.last_pfn - 1) && // Block is adjacent
		(bootmem_data.last_offset != 0) && // Last page wasn't full
		(align != PAGE_SIZE) // TODO: Handle alignment other than PAGE_SIZE
	) {
		// Cna merge the allocation! Shift our paddr back to start at the last
		//  offset.
		alloc_paddr -= (PAGE_SIZE - bootmem_data.last_offset);
	}

	// Update the last PFN and offset for the last byte of allocation
	bootmem_data.last_pfn = (alloc_paddr + size - 1) / PAGE_SIZE;
	bootmem_data.last_offset = (alloc_paddr + size) % PAGE_SIZE;

	// Finally, reserve this address space and return a virtual address for the
	//  allocation
	bootmem_reserve(alloc_paddr, size);
	return __to_virt(alloc_paddr);
}
