/// @file abort.c

#include <stdlib.h> // Implements

#if defined(__is_libk)
#include <namuos/system.h> // panic
#endif


// TODO: Implement abort
__attribute__((__noreturn__)) void abort(void) {
	#if defined(__is_libk)
	// If compiled with kernel, panic
	panic("abort() called.\n");
	__builtin_unreachable();
	#else
	#error "abort() is not implemented outside of kernel"
	#endif
}

// TODO: Implement exit
__attribute__((__noreturn__)) void exit(int exit_code) {
	#if defined(__is_libk)
	// If compiled with kernel, panic with exit code
	panic("exit(%d) called.\n", exit_code);
	__builtin_unreachable();
	#else
	#error "exit() is not implemented outside of kernel"
	#endif
}

// TODO: Implement quick_exit
__attribute__((__noreturn__)) void quick_exit(int exit_code) {
	#if defined(__is_libk)
	// If compiled with kernel, panic with exit code
	panic("quick_exit(%d) called.\n", exit_code);
	__builtin_unreachable();
	#else
	#error "quick_exit() is not implemented outside of kernel"
	#endif
}

// TODO: Implement _Exit
__attribute__((__noreturn__)) void _Exit(int exit_code) {
	#if defined(__is_libk)
	// If compiled with kernel, panic with exit code
	panic("_Exit(%d) called.\n", exit_code);
	__builtin_unreachable();
	#else
	#error "_Exit() is not implemented outside of kernel"
	#endif
}

// TODO: Implement atexit
// int atexit(void (*func)(void));

// TODO: Implement at_quick_exit
// int at_quick_exit(void (*func)(void));

// TODO: Implement system
// int system(const char* command);

// TODO: Implement getenv
// char* getenv(const char* name);


#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
#error "TODO: Implement in stdlib/program.c"
errno_t getenv_s(size_t* restrict len, char* restrict value, rsize_t valuesz, const char* restrict name);
#endif
