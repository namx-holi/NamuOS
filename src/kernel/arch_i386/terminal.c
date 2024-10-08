/// @file terminal.c

#include <namuos/terminal.h> // Implements

#include <stddef.h>
#include <string.h> // memmove
#include <arch_i386/bios_defines.h>
#include <arch_i386/terminal_colours.h>
#include <namuos/memory.h>


// TODO: Doxygen comments for internal method declarations


// Position to print next character on the terminal
static size_t next_row;
static size_t next_col;

// EGA terminal parameters
static size_t EGA_WIDTH = 0;
static size_t EGA_HEIGHT = 0;
static uint16_t* EGA_MEMORY_ADDR = (uint16_t*)0;

// Default terminal colour, and current colour
uint8_t default_colour;
uint8_t current_colour;

// Translates TERMINAL_ generic colour to EGA_ colour
enum ega_text_mode_colour translate_terminal_colour(enum terminal_colour terminal_colour);

// Adds a VGA entry of current colour at given index in EGA_MEMORY_ADDR, writing
//  to screen.
void terminal_set_entry(uint32_t index, char ch);

// Boundary checking for the display window of the terminal. Moves to the next
//  line or row if necessary.
#define _TERMINAL_SCROLLING_ENABLED 1 /* Will scroll display instead of moving to top */
void terminal_boundscheck();



int terminal_initialise(multiboot_info_t* mb_info) {
	// Check if we've been given framebuffer info by GRUB. If not, we can't
	//  set up the terminal.
	if (!(mb_info->flags & MULTIBOOT_FLAG_FRAME))
		return 0; // Failure
	
	// Check that we're in EGA text mode. If not, that's currently unhandled.
	if (mb_info->framebuffer_type != MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)
		return 0; // Failure
	
	// Set up the width, height, and memory address
	EGA_WIDTH = mb_info->framebuffer_width;
	EGA_HEIGHT = mb_info->framebuffer_height;
	EGA_MEMORY_ADDR = __to_virt(VIDEO_MEMORY_ADDR_COLOUR); // address is physical

	// Set the current and default terminal colour
	terminal_set_default_colour(TERMINAL_DEFAULT_BG, TERMINAL_DEFAULT_FG);
	terminal_reset_colour();

	// Clear the terminal with default colour
	for (uint32_t i = 0; i < EGA_WIDTH * EGA_HEIGHT; ++i) {
		terminal_set_entry(i, ' ');
	}

	return 1; // Success
}

void terminal_set_default_colour(enum terminal_colour bg, enum terminal_colour fg) {
	// `bg` is the first 4 bits of a colour, and `fg` is the last 4 bits
	default_colour = (
		(uint8_t)translate_terminal_colour(bg) << 4 |
		(uint8_t)translate_terminal_colour(fg));
}

void terminal_set_colour(enum terminal_colour bg, enum terminal_colour fg) {
	// `bg` is the first 4 bits of a colour, and `fg` is the last 4 bits
	current_colour = (
		(uint8_t)translate_terminal_colour(bg) << 4 |
		(uint8_t)translate_terminal_colour(fg));
}

void terminal_reset_colour() {
	// Set the current colour to default
	current_colour = default_colour;
}

int terminal_write_char(char ch) {
	// Special characters
	switch (ch) {
		case '\n': // Line feed (newline)
			++next_row; // Move to start of next line
			next_col = 0;
			terminal_boundscheck();
			return 1;
		
		case '\f': // Form feed
			++next_row; // Handle same as '\v'
			terminal_boundscheck();
			return 1;
		
		case '\r': // Carriage return
			next_col = 0; // Move to start of current line
			// Doesn't progress in row/col, no need for bounds check
			return 1;
		
		case '\t': // Horizontal tab
			// Move to the next tab boundary
			next_col += TERMINAL_TAB_WIDTH - (next_col & TERMINAL_TAB_WIDTH);
			terminal_boundscheck();
			return 1;
		
		case '\v': // Vertical tab
			++next_row; // Move down one row without changing column
			terminal_boundscheck();
			return 1;
	}

	// TODO: Check if ch is in range for printable characters. If not in range,
	//  return 0.

	// Not a special character, write `ch` to the EGA buffer
	terminal_set_entry(next_row * EGA_WIDTH + next_col, ch);

	// Move to next printing position and boundscheck row/col
	++next_col;
	terminal_boundscheck();

	return 1;
}



