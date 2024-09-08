
# Multiboot Header
.set MULTIBOOT_PAGE_ALIGN,   1<<0
.set MULTIBOOT_MEMORY_INFO,  1<<1
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set MULTIBOOT_MAGIC,        0x1BADB002
.set MULTIBOOT_CHECKSUM,     -(MULTIBOOT_MAGIC + MULTIBOOT_HEADER_FLAGS)

# Declare multiboot header that marks program as kernel
.section .multiboot.header, "aw"
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM

# Reserve a stack for the initial thread
.section .bss
# .align 16 # TODO: Does this need to be aligned?
stack_bottom:
.skip 0x4000 # 16384 B, 16 KiB
stack_top:

# Kernel entry point
.section .text
.global _start
.type _start, @function
_start:
	# Set stack pointer
	movl $stack_top, %esp

	# GRUB gives some data after setup in EAX and EBX, containing the multiboot
	#  header magic number and address to a multiboot_info_t struct
	#  respectively. Add these to the stack so we can access them as arguments
	#  to kernel_main
	push %eax # Magic number as second argument
	push %ebx # multiboot_info_t as first argument

	# Call global constructors
	call _init

	# Transfer control to the kernel
	call kernel_main

	# Hang if kernel_main unexpectedly returns
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
