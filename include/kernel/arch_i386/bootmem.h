/**
 * @file bootmem.h
 * @defgroup arch_i386_bios_defines <arch_i386/bootmem.h>
 * @brief i386 boot memory allocator
 * @ingroup arch_i386
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _ARCH_i386_BOOT_MEM_H
#define _ARCH_i386_BOOT_MEM_H 1

#include <stddef.h>
#include <stdint.h>

#include <namuos/multiboot.h>


// Contains info needed for the boot memory allocator
// TODO: Doxygen comment
typedef struct {
	uint32_t pfn_start;   ///< First physical page frame available to allocator
	uint32_t pfn_end;     ///< Last physical page frame available to allocator
	char* bitmap;         ///< Bitmap representing free/allocated pages
	uint32_t last_pfn;    ///< Last page used in an allocation
	uint32_t last_offset; ///< Offset in a page of the last allocation
} bootmem_data_t;

/// Global boot memory allocation data
extern bootmem_data_t bootmem_data;


/** @brief Initialises boot allocator 
 * 
 * Sets up the global @ref bootmem_data for memory between 0 and the end of
 * ZONE_NORMAL. Will initialise the bitmap, and reserve all addresses from the
 * start of memory up until where the bitmap ends in memory.
 * 
 * @note Bitmap is allocated within ZONE_DMA to make things easy, immediately
 * after the kernel image. This ends up using about 29 KiB of the precious 16
 * MiB in ZONE_DMA, but it'll be replaced with the physical page allocator soon
 * enough.
 * 
 * @param mb_info Multiboot info passed by GRUB.
*/
void bootmem_initialise(multiboot_info_t* mb_info);

/** @brief Marks pages between `paddr` and `paddr+size` as reserved.
 * 
 * Marks the pages between `paddr` and `paddr+size` as reserved. Partially used
 * pages will be fully reserved.
 * 
 * @param paddr Starting address for the reservation
 * @param size Size of the allocation
*/
void bootmem_reserve(uintptr_t paddr, size_t size);

/** @brief Marks pages between `paddr` and `paddr+size` as free.
 * 
 * Marks pages between `paddr` and `paddr+size` as free. If a page is only
 * partially freed by the address range, it will remain reserved.
 * 
 * If a page is already marked as free in the bitmap and it is freed by this
 * method, the kernel will panic to avoid double freeing.
 * 
 * @note A limitation of the boot allocator is that this only frees *full*
 * pages. If two page aligned 1 byte allocations are made in a row, neither of
 * them can be freed as neither span entire pages.
 * 
 * @param paddr Starting address of the block to free
 * @param size Size of the block to free
*/
void bootmem_free(uintptr_t paddr, size_t size);

/** @brief Allocate `size` bytes from ZONE_NORMAL
 * 
 * Allocates `size` bytes from ZONE_NORMAL.
 * 
 * @param size Number of bytes to allocate
 * 
 * @returns Pointer to allocated block
*/
void* bootmem_alloc(size_t size);

/** @brief Allocate `size` bytes from ZONE_DMA
 * 
 * Allocates `size` bytes from ZONE_DMA.
 * 
 * @param size Number of bytes to allocate
 * 
 * @returns Pointer to allocated block
*/
void* bootmem_alloc_low(size_t size);

/** @brief Allocates `size` bytes from ZONE_NORMAL. Allocation will be page aligned.
 * 
 * Allocates `size` bytes from ZONE_NORMAL. Allocation will be page aligned.
 * 
 * @param size Number of bytes to allocate
 * 
 * @returns Pointer to allocated block
*/
void* bootmem_aligned_alloc(size_t size);

/** @brief Allocates `size` bytes from ZONE_DMA. Allocation will be page aligned.
 * 
 * Allocates `size` bytes from ZONE_DMA. Allocation will be page aligned.
 * 
 * @note This should be used sparingly. One situation is in setting up paging,
 * where only the first 8 MiB are pre-mapped by boot, and ZONE_NORMAL starts at
 * 16 MiB.
 * 
 * @param size Number of bytes to allocate
 * 
 * @returns Pointer to allocated block
*/
void* bootmem_aligned_alloc_low(size_t size);

// Boot allocator end of life
// TODO: Doxygen comment
void bootmem_free_all();

#endif

/** @} */
