/**
 * @file terminal.h
 * @defgroup namuos_terminal <namuos/terminal.h>
 * @brief Architecture-independent methods for interfacing with the screen
 * @ingroup namuos
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _NAMUOS_TERMINAL_H
#define _NAMUOS_TERMINAL_H 1

#include <stdint.h>
#include <namuos/arch.h>
#include <namuos/multiboot.h>

// Architecture-independent colour codes. These should be translated by each
//  architecture to set the correct colour.
enum terminal_colour {
	TERMINAL_COLOUR_BLACK,
	TERMINAL_COLOUR_BLUE,
	TERMINAL_COLOUR_GREEN,
	TERMINAL_COLOUR_CYAN,
	TERMINAL_COLOUR_RED,
	TERMINAL_COLOUR_MAGENTA,
	TERMINAL_COLOUR_BROWN,
	TERMINAL_COLOUR_LIGHT_GREY,
	TERMINAL_COLOUR_DARK_GREY,
	TERMINAL_COLOUR_LIGHT_BLUE,
	TERMINAL_COLOUR_LIGHT_GREEN,
	TERMINAL_COLOUR_LIGHT_CYAN,
	TERMINAL_COLOUR_LIGHT_RED,
	TERMINAL_COLOUR_LIGHT_MAGENTA,
	TERMINAL_COLOUR_LIGHT_BROWN,
	TERMINAL_COLOUR_WHITE
};


// Default terminal colours when terminal is initialised
#define TERMINAL_DEFAULT_BG TERMINAL_COLOUR_BLACK ///< Default terminal background colour
#define TERMINAL_DEFAULT_FG TERMINAL_COLOUR_LIGHT_GREY ///< Default terminal text colour

/// Width of the tab character, in spaces
#define TERMINAL_TAB_WIDTH 4


/** @brief Sets up the terminal based on current architecture
 * 
 * Sets up the terminal based on the current architecture.
 * 
 * @param mb_info Multiboot info passed from GRUB
 * 
 * @returns Returns 1 on success, otherwise 0.
*/
ARCH_METHOD int terminal_initialise(multiboot_info_t* mb_info);

/** @brief Sets the default terminal colours
 * 
 * Sets the default terminal colours. This does not change the current colours
 * when printing, just what colours will be set when @ref terminal_reset_colour
 * is called.
 * 
 * @param bg Background colour
 * @param fg Text colour
*/
ARCH_METHOD void terminal_set_default_colour(enum terminal_colour bg, enum terminal_colour fg);

/** @brief Sets the current terminal colours
 * 
 * Sets the current terminal colours. This does not effect the default colours.
 * 
 * @param bg Background colour
 * @param fg Text colour
*/
ARCH_METHOD void terminal_set_colour(enum terminal_colour bg, enum terminal_colour fg);

/** @brief Resets the terminal colours to the set default colours
 * 
 * Sets the current terminal colours to the currently set default terminal
 * colours.
*/
ARCH_METHOD void terminal_reset_colour();

/** @brief Writes a single character to the terminal
 * 
 * @param ch Character to write to terminal
 * 
 * @returns Returns 1 if a character was written, 0 if nothing was written to
 *  screen.
*/
ARCH_METHOD int terminal_write_char(char ch);

#endif

/** @} */
