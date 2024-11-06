/// @file klog.c

#include <namuos/terminal.h> // Implements


int klog_debug(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_debug
	va_list vlist;
	va_start(vlist, format);
	int retval = kvlog_debug(format, vlist);
	va_end(vlist);
	return retval;
}

int klog_info(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_info
	va_list vlist;
	va_start(vlist, format);
	int retval = kvlog_info(format, vlist);
	va_end(vlist);
	return retval;
}

int klog_warning(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_warning
	va_list vlist;
	va_start(vlist, format);
	int retval = kvlog_warning(format, vlist);
	va_end(vlist);
	return retval;
}

int klog_error(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_error
	va_list vlist;
	va_start(vlist, format);
	int retval = kvlog_error(format, vlist);
	va_end(vlist);
	return retval;
}

int klog_critical(const char* restrict format, ...) {
	// Pull out VA and pass to kvlog_critical
	va_list vlist;
	va_start(vlist, format);
	int retval = kvlog_critical(format, vlist);
	va_end(vlist);
	return retval;
}

int kvlog_debug(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(KLOG_DEBUG_BG, KLOG_DEBUG_FG);
	int retval = kprintf("DEBUG: ");
	retval += kvprintf(format, vlist);
	terminal_reset_colour();
	return retval;
}

int kvlog_info(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(KLOG_INFO_BG, KLOG_INFO_FG);
	int retval = kprintf("INFO: ");
	retval += kvprintf(format, vlist);
	terminal_reset_colour();
	return retval;
}

int kvlog_warning(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(KLOG_WARNING_BG, KLOG_WARNING_FG);
	int retval = kprintf("WARNING: ");
	retval += kvprintf(format, vlist);
	terminal_reset_colour();
	return retval;
}

int kvlog_error(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(KLOG_ERROR_BG, KLOG_ERROR_FG);
	int retval = kprintf("ERROR: ");
	retval += kvprintf(format, vlist);
	terminal_reset_colour();
	return retval;
}

int kvlog_critical(const char* restrict format, va_list vlist) {
	// Set colour scheme, pass to kvprintf, and reset colour scheme
	terminal_set_colour(KLOG_CRITICAL_BG, KLOG_CRITICAL_FG);
	int retval = kprintf("WARNING: ");
	retval += kvprintf(format, vlist);
	terminal_reset_colour();
	return retval;
}
