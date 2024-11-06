/// @file panic.h
// TODO: Doxygen comment

#ifndef _PANIC_H
#define _PANIC_H 1

#include <stdint.h>

#define panic(format, ...) __panic(__FILE__, __LINE__, format, ##__VA_ARGS__)
void __panic(const char* file, uint32_t line, const char* restrict format, ...) __attribute__((__noreturn__));

#endif
