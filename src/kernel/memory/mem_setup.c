/// @file mem_setup.c

#include <kernel/memory.h> // Implements

#include <string.h> // memset
#include <kernel/system.h> // reset VGA memory address

// Begin placement of kmalloc memory after end of kernel image
extern void* end_addr; // See linker.ld for where this comes from
uintptr_t placement_pointer = (uintptr_t)&end_addr;

// Pointer to kernel page directory, and current page directory. Both in the
//  style CR3 expects
uintptr_t kernel_page_directory = (uintptr_t)NULL;
uintptr_t current_page_directory = (uintptr_t)NULL;

// NOTE: Currently unused
uintptr_t heap_end = (uintptr_t)NULL;



void kmalloc_startat(uintptr_t address) {
	placement_pointer = address;
}


void setup_paging(multiboot_info_t* mb_info) {
	// Allocate space for the page directory, and enable writing for all entries
	uintptr_t page_directory_phys;
	PDE_t* page_directory = (PDE_t*)kvmalloc_p(PAGE_STRUCTURE_SIZE, &page_directory_phys);
	memset(page_directory, 0, PAGE_STRUCTURE_SIZE);
	for (int i = 0; i < PAGE_ENTRY_COUNT; ++i)
		page_directory[i].rw = 1;
	
	// The first 8 MiB of physical address space is reserved at the start of
	//  kernel-space for the kernel image.
	// Allocate two pages worth of memory space and link to the page directory
	uintptr_t pte0_phys;
	uintptr_t pte1_phys;
	PTE_t* pte0 = (PTE_t*)kvmalloc_p(PAGE_STRUCTURE_SIZE, &pte0_phys);
	PTE_t* pte1 = (PTE_t*)kvmalloc_p(PAGE_STRUCTURE_SIZE, &pte1_phys);
	memset(pte0, 0, PAGE_STRUCTURE_SIZE);
	memset(pte1, 0, PAGE_STRUCTURE_SIZE);
	// Linear mapping of 4 MiB in PTE0, and next 4 MiB in PTE1
	// TODO: Get info on where .text and .rodata are, and mark as read-only
	for (int i = 0; i < PAGE_ENTRY_COUNT; ++i) { // PTE 0
		pte0[i].present = 1;
		pte0[i].rw      = 1; // Can read/write
		pte0[i].dirty   = 1; // Currently in use
		pte0[i].addr    = i;
	}
	for (int i = 0; i < PAGE_ENTRY_COUNT; ++i) { // PTE 1
		pte1[i].present = 1;
		pte1[i].rw      = 1; // Can read/write
		pte1[i].dirty   = 1; // Currently in use
		pte1[i].addr    = i + PAGE_ENTRY_COUNT; // Offset after PTE 0
	}
	// Kernel starts at PAGE_OFFSET
	uint32_t kernel_start_pde = PAGE_OFFSET / (PAGE_ENTRY_COUNT * PAGE_FRAME_SIZE);
	// Place PTE0 and PTE1 at where PAGE_OFFSET lines up in page directory
	page_directory[kernel_start_pde].present   = 1;
	page_directory[kernel_start_pde].addr      = (uint32_t)pte0 / PAGE_FRAME_SIZE;
	page_directory[kernel_start_pde+1].present = 1;
	page_directory[kernel_start_pde+1].addr    = (uint32_t)pte1 / PAGE_FRAME_SIZE;


	// TODO: Map all of 0xc0000000 - 0xc1000000 as ZONE_DMA
	// TODO: Reserve 0xc1000000 - 0xf8000000 as ZONE_NORMAL
	// TODO: Map all of 0xf8000000 - 0xffffffff as ZONE_HIGHMEM
	// TODO: Map last couple pages for fixmap, get from mb_info


	// Set up an identity mapping of kernel image so things still work once
	//  paging is initially turned on
	page_directory[0].present = 1;
	page_directory[0].addr    = (uint32_t)pte0 / PAGE_FRAME_SIZE;
	page_directory[1].present = 1;
	page_directory[1].addr    = (uint32_t)pte1 / PAGE_FRAME_SIZE;

	// Set up CR3 register by setting the paging directory address. We don't
	//  need to set any flags (bit 3 PWT or bit 4 PCD).
	// NOTE: We could just set it to page directory, as /PAGE_FRAME_SIZE is
	//  equivalent to >> 12, and it should be 4 KiB aligned anyway. It's left
	//  as this for now so it's much clearer as to what bits are set.
	uint32_t cr3 = ((uint32_t)page_directory / PAGE_FRAME_SIZE) << 12;
	asm volatile ("mov %0, %%cr3" : : "r"(cr3));

	// Set bits 31 and 16 in CR0 to enable paging and write protect respectively
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80010000;
	asm volatile ("mov %0, %%cr0" : : "r"(cr0));

	// NOTE: May need to force a TLB flush so our paging changes work properly.
	uint32_t temp;
	asm volatile ("mov %%cr3, %0" : "=r"(temp));
	asm volatile ("mov %0, %%cr3" : : "r"(temp));

	// Since paging is now enabled, VGA memory needs to be addressed with
	//  PAGE_OFFSET added.
	kernel_kprintf_update_page_offset(PAGE_OFFSET);

	// NOTE: Load-bearing kprintf... for some reason if this isn't called,
	//  the identity mapping can't be removed.
	kprintf("");

	// We no longer need the kernel image identity mapping since paging is now
	//  functioning! We can unmap now
	// TODO: This *doesn't* work when running via VirtualBox. Unsure why, but
	//  it seems to cause a triple page fault
	page_directory[0].present = 0;
	page_directory[0].addr    = 0;
	page_directory[1].present = 0;
	page_directory[1].addr    = 0;
}
