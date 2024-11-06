/**
 * @file terminal.h
 * @defgroup namuos_terminal <namuos/terminal.h>
 * @brief Methods for printing to the terminal
 * @ingroup namuos
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include <namuos/multiboot.h>


/** Available terminal colours for printing with EGA */
enum terminal_colour {
	TERMINAL_COLOUR_BLACK         =  0,
	TERMINAL_COLOUR_BLUE          =  1,
	TERMINAL_COLOUR_GREEN         =  2,
	TERMINAL_COLOUR_CYAN          =  3,
	TERMINAL_COLOUR_RED           =  4,
	TERMINAL_COLOUR_MAGENTA       =  5,
	TERMINAL_COLOUR_BROWN         =  6,
	TERMINAL_COLOUR_LIGHT_GREY    =  7,
	TERMINAL_COLOUR_DARK_GREY     =  8,
	TERMINAL_COLOUR_LIGHT_BLUE    =  9,
	TERMINAL_COLOUR_LIGHT_GREEN   = 10,
	TERMINAL_COLOUR_LIGHT_CYAN    = 11,
	TERMINAL_COLOUR_LIGHT_RED     = 12,
	TERMINAL_COLOUR_LIGHT_MAGENTA = 13,
	TERMINAL_COLOUR_LIGHT_BROWN   = 14,
	TERMINAL_COLOUR_WHITE         = 15
};
#define TERMINAL_DEFAULT_BG TERMINAL_COLOUR_BLACK ///< Default terminal background colour
#define TERMINAL_DEFAULT_FG TERMINAL_COLOUR_LIGHT_GREY ///< Default terminal text colour

/// Width of the tab character in spaces
#define TERMINAL_TAB_WIDTH 4

/// If terminal screen is scrolled upwards when reaching bottom of screen.
#define TERMINAL_SCROLLING_ENBALED 1

// Logging colours
#define KLOG_DEBUG_BG TERMINAL_COLOUR_BLACK ///< Debug message background colour
#define KLOG_DEBUG_FG TERMINAL_COLOUR_LIGHT_BLUE ///< Debug message text colour
#define KLOG_INFO_BG TERMINAL_COLOUR_BLACK ///< Info message background colour
#define KLOG_INFO_FG TERMINAL_COLOUR_LIGHT_GREEN ///< Info message text colour
#define KLOG_WARNING_BG TERMINAL_COLOUR_BLACK ///< Warning message background colour
#define KLOG_WARNING_FG TERMINAL_COLOUR_LIGHT_BROWN ///< Warning message text colour
#define KLOG_ERROR_BG TERMINAL_COLOUR_BLACK ///< Error message background colour
#define KLOG_ERROR_FG TERMINAL_COLOUR_RED ///< Error message text colour
#define KLOG_CRITICAL_BG TERMINAL_COLOUR_RED ///< Critical message background colour
#define KLOG_CRITICAL_FG TERMINAL_COLOUR_WHITE ///< Critical message text colour


/** @brief Sets up the terminal for printing
 * 
 * Sets up the terminal for printing.
 * 
 * @param mb_info Multiboot info passed from GRUB
 * 
 * @returns Returns true on success, otherwise false.
*/
bool terminal_initialise(multiboot_info_t* mb_info);

/** @brief Sets the default terminal colours
 * 
 * Sets the default terminal colours. This does not change the current colours
 * when printing, just what colours will be set when @ref terminal_reset_colour
 * is called.
 * 
 * @param bg Background colour
 * @param fg Text colour
*/
void terminal_set_default_colour(enum terminal_colour bg, enum terminal_colour fg);

/** @brief Sets the current terminal colours
 * 
 * Sets the current terminal colours. This does not effect the default colours.
 * 
 * @param bg Background colour
 * @param fg Text colour
*/
void terminal_set_colour(enum terminal_colour bg, enum terminal_colour fg);

/** @brief Resets the terminal colours to default colours
 * 
 * Sets the current terminal colours to the currently set default terminal
 * colours.
*/
void terminal_reset_colour();

/** @brief Writes a single character to the terminal
 * 
 * Writes a single character to the terminal in the current colour.
 * 
 * @param ch Character to write to terminal
 * 
 * @returns Returns 1 if a character was written to screen, otherwise 0.
*/
int terminal_write_char(char ch);


// TODO: Doxygen description
int kprintf(const char* restrict format, ...);
int klog_debug(const char* restrict format, ...);
int klog_info(const char* restrict format, ...);
int klog_warning(const char* restrict format, ...);
int klog_error(const char* restrict format, ...);
int klog_critical(const char* restrict format, ...);

int kvprintf(const char* restrict format, va_list vlist);
int kvlog_debug(const char* restrict format, va_list vlist);
int kvlog_info(const char* restrict format, va_list vlist);
int kvlog_warning(const char* restrict format, va_list vlist);
int kvlog_error(const char* restrict format, va_list vlist);
int kvlog_critical(const char* restrict format, va_list vlist);

#endif

/** @} */
