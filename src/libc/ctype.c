/// @file ctype.c

#include <ctype.h> // Implements

// NOTE: See https://en.cppreference.com/w/c/string/byte for how a lot of the
//  character ranges are defined

// NOTE: Making the assumption that C local will *always* be default


int isalnum(int ch) {
	if (isdigit(ch) || isalpha(ch))
		return 1;
	return 0;
}

int isalpha(int ch) {
	if (isupper(ch) || islower(ch))
		return 1;
	return 0;
}

int islower(int ch) {
	if (ch >= 'a' && ch <= 'z')
		return 1;
	return 0;
}

int isupper(int ch) {
	if (ch >= 'A' && ch <= 'Z')
		return 1;
	return 0;
}

int isdigit(int ch) {
	if (ch >= '0' && ch <= '9')
		return 1;
	return 0;
}

int isxdigit(int ch) {
	if (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
		return 1;
	return 0;
}

int iscntrl(int ch) {
	if ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f)
		return 1;
	return 0;
}

int isgraph(int ch) {
	if (ch >= '!' && ch <= '~')
		return 1;
	return 0;
}

int isspace(int ch) {
	if ((ch >= '\t' && ch <= '\r') || ch == ' ')
		return 1;
	return 0;
}

int isblank(int ch) {
	if (ch == '\t' || ch == ' ')
		return 1;
	return 0;
}

int isprint(int ch) {
	if (ch >= ' ' && ch <= '~')
		return 1;
	return 0;
}

int ispunct(int ch) {
	// Punctuation is all graphical symbols, excluding alphanumeric
	if (isgraph(ch) && !isalnum(ch))
		return 1;
	return 0;
}

int tolower(int ch) {
	// Only make lowercase if already uppercase
	if (!isupper(ch))
		return ch;
	
	// Lowercase letters in default locale are offset by 32, can just add
	return ch + 32;
}

int toupper(int ch) {
	// Only make uppercase if already lowercase
	if (!islower(ch))
		return ch;
	
	// Uppercase letters in default locale are offset by 32, can just subtract
	return ch - 32;
}
