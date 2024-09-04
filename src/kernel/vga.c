/// @file vga.c

#include <kernel/vga.h> // Implements

inline uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg) {
	// Background colour, followed by foreground colour
	return bg << 4 | fg;
}

inline uint16_t vga_entry(unsigned char ch, uint8_t colour) {
	// Colour first, followed by printing character
	return (uint16_t)colour << 8 | (uint16_t)ch;
}
