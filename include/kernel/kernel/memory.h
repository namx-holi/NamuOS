/**
 * @file memory.h
 * @defgroup kernel_memory Kernel Memory
 * @brief TODO
 * @ingroup kernel
 * 
 * @todo Detailed description
 * 
 * @{
*/

#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <kernel/multiboot.h>
#include <stdbool.h> // bool


// TODO: Docstring
bool memory_initialise(multiboot_info_t* mb_info);

#endif

/** @} */
