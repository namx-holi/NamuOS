/**
 * @file multiboot.h
 * @defgroup namuos_multiboot <namuos/multiboot.h>
 * @brief Structures for GRUB multiboot info
 * @ingroup namuos
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H 1

#include <stdint.h>

// Ref: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format

// TODO: Do we need other defines?
// TODO: Method to fix addresses to virtual?
// TODO: Doxygen comments

#define MULTIBOOT_HEADER_MAGIC     0x1BADB002 ///< Header magic, starts with 1
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002 ///< Bootloader magic, starts with 2


// Multiboot module
struct multiboot_module {
	// Memory used goes from `mod_start` to `mod_end-1` inclusive
	uint32_t mod_start; ///< Start address of boot module
	uint32_t mod_end;   ///< End address of boot module

	// Module command line
	/* uint32_t */ char* string; ///< Arbitrary string associaed with module.
	// NOTE: Above could be a command line, or a pathname

	// Padding to take struct to 16 bytes (must be 0)
	uint32_t reserved;
};
typedef struct multiboot_mod multiboot_module_t;

// Symbol table for a.out
struct multiboot_aout_symbol_table {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr; // TODO: Doc
	uint32_t reserved; // Must be 0
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

// Section header table for ELF
struct multiboot_elf_section_header_table {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx; // TODO: Doc
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

// Multiboot memory map entry
struct multiboot_mmap_entry {
	uint32_t size; ///< Size of the associated structure (this)
	uint64_t base_addr; ///< Starting address of region, TODO: pointer
	uint64_t length; ///< Size of memory region in bytes
	uint32_t type;
	#define MULTIBOOT_MEMORY_AVAILABLE        1 // Available
	#define MULTIBOOT_MEMORY_RESERVED         2 // Reserved
	#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3 // Usable, but containing ACPI information
	#define MULTIBOOT_MEMORY_NVS              4 // Non-volatile, needs to be preserved on hibernate
	#define MULTIBOOT_MEMORY_BADRAM           5 // Defective
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

// Multiboot drive
struct multiboot_drive {
	uint32_t  size;            ///< Size of the structure. Varies depending on ports
	uint8_t   drive_number;    ///< BIOS drive number
	uint8_t   drive_mode;
	#define MULTIBOOT_DRIVE_MODE_CHS 0 // Traditional cylinder/head/sector addressing mode
	#define MULTIBOOT_DRIVE_MODE_LBA 1 // Logical Block Addressing mode

	// Geometry of the drive
	uint16_t  drive_cylinders;
	uint8_t   drive_heads;
	uint8_t   drive_sectors;

	// Array of I/O ports used in BIOS code. Zero or more unsigned two-byte
	//  integers, and terminated with zero.
	uint16_t* drive_ports;
};
typedef struct multiboot_drive multiboot_drive_t;

// AMP BIOS info
struct multiboot_amp_info {
	uint16_t version;     ///< Version number
	uint16_t cseg;        ///< Protected mode 32-bit code segment
	uint32_t offset;      ///< Offset to entry point
	uint16_t cseg_16;     ///< Protected mode 16-bit code segment
	uint16_t dseg;        ///< Protected mode 16-bit data segment
	uint16_t flags;       ///< Flags
	uint16_t cseg_len;    ///< Length of protected mode 32-bit code segment
	uint16_t cseg_16_len; ///< Length of protected 16-bit code segment
	uint16_t dseg_len;    ///< Length of protected mode 16-bit data segment
};
typedef struct multiboot_amp_info multiboot_amp_info_t;

// Colour used in framebuffer type 0
struct multiboot_colour {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};
typedef struct multiboot_colour multiboot_colour_t;


// Multiboot info loaded by GRUB
struct multiboot_info {
	// Multiboot info version number
	uint32_t flags;

	#define MULTIBOOT_FLAG_MEM     0x00000001
	// Available memory from BIOS. Present if flags[0] set
	uint32_t mem_lower; ///< KB of lower memory starting at addr 0
	uint32_t mem_upper; ///< KB of upper memory starting at 1 MB

	#define MULTIBOOT_FLAG_DEVICE  0x00000002
	// "root" partition. Present if flags[1] set
	uint32_t boot_device; // TODO: Docs

