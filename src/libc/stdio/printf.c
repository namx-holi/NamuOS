/// @file printf.c

#include <stdio.h> // Implements

#include <stdint.h> // uint types
#include <string.h> // strlen


// Print helpers
// TODO: Move this elsewhere, will be used in multiple printf methods
int _print_valist_d(va_list* vlist); // int
int _print_valist_u(va_list* vlist); // uint
int _print_valist_x(va_list* vlist); // uint, hex
int _print_valist_c(va_list* vlist); // character
int _print_valist_s(va_list* vlist); // string
int _print_valist_p(va_list* vlist); // pointer
int _print_valist_ld(va_list* vlist); // long int
int _print_valist_lu(va_list* vlist); // long uint
int _print_valist_lx(va_list* vlist); // long uint, hex

// Helpers to print numbers
int _print_uint(uint64_t x);
int _print_uint_hex(uint64_t x);



int printf(const char* restrict format, ...) {
	// Fetch variadic args from ...
	va_list vlist;
	va_start(vlist, format);

	// Count bytes written
	int written = 0;

	// Keep writing from format until we reach a null terminator
	// NOTE: We increment format pointer after each char read
	while (*format != '\0') {
		// TODO: Handle if we have a string too long
		// size_t maxrem = INT_MAX - written;

		// If the next character is not '%', or next two is '%%',
		//  print the character or '%' respectively
		if (format[0] != '%' || format[1] == '%') {
			// If '%%', move to second '%' as char to print
			if (format[0] == '%') ++format;

			// TODO: Instead of printing one character at a time, can count
			//  non-% and print a chunk.

			// Print the character!
			putchar(format[0]); // TODO: Check return value

			// Update the number of bytes written, and next char to check
			++format;
			++written;
			continue;
		}

		// By reaching here, we know we are reading a conversion specificiation
		//  prefixed by '%'. Skip to the next character to start reading this
		//  specificiation.
		++format;
		// NOTE: May need to store pointer to format to print non-valid format
		//  specifiers

		// TODO: Handle format sub-specifiers:
		// - flags, width, .precision, modifiers
		// Ref: https://cplusplus.com/reference/cstdio/printf/

		// TODO: Handle setting errno

		// Check if we've got a length sub-specifier
		// TODO: Fill this in more!
		int print_retval;
		if (*format == 'l') { // Long sub-specifier
			format++;
			switch (*format) {
				case 'd': print_retval = _print_valist_ld(&vlist); break;
				case 'u': print_retval = _print_valist_lu(&vlist); break;
				case 'x': print_retval = _print_valist_lx(&vlist); break;
				default: print_retval = -1;
			}
		} else { // No length sub-specifier
			switch (*format) {
				case 'd': print_retval = _print_valist_d(&vlist); break;
				case 'u': print_retval = _print_valist_u(&vlist); break;
				case 'x': print_retval = _print_valist_x(&vlist); break;
				case 'c': print_retval = _print_valist_c(&vlist); break;
				case 's': print_retval = _print_valist_s(&vlist); break;
				case 'p': print_retval = _print_valist_p(&vlist); break;
				default: print_retval = -1;
			}
		}

		// If print_retval was negative, we had an issue printing that argument
		if (print_retval < 0) {
			va_end(vlist);
			return -1;
		}

		// Otherwise, update written length and move to next character
		++written;
		++format;
	}

	// Done!
	va_end(vlist);
	return written;
}



int _print_valist_d(va_list* vlist) { // int
	int written = 0; // Keep track of number of chars written
	int32_t num = va_arg(*vlist, int32_t); // Pull next argument as an int

	// If negative, print '-' and handle as if unsigned
	if (num < 0) {
		int retval = putchar('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}

	// Print unsigned int (negative signed handled above)
	int retval = _print_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _print_valist_u(va_list* vlist) { // uint
	uint32_t num = va_arg(*vlist, uint32_t); // Pull next argument as uint
	return _print_uint(num); // Pass errors up on error, otherwise nb written
}

int _print_valist_x(va_list* vlist) { // uint, hex
	uint32_t num = va_arg(*vlist, uint32_t); // Pull next argument as uint
	return _print_uint_hex(num); // Pass errors up on error, otherwise nb written
}

int _print_valist_c(va_list* vlist) { // character
	int num = va_arg(*vlist, int); // Pull char as int (gets promoted)

	// Cast to a char, and print it directly
	unsigned char ch = (unsigned char)num;
	int retval = putchar(ch);
	if (retval < 0) return retval; // Pass errors up
	return 1; // Successfully printed 1 char
}

int _print_valist_s(va_list* vlist) { // string
	const char* str = va_arg(*vlist, const char*); // Pull next argument as string

	// Write string, and return length of that string
	int retval = puts(str);
	if (retval < 0) return retval; // Pass errors up
	return strlen(str);
}

int _print_valist_p(va_list* vlist) { // pointer
	void* ptr = va_arg(*vlist, void*); // Pull next argument as pointer
	return _print_uint_hex((uint32_t)ptr); // Pass errors up on error, otherwise nb written
}

int _print_valist_ld(va_list* vlist) { // long int
	int written = 0; // Keep track of number of chars written
	int64_t num = va_arg(*vlist, int64_t); // Pull next argument as long int

	// If negative, print '-' and handle as if unsigned
	if (num < 0) {
		int retval = putchar('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}

	// Pring unsigned int (negative signed handled above)
	int retval = _print_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _print_valist_lu(va_list* vlist) { // long uint
	uint64_t num = va_arg(*vlist, uint64_t); // Pull next argument as long uint
	return _print_uint(num); // Pass errors up on error, otherwise nb written
}

int _print_valist_lx(va_list* vlist) { // long uint, hex
	uint64_t num = va_arg(*vlist, uint64_t); // Pull next argument as long uint
	return _print_uint_hex(num); // Pass errors up on error, otherwise nb written
}



int _print_uint(uint64_t x) {
	int written = 0; // Keep track of number of chars written

	// If number has more than two decimal digits, need to recurse
	if (x >= 10) {
		int retval = _print_uint(x / 10);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print the next single digit, and return (to recurse or finish)
	char digit = '0' + (x % 10);
	int retval = putchar(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1; // putchar is 1 char
}

int _print_uint_hex(uint64_t x) {
	int written = 0; // Keep track of number of chars written

	// If number has more than two hex digits, need to recurse
	if (x >= 16) {
		int retval = _print_uint_hex(x / 16);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print the next single digit, and return (to recurse or finish)
	char digit = x % 16;
	if (digit < 10) digit += '0'; // 0-9
	else digit += ('a' - 10); // 10-15 (a-f)

	int retval = putchar(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1; // putchar is 1 char
}
