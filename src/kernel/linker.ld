OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)
phys = 0x00100000;
SECTIONS {
	/* Actual code */
	.text phys : AT(phys) {
		code = .;
		*(.multiboot.header)
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}

	/* Kernel data */
	.data : AT(phys + (data - code)) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}

	/* Statically defined, uninitialised values */
	.bss : AT(phys + (bss - code)) {
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	/* End of kernel */
	end = .;

	/* Get rid of unnecessary GCC bits */
	/DISCARD/ : {
		*(.comment)
		/* *(.eh_frame) */
		*(.note.gnu.build-id)
	}
}
