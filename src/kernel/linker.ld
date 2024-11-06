OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

PHYSICAL_BASE = 0x00100000;
PAGE_OFFSET   = 0xC0000000;

SECTIONS {
	/* Our kernel image will begin 1 MiB into physical address space */
	. = PHYSICAL_BASE;

	/* Multiboot header and bootstrap code */
	.multiboot.data : {
		/* Multiboot header */
		_paddr_multiboot_header_start = .;
		*(.multiboot.data)
		_paddr_multiboot_header_end = .;
	}
	.multiboot.text : {
		/* Bootstrap code */
		_paddr_bootstrap_start = .;
		*(.multiboot.text)
		_paddr_bootstrap_end = .;
	}

	/* Operating system code. Needs to be placed at physical addresses */
	. += PAGE_OFFSET;
	_paddr_kernel_start = . - PAGE_OFFSET; /* Beginning of kernel image */
	.text ALIGN(4096) : AT(ADDR(.text) - PAGE_OFFSET) {
		_paddr_kernel_ro_start = . - PAGE_OFFSET; /* Beginning of read-only region */
		*(.text)
	}
	.rodata ALIGN(4096) : AT(ADDR(.rodata) - PAGE_OFFSET) {
		*(.rodata)
		_paddr_kernel_ro_end = . - PAGE_OFFSET; /* End of read-only region */
	}
	.data ALIGN(4096) : AT(ADDR(.data) - PAGE_OFFSET) {
		_paddr_kernel_rw_start = . - PAGE_OFFSET; /* Beginning of writable region */
		*(.data)
	}
	.bss ALIGN(4096) : AT(ADDR(.bss) - PAGE_OFFSET) {
		*(COMMON) /* TODO: Remove if not needed */
		*(.bss)
		*(.bootstrap_stack) /* Set up in boot.S */
		_paddr_kernel_rw_end = . - PAGE_OFFSET; /* End of writable region */
	}
	_paddr_kernel_end = . - PAGE_OFFSET; /* End of kernel image */

	/* Get rid of unnecessary GCC bits */
	/DISCARD/ : {
		*(.comment)
		/* (.eh_frame) */
		*(.note.gnu.build-id)
	}
}
