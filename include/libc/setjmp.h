/**
 * @file setjmp.h
 * @defgroup libc_setjmp <setjmp.h>
 * @brief Nonlocal jumps
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/program)
 * for **Program support utilities**
 * 
 * @{
*/

#ifndef _LIBC_SETJMP_H
#define _LIBC_SETJMP_H 1


/************************************************/
/* Program Support Functions                    */
/* Ref: https://en.cppreference.com/w/c/program */
/************************************************/

// TODO: Documentation, Execution context type
typedef struct {} jmp_buf;
// TODO: Define jmp_buf

// TODO: Documentation, Saves the context
#define setjmp(env) /* implementation defined */
// TODO: Define setjmp

// TODO: Documentation, Jumps to specified location
extern void longjmp(jmp_buf env, int status) __attribute__((__noreturn__));

#endif

/** @} */
