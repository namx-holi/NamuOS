/// @file panic.c

#include <namuos/system.h> // Implements


// TODO: Remove old panic, reinstate _panic_new as standard
__attribute__((__noreturn__)) void panic(const char* restrict format, ...) {
	klog_critical("kernel panic in file %s, line %d\n", __FILE__, __LINE__);
	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);

	while(1) {}
	__builtin_unreachable();
}

__attribute__((__noreturn__)) void _panic_new(const char* file, uint32_t line, const char* restrict format, ...) {
	klog_critical("kernel panic: file '%s', line %d!\n", file, line);
	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);

	while(1) {}
	__builtin_unreachable();
}
