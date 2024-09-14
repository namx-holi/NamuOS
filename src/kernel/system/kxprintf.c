/// @file kprint.c

#include <kernel/system.h> // Implements

#include <stdint.h> // uint types
#include <string.h> // strlen
#include <stdarg.h> // va_list

#include <kernel/bios_defines.h> // VGA write address
#include <kernel/vga.h>


// Position to print next character on screen
static size_t next_row;
static size_t next_col;

// EGA mode handling
#define EGA_TAB_WIDTH 4
static size_t EGA_WIDTH = 0;
static size_t EGA_HEIGHT = 0;
static uint16_t* EGA_MEMORY_ADDR = (uint16_t*)0;
static uint8_t EGA_COLOUR;
static const enum vga_colour EGA_DEFAULT_FG = VGA_COLOUR_LIGHT_GREY;
static const enum vga_colour EGA_DEFAULT_BG = VGA_COLOUR_BLACK;
void ega_boundscheck();


// Print helpers for format specifiers
// Returns number of chars printed (0 or 1 if successful)
int _kprint_valist_d(va_list* vlist); // int
int _kprint_valist_u(va_list* vlist); // uint
int _kprint_valist_x(va_list* vlist); // uint, hex
int _kprint_valist_c(va_list* vlist); // character
int _kprint_valist_s(va_list* vlist); // string
int _kprint_valist_p(va_list* vlist); // pointer
int _kprint_valist_ld(va_list* vlist); // long int
int _kprint_valist_lu(va_list* vlist); // long uint
int _kprint_valist_lx(va_list* vlist); // long uint, hex

// Helpers for printing numbers
// Returns number of chars printed (0 or 1 if successful)
int _kprint_uint(uint64_t x);
int _kprint_uint_hex(uint64_t x);

// Puts a character onto the terminal, based on what mode we're on
// Returns number of chars printed (0 or 1 if successful)
int _kprint_char(char ch);


int kprintf(const char* restrict format, ...) {
	// Pull out variadic args and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	int retval = kvprintf(format, vlist);
	va_end(vlist);
	return retval;
}


int kvprintf(const char* restrict format, va_list vlist) {
	// Count bytes written
	int written = 0;

	// Keep writing from format until we reach a null terminator
	// NOTE: We increment format pointer after each char print
	while (*format != '\0') {
		// Not a format-specifier (or %%)
		if (format[0] != '%' || format[1] == '%') {
			// Skip 'format specifier' if %%
			if (format[0] == '%') ++format;

			// Lone character
			int retval = _kprint_char((char)format[0]); // Any other character
			if (retval < 0) return retval; // Pass errors up
			++written;

			// Move to next character
			++format;
			continue;
		}

		// Format specifiers
		++format; // Skip % to read first char of specifier
		int retval;
		if (*format == 'l') { // Long sub-specifier
			format++; // Read specifier
			switch(*format) {
				case 'd': retval = _kprint_valist_ld(&vlist); break;
				case 'u': retval = _kprint_valist_lu(&vlist); break;
				case 'x': retval = _kprint_valist_lx(&vlist); break;
				default: retval = -1;
			}
		} else { // No sub-specifier, read specifier
			switch(*format) {
				case 'd': retval = _kprint_valist_d(&vlist); break;
				case 'u': retval = _kprint_valist_u(&vlist); break;
				case 'x': retval = _kprint_valist_x(&vlist); break;
				case 'c': retval = _kprint_valist_c(&vlist); break;
				case 's': retval = _kprint_valist_s(&vlist); break;
				case 'p': retval = _kprint_valist_p(&vlist); break;
				default: retval = -1;
			}
		}

		// If error, pass error up. Otherwise, increment written by retval
		if (retval < 0) return retval; // Pass errors up
		written += retval;
		
		// Move to next character
		++format;
	}

	return written;
}


int kernel_setup_kprintf(multiboot_info_t* mb_info) {
	// TODO: Handle other modes
	// https://en.wikipedia.org/wiki/VESA_BIOS_Extensions

	// TODO: Debug this for other modes
	// NOTE: Assuming framebuffer set up, and we can fetch terminal size?
	if (!(mb_info->flags & MULTIBOOT_FLAG_FRAME)) {
		return -1;
	}

	switch (mb_info->framebuffer_type) {
		case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT: {
			// TODO: Set global as EGA mode
			EGA_WIDTH = mb_info->framebuffer_width;
			EGA_HEIGHT = mb_info->framebuffer_height;
			EGA_MEMORY_ADDR = VIDEO_MEMORY_ADDR_COLOUR; // TODO: Need to check if monochrome?
			EGA_COLOUR = (EGA_DEFAULT_BG | EGA_DEFAULT_FG);
		}; break;

		default: return -2;
	}

	return 0; // success
}

