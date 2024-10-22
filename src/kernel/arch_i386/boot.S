# Multiboot Header
.set MULTIBOOT_PAGE_ALIGN,   1<<0 # Align all boot modules on i386 page boundary
.set MULTIBOOT_MEMORY_INFO,  1<<1 # Get memory info from bootloader
# .set MULTIBOOT_VIDEO_INFO,   1<<2 # Get video info from bootloader
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO # | MULTIBOOT_VIDEO_INFO
# TODO: MULTIBOOT_AOUT_KLUDGE? 0x00010000
.set MULTIBOOT_MAGIC,    0x1BADB002
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_HEADER_FLAGS)


# Kernel settings
.set PAGE_OFFSET, 0xC0000000 # Where kernel is located after paging
.set PGD_OFFSET,  0x300      # Where in PGD the kernel mapping starts
.set BOOTSTRAP_STACK_SIZE, 0x4000 # 16 KiB


# Declare a multiboot header that marks this as the kernel
.section .multiboot.data, "aw"
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM


# Reserve space for bootstrap stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip BOOTSTRAP_STACK_SIZE
stack_top:


# Reserve space for the kernel page directory, and two page tables to cover the
#  kernel image (8 MiB)
.section .bss, "aw", @nobits
.global _kernel_pgd
.global _kernel_pg0
.global _kernel_pg1
.align 4096
_kernel_pgd:
.skip 4096
_kernel_pg0:
.skip 4096
_kernel_pg1:
.skip 4096


# Kernel entry point
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# Set %edi to the physical address of first page table entry
	movl $(_kernel_pg0 - PAGE_OFFSET), %edi
	# %edi - address of page table entry

	# We'll map two page tables worth of entries in pg0 and pg1. Since tables
	#  are contiguously allocated, we can overflow in to adjacent tables.
	movl $0, %esi # Starting at address 0x00000000
	movl $2048, %ecx # Two page tables worth of entries
	# %esi - address being mapped in entry

page_table_loop:
	# Map this entry as present and writable (bit 0 and bit 1)
	movl %esi, %edx   # 20 most sig bits will be (address >> PAGE_SHIFT)
	orl $0x003, %edx  # Add present and writable bits
	movl %edx, (%edi) # Write entry

	# Loop to next entry
	addl $4096, %esi # Move to address of next frame to be mapped (4 KiB)
	addl $4, %edi    # Move to address of next page table entry (each 32 bits)
	loop page_table_loop # Step down %ecx by one and repeat if non-zero
page_table_loop_end:

	# Create an identity mapping and a mapping for kernel-space in PGD that is
	#  present and writable for both pages
	# Identity
	movl $(_kernel_pg0 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + 0
	movl $(_kernel_pg1 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + 4
	# Kernel-space
	movl $(_kernel_pg0 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + PGD_OFFSET*4
	movl $(_kernel_pg1 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + PGD_OFFSET*4 + 4
	# NOTE: *4 for PGD_OFFSET so we get byte offset (each entry 32 bits)
	# NOTE: Similar to PTEs, setting entry to the address sets the 20 most sig
	#  bits to (address >> PAGE_SHIFT)

	# Set CR3 register to the address of _kernel_pgd. We don't need to set any
	#  PWT or PCD flags (bit 3 and 4 respectively).
	movl $(_kernel_pgd - PAGE_OFFSET), %ecx
	movl %ecx, %cr3

	# Finally, enable paging (bit 31) with write-protect (bit 16) in CR0
	movl %cr0, %ecx       # Copy current set bits in CR0
	orl $0x80010000, %ecx # Set paging and write-protect bits
	movl %ecx, %cr0       # Set CR0

	# Now paging is enabled, we can jump to the kernel starting section!
	lea boot_kernel, %ecx
	jmp *%ecx


# Boots the main kernel method
.section .text
# NOTE: Paging is now fully enabled. No need for PAGE_OFFSET on addresses!
boot_kernel:
	# Remove the identity mapping, and force a TLB flush by reloading CR3
	movl $0, _kernel_pgd + 0
	movl $0, _kernel_pgd + 4
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set stack pointer
	movl $stack_top, %esp

	# Push the stack pointer to the stack so it can be the third argument to
	#  kernel_main
	push %esp

	# GRUB gives some data after setup in %eax and %ebx containing the multiboot
	#  header magic number and multiboot_info_t structs respectively. Add these
	#  to the stack so these are args to kernel_main
	push %eax # Magic number as second arg
	push %ebx # multiboot_info_t as first arg

	# Call any global constructors
	call _init

	# Transfer control to kernel. Hang if it unexpectedly returns.
	call kernel_main
	cli
kernel_end_loop:
	hlt
	jmp kernel_end_loop
