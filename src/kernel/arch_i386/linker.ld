OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

PHYSICAL_BASE = 0x00100000;
VIRTUAL_BASE  = 0xC0000000;

SECTIONS {
	/* Our kernel code will begin 1 MiB into physical address space */
	. = PHYSICAL_BASE;
	.multiboot.data : {
		/* Multiboot header */
		_paddr_multiboot_header_start = .;
		*(.multiboot.data)
		_paddr_multiboot_header_end = .;
	}
	.multiboot.text : {
		/* Bootstrap */
		_paddr_bootloader_start = .;
		*(.multiboot.text)
		_paddr_bootloader_end = .;
	}

	/* The operating system code, started by bootstrap. Needs to be placed at */
	/* physical addresses */
	. += VIRTUAL_BASE; /* PAGE_OFFSET */
	_paddr_kernel_start = . - VIRTUAL_BASE; /* Beginning of kernel image */
	.text ALIGN(4096) : AT(ADDR(.text) - VIRTUAL_BASE) {
		_paddr_kernel_ro_start = . - VIRTUAL_BASE; /* Beginning of read-only region */
		/* Code */
		*(.text)
	}
	.rodata ALIGN(4096) : AT(ADDR(.rodata) - VIRTUAL_BASE) {
		/* Constant variables */
		*(.rodata)
		_paddr_kernel_ro_end = . - VIRTUAL_BASE; /* End of read-only region */
	}
	.data ALIGN(4096) : AT(ADDR(.data) - VIRTUAL_BASE) {
		_paddr_kernel_rw_start = . - VIRTUAL_BASE; /* Beginning of read-write region */
		/* Initialised data, could be extern or global */
		*(.data)
	}
	.bss ALIGN(4096) : AT(ADDR(.bss) - VIRTUAL_BASE) {
		/* Uninitialised global variables */
		*(COMMON) /* TODO: Remove if not needed */
		*(.bss)
		*(.bootstrap_stack) /* Set up in boot.S */
		_paddr_kernel_rw_end = . - VIRTUAL_BASE; /* End of read-write region */
	}
	_paddr_kernel_end = . - VIRTUAL_BASE; /* End of kernel image */

	/* Get rid of unnecessary GCC bits */
	/DISCARD/ : {
		*(.comment)
		/* (.eh_frame) */
		*(.note.gnu.build-id)
	}
}
