/// @file mem_setup.c

#include <kernel/memory.h> // Implements

#include <string.h> // memset
#include <kernel/system.h> // reset VGA memory address


extern void* end; // Free space located after stack. See boot.S
physical_addr_t placement_pointer = (physical_addr_t)&end;
physical_addr_t heap_end = (physical_addr_t)NULL; // NOTE: Currently not used

CR3_register_t cr3; // Spoof CR3 register


void kmalloc_startat(physical_addr_t address) {
	placement_pointer = address;
}

void setup_paging(multiboot_info_t* mb_info) {
	// TODO: Reserve entire 0xC...0xF block as kernel only
	// TODO: Linear map 0x0...1MiB + mem_lower as kernel
	// TODO: Reserve space for anything past mem_lower for non-contiguous mapping
	//  in kernel space
	// TODO: Every address 0x0...-0xC is user space. Save PAGE_OFFSET somewhere

	// TODO: Reserve 0xC0000000 - 0xFFFFFFFF as kernel only
	// TODO: Use mem_lower as reference of how much to map for kernel

	// Initialise dummy CR3 register
	cr3.ignored_1 = 0;
	cr3.pwt = 0;
	cr3.pcd = 0;
	cr3.ignored_2 = 0;
	cr3.addr = 0;

	// Allocate space for the page directory, and kernel page table.
	physical_addr_t page_directory_phys;
	physical_addr_t kernel_page_table_phys;
	PDE_t* page_directory = (PDE_t*)kvmalloc_p(1024 * sizeof(PDE_t), &page_directory_phys);
	PTE_t* kernel_page_table = (PTE_t*)kvmalloc_p(1024 * sizeof(PTE_t), &kernel_page_table_phys);
	// Make sure directory and tables are cleared
	memset(page_directory, 0, 1024 * sizeof(PDE_t));
	memset(kernel_page_table, 0, 1024 * sizeof(PTE_t));

	// Enable writing for all page directory entries
	for (int i = 0; i < 1024; ++i)
		page_directory[i].rw = 1;
	
	// Map the first 4 MiB as 'kernel'. This includes any BIOS flags, VGA memory
	//  address, the kernel itself, and 3 MiB for these tables and any heap
	for (int i = 0; i < 1024; ++i) {
		kernel_page_table[i].present = 1;
		kernel_page_table[i].rw      = 1; // Can read/write
		kernel_page_table[i].user    = 0; // No user-mode access
		kernel_page_table[i].dirty   = 1; // These pages are written to
		kernel_page_table[i].addr    = i;
	}

	// TODO: Set .text and .rodata regions to read-only
	// TODO: After above, disable execution of non-.text sections

	// Identity map the kernel page table by placing in page directory at 0
	page_directory[0x0].present = 1;
	page_directory[0x0].user    = 1; // No user-mode access
	page_directory[0x0].addr = (uint32_t)kernel_page_table / 0x1000;

	// Also map the kernel to higher half using page directory entry 768. This
	//  number in particular is used as it's 75% of the way through memory
	//  space, mapping the kernel to start at 0xC0000000.
	// NOTE: 1024 == 0x400, 768 == 0x300
	page_directory[0x300].present = 1;
	page_directory[0x300].user    = 1; // No user-mode access
	page_directory[0x300].addr = (uint32_t)kernel_page_table / 0x1000;

	// Enable paging by setting the CR3 register
	cr3.addr = (uint32_t)page_directory / 0x1000;
	asm volatile ("mov %0, %%cr3" : : "r"(cr3.raw)); // Set location of PD
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0)); // Pull current value of CR0 register
	cr0 |= 0x80010000; // Update CR0 to enable paging (bit 31) with write protect (bit 16)
	asm volatile ("mov %0, %%cr0" : : "r"(cr0)); // Tell CPU to enable paging

	// Readjust the VGA memory address so we can still use kprintf
	kernel_kprintf_shift_ega_addr(0xC0000000); // New kernel start address!
	kprintf("PDE[0] addr is 0x%p\n", &page_directory[0]);

	// We no longer need the kernel identity mapping since paging enabled, unmap
	page_directory[0].present = 0;
	while(1){}
	page_directory[0].addr = 0;

	// TODO: Reload CR3 to force TLB flush so our paging changes can take effect
	//  This might require an assembly routine instead of inline.
	// asm volatile ("mov %%cr3, %0" : "=r"(temp));
	// asm volatile ("mov %0, %%cr3" : : "r"(cr3.raw));

	// TODO: Map the remaining space, using mem-map entries as reference!



	// We also have some extra free memory usable 



}
