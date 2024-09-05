/**
 * @file threads.h
 * @defgroup libc_threads <threads.h>
 * @brief Thread library
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [_Atomic type specifier and qualifier](https://en.cppreference.com/w/c/language/atomic)
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/thread)
 * for **Concurrency support library**
 * 
 * @{
*/

#ifndef _LIBC_THREADS_H
#define _LIBC_THREADS_H 1

#include <time.h> // struct timespec


/***********************************************/
/* Concurrency Support Functions               */
/* Ref: https://en.cppreference.com/w/c/thread */
/***********************************************/

/** @brief Implementation-defined complete object type identifying a thread */
typedef unsigned long thrd_t;

/** @brief Mutex identifier */
typedef struct {} mtx_t; // TODO: Define mtx_t

/** @brief Condition variable identifier */
typedef struct {} cnd_t; // TODO: Define cnd_t

/** @brief Thread-specific storage pointer */
typedef unsigned int tss_t;

/** @brief A typedef of the function pointer type int(*)(void*), used by @ref thrd_create */
typedef int (*thrd_start_t)(void*);

/** @brief Function pointer type `void(*)(void*)`, used for TSS destructor */
typedef void (*tss_dtor_t)(void*);

// TODO: Documentation, Creates a thread
extern int thrd_create(thrd_t* thr, thrd_start_t func, void* arg);

// TODO: Documentation, Checks if two identifiers refer to the same thread
extern int thrd_equal(thrd_t lhs, thrd_t rhs);

// TODO: Documentation, Obtains the current thread identifier
extern thrd_t thrd_current(void);

// TODO: Documentation, Suspends execution of the calling thread for the given period of time
extern int thrd_sleep(const struct timespec* duration, struct timespec* remaining);

// TODO: Documentation, Yields the current time slice
extern void thrd_yield(void);

// TODO: Documentation, Terminates the calling thread
extern void thrd_exit(int res) __attribute__((__noreturn__));

// TODO: Documentation, Detaches a thread
extern int thrd_detach(thrd_t thr);

// TODO: Documentation, Blocks until a thread terminates
extern int thrd_join(thrd_t thr, int* res);

/** @brief Indicates a thread error status
 * 
 * Identifiers for thread states and errors.
*/
enum {
	thrd_success  = 0, ///< Indicates successful return value
	thrd_busy     = 1, ///< Indicates unsuccessful return value due to resource temporary unavailable
	thrd_error    = 2, ///< Indicates unsuccessful return value
	thrd_nomem    = 3, ///< Indicates unsuccessful return value due to out of memory condition
	thrd_timedout = 4  ///< Indicates timed out return value
};

// TODO: Documentation, Creates a mutex
extern int mtx_init(mtx_t* mutex, int type);

// TODO: Documentation, Blocks until locks a mutex
extern int mtx_lock(mtx_t* mutex);

// TODO: Documentation, Blocks until locks a mutex or times out
extern int mtx_timedlock(mtx_t* restrict mutex, const struct timespec* restrict time_point);

// TODO: Documentation, Locks a mutex or returns without blocking if already locked
extern int mtx_trylock(mtx_t* mutex);

// TODO: Documentation, Unlocks a mutex
extern int mtx_unlock(mtx_t* mutex);

// TODO: Documentation, Destroys a mutex
extern void mtx_destroy(mtx_t* mutex);

/** @brief Defines the type of a mutex
 * 
 * When passed to @ref mtx_init, identifies the type of a mutex to create.
 * 
 * @see @ref mtx_init @copybrief mtx_init
*/
enum {
	mtx_plain     = 0, ///< Plain mutex
	mtx_recursive = 1, ///< Recursive mutex
	mtx_timed     = 2  ///< Timed mutex
};

// TODO: Documentation, Calls a function exactly once
typedef struct {} once_flag; // TODO: Define once_flag
extern void call_once(once_flag* flag, void (*func)(void));
#define ONCE_FLAG_INIT /* unspecified */
// TODO: Define ONCE_FLAG_INIT, and docs

// TODO: Documentation, Creates a condition variable
extern int cnd_init(cnd_t* cond);

// TODO: Documentation, Unblocks one thread blocked on a condition variable
extern int cnd_signal(cnd_t* cond);

// TODO: Documentation, Unblocks all threads blocked on a condition variable
extern int cnd_broadcast(cnd_t* cond);

// TODO: Documentation, Blocks on a condition variable
extern int cnd_wait(cnd_t* cond, mtx_t* mutex);

// TODO: Documentation, Blocks on a condition variable, with a timeout
extern int cnd_timedwait(cnd_t* restrict cond, mtx_t* restrict mutex, const struct timespec* restrict time_point);

// TODO: Documentation, Destroys a condition variable
extern void cnd_destroy(cnd_t* cond);

/** @brief Maximum number of times destructors are called
 * 
 * Expands to a positive integral
 * [constant expression](https://en.cppreference.com/w/c/language/constant_expression)
 * defining the maximum number of times a destructor for thread-local storage
 * pointer will be called by @ref thrd_exit.
 * 
 * This constant is equivalent to the POSIX PTHREAD_DESTRUCTOR_ITERATIONS.
*/
#define TSS_DTOR_ITERATIONS 4

// TODO: Documentation, Creates thread-specific storage pointer with a given destructor
extern int tss_create(tss_t* tss_key, tss_dtor_t destructor);

// TODO: Documentation, Reads from thread-specific storage
extern void* tss_get(tss_t tss_key);

// TODO: Documentation, Write to thread-specific storage
extern int tss_set(tss_t tss_id, void* val);

// TODO: Documentation, Releases the resources held by a given thread-specific pointer
extern void tss_delete(tss_t tss_id);

#endif

/** @} */
