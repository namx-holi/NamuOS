/// @file printf.c

#include <stdio.h> // Implements

#if defined(__is_libk)
#include <namuos/terminal.h> // kvprintf
#endif


int printf(const char* restrict format, ...) {
	// TODO: Pass to vfprintf with stdin
	va_list vlist;
	va_start(vlist, format);
	int retval = vprintf(format, vlist);
	va_end(vlist);
	return retval;
}

// TODO: Implement fprintf
// int fprintf(FILE* restrict stream, const char* restrict format, ...);

// TODO: Implement sprintf
// int sprintf(char* restrict buffer, const char* restrict format, ...);

// TODO: Are we going to implement snprintf?
// int snprintf(char* restrict buffer, size_t bufsz, const char* restrict format, ...);

// TODO: Implement vprintf
int vprintf(const char* restrict format, va_list vlist) {
	#if defined(__is_libk)
	// If compiled with kernel, pass to kvprintf
	return kvprintf(format, vlist);
	#else
	#error "vsprintf() is not implemented outside of kernel"
	#endif
}

// TODO: Implement vfprintf
// int vfprintf(FILE* restrict stream, const char* restrict format, va_list vlist);

// TODO: Implement vsprintf
// int vsprintf(char* restrict buffer, const char* restrict format, va_list vlist);

// TODO: Implement vsnprintf
// int vsnprintf(char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist);


#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
#error "TODO: Implement in stdio/printf.c"
int printf_s(const char* restrict format, ...);
int fprintf_s(FILE* restrict stream, const char* restrict format, ...);
int sprintf_s(char* restrict buffer, rsize_t bufsz, const char* restrict format, ...);
int sprintf_s(char* restrict buffer, rsize_t bufsz, const char* restrict format, ...);
int vprintf_s(const char* restrict format, va_list vlist);
int vfprintf_s(FILE* restrict stream, const char* restrict format, va_list vlist);
int vsprintf_s(char* restrict buffer, rsize_t bufsz, const char* restrict format, va_list vlist);
int vsnprintf_s(char* restrict buffer, rsize_t bufsz, const char* restrict format, va_list vlist);
#endif
