/// @file memmap.c

#include <namuos/memory.h> // Implements


void memmap_bitmap_flag_used(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Mark that bit as 0
	memmap_bitmap[index] &= ~((uint32_t)1 << bit_index);
}

void memmap_bitmap_flag_free(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Mark that bit as 1
	memmap_bitmap[index] |= (uint32_t)1 << bit_index;
}

uint32_t memmap_bitmap_test(uint32_t pfn) {
	// Calculate index and bit number in bitmap
	uint32_t index = pfn / 32; // 32 bits per value
	uint32_t bit_index = pfn % 32;

	// Check if bit is 1
	return memmap_bitmap[index] & ((uint32_t)1 << bit_index);
}
