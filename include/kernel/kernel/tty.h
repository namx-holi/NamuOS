/**
 * @file tty.h
 * @defgroup kernel_tty Kernel TTY
 * @brief Methods to write to terminal
 * @ingroup kernel
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H 1

#include <stdint.h> // uint8_t, uint16_t
#include <stddef.h> // size_t


// TODO: Method to print to certain page
// TODO: Method to enable page


#define TTY_TAB_WIDTH 4

// TODO: Document
typedef uint8_t vga_colour_t;
typedef uint16_t vga_entry_t;


// TODO: Document
// Used to identify if colour or monochrome video card is being used
// Ref: https://wiki.osdev.org/Detecting_Colour_and_Monochrome_Monitors
enum video_mode {
	VIDEO_MODE_UNUSED = 0,
	VIDEO_MODE_40x25_COLOUR = 1,
	VIDEO_MODE_80x25_COLOUR = 2,
	VIDEO_MODE_80x25_MONOCHROME = 3
};

// TODO: Document
// Ref: https://wiki.osdev.org/Printing_To_Screen#Color_Table
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
void terminal_initialise(void);

// TODO: Document
void terminal_setcolour(vga_colour_t colour);

// TODO: Document
void terminal_putentryat(unsigned char ch, vga_colour_t colour, size_t x, size_t y);

// TODO: Document
void terminal_putchar(char ch);

// TODO: Document
void terminal_write(const char* data, size_t size);

// TODO: Document
void terminal_writestring(const char* str);

// TODO: Document
vga_colour_t vga_colour_pair(enum vga_colour fg, enum vga_colour bg);

// TODO: Document
vga_entry_t vga_entry(unsigned char ch, vga_colour_t colour);

#endif

/** @} */
