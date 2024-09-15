OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)
phys = 0x00100000;
SECTIONS {
	/* Actual code */
	.text phys : AT(phys) {
		code_addr = .;
		*(.multiboot.header)
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}

	/* Kernel data */
	.data : AT(phys + (data_addr - code_addr)) {
		data_addr = .;
		*(.data)
		. = ALIGN(4096);
	}

	/* Statically defined, uninitialised values */
	.bss : AT(phys + (bss_addr - code_addr)) {
		bss_addr = .;
		*(.bss)
		. = ALIGN(4096);
	}

	/* End of kernel */
	end_addr = .;

	/* Get rid of unnecessary GCC bits */
	/DISCARD/ : {
		*(.comment)
		/* *(.eh_frame) */
		*(.note.gnu.build-id)
	}
}
