/**
 * @file terminal_colours.h
 * @defgroup arch_i386_terminal_colours <arch_i386/terminal_colours.h>
 * @brief i386 EGA text mode colour codes
 * @ingroup arch_i386
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _ARCH_i386_TERMINAL_H
#define _ARCH_i386_TERMINAL_H 1

// Architecture-dependent colour codes
enum ega_text_mode_colour {
	EGA_COLOUR_BLACK         =  0,
	EGA_COLOUR_BLUE          =  1,
	EGA_COLOUR_GREEN         =  2,
	EGA_COLOUR_CYAN          =  3,
	EGA_COLOUR_RED           =  4,
	EGA_COLOUR_MAGENTA       =  5,
	EGA_COLOUR_BROWN         =  6,
	EGA_COLOUR_LIGHT_GREY    =  7,
	EGA_COLOUR_DARK_GREY     =  8,
	EGA_COLOUR_LIGHT_BLUE    =  9,
	EGA_COLOUR_LIGHT_GREEN   = 10,
	EGA_COLOUR_LIGHT_CYAN    = 11,
	EGA_COLOUR_LIGHT_RED     = 12,
	EGA_COLOUR_LIGHT_MAGENTA = 13,
	EGA_COLOUR_LIGHT_BROWN   = 14,
	EGA_COLOUR_WHITE         = 15
};

#endif

/** @} */
