/// @file panic.c

#include <namuos/panic.h> // Implements

#include <stdarg.h>
#include <namuos/terminal.h>


__attribute__((__noreturn__)) void __panic(const char* file, uint32_t line, const char* restrict format, ...) {
	klog_critical("kernel panic: file '%s', line %d!\n", file, line);

	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);

	while(1) {}
	__builtin_unreachable();
}
