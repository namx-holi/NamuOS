/**
 * @file signal.h
 * @defgroup libc_signal <signal.h>
 * @brief Signal handling
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/program)
 * for **Program support utilities**
 * 
 * @{
*/

#ifndef _LIBC_SIGNAL_H
#define _LIBC_SIGNAL_H 1


/************************************************/
/* Program Support Functions                    */
/* Ref: https://en.cppreference.com/w/c/program */
/************************************************/

// TODO: Documentation, Defines signal handling strategies
#define SIG_DFL 0 /* implementation defined */
#define SIG_IGN 1 /* implementation defined */
// TODO: Check values for SIG_DFL, SIG_IGN

// TODO: Documentation, Error was encountered
#define SIG_ERR /* implementation defined */
// TODO: Define SIG_ERR

// TODO: Documentation, Defines signal types
#define SIGTERM /* implementation defined */
#define SIGSEGV /* implementation defined */
#define SIGINT  /* implementation defined */
#define SIGILL  /* implementation defined */
#define SIGABRT /* implementation defined */
#define SIGFPE  /* implementation defined */
// TODO: Define SIGx values
// TODO: Documentation for each signal

// TODO: Documentation, The integer type that can be accessed as an atomic entity from an asynchronous signal handler
typedef struct {} sig_atomic_t;
// TODO: Define sig_atomic_t

// TODO: Documentation, Sets a signal handler for particular signal
extern void (*signal(int sig, void (*handler)(int)))(int);

// TODO: Documentation, Runs the signal handler for particular signal
extern int raise(int sig);

#endif

/** @} */
