/// @file alloc.c

#include <stdlib.h> // Implements
#include <kernel/system.h> // DEBUG: kprintf

void* malloc(size_t size) {
	// TODO: Implement
	kprintf("TODO: malloc(size=%d)\n", size);
	return NULL;
}

void* calloc(size_t num, size_t size) {
	// TODO: Implement
	kprintf("TODO: calloc(num=%d, size=%d)\n", num, size);
	return NULL;
}

void* realloc(void* ptr, size_t new_size) {
	// TODO: Implement
	kprintf("TODO: realloc(ptr=0x%p, new_size=%d)\n", ptr, new_size);
	return NULL;
}

void free(void* ptr) {
	// TODO: Implement
	kprintf("TODO: free(ptr=0x%p)\n", ptr);
}

void free_sized(void* ptr, size_t size) {
	// TODO: Implement
	kprintf("TODO: free_sized(ptr=0x%p, size=%d)\n", ptr, size);
}

void free_aligned_sized(void* ptr, size_t alignment, size_t size) {
	// TODO: Implement
	kprintf("TODO: free_aligned_sized(ptr=0x%p, alignment=%d, size=%d)\n", ptr, alignment, size);
}

void* aligned_alloc(size_t alignment, size_t size) {
	// TODO: Implement
	kprintf("TODO: aligned_alloc(alignment=0x%p, size=%d)\n", alignment, size);
	return NULL;
}
