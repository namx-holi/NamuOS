ENTRY(_start)

SECTIONS {
	/* Load kernel at 1M */
	. = 1M;

	/* Start with multiboot header and text section */
	.multiboot.header BLOCK(4K) : ALIGN(4K) {
		*(.multiboot.header)
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}

	/* Read-write data (initialised) */
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}

	/* Read-write data (uninitialised) and stack */
	.data BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
}