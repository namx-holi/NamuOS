/// @file kprintf.c

#include <namuos/system.h> // Implements

#include <namuos/terminal.h> // terminal_write_char


// Print helpers for format specifiers. Returns 1 if successful, 0 otherwise
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
	// Keep track of the number of bytes written
	int written = 0;

	// Keep writing to termnal until we reach a null terminator. We increment
	//  the `format` pointer after each char print
	while (format[0] != '\0') {
		// Regular character, or '%%'
		if (format[0] != '%' || format[1] == '%') {
			// Skip the 'format specifier' if it's prefixing another '%'
			if (format[0] == '%') ++format;

			// Print a lone character
			int retval = terminal_write_char(format[0]);
			if (retval < 0) return retval; // Pass errors up
			written += retval;

			// Move to next character
			++format;
			continue;
		}

		// Format specifiers. Check what specifier or sub-specifiers we have and
		//  handle.
		++format;
		int retval;
		if (format[0] == 'l') { // Long sub-specifier
			format++; // We've read specifier
			switch(format[0]) {
				case 'd': retval = _kprint_valist_ld(&vlist); break;
				case 'u': retval = _kprint_valist_lu(&vlist); break;
				case 'x': retval = _kprint_valist_lx(&vlist); break;
				default: retval = -1;
			}
		} else { // No sub-specifier, just check specifier
			switch(format[0]) {
				case 'd': retval = _kprint_valist_d(&vlist); break;
				case 'u': retval = _kprint_valist_u(&vlist); break;
				case 'x': retval = _kprint_valist_x(&vlist); break;
				case 'c': retval = _kprint_valist_c(&vlist); break;
				case 's': retval = _kprint_valist_s(&vlist); break;
				case 'p': retval = _kprint_valist_p(&vlist); break;
				default: retval = -1;
			}
		}
		if (retval < 0) return retval; // Pass errors up

		// Increment written characters, and move to next
		written += retval;
		++format;
	}

	// Return the number of bytes written
	return written;
}



// Methods declared in this file:

int _kprint_valist_d(va_list* vlist) {
	int written = 0;
	int32_t num = va_arg(*vlist, int32_t); // Printing int
	int retval;

	// if negative, print '-' and handle if unsigned
	if (num < 0) {
		retval = terminal_write_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}

	retval = _kprint_uint(num);
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
	int ch = va_arg(*vlist, int); // Pull char as int (gets promoted)
	return terminal_write_char((char)ch);
}

int _kprint_valist_s(va_list* vlist) {
	int written = 0;
	const char* str = va_arg(*vlist, const char*); // Null-terminated string
	int retval;

	// Write string, and return length of string printed
	int i = 0;
	while (str[i] != '\0') {
		retval = terminal_write_char(str[i]);
		if (retval < 0) return retval; // Pass errors up
		written += retval;
		++i; // Next char
	}

	return written;
}

int _kprint_valist_p(va_list* vlist) {
	// TODO: Always pad pointer to width of 8
	void* ptr = va_arg(*vlist, void*); // Printing pointer as uint hex
	return _kprint_uint_hex((uint32_t)ptr);
}

int _kprint_valist_ld(va_list* vlist) {
	int written = 0;
	int64_t num = va_arg(*vlist, int64_t); // Printing long int
	int retval;

	// if negative, print '-' and handle if unsigned
	if (num < 0) {
		retval = terminal_write_char('-');
		if (retval < 0) return retval; // Pass errors up
		num = -num;
		++written;
	}

	retval = _kprint_uint(num);
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
