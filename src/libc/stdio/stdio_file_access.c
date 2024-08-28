/// @file stdio_file_access.c

#include <stdio.h> // Implements


/**
 * @brief Close file
 * 
 * Closes the file associated with the `stream` and disassociates it.
 * 
 * All internal buffers associated with the stream are disassociated from it and
 * flushed: the content of any unwritten output buffer is written and the
 * content of any unread input buffer is discarded.
 * 
 * Even if the call fails, the stream passed as parameter will no longer be
 * associated with the file nor its buffers.
 * 
 * @param stream Pointer to a @ref FILE object that specifies the stream to be
 * closed.
 * 
 * @returns If the stream is successfully closed, a zero value is returned.
 * On failure, @ref EOF is returned.
 * 
 * @see fopen
 * @see fflush
*/
int fclose(FILE* stream) {
	return 0; // TODO: Implement fclose
}

/**
 * @brief Flush stream
 * 
 * If the given `stream` was open for writing (or if it was open for updating
 * and the last i/o operation was an output operation) any unwritten data in its
 * output buffer is written to the file.
 * 
 * If `stream` is a null pointer, all such streams are flushed.
 * 
 * In all other cases, the behaviour depends on the specific library
 * implementation. In some implementations, flushing a stream open for reading
 * causes its input buffer to be cleared (but this is not portable expected
 * behaviour).
 * 
 * The stream remains open after this call.
 * 
 * When a file is closed, either because of a call to @ref fclose or because the
 * program terminates, all the buffers associated with it are automatically
 * flushed.
 * 
 * @param stream Pointer to a @ref FILE object that specifies a buffered stream.
 * 
 * @returns A zero value indicates success.
 * If an error occurs, @ref EOF is returned and the error indicator is set (see
 * @ref ferror).
 * 
 * @see fclose
 * @see fopen
 * @see setbuf
 * @see setvbuf
*/
int fflush(FILE* stream) {
	return 0; // TODO: Implement fflush
}

/**
 * @brief Open file
 * 
 * Opens the file whose name is specified in the parameter `filename` and
 * associates it with a stream that can be identified in future operations by
 * the @ref FILE pointer returned.
 * 
 * The operations that are allowed on the stream and how these are performed are
 * defined by the `mode` parameter.
 * 
 * The returned stream is *fully buffered* by default if it is known to not
 * refer to an interactive device (see @ref setbuf).
 * 
 * The returned pointer can be disassociated from the file by calling
 * @ref fclose or @ref freopen. All opened files are automatically closed on
 * normal program termination.
 * 
 * The running environment supports at least @ref FOPEN_MAX files open
 * simultaneously.
 * 
 * @param filename C string containing the name of the file to be opened.
 * 
 * @param mode C string containing a file access mode. It can be:
 * <table>
 * <tr>
 *  <td> "r" </td>
 *  <td>
 *   <b>read</b>: Open file for input operations. The file must exist.
 *  </td>
 * </tr>
 * <tr>
 *  <td> "w" </td>
 *  <td>
 *   <b>write</b>: Create an empty file for output operations. if a file with
 *   the same name already exists, its contents are discarded and the file is
 *   treated as a new empty file.
 *  </td>
 * </tr>
 * <tr>
 *  <td> "a" </td>
 *  <td>
 *   <b>append</b>: Open file for output at the end of a file. Output operations
 *   always write data at the end of the file, expanding it. Repositioning
 *   operations (@ref fseek, @ref fsetpos, @ref rewind) are ignored. The file is
 *   created if it does not exist.
 *  </td>
 * </tr>
 * <tr>
 *  <td> "r+" </td>
 *  <td>
 *   <b>read/update</b>: Open a file for update (both for input and output). The
 *   file must exist.
 *  </td>
 * </tr>
 * <tr>
 *  <td> "w+" </td>
 *  <td>
 *   <b>write/update</b>: Create an empty file and open it for update (both for
 *   input and output). If a file with the same name already exists its contents
 *   are discarded and the file is treated as a new empty file.
 *  </td>
 * </tr>
 * <tr>
 *  <td> "a+" </td>
 *  <td>
 *   <b>append/update</b>: Open a file for update (both for input and output)
 *   with all output operations writing data at the end of the file.
 *   Repositioning operations (@ref fseek, @ref fsetpos, @ref rewind) affects
 *   the next input operations, but output operations move the position back to
 *   the end of the file. The file is created if it does not exist.
 *  </td>
 * </rd>
 * </table>
 * With the `mode` specifiers above the file is opened as a *text file*. In
 * order to open a file as a *binary file*, a "b" character has to be included
 * in the `mode` string. This additional "b" character can either be appended at
 * the end of the string (thus making the following compound modes: "rb", "wb",
 * "ab", "r+b", "w+b", "a+b") or be inserted between the letter and the "+" sign
 * for the mixed modes ("rb+", "wb+", "ab+").
 * 
 * The new C standard (C2011) adds a new standard subspecifier ("x") that can be
 * appended to any "w" specifier (to form "wx", "wbx", "w+x" or "w+bx"/"wb+x").
 * This subspecifier forces the function to fail if the file exists, instead of
 * overwriting it.
 * 
 * If additional characters follow the sequence, the behaviour depends on the
 * library implementations: some implementations may ignore additional
 * characters so that for example an additional "t" (sometimes used to
 * explicitly state a *text file*) is accepted.
 * 
 * On some library implementations, opening or creating a text file with update
 * mode may treat the stream instead as a binary file.
 * 
 * @note *Text files* are files containing sequences of lines of text. Depending
 * on the environment where the application runs, some special character
 * conversion may occur in input/output operations in *text mode* to adapt them
 * to a system-specific text file format. Although on some environments no
 * conversions occur and both *text files* and *binary files* are treated the
 * same way, using the appropriate mode improves portability.
 * 
 * @note For files open for update (those which include a "+" sign), on which
 * both input and output operations are allowed, the stream shall be flushed
 * (@ref fflush) or repositioned (@ref fseek, @ref fsetpos, @ref rewind) before
 * a reading operation that follows a writing operation. The stream shall be
 * repositioned (@ref fseek, @ref fsetpos, @ref rewind) before a writing
 * operation that follows a reading operation (whenever that operation did not
 * reach the end-of-file).
 * 
 * @returns If the file is successfully opened, the function returns a pointer
 * to a @ref FILE object that can be used to identify the stream on future
 * operations.
 * Otherwise, a null pointer is returned.
 * On most library implementations, the @ref errno variable is also set to a
 * system-specific error code.
 * 
 * @see fclose
 * @see freopen
 * @see setbuf
 * @see setvbuf
 * @see tmpfile
 * @see tmpnam
*/
FILE* fopen(const char* __restrict filename, const char* __restrict mode) {
	return 0; // TODO: Implement fopen
}

/**
 * 
*/
FILE* freopen(const char* __restrict filename, const char* __restrict mode, FILE* __restrict stream) {
	return 0; // TODO: Implement freopen
}

/**
 * 
*/
void setbuf(FILE* __restrict stream, char* __restrict buffer) {
	return; // TODO: Implement setbuf
}

/**
 * 
*/
int setvbuf(FILE* __restrict stream, char* __restrict buffer, int mode, size_t size) {
	return 0; // TODO: Implement setvbuf
}
