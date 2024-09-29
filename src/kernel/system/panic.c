/// @file panic.c

#include <kernel/system.h> // Implements

#include <stdlib.h> // abort

extern void panic(const char* restrict format, ...) {
	// TODO: Use klog_error method?
	va_list vlist;
	va_start(vlist, format);
	kvprintf(format, vlist);
	va_end(vlist);

	abort();
}
