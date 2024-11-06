# Multiboot Header
.set MULTIBOOT_PAGE_ALIGN,   1<<0 # Align all boot modules on 4 KiB page boundary
.set MULTIBOOT_MEMORY_INFO,  1<<1 # Get memory info from GRUB
# .set MULTIBOOT_VIDEO_INFO,   1<<2 # Get video info from GRUB
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO # | MULTIBOOT_VIDEO_INFO
# TODO: MULTIBOOT_AOUT_KLUDGE? 1<<4
.set MULTIBOOT_MAGIC,    0x1BADB002
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_HEADER_FLAGS)


# Kernel Settings
.set PAGE_OFFSET, 0xC0000000 # Starting virtual address of kernel
.set PGD_OFFSET,  0x300      # Where in the global PGD the kernel mapping starts
.set BOOTSTRAP_STACK_SIZE, 0x4000 # 16 KiB


# Declare a multiboot header that marks this as an operating system
.section .multiboot.data, "aw"
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM


# Reserve space for the bootstrap stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip BOOTSTRAP_STACK_SIZE
stack_top:


# Reserve space for the global page directory, and two page tables to cover the
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
	# We want to map two page tables worth of entries in pg0 and pg1. Since
	#  tables are contiguously allocated, we can overflow into adjacent tables
	#  to fill them.
	movl $(_kernel_pg0 - PAGE_OFFSET), %edi # Physical address of page table entry
	movl $0, %esi # Start the linear mapping at address 0
	movl $2048, %ecx # Mapping two PDEs

page_table_loop:
	# Map this entry as present and writable (bit 0 and bit 1)
	movl %esi, %edx   # 20 most sig bits will be address >> PAGE_SHIFT
	orl $0x003, %edx  # Add present and writable bits
	movl %edx, (%edi) # Write %edx to page table

	# Loop to next entry
	addl $4096, %esi # Move to address of next frame to be mapped
	addl $4, %edi    # Move to address of next PDE (each are 32 bits)
	loop page_table_loop # Decrements %ecx and loops and repeats if non-zero
page_table_loop_end:

	# Create an identity mapping and a mapping for kernel-space in PGD that is
	#  present and writable for both pages
	# Identity
	movl $(_kernel_pg0 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + 0
	movl $(_kernel_pg1 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + 4
	# Kernel-space
	movl $(_kernel_pg0 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + PGD_OFFSET*4
	movl $(_kernel_pg1 - PAGE_OFFSET + 0x003), _kernel_pgd - PAGE_OFFSET + PGD_OFFSET*4 + 4

	# Set CR3 register to _kernel_pgd as the page table in use. No flags needed
	#  (PWT or PCD flags are bits 3 and 4 respectively if needed).
	movl $(_kernel_pgd - PAGE_OFFSET), %ecx
	movl %ecx, %cr3

	# Enable paging with write-protect (bits 31 and 16 respectively)
	movl %cr0, %ecx       # Copy current flags from CR0
	orl $0x80010000, %ecx # Set paging/write-protect bits
	movl %ecx, %cr0       # Enable paging!

	# Jump to kernel starting section
	lea boot_kernel, %ecx
	jmp *%ecx


# Boots the kernel with `kernel_main` method
.section .text
boot_kernel:
	# Remove identity mapping and force TLB flush by reloading CR3
	movl $0, _kernel_pgd + 0
	movl $0, _kernel_pgd + 4
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set stack pointer
	movl $stack_top, %esp

	# GRUB gives magic number and multiboot struct via %eax and %ebx. Add stack
	#  pointer, magic number, and multiboot struct as arguments to `kernel_main`
	push %esp # 3rd arg - stack pointer
	push %eax # 2nd arg - magic number
	push %ebx # 1st arg - multiboot struct

	# Call any global constructors, and then transfer control to `kernel_main`
	call _init
	call kernel_main

	# Hang if kernel_main returns
	cli
kernel_end_loop:
	hlt
	jmp kernel_end_loop
