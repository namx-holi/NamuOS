
#ifndef _KERNEL_ATOMIC_H
#define _KERNEL_ATOMIC_H 1

#include <stdint.h>

// See https://github.com/EmbolismSoil/Linux-2.4.0-/blob/master/include/asm-i386/atomic.h

// DEBUG: What is SMP?
#define CONFIG_SMP

#ifdef CONFIG_SMP
#define LOCK "lock ; "
#else
#define LOCK ""
#endif


typedef struct {volatile int counter;} atomic_t;

#define ATOMIC_INIT(i)  { (i) }

#define atomic_read(v)  ((v)->counter)
#define atomic_set(v,i) (((v)->counter) = (i))

static inline void atomic_add(int i, atomic_t* v) {
	// Add `i` to `v->counter`
	asm volatile (
		LOCK "addl %1, %0"
		:"=m" (v->counter)
		:"ir" (i), "m" (v->counter));
}

static inline void atomic_sub(int i, atomic_t* v) {
	// Subtract `i` from `v->counter`
	asm volatile(
		LOCK "subl %1, %0"
		:"=m" (v->counter)
		:"ir" (i), "m" (v->counter));
}

static inline int atomic_sub_and_test(int i, atomic_t* v) {
	// Subtract `i` from `v->counter`, and set `c` to 1 if zero flag set.
	unsigned char c;
	asm volatile(
		LOCK "subl %2, %0; sete %1"
		:"=m" (v->counter), "=qm" (c)
		:"ir" (i), "m" (v->counter) : "memory");
	return c;
}

static inline void atomic_inc(atomic_t* v) {
	// Increment `v->counter` by 1
	asm volatile(
		LOCK "incl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

static inline void atomic_dec(atomic_t* v) {
	// Decrement `v->counter` by 1
	asm volatile(
		LOCK "decl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

static inline int atomic_dec_and_test(atomic_t* v) {
	// Decrement `v->counter` by 1, and set `c` to if zero flag set.
	unsigned char c;
	asm volatile(
		LOCK "decl %0; sete %1"
		:"=m" (v->counter), "=qm" (c)
		:"m" (v->counter) : "memory");
	return c != 0; // If non-zero?
}

static inline int atomic_inc_and_test(atomic_t* v) {
	// Increment `v->counter` by 1, and set `c` to if zero flag set.
	unsigned char c;
	asm volatile(
		LOCK "incl %0; sete %1"
		:"=m" (v->counter), "=qm" (c)
		:"m" (v->counter) : "memory");
	return c != 0; // If non-zero?
}

static inline int atomic_add_negative(int i, atomic_t* v) {
	// TODO: Describe what this does!
	unsigned char c;
	asm volatile (
		LOCK "addl %2, %0; sets %1"
		:"=m" (v->counter), "=qm" (c)
		:"ir" (i), "m" (v->counter) : "memory");
	return c;
}

#define atomic_clear_mask(mask, addr) \
	asm volatile( \
		LOCK "andl %0, %1" \
		: : "r"(~(mask)), "m" (*addr) : "memory")

#define atomic_set_mask(mask, addr) \
	asm volatile( \
		LOCK "orl %0, %1" \
		: : "r" (mask), "m" (*addr) : "memory")

#endif
