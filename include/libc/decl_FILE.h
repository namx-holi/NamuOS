/// @file decl_FILE.h

// Just the FILE struct, to avoid including stdio.h in libc

#ifndef _LIBC_DECL_FILE_H
#define _LIBC_DECL_FILE_H 1


/** @brief Object type, capable of holding all information needed to control a C I/O stream
 * 
 * Each `FILE` object denotes a C stream.
 * 
 * C standard does not specify whether `FILE` is a complete object type. While
 * it may be possible to copy a valid `FILE`, using a pointer to such a copy as
 * an argument for an I/O function invokes unspecified behaviour. In other
 * words, `FILE` may be semantically non-copyable.
 * 
 * I/O streams can be used for both unformatted and formatted input and output.
 * Furthermore, the functions that handle input and output can also be
 * locale-sensitive, such that wide/multibyte conversions are performed as
 * necessary.
 * 
 * @todo Documentation for stream state
 * @todo Documentation for narrow and wide orientation
 * @todo Documentation for binary and text modes
 * @todo Documentation for notes
 * @todo FILE type implementation
 * 
 * @see @ref stdin @copybrief stdin
 * @see @ref stdout @copybrief stdout
 * @see @ref stderr @copybrief stderr
*/
typedef struct {} FILE; // TODO: Implement

#endif
