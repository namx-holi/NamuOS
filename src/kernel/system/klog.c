/// @file klog.c

#include <kernel/system.h> // Implements

#include <kernel/vga.h>


static const enum vga_colour DEBUG_BG = VGA_COLOUR_BLACK;
static const enum vga_colour DEBUG_FG = VGA_COLOUR_LIGHT_BLUE;
void klog_debug(const char* restrict format, ...) {
	// Set colour scheme
	uint8_t prev_colour = kprint_get_colour();
	kprint_set_colour(DEBUG_BG<<4 | DEBUG_FG);

	// Pull out VA and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	kprintf("DEBUG: ");
	kvprintf(format, vlist);
	va_end(vlist);

	// Reset colour scheme
	kprint_set_colour(prev_colour);
}

static const enum vga_colour INFO_BG = VGA_COLOUR_BLACK;
static const enum vga_colour INFO_FG = VGA_COLOUR_LIGHT_GREEN;
void klog_info(const char* restrict format, ...) {
	// Set colour scheme
	uint8_t prev_colour = kprint_get_colour();
	kprint_set_colour(INFO_BG<<4 | INFO_FG);

	// Pull out VA and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	kprintf("INFO: ");
	kvprintf(format, vlist);
	va_end(vlist);

	// Reset colour scheme
	kprint_set_colour(prev_colour);
}

static const enum vga_colour WARNING_BG = VGA_COLOUR_BLACK;
static const enum vga_colour WARNING_FG = VGA_COLOUR_LIGHT_BROWN;
void klog_warning(const char* restrict format, ...) {
	// Set colour scheme
	uint8_t prev_colour = kprint_get_colour();
	kprint_set_colour(WARNING_BG<<4 | WARNING_FG);

	// Pull out VA and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	kprintf("WARNING: ");
	kvprintf(format, vlist);
	va_end(vlist);

	// Reset colour scheme
	kprint_set_colour(prev_colour);
}

static const enum vga_colour ERROR_BG = VGA_COLOUR_BLACK;
static const enum vga_colour ERROR_FG = VGA_COLOUR_RED;
void klog_error(const char* restrict format, ...) {
	// Set colour scheme
	uint8_t prev_colour = kprint_get_colour();
	kprint_set_colour(ERROR_BG<<4 | ERROR_FG);

	// Pull out VA and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	kprintf("ERROR: ");
	kvprintf(format, vlist);
	va_end(vlist);

	// Reset colour scheme
	kprint_set_colour(prev_colour);
}

static const enum vga_colour CRITICAL_BG = VGA_COLOUR_RED;
static const enum vga_colour CRITICAL_FG = VGA_COLOUR_BLACK;
void klog_critical(const char* restrict format, ...) {
	// Set colour scheme
	uint8_t prev_colour = kprint_get_colour();
	kprint_set_colour(CRITICAL_BG<<4 | CRITICAL_FG);

	// Pull out VA and pass to kvprintf
	va_list vlist;
	va_start(vlist, format);
	kprintf("CRITICAL: ");
	kvprintf(format, vlist);
	va_end(vlist);

	// Reset colour scheme
	kprint_set_colour(prev_colour);
}
