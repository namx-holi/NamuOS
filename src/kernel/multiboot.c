/// @file multiboot.c

#include <namuos/multiboot.h> // Implements

#include <namuos/paging.h>


/// Helper that fixes a given address. Will adjust physical addresses to virtual
///  addresses.
void _fix_addr(void** addr) {
	if (*addr != 0 && *addr < (void*)PAGE_OFFSET)
		*addr = __to_virt(*addr);
}
#define FIX_ADDR(addr) _fix_addr((void**)(addr))


void multiboot_fix_addresses(multiboot_info_t** mb_info_ptr) {
	// First, fix the address of mb_info itself if it's physical
	FIX_ADDR(mb_info_ptr);
	multiboot_info_t* mb_info = *mb_info_ptr;

	// Fix any addresses that reference physical locations (<PAGE_OFFSET)
	FIX_ADDR(&mb_info->cmdline);
	FIX_ADDR(&mb_info->mods_addr);
	FIX_ADDR(&mb_info->mmap_addr);
	FIX_ADDR(&mb_info->drives_addr);
	FIX_ADDR(&mb_info->boot_loader_name);
	FIX_ADDR(&mb_info->amp_table);

	if (mb_info->framebuffer_addr & MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED)
		FIX_ADDR(&mb_info->colour_info.framebuffer_palette_addr);
}
