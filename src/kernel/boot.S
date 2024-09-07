# TODO: Set up similar to https://github.com/cstack/osdev/blob/master/loader.s

# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Preallocate pages used for paging. Don't hard-code addresses and assume they
# are available, as the bootloader might have loaded its multiboot structures or
# modules there. This lets the bootloader know it must avoid the addresses.
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096
# Further page tables may be required if the kernel grows beyond 3 MiB.

# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# Physical address of boot_page_table1
	# TODO: Macro to convert to/from physical?
	movl $(boot_page_table1 - 0xC0000000), %edi

	# First address to map is 0
	# TODO: Start at the first kernel page instead. Alternatively map the first
	#  1 MiB as it can be generally useful, and there's no need to specially map
	#  the VGA buffer.
	movl $0, %esi

	# Map 1024 pages.
	movl $1024, %ecx

1:
	# Only map the kernel
	# DEBUG: Mapping first 1 MiB instead, don't skip until reaching start of kernel
	# TODO: Only map what's needed instead of wasting page table space
	# cmpl $_kernel_start, %esi
	# jl 2f
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	# Map physical address as "present, writeable". Note that this maps
	#  .text and .rodata as writeable. Mind security and map them as
	#  non-writeable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of pages is 4096 bytes
	addl $4096, %esi

	# Size of entries in boot_page_table1 is 4 bytes
	addl $4, %edi

	# Loop to the next entry if we haven't finished
	loop 1b

3:
	# Map VGA video memory to 0xC03FF000 as "present, writeable"
	# NOTE: 0x003 uses bits 0 and 1 to signal present and read/write
	# movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
	# DEBUG: This should be mapped as part of mapping first MiB.

	# The page table is used at both page directory entry 0 (virtually from 0x0
	#  to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	#  768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	#  higher half). The kernel is identity mapped because enabling paging does
	#  not change the next instruction, which continues to be physical. The CPU
	#  would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	# Set cr3 to the address of the boot_page_directory
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	# At this point, paging is fully set up and enabled

	# Unmap the identity mapping as it is now unnecessary
	movl $0, boot_page_directory + 0

	# Reload crc3 to force a TLB flush so the changes take effect
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack
	mov $stack_top, %esp

	# GRUB gives some data after setup in EAX and EBX, containing the multiboot
	#  magic number and address to a multiboot_info_t struct respectively. Add
	#  these onto the stack so we can access them as arguments to kernel_main.
	push %eax # Magic number as second argument
	addl $0xC0000000, %ebx # Add offset to address so it's accessible
	push %ebx # multiboot_info_t address as first argument

	# Enter the high-level kernel
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
5:	hlt
	jmp 5b