void kernel_kprintf_shift_ega_addr(physical_addr_t add) {
	EGA_MEMORY_ADDR = (uint16_t*)((uintptr_t)EGA_MEMORY_ADDR + add);
}


int _kprint_char(char ch) {
	// TODO: Check global mode. Assuming EGA currently

	// Special characters
	switch (ch) {
		case '\n': // Line feed (newline)
			++next_row;
			next_col = 0;
			ega_boundscheck();
			return 1; // One character printed, successful!
		
		case '\f': // Form feed
			++next_row; // Handle same as '\v'
			ega_boundscheck();
			return 1; // One character printed, successful!
		
		case '\r': // Carriage return
			next_col = 0;
			return 1; // One character printed, successful!
		
		case '\t': // Horizontal tab
			next_col += EGA_TAB_WIDTH - (next_col % EGA_TAB_WIDTH);
			ega_boundscheck();
			return 1; // One character printed, successful!
		
		case '\v': // Vertical tab
			++next_row;
			ega_boundscheck();
			return 1; // One character printed, successful!
	}

	// Normal character
	const size_t index = next_row * EGA_WIDTH + next_col;
	EGA_MEMORY_ADDR[index] = (vga_entry_t)EGA_COLOUR << 8 | (vga_entry_t)ch;
	
	// Move to next position
	++next_col;
	ega_boundscheck();

	return 1; // One character printed, successful!
}

void ega_boundscheck() {
	// TODO: Handle scrolling down?

	// If width exceeded, move to start of next line
	if (next_col >= EGA_WIDTH) {
		next_col = 0;
		++next_row;
	}

	// If we reached end of screen, move to top of screen
	if (next_row >= EGA_HEIGHT)
		next_row = 0;
}



int _kprint_uint(uint64_t x) {
	int written = 0; // Keep track of numbers of chars written

	// If more than two decimal digits, recurse
	if (x >= 10) {
		int retval = _kprint_uint(x / 10);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print next single digit
	char digit = '0' + (x % 10);
	int retval = _kprint_char(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1;
}

int _kprint_uint_hex(uint64_t x) {
	int written = 0; // Keep track of numbers of chars written

	// If more than two hexadecimal digits, recurse
	if (x >= 16) {
		int retval = _kprint_uint_hex(x / 16);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print next single digit
	char digit = x % 16;
	if (digit < 10) digit += '0'; // 0-9
	else digit += ('a' - 10); // 10-15 (a-f)
	int retval = _kprint_char(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1;
}

int _kprint_valist_d(va_list* vlist) {
	int written = 0; // Keep track of number of chars written
	int32_t num = va_arg(*vlist, int32_t); // Printing int

	if (num < 0) { // If negative, print '-' and handle as if unsigned
		int retval = _kprint_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}
	int retval = _kprint_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _kprint_valist_u(va_list* vlist) {
	uint32_t num = va_arg(*vlist, uint32_t); // Printing uint
	return _kprint_uint(num);
}

int _kprint_valist_x(va_list* vlist) {
	uint32_t num = va_arg(*vlist, uint32_t); // Printing uint as hex
	return _kprint_uint_hex(num);
}

int _kprint_valist_c(va_list* vlist) {
	int num = va_arg(*vlist, int); // Pull char as int (gets promoted)
	return _kprint_char((char)num);
}

int _kprint_valist_s(va_list* vlist) {
	const char* str = va_arg(*vlist, const char*); // Null-terminated string

	// Write string, and return length of string
	int written = 0;
	int i = 0;
	int retval;
	while (str[i] != '\0') {
		retval = _kprint_char(str[i]);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
		++i; // Next char
	}
	return retval;
}

int _kprint_valist_p(va_list* vlist) {
	void* ptr = va_arg(*vlist, void*); // Printing pointer as uint hex
	return _kprint_uint_hex((uint32_t)ptr);
}

int _kprint_valist_ld(va_list* vlist) {
	int written = 0; // Keep track of number of chars written
	int64_t num = va_arg(*vlist, int64_t); // Printing long int

	if (num < 0) { // If negative, print '-' and handle as if unsigned
		int retval = _kprint_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}
	int retval = _kprint_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _kprint_valist_lu(va_list* vlist) {
	uint64_t num = va_arg(*vlist, uint64_t); // Printing long uint
	return _kprint_uint(num);
}

int _kprint_valist_lx(va_list* vlist) {
	uint64_t num = va_arg(*vlist, uint64_t); // Printing long uint as hex
	return _kprint_uint_hex(num);
}
