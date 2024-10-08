/// @file panic.c

#include <namuos/system.h> // Implements


void panic(const char* restrict format, ...) {
	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);

	klog_critical("kernel: panic: abort()\n");
	while(1) {}
	__builtin_unreachable();
}
