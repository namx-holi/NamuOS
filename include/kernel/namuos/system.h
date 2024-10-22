/**
 * @file system.h
 * @defgroup namuos_system <namuos/system.h>
 * @brief Common methods for printing etc used by kernel
 * @ingroup namuos
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _NAMUOS_SYSTEM_H
#define _NAMUOS_SYSTEM_H 1

#include <stdarg.h>
#include <stdint.h>

// TODO: Doxygen comments

// Logging colours
#define _KLOG_DEBUG_BG TERMINAL_COLOUR_BLACK ///< Debug message background colour
#define _KLOG_DEBUG_FG TERMINAL_COLOUR_LIGHT_BLUE ///< Debug message text colour
#define _KLOG_INFO_BG TERMINAL_COLOUR_BLACK ///< Info message background colour
#define _KLOG_INFO_FG TERMINAL_COLOUR_LIGHT_GREEN ///< Info message text colour
#define _KLOG_WARNING_BG TERMINAL_COLOUR_BLACK ///< Warning message background colour
#define _KLOG_WARNING_FG TERMINAL_COLOUR_LIGHT_BROWN ///< Warning message text colour
#define _KLOG_ERROR_BG TERMINAL_COLOUR_BLACK ///< Error message background colour
#define _KLOG_ERROR_FG TERMINAL_COLOUR_RED ///< Error message text colour
#define _KLOG_CRITICAL_BG TERMINAL_COLOUR_RED ///< Critical message background colour
#define _KLOG_CRITICAL_FG TERMINAL_COLOUR_WHITE ///< Critical message text colour


// Kernel printing methods
int kprintf(const char* restrict format, ...);
int kvprintf(const char* restrict format, va_list vlist);

// Logging methods for kernel printing
void klog_debug(const char* restrict format, ...);
void klog_info(const char* restrict format, ...);
void klog_warning(const char* restrict format, ...);
void klog_error(const char* restrict format, ...);
void klog_critical(const char* restrict format, ...);
void kvlog_debug(const char* restrict format, va_list vlist);
void kvlog_info(const char* restrict format, va_list vlist);
void kvlog_warning(const char* restrict format, va_list vlist);
void kvlog_error(const char* restrict format, va_list vlist);
void kvlog_critical(const char* restrict format, va_list vlist);

// Panic!
// TODO: Rename panic_new, make the new standard instead of old panic
#define panic_new(format, ...) _panic_new(__FILE__, __LINE__, format, __VA_ARGS__)
void panic(const char* restrict format, ...) __attribute__((__noreturn__));
void _panic_new(const char* file, uint32_t line, const char* restrict format, ...) __attribute__((__noreturn__));

#endif

/** @} */
