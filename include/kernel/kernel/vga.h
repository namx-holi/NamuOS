/**
 * @file vga.h
 * @defgroup kernel_vga Kernel VGA helpers
 * @brief Helper methods for writing VGA data to terminal
 * @ingroup kernel
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h> // uint8_t

// TODO: Document
enum vga_colour {
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_LIGHT_MAGENTA = 13,
	VGA_COLOUR_LIGHT_BROWN = 14,
	VGA_COLOUR_WHITE = 15,
};

// TODO: Document
extern uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg);

// TODO: Document
extern uint16_t vga_entry(unsigned char ch, uint8_t colour);

#endif

/** @} */
