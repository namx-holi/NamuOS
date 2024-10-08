/// @file paging.c

#include <arch_i386/paging.h> // Implements


void invalidate_page(uintptr_t vaddr) {
	asm volatile ("invlpg (%0)" : : "r"(vaddr) : "memory");
}