// Methods declared in this file:

enum ega_text_mode_colour translate_terminal_colour(enum terminal_colour terminal_colour) {
	// Turn a TERMINAL_COLOUR into architecture specific colour
	switch (terminal_colour) {
		case TERMINAL_COLOUR_BLACK: return EGA_COLOUR_BLACK;
		case TERMINAL_COLOUR_BLUE: return EGA_COLOUR_BLUE;
		case TERMINAL_COLOUR_GREEN: return EGA_COLOUR_GREEN;
		case TERMINAL_COLOUR_CYAN: return EGA_COLOUR_CYAN;
		case TERMINAL_COLOUR_RED: return EGA_COLOUR_RED;
		case TERMINAL_COLOUR_MAGENTA: return EGA_COLOUR_MAGENTA;
		case TERMINAL_COLOUR_BROWN: return EGA_COLOUR_BROWN;
		case TERMINAL_COLOUR_LIGHT_GREY: return EGA_COLOUR_LIGHT_GREY;
		case TERMINAL_COLOUR_DARK_GREY: return EGA_COLOUR_DARK_GREY;
		case TERMINAL_COLOUR_LIGHT_BLUE: return EGA_COLOUR_LIGHT_BLUE;
		case TERMINAL_COLOUR_LIGHT_GREEN: return EGA_COLOUR_LIGHT_GREEN;
		case TERMINAL_COLOUR_LIGHT_CYAN: return EGA_COLOUR_LIGHT_CYAN;
		case TERMINAL_COLOUR_LIGHT_RED: return EGA_COLOUR_LIGHT_RED;
		case TERMINAL_COLOUR_LIGHT_MAGENTA: return EGA_COLOUR_LIGHT_MAGENTA;
		case TERMINAL_COLOUR_LIGHT_BROWN: return EGA_COLOUR_LIGHT_BROWN;
		case TERMINAL_COLOUR_WHITE: return EGA_COLOUR_WHITE;
	}

	// Invalid colour just mapped to black
	return EGA_COLOUR_BLACK;
}

void terminal_set_entry(uint32_t index, char ch) {
	// Entry is an 8 bit colour, followed by the character.
	EGA_MEMORY_ADDR[index] = ((uint16_t)current_colour << 8) | (uint16_t)ch;
}

void terminal_boundscheck() {
	// Move to the next line if the terminal width has been exceeded
	if (next_col >= EGA_WIDTH) {
		++next_row;
		next_col = 0;
	}

	#if _TERMINAL_SCROLLING_ENABLED
	// If we reached the end of the screen, scroll everything up one line so we
	//  can continue printing at the bottom of the screen.
	if (next_row >= EGA_HEIGHT) {
		// Reset back to previous line so we can continue printing at the bottom
		--next_row;

		// Shift everything we've printed so far back one line
		uint32_t copied_entry_count = EGA_WIDTH * (EGA_HEIGHT - 1);
		memmove(
			(void*)EGA_MEMORY_ADDR, // Moving to here
			(void*)EGA_MEMORY_ADDR + EGA_WIDTH*2, // From here (each entry 2 bytes)
			copied_entry_count*2
		);

		// Clear the bottom line with empty characters so we can start printing
		for (uint32_t i = 0; i < EGA_WIDTH; ++i)
			terminal_set_entry(copied_entry_count+i, ' ');
	}
	#else
	// If we reached the end of the screen, move back to the top
	if (next_row >= EGA_HEIGHT)
		next_row = 0;
	#endif
}
