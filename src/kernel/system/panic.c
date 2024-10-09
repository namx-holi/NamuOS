/// @file panic.c

#include <namuos/system.h> // Implements


__attribute__((__noreturn__)) void panic(const char* restrict format, ...) {
	klog_critical("kernel panic!\n");
	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);

	while(1) {}
	__builtin_unreachable();
}
