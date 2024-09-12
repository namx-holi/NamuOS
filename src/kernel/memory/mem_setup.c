/// @file mem_setup.c

#include <kernel/memory.h> // Implements

#include <string.h> // memset
#include <kernel/system.h> // reset VGA memory address


extern void* end; // Free space located after stack. See boot.S
uintptr_t placement_pointer = (uintptr_t)&end;
uintptr_t heap_end = (uintptr_t)NULL; // NOTE: Currently not used

CR3_register_t cr3; // Spoof CR3 register


void kmalloc_startat(uintptr_t address) {
	placement_pointer = address;
}

void setup_paging(uint32_t memsize_kb) {
	// TODO: Use memsize somehow
	kprintf("TODO: Use memsize_kb (%d KiB) in mapping frames\n", memsize_kb);

	// Initialise dummy CR3 register
	cr3.ignored_1 = 0;
	cr3.pwt = 0;
	cr3.pcd = 0;
	cr3.ignored_2 = 0;
	cr3.addr = 0;

	// Allocate space for the page directory and page table (each 1024 entries),
	//  and make sure both are cleared
	kprintf(" [*] Allocating space for PD and PT\n");
	uintptr_t pd_phys;
	uintptr_t pt_phys;
	PDE_t* pd = (PDE_t*)kvmalloc_p(1024 * sizeof(PDE_t), &pd_phys);
	PTE_t* pt = (PTE_t*)kvmalloc_p(1024 * sizeof(PTE_t), &pt_phys);
	memset(pd, 0, 1024 * sizeof(PDE_t));
	memset(pt, 0, 1024 * sizeof(PTE_t));
	kprintf("       PD physical address = 0x%p\n", pd_phys);
	kprintf("       PT physical address = 0x%p\n", pt_phys);

	// Enable writing for all PDEs
	for (int i = 0; i < 1024; ++i)
		pd[i].rw = 1;
	
	// Map the first 1 MiB of memory in page table
	kprintf(" [*] Mapping first 1 MiB in PT\n");
	for (int i = 0; i < 1024; ++i) {
		pt[i].addr = i;
		pt[i].present = 1;
		pt[i].rw = 1; // Read/write
	}

	// Identity map the kernel by placing first 1 MiB in PDE 0
	kprintf(" [*] Identity mapping kernel to PDE 0\n");
	pd[0].addr = (uint32_t)pt / 0x1000;
	pd[0].present = 1;

	// And map the kernel to higher half using PDE 768 (75% through). This maps
	//  the kernel to start at 0xC0000000
	kprintf(" [*] Mapping kernel to higher half (0xC0000000)\n");
	pd[768].addr = (uint32_t)pt / 0x1000;
	pd[768].present = 1;

	// Set the page directory address in our spoof CR3 register, and enable
	//  paging on the CPU
	kprintf(" [*] Enabling paging\n");
	cr3.addr = (uint32_t)pd / 0x1000;
	// @c-ignore
	asm volatile ("mov %0, %%cr3" : : "r"(cr3.raw)); // Set CR3 address to location of PD
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0)); // Tell CPU to enable paging!
	cr0 |= 0x80000000; // Enable paging (set as 0x8001000 for write protect)
	asm volatile ("mov %0, %%cr0" : : "r"(cr0));

	// Readjust VGA memory address
	kernel_kprintf_shift_ega_addr(0xC0000000);
	kprintf("Test printing post-paging enable! I'm printing while paging enabled!\n");

	// We no longer need the identity mapping, unmap
	pd[0].addr = 0;
	pd[0].present = 0;

	// TODO: Reload CR3 to force TLB flush so our paging changes can take effect
	//  Above might require us to make a copy of PD so we can still map?
	// uint32_t temp;
	// asm volatile ("mov %%cr3, %0" : "=r"(temp));
	// asm volatile ("mov %0, %%cr3" : : "r"(cr3.raw));
}
