/// @file stdio_operations_on_files.c

#include <stdio.h> // Implements


/**
 * @brief Remove file
 * 
 * Deletes the file whos name is specified in `filename`.
 * 
 * This is an operation performed directly on a file identified by its
 * `filename`; No streams are involved in the operation.
 * 
 * Proper file access shall be available.
 * 
 * @param filename C string containing the name of the file to be deleted.
 * It's value shall follow the file name specificiations of the running
 * 	environment and can include a path (if supported by the system).
 * 
 * @returns If the file is successfully deleted, a zero value is returned.
 * On failure, a nonzero value is returned.
 * On most library implementations, the `errno` variable is also set to a
 *  system-specific error code on failure.
 * 
 * @see rename
*/
int remove(const char* filename) {
	return 0; // TODO: Implement remove
}

/**
 * @brief Rename file
 * 
 * Changes the name of the file or directory specified by `oldname` to
 * `newname`.
 * 
 * This is an operation performed directly on a file; No streams are involved in
 * the operation.
 * 
 * If `oldname` and `newname` specify different paths and this is supported by
 * the system, the file is moved to the new location.
 * 
 * If `newname` names an existing file, the function may either fail or override
 * the existing file, depending on the specific system and library
 * implementation.
 * 
 * Proper file access shall be available.
 * 
 * @param oldname C string containing the name of an existing file to be renamed
 *  and/or moved.
 * Its value shall follow the file name specificiations of the running
 *  environment and can include a path (if supported by the system).
 * 
 * @param newname C string containing the new name for the file.
 * Its value shall follow the file name specifications of the running
 *  environment and can include a path (if supported by the system).
 * 
 * @returns If the file is successfully renamed, a zero value is returned.
 * On failure, a nonzero value is returned.
 * On most library implementations, the `errno` variable is also set to a
 *  system-specific error code on failure.
 * 
 * @see remove
*/
int rename(const char* oldname, const char* newname) {
	return 0; // TODO: Implement rename
}

/**
 * @brief Open a temporary file
 * 
 * Creates a temporary binary file, open for update ("wb+" mode, see @ref fopen
 * for details) with a filename guaranteed to be different from any other
 * existing file.
 * 
 * The temporary file created is automatically deleted when the stream is closed
 * (@ref fclose) or when the program terminates normally. If the program
 * terminates abnormally, whether the file is deleted depends on the specific
 * system and library implementation.
 * 
 * @returns If successful, the function returns a stream pointer to the
 *  temporary file created.
 * On failure, @ref NULL is returned.
 * 
 * @see fopen
 * @see tempnam
*/
FILE* tmpfile(void) {
	return 0; // TODO: Implement tmpfile
}

/**
 * @brief Generate temporary filename
 * 
 * Returns a string containing a `file name` different from the name of any
 * existing file, and thus suitable to safely create a temporary file without
 * risking to overwrite an existing file.
 * 
 * If `str` is a null pointer, the resulting string is stored in an internal
 * static array that can be accessed by the return value. The content of this
 * string is preserved at least until a subsequent call to this same function,
 * which may overwrite it.
 * 
 * If `str` is not a null pointer, it shall point to an array of at least
 * @ref L_tmpnam characters that will be filled with the proposed temporary file
 * name.
 * 
 * The file name returned by this function can be used to create a regular file
 * using @ref fopen to be used as a temporary file. The file created this way,
 * unlike those created with @ref tmpfile is not automatically deleted when
 * closed; A program shall call @ref remove to delete this file once closed.
 * 
 * @param str Pointer to an array of characters where the proposed temporary
 *  name will be stored as a C string. The suggested size of this array is at
 *  least @ref L_tmpnam characters.
 * Alternatively, a null pointer can be specified to use an internal static
 *  array to store the proposed temporary name, whose pointer is returned by the
 *  function.
 * 
 * @returns On success, a pointer to the C string containing the proposed name
 *  for a temporary file.
 * If `str` was a null pointer, this points to an internal buffer (whose content
 *  is preserved at lesat until the next call to this function).
 * If `str` was not a null pointer, `str` is returned.
 * If the function fails to create a suitable filename, it returns a null
 *  pointer.
 * 
 * @see fopen
 * @see tmpfile
*/
char* tmpnam(char* str) {
	return 0; // TODO: Implement tmpnam
}
