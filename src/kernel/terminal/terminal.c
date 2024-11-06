/// @file terminal.c

#include <namuos/terminal.h> // Implements

#include <stddef.h>
#include <string.h> // memmove
#include <namuos/bios_defines.h>
#include <namuos/paging.h> // __to_virt


// Location fo the next location to print a character
static size_t next_row;
static size_t next_col;

// EGA terminal parameters
static size_t EGA_WIDTH = 0;
static size_t EGA_HEIGHT = 0;
static uint16_t* EGA_BUFFER = (uint16_t*)0;

// Default and current terminal colours
uint8_t default_colour;
uint8_t current_colour;


/** @brief Adds a VGA entry to @ref EGA_BUFFER at index
 * 
 * Updates VGA entry of the current colour to @ref EGA_BUFFER at the given
 * index, writing it to screen.
 * 
 * @param index Index in @ref EGA_BUFFER
 * @param ch Character to add to @ref EGA_BUFFER
*/
void _terminal_set_entry(uint32_t index, char ch);

/** @brief Boundary checks terminal, moving to next row if necessary
 * 
 * Boundary checks the display window of the terminal. Moves to the next line
 * or row if necessary, and scrolls text if @ref _TERMINAL_SCROLLING_ENABLED is
 * set.
*/
void _terminal_boundscheck();


bool terminal_initialise(multiboot_info_t* mb_info) {
	// Check if we've been given framebuffer info by GRUB. If not, we can't set
	//  up the terminal.
	if (!(mb_info->flags & MULTIBOOT_FLAG_FRAME))
		return false;
	
	// Check that we're in EGA text mode. If not, unhandled
	if (mb_info->framebuffer_type != MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)
		return false;
	
	// Set up the width, height, and address of buffer
	EGA_WIDTH  = mb_info->framebuffer_width;
	EGA_HEIGHT = mb_info->framebuffer_height;
	EGA_BUFFER = __to_virt(VIDEO_MEMORY_ADDR_COLOUR);

	// Set the current and default terminal colours
	terminal_set_default_colour(TERMINAL_DEFAULT_BG, TERMINAL_DEFAULT_FG);
	terminal_reset_colour();

	// Clear the terminal!
	for (uint32_t i = 0; i < EGA_WIDTH * EGA_HEIGHT; ++i) {
		_terminal_set_entry(i, ' ');
	}

	return true;
}

void terminal_set_default_colour(enum terminal_colour bg, enum terminal_colour fg) {
	// Background colour is the first 4 bits, and text colour is last 4 bits
	default_colour = ((uint8_t)bg << 4 | (uint8_t)fg);
}

void terminal_set_colour(enum terminal_colour bg, enum terminal_colour fg) {
	// Background colour is the first 4 bits, and text colour is last 4 bits
	current_colour = ((uint8_t)bg << 4 | (uint8_t)fg);
}

void terminal_reset_colour() {
	// Set the current colour to default colour
	current_colour = default_colour;
}

int terminal_write_char(char ch) {
	// Special characters
	switch (ch) {
		case '\n': // Line feed (newline)
			++next_row; // Move to start of next line
			next_col = 0;
			_terminal_boundscheck();
			return 1;
		
		case '\f': // Form feed
			++next_row; // Handle same as '\v'
			_terminal_boundscheck();
			return 1;
		
		case '\r': // Carriage return
			next_col = 0; // Move to start of current line
			// Doesn't progress in row/col, no needs for boundscheck
			return 1;
		
		case '\t': // Horizontal tab
			// Move to next tab boundary
			next_col += TERMINAL_TAB_WIDTH - (next_col % TERMINAL_TAB_WIDTH);
			_terminal_boundscheck();
			return 1;
		
		case '\v': // Vertical tab
			++next_row; // Move down one row without changing column
			_terminal_boundscheck();
			return 1;
	}

	// TODO: Check if `ch` is in range for printable characters. If not in
	//  range, return 0 and skip printing.

	// Not a special character, just write `ch` to the EGA buffer
	_terminal_set_entry(next_row * EGA_WIDTH + next_col, ch);

	// Move to the next printing position and boundscheck row/col
	++next_col;
	_terminal_boundscheck();

	return 1;
}

void _terminal_set_entry(uint32_t index, char ch) {
	// Entry is 16 bits, structured as 8 bit colour followed by character.
	EGA_BUFFER[index] = ((uint16_t)current_colour << 8) | (uint16_t)ch;
}

void _terminal_boundscheck() {
	// Move tot he next line if the terminal width has been exceeded
	if (next_col >= EGA_WIDTH) {
		++next_row;
		next_col = 0;
	}

	#if TERMINAL_SCROLLING_ENBALED
	// If we reached the end of the screen, scroll everything up one line so we
	//  can continue printing at the bottom of the screen.
	if (next_row >= EGA_HEIGHT) {
		// Reset back to the previous line so we can continue printing at bottom
		--next_row;

		// Shift everything we've printed so far back one line
		uint32_t copied_entry_count = EGA_WIDTH * (EGA_HEIGHT - 1);
		memmove(
			(void*)EGA_BUFFER, // Moving here
			(void*)EGA_BUFFER + EGA_WIDTH*2, // From here (each entry 2 bytes)
			copied_entry_count * 2
		);

		// Clear the bottom line with empty characters so we can start printing
		for (uint32_t i = 0; i < EGA_WIDTH; ++i)
			_terminal_set_entry(copied_entry_count+i, ' ');
	}
	#else
	// If we reached the end of the screen, move back to the top
	if (next_row >= EGA_HEIGHT)
		next_row = 0;
	#endif
}