	#define MULTIBOOT_FLAG_CMDLINE 0x00000004
	// Kernel command line. Present if flags[2] set
	/* uint32_t*/ char* cmdline; ///< Physical address of command line to pass to kernel

	#define MULTIBOOT_FLAG_MODS    0x00000008
	// Boot-Module list. Present if flags[3] set
	uint32_t mods_count; ///< Number of modules loaded.
	/* uint32_t */ multiboot_module_t* mods_addr; ///< Physical address of first module

	#define MULTIBOOT_FLAG_AOUT    0x00000010
	#define MULTIBOOT_FLAG_ELF     0x00000020
	// AOUT or ELF info
	union {
		multiboot_aout_symbol_table_t aout_sym; // Present if flags[4] set
		multiboot_elf_section_header_table_t elf_sec; // Present if flags[5] set
	} syms;

	#define MULTIBOOT_FLAG_MMAP    0x00000040
	// Memory Mapping buffer. Present if flags[6] set
	uint32_t mmap_length; ///< *Total* size of the buffer
	/* uint32_t */ multiboot_memory_map_t* mmap_addr; ///< Address of the first map

	#define MULTIBOOT_FLAG_DRIVE   0x00000080
	// Drive Info buffer. Present if flags[7] set
	uint32_t drives_length; ///< *Total* size of drive structures
	/* uint32_t */ multiboot_drive_t* drives_addr; ///< Address to first drive struct

	#define MULTIBOOT_FLAG_CONFIG  0x00000100
	// ROM configuration table. Present if flags[8] set
	uint32_t config_table; ///< Address of the ROM config table

	#define MULTIBOOT_FLAG_LOADER  0x00000200
	// Boot Loader Name. Present if flags[9] set
	/* uint32_t */ char* boot_loader_name; ///< Physical address of name of boot loader

	#define MULTIBOOT_FLAG_AMP     0x00000400
	// AMP table. Present if flags[10] set
	/* uint32_t */ multiboot_amp_info_t* amp_table; ///< Physical address of AMP table

	#define MULTIBOOT_FLAG_VBE     0x00000800
	// Video. Present if flags[11] set
	uint32_t vbe_control_info; ///< Physical address of VBE control info
	uint32_t vbe_mode_info;    ///< VBE mode information
	uint16_t vbe_mode;         ///< Current video mode specified in VBE 3.0
	// Table of a protected mode interface defined in VBE 2.0+
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;

	#define MULTIBOOT_FLAG_FRAME   0x00001000
	// Framebuffer. Present if flags[12] set
	uint64_t framebuffer_addr; ///< Framebuffer physical address
	uint32_t framebuffer_pitch; ///< Pitch in bytes (bytes per text line if EGA)
	uint32_t framebuffer_width; ///< Width in pixels (chars if EGA)
	uint32_t framebuffer_height; ///< Height in pixels (chars if EGA)
	uint8_t  framebuffer_bpp; ///< Bits per pixel (16 if EGA)
	uint8_t  framebuffer_type;
	#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED  0 // Indexed colour
	#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB      1 // RGB colour
	#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2 // EGA-standard text mode
	union {
		struct { // If MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED
			/* uint32_t */ multiboot_colour_t* framebuffer_palette_addr;
			uint16_t framebuffer_palette_num_colours; ///< Number of colours
		};
		struct { // If MULTIBOOT_FRAMEBUFFER_TYPE_RGB
			// TODO: Docs
			uint8_t framebuffer_red_field_position;
			uint8_t framebuffer_red_mask_size;
			uint8_t framebuffer_green_field_position;
			uint8_t framebuffer_green_mask_size;
			uint8_t framebuffer_blue_field_position;
			uint8_t framebuffer_blue_mask_size;
		};
	} colour_info;
};
typedef struct multiboot_info multiboot_info_t;


/** @brief Fix all addresses in a given multiboot info
 * 
 * Given a pointer to a multiboot info, this will fix all references to
 * addresses such that they are virtual addresses.
 * 
 * @param mb_info_ptr Pointer to the multiboot object.
*/
void multiboot_fix_addresses(multiboot_info_t** mb_info_ptr);

#endif

/** @} */
