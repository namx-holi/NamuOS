/// @file system.h

#ifndef _KERNEL_SYSTEM_H
#define _KERNEL_SYSTEM_H 1

#include <stdarg.h>

#include <kernel/multiboot.h>

// Type used for pointers to physical addresses
typedef uintptr_t physical_addr_t;

// kprintf
extern int kernel_setup_kprintf(multiboot_info_t* mb_info);
extern void kernel_kprintf_shift_ega_addr(physical_addr_t add);
extern int kprintf(const char* restrict format, ...);
extern int kvprintf(const char* restrict format, va_list arg);

// panic
extern void panic(const char* restrict format, ...);

#endif