
# Multiboot Header
.set MULTIBOOT_PAGE_ALIGN,   1<<0 # Align all boot modules on i386 page (4KB) boundaries
.set MULTIBOOT_MEMORY_INFO,  1<<1 # Must pass memory information to OS
# .set MULTIBOOT_VIDEO_INFO,   1<<2 # Must pass video information to OS # DISABLED
.set MULTIBOOT_HEADER_FLAGS, MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO # | MULTIBOOT_VIDEO_INFO
# TODO: MULTIBOOT_AOUT_KLUDGE? 0x00010000
.set MULTIBOOT_MAGIC,        0x1BADB002
.set MULTIBOOT_CHECKSUM,     -(MULTIBOOT_MAGIC + MULTIBOOT_HEADER_FLAGS)

# Declare multiboot header that marks program as kernel
; .section .multiboot.header, "aw"
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM
# TODO: May need header_addr, load_addr, load_end_addr, ... if flags[16] set
# TODO: May need mode_type, width, height, depth if flags[2] set
# See https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout


# Some external references
.extern code, data, bss, end


# Kernel entry point
.section .text
.global _start
.type _start, @function
_start:
	# Set stack pointer
	movl $stack_top, %esp

	# Push stack pointer to stack as third argument to kernel_main
	push %esp

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


# Reserve space for bootstrap stack
.section .bss
stack_bottom:
.skip 0x2000 # 8192 B, 8 KiB
stack_top:
