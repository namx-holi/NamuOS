/// @file tty.c

#include <kernel/tty.h> // Implements

#include <stdbool.h> // bool

#include <kernel/bios_defines.h>


// TODO: Bounds check

// Loaded on initialise from BIOS
static size_t TERMINAL_WIDTH = 0;
static size_t TERMINAL_HEIGHT = 0;
static bool   TERMINAL_MONOCHROME = false;
static uint16_t* VIDEO_MEMORY_ADDR = (uint16_t*)0;

// Position to print next character on screen
static size_t terminal_next_row;
static size_t terminal_next_col;

// Current printing colour for normal printing
static vga_colour_t terminal_colour;

// Default colour config for terminal startup
static const enum vga_colour terminal_default_fg = VGA_COLOUR_LIGHT_GREY;
static const enum vga_colour terminal_default_bg = VGA_COLOUR_BLACK;


// TODO: Documentation
void terminal_bounds_check() {
    // TODO: Handle scrolling down?

    // If width exceeded, move to start of next line
    if (terminal_next_col >= TERMINAL_WIDTH) {
        terminal_next_col = 0;
        ++terminal_next_row;
    }

    // If we reached end of screen, move to top of screen
    if (terminal_next_row >= TERMINAL_HEIGHT)
        terminal_next_row = 0;
}



void terminal_initialise(void) {
    // First fetch the detected equipment flags from BIOS (2 bytes)
    uint16_t detected_hardware = *BDA_EQUIPMENT_LIST_FLAGS;

    // Check what video mode is being used by the monitor
    enum video_mode vmode = (enum video_mode)(
        (detected_hardware & BDA_EQUIPT_INITIAL_VIDEO_MODE_MASK)
        >> BDA_EQUIPT_INITIAL_VIDEO_MODE_SHIFT);
    
    // Set up the width/height/colour for the monitor
    switch (vmode) {
        case VIDEO_MODE_UNUSED:
            // TODO: How to handle this on boot?
        
        case VIDEO_MODE_40x25_COLOUR:
            TERMINAL_WIDTH = 40;
            TERMINAL_HEIGHT = 25;
            TERMINAL_MONOCHROME = false;
            break;
        
        case VIDEO_MODE_80x25_COLOUR:
            TERMINAL_WIDTH = 80;
            TERMINAL_HEIGHT = 25;
            TERMINAL_MONOCHROME = false;
            break;
        
        case VIDEO_MODE_80x25_MONOCHROME:
            TERMINAL_WIDTH = 80;
            TERMINAL_HEIGHT = 25;
            TERMINAL_MONOCHROME = true;
            break;
    }

    // Base on if monochrome or not, set the video memory address
    if (TERMINAL_MONOCHROME)
        VIDEO_MEMORY_ADDR = VIDEO_MEMORY_ADDR_MONOCHROME;
    else
        VIDEO_MEMORY_ADDR = VIDEO_MEMORY_ADDR_COLOUR;
    
    // Initialsie the next position to print in, and terminal colour
    terminal_colour = vga_colour_pair(terminal_default_fg, terminal_default_bg);
    terminal_next_row = 0;
    terminal_next_col = 0;

    // Completely clear the screen by printing spaces
    for (size_t y = 0; y < TERMINAL_HEIGHT; ++y) {
        for (size_t x = 0; x < TERMINAL_WIDTH; ++x) {
            terminal_putentryat(' ', terminal_colour, x, y);
        }
    }
}

void terminal_setcolour(vga_colour_t colour) {
    terminal_colour = colour;
}

void terminal_putentryat(unsigned char ch, vga_colour_t colour, size_t x, size_t y) {
    // TODO: Bounds check x and y?

    // Write character at given position
    const size_t index = y * TERMINAL_WIDTH + x;
    VIDEO_MEMORY_ADDR[index] = vga_entry(ch, colour);
}

void terminal_putchar(char ch) {
    // TODO: Does `ch` need to be cast to unsigned?

    // Handle any special printing characters
	switch (ch) {
		// TODO: Handle alert (\a), backspace (\b), form feed (\f)

		case '\n': // Line feed (newline)
			// Treat newline like unix, move to the start of the next line. Also
			//  bounds check
			++terminal_next_row;
			terminal_next_col = 0;
			terminal_bounds_check();
			return;
		
		case '\f': // Form feed
			// TODO: Handle form feed. For now, is ignored if printed. I think
			//  it should act in the same way as vertical tab, but unsure.
			return;
		
		case '\r': // Carriage return
			// Move the cursor back to start of the line
			terminal_next_col = 0;
			return;
		
		case '\t': // Horizontal tab
			// Move across to next alignment of tab width
			terminal_next_col += TTY_TAB_WIDTH - (terminal_next_col % TTY_TAB_WIDTH);
			terminal_bounds_check();
			return;

		case '\v': // Vertical tab
			// Move down one row, and bounds check for height
			++terminal_next_row;
			terminal_bounds_check();
			return;
	}

    // If just a regular character, place it on the screen at next position
    terminal_putentryat(ch, terminal_colour, terminal_next_col, terminal_next_row);

    // Update the last printed-in column, and bounds check so we don't print off
    //  the screen on the next write
    ++terminal_next_col;
    terminal_bounds_check();
}

void terminal_write(const char* data, size_t size) {
    // Print each character in `data` array
    for (size_t i = 0; i < size; ++i)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* str) {
    // Write characters until we reach a null terminator
    size_t i = 0;
    while (str[i] != '\0')
        terminal_putchar(str[i++]);
}

vga_colour_t vga_colour_pair(enum vga_colour fg, enum vga_colour bg) {
    // A colour is a uint8, background colour followed by foreground colour
    return bg << 4 | fg;
}

vga_entry_t vga_entry(unsigned char ch, vga_colour_t colour) {
    // TODO: Handle monochrome?

    // Colour first, followed by printing character
    return (vga_entry_t)colour << 8 | (vga_entry_t)ch;
}
