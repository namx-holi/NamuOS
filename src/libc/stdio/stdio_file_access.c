/// @file stdio_file_access.c

#include <stdio.h> // Implements


/** @brief Close file
 * 
 * Closes the file associated with the ***stream*** and disassociates it.
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
 * <br>
 * On failure, @ref EOF is returned.
 * 
 * @see fopen
 * @see fflush
*/
int fclose(FILE* stream) {
	return 0; // TODO: Implement fclose
}

/** @brief Flush stream
 * 
 * If the given ***stream*** was open for writing (or if it was open for updating
 * and the last i/o operation was an output operation) any unwritten data in its
 * output buffer is written to the file.
 * 
 * If ***stream*** is a null pointer, all such streams are flushed.
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
 * <br>
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

/** @brief Open file
 * 
 * Opens the file whose name is specified in the parameter ***filename*** and
 * associates it with a stream that can be identified in future operations by
 * the @ref FILE pointer returned.
 * 
 * The operations that are allowed on the stream and how these are performed are
 * defined by the ***mode*** parameter.
 * 
 * The returned stream is ***fully buffered*** by default if it is known to not
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
 *     <br><br>
 * The new C standard (C2011) adds a new standard subspecifier ("x") that can be
 * appended to any "w" specifier (to form "wx", "wbx", "w+x" or "w+bx"/"wb+x").
 * This subspecifier forces the function to fail if the file exists, instead of
 * overwriting it.
 *     <br><br>
 * If additional characters follow the sequence, the behaviour depends on the
 * library implementations: some implementations may ignore additional
 * characters so that for example an additional "t" (sometimes used to
 * explicitly state a *text file*) is accepted.
 *     <br><br>
 * On some library implementations, opening or creating a text file with update
 * mode may treat the stream instead as a binary file.
 * 
 * @note ***Text files*** are files containing sequences of lines of text.
 * Depending on the environment where the application runs, some special
 * character conversion may occur in input/output operations in ***text mode***
 * to adapt them to a system-specific text file format. Although on some
 * environments no conversions occur and both ***text files*** and
 * ***binary files*** are treated the same way, using the appropriate mode
 * improves portability.
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
 * <br>
 * Otherwise, a null pointer is returned.
 * <br>
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

/** @brief Reopen stream with different file or mode
 * 
 * Resuses ***stream*** to either open the file specified by ***filename*** or
 * to change its access ***mode***.
 * 
 * If a new ***filename*** is specified, the function first attempts to close
 * any file already associated with ***stream*** (third parameter) and
 * disassociates it. Then, independently of whether that stream was successfully
 * closed or not, `freopen` opens the file specified by ***filename*** and
 * associates it with the ***stream*** just as @ref fopen would do using the
 * specified ***mode***.
 * 
 * If ***filename*** is a null pointer, the function attempts to change the
 * ***mode*** of the stream. Although a particular library implementation is
 * allowed to restrict the changes permitted, and under which circumstances.
 * 
 * The ***error indicator*** and ***eof indicator*** are automatically cleared
 * (as if @ref clearerr was called).
 * 
 * This function is especially useful for redirecting predefined streams like
 * `stdin`, `stdout`, and `stderr` to specific files.
 * 
 * @param filename C string containing the name of the file to be opened.
 * Its value shall follow the file name specifications of the running
 *  environment and can include a path (if supported by the system).
 * If this parameter is a null pointe,r the function attempts to change the mode
 *  of the ***stream***, as if the file name currently associated with that
 *  stream had been used.
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
 *     <br><br>
 * The new C standard (C2011) adds a new standard subspecifier ("x") that can be
 * appended to any "w" specifier (to form "wx", "wbx", "w+x" or "w+bx"/"wb+x").
 * This subspecifier forces the function to fail if the file exists, instead of
 * overwriting it.
 *     <br><br>
 * If additional characters follow the sequence, the behaviour depends on the
 * library implementations: some implementations may ignore additional
 * characters so that for example an additional "t" (sometimes used to
 * explicitly state a *text file*) is accepted.
 *     <br><br>
 * On some library implementations, opening or creating a text file with update
 * mode may treat the stream instead as a binary file.
 * 
 * @param stream pointer to a @ref FILE object that identifies the stream to be
 * reopened.
 * 
 * @returns If the file is successfully reopened, the function returns the
 * pointer passed as parameter ***stream***, which can be used to identify the
 * reopened stream.
 * <br>
 * Otherwise, a null pointer is returned.
 * <br>
 * On most library implementations the @ref errno variable is also set to a
 * system-specific error code on failure.
 * 
 * @see fopen
 * @see fclose
*/
FILE* freopen(const char* __restrict filename, const char* __restrict mode, FILE* __restrict stream) {
	return 0; // TODO: Implement freopen
}

