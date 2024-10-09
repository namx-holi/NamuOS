/// @file ctype.c

#include <ctype.h> // Implements

// NOTE: Making the assumption in all cases that C locale will always be default


int isalnum(int ch) {
	return (isdigit(ch) || isalpha(ch));
}

int isalpha(int ch) {
	return (isupper(ch) || islower(ch));
}

int islower(int ch) {
	return (ch >= 'a' && ch <= 'z');
}

int isupper(int ch) {
	return (ch >= 'A' && ch <= 'Z');
}

int isdigit(int ch) {
	return (ch >= '0' && ch <= '9');
}

int isxdigit(int ch) {
	return (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
}

int iscntrl(int ch) {
	return ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f);
}

int isgraph(int ch) {
	return (ch >= '!' && ch <= '~');
}

int isspace(int ch) {
	return ((ch >= '\t' && ch <= '\r') || ch == ' ');
}

int isblank(int ch) {
	return (ch == '\t' || ch == ' ');
}

int isprint(int ch) {
	return (ch >= ' ' && ch <= '~');
}

int ispunct(int ch) {
	// Punctuation is all graphical symbols, excluding alphanumeric
	return (isgraph(ch) && !isalnum(ch));
}

int tolower(int ch) {
	// Don't change non-lowercase characters
	if (!isupper(ch))
		return ch;
	
	// Lowercase letters in default locale are offset by 32
	return ch + 32;
}

int toupper(int ch) {
	// Don't change non-uppercase characters
	if (!islower(ch))
		return ch;
	
	// Uppercase letters in default locale are offset by 32
	return ch - 32;
}
