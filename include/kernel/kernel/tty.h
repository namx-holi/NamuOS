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

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t


/** @brief Sets up terminal interface, and clears screen
 * 
 * Sets up the pointer @ref terminal_buffer to point to where VGA data needs to
 * be written for displaying characters on the terminal. Also sets the default
 * printing colour, and clears the screen.
*/
void terminal_initialise(void);

/** @brief Sets the current colour scheme of the terminal
 * 
 * Sets the colour for printing characters to the terminal. A colour is
 * characterised as 4 bits representing the background colour, followed by 4
 * bits representing the colour of the character. THe colours available are
 * outlined in @ref vga_colour, and a colour can be constructed using the
 * @ref vga_entry_colour method.
 * 
 * @param colour New colour scheme of the terminal
*/
void terminal_setcolour(uint8_t colour);

/** @brief Writes a character of given colour at given position on the terminal
 * 
 * Writes a character `ch` to the terminal in the given `colour` and position.
 * No checks are made for if the given character is printable or not.
 * 
 * @param ch Character to write to terminal
 * @param colour Colour of the character to print
 * @param x Column of where to print the character
 * @param y Row of where to print the character
*/
void terminal_putentryat(unsigned char ch, uint8_t colour, size_t x, size_t y);

/** @brief Writes a character to the terminal
 * 
 * Writes a character after the last written character using this method,
 * @ref terminal_write, or @ref terminal_writestring. Printing is bounds checked
 * against the size of the terminal, and if a character would go off the screen
 * in its row, it is moved to the next column.
 * 
 * Some whitespace characters (e.g. <tt>'\\n'</tt>) adjust the current printing
 * position.
 * 
 * The position of the last written character is updated after writing.
 * 
 * @param ch Character to write to terminal
*/
void terminal_putchar(char ch);

/** @brief Writes a character array to the terminal
 * 
 * Writes an array of size `size` after the last written character using
 * @ref terminal_putchar, this method, or @ref terminal_writestring. Printing
 * is bounds checked against the size of the terminal, and if a character would
 * go off the screen in its row, it is moved to the next column.
 * 
 * Some whitespace characters (e.g. <tt>'\\n'</tt>) adjust the current printing
 * position.
 * 
 * The position of the last written character is updated after writing.
 * 
 * @param data Array of characters to write to terminal
 * @param size Number of characters to write from `array`
*/
void terminal_write(const char* data, size_t size);

/** @brief Writes a null-terminated string to the terminal
 * 
 * Writes a null-terminated C string after the last written character using
 * @ref terminal_putchar, @ref terminal_write, or this method. Printing is
 * bounds checked against the size of the terminal, and if a character would go
 * off the screen in its row, it is moved to the next column.
 * 
 * Some whitespace characters (e.g. <tt>'\\n'</tt>) adjust the current printing
 * position.
 * 
 * The position of the last written character is updated after writing.
 * 
 * @param str Null-terminated C string to write to terminal
*/
void terminal_writestring(const char* str);

#endif

/** @} */
