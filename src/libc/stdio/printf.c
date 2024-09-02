/// @file printf.c

#include <stdio.h> // Implements

#include <stdbool.h> // true, false
#include <stddef.h> // size_t
#include <stdarg.h> // va_list
#include <string.h> // strlen
#include <limits.h> // INT_MAX

// TODO: Rewrite


// TODO: Handle:
//  - \a (alert), make sound
//  - \b (backspace), move back one position on current line
//  - \f (form feed), move to start of next page
//  - \n (new line), move to start of next line
//  - \r (carriage return), move to start of current line
//  - \t (horizontal tab), move to next tabulation position
//  - \v (vertical tab), move to next vertical tabulation position
static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}


// Helper for printing an unsigned integer
int print_uint(unsigned int x) {
	int written = 0; // Keep track of number of chars written

	// If number has more than two digits, need to recurse to print next
	//  largest digit
	if (x >= 10) {
		int rv = print_uint(x / 10);

		// If return value of recursive print was -1, error
		if (rv == -1) {
			return -1;
		}

		// Otherwise, add to output count
		written += rv;
	}

	// Print the next single digit!
	char digit = '0' + x%10;
	int rv = print(&digit, 1);

	// If rv is -1, error. Otherwise, add to written and return
	if (rv == -1) {
		return -1;
	}
	return written + rv;
}



// https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
//  Page 327, or section 7.21.6.1
int printf(const char* restrict format, ...) {
	// Handle ... args
	va_list parameters;
	va_start(parameters, format);

	// Count bytes written
	int written = 0;

	// Keep reading from format until finished
	while (*format != '\0') {
		// Check if we're reading aconversion specification (%).

		// After the %, the following appear in sequence:
		// - zero or more flags (in any order) that modify the meaning
		//    of the conversion specification.
		// - an optional minimum field width. If the converted value has
		//    fewer characters than the field width, it is padded with
		//    spaces (by default) on the left (or right, if the left
		//    adjustment flag has been given) to the field width. The
		//    field width takes the form of an asterisk (*) or a
		//    nonnegative decimal integer.
		// - an optional precision that gives the minimum number of
		//    digits to appear for the d,i,o,u,x,X conversions, the
		//    number of digits to appear after the decimal-point char
		//    for a,A,e,E,f,F cnversions, the maximum number of sigfigs
		//    for the g,G conversions, or the maximum number of bytes to
		//    be written for s. The precision takes the form of a
		//    period (.) followed either by an aserisk (*) or by an
		//    optional decimal integer; if only the period is specified,
		//    the precision is taken as zero. If a precision appears
		//    with any other conversion specifier, undefined.
		// - an optional length modifier that specifies the size of the
		//    argument
		// - a conversion specifier character that specifies the type of
		//    conversion to be applied.

		// If character is not (%), print the character. If the
		//  next two characters are (%%), skip the first % and print %
		if (format[0] != '%' || format[1] == '%') {
			// If (%%), skip the first %
			if (format[0] == '%') format++;

			// Count how many characters we can print (up to end of
			//  format, or next (%))
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') amount++;

			// Print those characters! Handle if there's an error
			if (!print(format, amount)) return -1;

			// Update the number of characters printed, and where we are
			//  up to in the format
			format += amount;
			written += amount;
			continue;
		}

		// If up to here, we are reading a conversion specification (%).
		//  Store where this format started at, and progress to reading
		//  the specification.
		const char* format_begun_at = format++;

		// Handle single character %c
		if (*format == 'c') {
			// Jump to the next symbol so we can print it
			format++;

			// Pull the next argument as a char
			char c = (char) va_arg(parameters, int); // char promotes to int

			// Print that char! Handle if there's an error
			if (!print(&c, sizeof(c))) return -1;
			
			// Update number of characters printed
			written += 1;
		}

		// Handle string %s
		else if (*format == 's') {
			// Jump to the next symbol so we can start printing
			format++;

			// Pull the next argument as a string, and get length
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			// Print that string! Handle if there's an error
			if (!print(str, len)) return -1;

			// Update number of characters printed
			written += len;
		}

		// Handle int %d
		else if (*format == 'd') {
			// Jump to the next symbol so we can start printing
			format++;

			// Pull the next argument as an int
			int num = va_arg(parameters, int);

			if (num < 0) {
				if (!print("-", 1)) return -1;
				written += 1;
				num = -num;
			}
			int int_written = print_uint(num);
			if (int_written == -1) return -1;
			written += int_written;
		}

		// If we got an unhandled conversion specifier, ignore it and
		//  print as if it were a string
		else {
			// Step back to (%)
			format = format_begun_at;

			// Count how many characters we can print (up to end of
			//  format, or next (%))
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') amount++;

			// Print those characters! Handle if there's an error
			if (!print(format, amount)) return -1;

			// Update the number of characters printed, and where we are
			//  up to in the format
			format += amount;
			written += amount;
		}
	}





	// Keep reading characters from format until finished
	while (*format != '\0') {
		// TODO: Handle if have a string too long
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
