/// @file kprint.c

#include <namuos/terminal.h> // Implements


// Print helpers for format specifiers. Returns number of printed characters.
int _kprint_valist_d(va_list* vlist); // int
int _kprint_valist_u(va_list* vlist); // unsigned int
int _kprint_valist_x(va_list* vlist); // unsigned int, hex
int _kprint_valist_c(va_list* vlist); // char
int _kprint_valist_s(va_list* vlist); // null-terminated string
int _kprint_valist_p(va_list* vlist); // pointer (void*)
int _kprint_valist_ld(va_list* vlist); // long int
int _kprint_valist_lu(va_list* vlist); // unsigned long int
int _kprint_valist_lx(va_list* vlist); // unsigned long int, hex

// Helpers for printing numbers
int _kprint_uint(uint64_t x);
int _kprint_uint_hex(uint64_t x);


int kprintf(const char* restrict format, ...) {
	// Pull out variadic args and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	int retval = kvprintf(format, vlist);
	va_end(vlist);
	return retval;
}

int kvprintf(const char* restrict format, va_list vlist) {
	// Keep track of the number of characters written
	int written = 0;

	// Keep writing to terminal until we reach a null terminator. We increment
	//  the `format` pointer after each char print.
	int retval;
	while (format[0] != '\0') {
		// Regular character, or '%%'
		if (format[0] != '%' || format[1] == '%') {
			// Skip the 'format specifier' if it's prefixing another '%'
			if (format[0] == '%') ++format;

			// Print a lone character
			retval = terminal_write_char(format[0]);
			if (retval < 0) return retval; // Pass errors up
			written += retval;

			// Move to next character
			++format;
			continue;
		}

		// Format specifiers. Check what specifier or sub-specifiers we have and
		//  handle them.
		++format; // Next character after '%'
		if (format[0] == 'l') { // Long sub-specifier
			++format; // Next character after 'l'
			switch (format[0]) {
				case 'd': retval = _kprint_valist_ld(&vlist); break;
				case 'u': retval = _kprint_valist_lu(&vlist); break;
				case 'x': retval = _kprint_valist_lx(&vlist); break;
				default: retval = -1; // error
			}
		} else { // No sub-specifier, just check specifier
			switch(format[0]) {
				case 'd': retval = _kprint_valist_d(&vlist); break;
				case 'u': retval = _kprint_valist_u(&vlist); break;
				case 'x': retval = _kprint_valist_x(&vlist); break;
				case 'c': retval = _kprint_valist_c(&vlist); break;
				case 's': retval = _kprint_valist_s(&vlist); break;
				case 'p': retval = _kprint_valist_p(&vlist); break;
				default: retval = -1; // error
			}
		}
		if (retval < 0) return retval; // Pass errors up

		// Update the written character count, and move to next char
		written += retval;
		++format;
	}

	// Return the number of bytes written
	return written;
}

int _kprint_valist_d(va_list* vlist) {
	int written = 0;
	int32_t num = va_arg(*vlist, int32_t); // Printing int
	int retval;

	// If negative, print '-' and handle if unsigned
	if (num < 0) {
		retval = terminal_write_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written; // '-'
	}

	retval = _kprint_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _kprint_valist_u(va_list* vlist) {
	uint32_t num = va_arg(*vlist, uint32_t); // Printing unsigned int
	return _kprint_uint(num);
}

int _kprint_valist_x(va_list* vlist) {
	uint32_t num = va_arg(*vlist, uint32_t); // Printing unsigned int, hex
	return _kprint_uint_hex(num);
}

int _kprint_valist_c(va_list* vlist) {
	int ch = va_arg(*vlist, int); // Printing char (char gets promoted to int)
	return terminal_write_char((char)ch);
}

int _kprint_valist_s(va_list* vlist) {
	int written = 0;
	const char* str = va_arg(*vlist, const char*); // Printing null-terminated string
	int retval;

	// Write string, and return length of string printed
	for (int i = 0; str[i] != '\0'; ++i) {
		retval = terminal_write_char(str[i]);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}
	return written;
}

int _kprint_valist_p(va_list* vlist) {
	// TODO: Always pad pointer to width of 8
	void* ptr = va_arg(*vlist, void*); // Printing pointer (void*)
	return _kprint_uint_hex((uint32_t)ptr);
}

int _kprint_valist_ld(va_list* vlist) {
	int written = 0;
	int64_t num = va_arg(*vlist, int64_t); // Printing long int
	int retval;

	// If negative, print '-' and handle if unsigned
	if (num < 0) {
		retval = terminal_write_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written; // '-'
	}

	retval = _kprint_uint(num);
	if (retval < 0) return retval; // Pass errors up
	return written + retval;
}

int _kprint_valist_lu(va_list* vlist) {
	uint64_t num = va_arg(*vlist, uint64_t); // Printing unsigned long int
	return _kprint_uint(num);
}

int _kprint_valist_lx(va_list* vlist) {
	uint64_t num = va_arg(*vlist, uint64_t); // Printing unsigned long int, hex
	return _kprint_uint_hex(num);
}

int _kprint_uint(uint64_t x) {
	int written = 0;
	int retval;

	// If more than two decimal digits, recurse
	if (x >= 10) {
		retval = _kprint_uint(x / 10);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print next single digit
	char digit = '0' + (x % 10);
	retval = terminal_write_char(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1;
}

int _kprint_uint_hex(uint64_t x) {
	int written = 0;
	int retval;

	// If more than two hexadecimal digits, recurse
	if (x >= 16) {
		retval = _kprint_uint_hex(x / 16);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
	}

	// Print next single digit
	char digit = x % 16;
	if (digit < 10) digit += '0'; // 0-9
	else digit += ('a' - 10); // a-f
	retval = terminal_write_char(digit);
	if (retval < 0) return retval; // Pass errors up
	return written + 1;
}
