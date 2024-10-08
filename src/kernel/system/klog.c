/// @file klog.c

#include <namuos/system.h> // Implements

#include <namuos/terminal.h>


void klog_debug(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_debug
	va_list vlist;
	va_start(vlist, format);
	kvlog_debug(format, vlist);
	va_end(vlist);
}

void klog_info(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_info
	va_list vlist;
	va_start(vlist, format);
	kvlog_info(format, vlist);
	va_end(vlist);
}

void klog_warning(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_warning
	va_list vlist;
	va_start(vlist, format);
	kvlog_warning(format, vlist);
	va_end(vlist);
}

void klog_error(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_error
	va_list vlist;
	va_start(vlist, format);
	kvlog_error(format, vlist);
	va_end(vlist);
}

void klog_critical(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_critical

	va_list vlist;
	va_start(vlist, format);
	kvlog_critical(format, vlist);
	va_end(vlist);
}

void kvlog_debug(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(_KLOG_DEBUG_BG, _KLOG_DEBUG_FG);
	kprintf("DEBUG: ");
	kvprintf(format, vlist);
	terminal_reset_colour();
}

void kvlog_info(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(_KLOG_INFO_BG, _KLOG_INFO_FG);
	kprintf("INFO: ");
	kvprintf(format, vlist);
	terminal_reset_colour();
}

void kvlog_warning(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(_KLOG_WARNING_BG, _KLOG_WARNING_FG);
	kprintf("WARNING: ");
	kvprintf(format, vlist);
	terminal_reset_colour();
}

void kvlog_error(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(_KLOG_ERROR_BG, _KLOG_ERROR_FG);
	kprintf("ERROR: ");
	kvprintf(format, vlist);
	terminal_reset_colour();
}

void kvlog_critical(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(_KLOG_CRITICAL_BG, _KLOG_CRITICAL_FG);
	// kprintf("CRITICAL: ");
	kvprintf(format, vlist);
	terminal_reset_colour();
}

