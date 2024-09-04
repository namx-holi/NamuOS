/**
 * @file tty.c
 * @brief Methods to write to terminal
 * @ingroup kernel_tty
 * 
 * @todo Detailed description
 * 
 * @{
*/

#include <kernel/tty.h> // Implements

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint16_t

#include <kernel/vga.h>


// TODO: Move these to a config file somewhere
static const size_t VGA_WIDTH = 80;  ///< Default width of terminal
static const size_t VGA_HEIGHT = 25; ///< Default height of terminal

/// @brief Location of VGA display buffer
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
// TODO: Documentation on where this is found

static size_t terminal_row;       ///< Current row of terminal cursor
static size_t terminal_column;    ///< Current column of terminal cursor
static uint8_t terminal_colour;   ///< Current colour for terminal printing

/** @brief Memory location containing the contents of the terminal
 * 
 * Memory location used for the contents of the terminal. By writing to this
 * buffer, indexed by `height * VGA_WIDTH + width`, characters are written to
 * the screen.
 * 
 * Values in this buffer are unsigned int16s, where the lower byte represents
 * the character to display, and the upper byte represents the colour of the
 * display character.
*/
static uint16_t* terminal_buffer;
/** @} */

/** @brief Bounds checks the position of @ref terminal_row and @ref terminal_column
 * 
 * Updates the position of @ref terminal_row to the start of a new line if
 * exceeds @ref TERMINAL_WIDTH, and updates the position of @ref terminal_column
 * to the next line if @ref terminal_row was incremented, or the top of the
 * screen if @ref TERMINAL_HEIGHT was exceeded.
*/
void terminal_boundscheck(void) {
	// If width exceeded, move to start of next line
	if ((terminal_column + 1) == VGA_WIDTH) {
		terminal_column = 0; 
		++terminal_row;

		// If reached end of screen, move to top of screen
		if (terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_initialise(void) {
	// Set the 'last printed position' to top left corner of screen
	terminal_row = 0;
	terminal_column = 0;

	// Set the default printing colour of the terminal
	// TODO: Move these to a config file somewhere
	terminal_colour = vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK);

	// Set the pointer to location we need to write to to print to terminal
	terminal_buffer = VGA_MEMORY;

	// Completely clear the screen by printing spaces
	for (size_t y = 0; y < VGA_HEIGHT; ++y) {
		for (size_t x = 0; x < VGA_WIDTH; ++x) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}
}

void terminal_setcolour(uint8_t colour) {
	terminal_colour = colour;
}

void terminal_putentryat(unsigned char ch, uint8_t colour, size_t x, size_t y) {
	// TODO: Bounds check x and y?

	// Write character at given position
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(ch, colour);
}

void terminal_putchar(char ch) {
	// TODO: Does `ch` need to be casted to unsigned?
	
	// Handle any special printing characters
	switch (ch) {
		// TODO: Handle alert (\a), backspace (\b), form feed (\f)

		case '\n': // Line feed (newline)
			// Treat newline like unix, move to the start of the next line. Also
			//  bounds check
			++terminal_row;
			terminal_column = 0;
			terminal_boundscheck();
			return;
		
		case '\f': // Form feed
			// TODO: Handle form feed. For now, is ignored if printed. I think
			//  it should act in the same way as vertical tab, but unsure.
			return;
		
		case '\r': // Carriage return
			// Move the cursor back to start of the line
			terminal_column = 0;
			return;
		
		case '\t': // Horizontal tab
			// Move across the tab size, and bounds check for width
			// TODO: Define default tab width somewhere
			// TODO: Handle alignment
			terminal_column += 4; 
			terminal_boundscheck();
			return;

		case '\v': // Vertical tab
			// Move down one row, and bounds check for height
			++terminal_row;
			terminal_boundscheck();
			return;
	}

	// If just a regular character, place it on the screen at the next position
	terminal_putentryat(ch, terminal_colour, terminal_column, terminal_row);

	// Update the last printed in column, and bounds check so we don't print off
	//  the screen on the next write
	++terminal_column;
	terminal_boundscheck();
}

void terminal_write(const char* data, size_t size) {
	// Print each character in `data` array
	for (size_t i = 0; i < size; ++i)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* str) {
	// We *could* use terminal_write and strlen in conjunction here, but there's
	//  no point in scanning over the array to find length and scanning it
	//  again to print. Instead, we iterate until we find a null terminator
	size_t i = 0;
	while (str[i] != '\0')
		terminal_putchar(str[i++]);
}