/** @brief Set stream buffer
 * 
 * Specifies the ***buffer*** to be used by the ***stream*** for I/O operations,
 * which becomes a ***fully buffered*** stream. Or, alternatively, if
 * ***buffer*** is a null pointer, buffering is disabled for the ***stream***,
 * which becomes an ***unbuffered*** stream.
 * 
 * This function should be called once the ***stream*** has been associated with
 * an open file, but before any input or output operation is performed with it.
 * 
 * The buffer is assumed to be at least @ref BUFSIZ bytes in size
 * (see @ref setvbuf to specify a size of the buffer).
 * 
 * A *stream buffer* is a block of data that acts as intermediary between the
 * i/o operations and the physical file associated to the stream: For output
 * buffers, data is output to the buffer until its maximum capacity is reached,
 * then it is flushed (@ref fflush) (i.e.: all data is sent to the physical file
 * at once and the buffer cleared). Likewise, input buffers are filled from the
 * physical file, from which data is sent to the operations until exhausted, at
 * which point new data is acquired from the file to fill the buffer again.
 * 
 * Stream buffers can be explicity flushed by calling @ref fflush. They are also
 * automaically flushed by @ref fclose and @ref freopen, or when the program
 * terminates normally.
 * 
 * A ***full buffered stream*** uses the entire size of the buffer as buffer
 * whenever enough data is available (see @ref setvbuf for other buffer modes).
 * 
 * All files are opened with a default allocated buffer (***fully buffered***)
 * if they are known to not refer to an interactive device. This function can be
 * used to either set a specific memory block to be used as buffer or to disable
 * buffering for the stream.
 * 
 * The default streams `stdin` and `stdout` are ***fully buffered*** by default
 * if they are known to not refer to an interactive device. Otherwise, they may
 * either be ***line buffered*** or ***unbuffered*** by default, depending on
 * the system and library implementation. The same is true for `stderr`, which
 * is always either ***line buffered*** or ***unbuffered*** by default.
 * 
 * A call to this function is equivalent to calling @ref setvbuf with `_IOFBF`
 * as ***mode*** and @ref BUFSIZE as ***size*** (when ***buffer*** is not a null
 * pointer), or equivalent to calling it with `_IONBF` as ***mode*** (when it is
 * a null pointer).
 * 
 * @param stream Pointer to a @ref FILE object that identifies an open stream.
 * 
 * @param buffer User allocated buffer. Shall be at least @ref BUFSIZ bytes
 * long.
 * <br>
 * Alternatively, a null pointer can be specified to disable buffering.
 * 
 * @see setvbuf
 * @see fopen
 * @see fflush
*/
void setbuf(FILE* __restrict stream, char* __restrict buffer) {
	return; // TODO: Implement setbuf
}

/** @brief Change stream buffering
 * 
 * Specifies a ***buffer*** for ***stream***. This function allows to specify
 * the ***mode*** and ***size*** of the buffer (in bytes).
 * 
 * If ***buffer*** is a null pointer, the function automatically allocates a
 * buffer (using ***size*** as a hint on the size to use). Otherwise, the array
 * pointed by ***buffer*** may be used as a buffer of ***size*** bytes.
 * 
 * This function should be called once the ***stream*** has been associated with
 * an open file, but before any input or output operation is performed with it.
 * 
 * A *stream buffer* is a block of data that acts as intermediary between the
 * i/o operations and the physical file associated to the stream: For output
 * buffers, data is output to the buffer until its maximum capacity is reached,
 * then it is flushed (@ref fflush) (i.e.: all data is sent to the physical file
 * at once and the buffer cleared). Likewise, input buffers are filled from the
 * physical file, from which data is sent to the operations until exhausted, at
 * which point new data is acquired from the file to fill the buffer again.
 * 
 * Stream buffers can be explicity flushed by calling @ref fflush. They are also
 * automaically flushed by @ref fclose and @ref freopen, or when the program
 * terminates normally.
 * 
 * All files are opened with a default allocated buffer (***fully buffered***)
 * if they are known to not refer to an interactive device. This function can be
 * used to either redefine the buffer ***size*** or ***mode***, to define a
 * user-allocated buffer or to disable buffering for the stream.
 * 
 * The default streams `stdin` and `stdout` are ***fully buffered*** by default
 * if they are known to not refer to an interactive device. Otherwise, they may
 * either be ***line buffered*** or ***unbuffered*** by default, depending on
 * the system and library implementation. The same is true for `stderr`, which
 * is always either ***line buffered*** or ***unbuffered*** by default.
 * 
 * @param stream Pointer to a @ref FILE object that identifies an open stream.
 * 
 * @param buffer User allocated buffer. Shall be at least ***size*** bytes long.
 * <br>
 * If set to a null pointer, the function automatically allocates a buffer.
 * 
 * @param mode Specifies a mode for file buffering. Three special macro
 * constants (`_IOFBF`, `_IOLBF` and `_IONBF`) are defined to be used as the
 * value for this parameter:
 * <table>
 * <tr>
 *  <td>`_IOFBF`</td>
 *  <td>
 *   <b>Full buffering</b>: On output, data is written once the buffer is full
 *   (or flushed). On input, the buffer is filled when an input operation is
 *   requested and the buffer is empty.
 *  </td>
 * </tr>
 * <tr>
 *  <td>`_IOLBF`</td>
 *  <td>
 *   <b>Line buffering</b>: On output, data is written when a newline character
 *   is inserted into the stream or when the buffer is full (or flushed),
 *   whatever happens first. On input, the buffer is filled up to the next
 *   newline character when an input operation is requested and the buffer is
 *   empty
 *  </td>
 * </tr>
 * <tr>
 *  <td>`_IONBF`</td>
 *  <td>
 *   <b>No buffering</b>: No buffer is used. Each I/O operation is written as
 *   soon as possible. In this case, the ***buffer*** and ***size*** parameters
 *   are ignored.
 *  </td>
 * </tr>
 * </table>
 * 
 * @param size Buffer size, in bytes.
 * <br>
 * If the ***buffer*** argument is a null pointer, this value may determine the
 * size automatically allocated by the function for the buffer.
 * 
 * @returns If the buffer is correctly assigned to the file, a zero value is
 * returned.
 * <br>
 * Otherwise, a non-zero value is returned; This may be due to an invalid
 * ***mode*** parameter or to some other error allocating or assigning the
 * buffer.
 * 
 * @see setbuf
 * @see fopen
 * @see fflush
*/
int setvbuf(FILE* __restrict stream, char* __restrict buffer, int mode, size_t size) {
	return 0; // TODO: Implement setvbuf
}
