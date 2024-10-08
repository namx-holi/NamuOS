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


# Reserve space for boot page directory, and one page table
.section .bss, "aw", @nobits
.align 4096
boot_pgd:
.skip 4096
boot_pg0:
.skip 4096
end_boot_pg0:
# NOTE: May need more page tables if kernel gets too big.


# Kernel entry point
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# Set %edi to the physical address of first page table entry
	movl $(boot_pg0 - PAGE_OFFSET), %edi
	# %edi - address of page table entry

	# We'll map 1024 entries in pg0 starting at address 0x0
	movl $0, %esi
	movl $1024, %ecx
	# %esi - address of frame

pg0_loop: # NOTE: This can be repurposed to map any number of contiguous pages if needed
	# Map this entry as present and writable (bit 0 and bit 1)
	movl %esi, %edx   # 20 most sig bits will be address << PAGE_SHIFT
	orl $0x003, %edx  # Add present and writable bits
	movl %edx, (%edi) # Write entry

	# Move to the next entry to loop
	addl $4096, %esi # Move to address of the next frame (4 KiB)
	addl $4, %edi    # Move to address of next entry (each entry 32 bits)
	loop pg0_loop    # Step down %ecx by one, and go back to pg0_loop!

pg0_end:
	# Create an identity mapping and a mapping for kernel-space in PGD that is
	#  present and writable (bit 0 and bit 1)
	movl $(boot_pg0 - PAGE_OFFSET + 0x003), boot_pgd - PAGE_OFFSET + 0 # Identity
	movl $(boot_pg0 - PAGE_OFFSET + 0x003), boot_pgd - PAGE_OFFSET + PGD_OFFSET*4 # Kernel
	# NOTE: *4 for PGD_OFFSET so we get byte offset (each entry is 32 bits)
	# NOTE: Similar to pg0, setting entry to address sets the sig 20 bits to
	#  address << PAGE_SHIFT

	# Set CR3 register to the address of boot_pgd. We don't need to set any
	#  PWT or PCD flags (bit 3 and 4 respectively).
	movl $(boot_pgd - PAGE_OFFSET), %ecx
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
	movl $0, boot_pgd + 0
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
