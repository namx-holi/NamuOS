/// @file abort.c

#include <stdlib.h> // Implements

#include <stdio.h> // printf

#ifdef __is_libk
#include <kernel/system.h> // kprintf
#endif


__attribute__((__noreturn__)) void abort(void) {
	#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	kprintf("kernel: panic: abort()\n");
	#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
	#endif
	while (1) { }
	// TODO: Sleep? So it doesn't nuke
	__builtin_unreachable();
}
