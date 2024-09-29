/// @file system.h

#ifndef _KERNEL_SYSTEM_H
#define _KERNEL_SYSTEM_H 1

#include <stdarg.h>

#include <kernel/multiboot.h>

// kprintf
extern int kernel_setup_kprintf(multiboot_info_t* mb_info);
extern void kernel_kprintf_update_page_offset(uintptr_t add);
extern int kprintf(const char* restrict format, ...);
extern int kvprintf(const char* restrict format, va_list arg);
extern uint8_t kprint_get_colour();
extern void kprint_set_colour(uint8_t colour);
// Logging methods
extern void klog_debug(const char* restrict format, ...);
extern void klog_info(const char* restrict format, ...);
extern void klog_warning(const char* restrict format, ...);
extern void klog_error(const char* restrict format, ...);
extern void klog_critical(const char* restrict format, ...);

// panic
extern void panic(const char* restrict format, ...);


#endif