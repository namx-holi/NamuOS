/// @file stdio.h

/** \addtogroup stdio stdio.h
 * @{
 */

#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif


// Ref: https://cplusplus.com/reference/cstdio/

// TODO: Include something for FILE type
// TODO: Include something for va_list type



/* Macros */
// TODO: Move defines to above __cplusplus?
#define BUFSIZ 8192 ///< Default buffer size
#define EOF (-1) ///< End-of-File 
#define _IOFBF 0 ///< For use in `setvbuf`, full buffering
#define _IOLBF 1 ///< For use in `setvbuf`, line buffering
#define _IONBF 2 ///< For use in `setvbuf`, no buffering
#define SEEK_SET 0 ///< For use in `fseek`, beginning of file
#define SEEK_CUR 1 ///< For use in `fseek`, current position of the file pointer
#define SEEK_END 2 ///< For use in `fseek`, end of file

// Needed for:
// - L_tmpnam, minimum length for temporary file name
// - TMP_MAX, number of temporary files
// - FOPEN_MAX, potential limit of simultaneous open streams
// - FILENAME_MAX, maximum length of file names
#include <bits/stdio_lim.h>

// Needed for:
// - NULL, null pointer
// - size_t, unsigned integral type
#include <stddef.h>

// Needed for:
// - va_list
#include <stdarg.h>


/* Types */
/// Object containing information to control a stream
typedef struct _IO_FILE {} FILE; // TODO: Implement FILE type 

/// Object containing information to specify a position within a file
typedef struct __fpos_t {
	long __pos; ///< Type of file sizes and offsets
	struct {
		int __count;
		union {
			unsigned int __wch;
			char __wchb[4];
		} __value; ///< Value so far
	} __state; ///< Conversion state information
} fpos_t;


/* Standard streams */
// TODO: Somehow define these. Has an issue with multiple definitions
// FILE* stdin;  ///< Standard input stream
// FILE* stdout; ///< Standard output stream
// FILE* stderr; ///< Standard error output stream
// // Quote: "C89/C99 say they're macros. Make them happy."
// #define stdin stdin
// #define stdout stdout
// #define stderr stderr


/** \addtogroup stdio_operations_on_files Operations on files
 * @brief Remove/rename files, and temporary files
 * @{
 */
int   remove(const char* filename); // Remove file
int   rename(const char* oldname, const char* newname); // Rename file
FILE* tmpfile(void); // Open a temporary file
char* tmpnam(char* str); // Generate temporary filename // TODO: char[L_tmpnam] instead of char*
/** @} */


/** \addtogroup stdio_file_access File access
 * @brief Open, flose, flush, and stream buffers
 * @{
 */
int   fclose(FILE* stream); // Close file
int   fflush(FILE* stream); // Flush stream
FILE* fopen(const char* __restrict filename, const char* __restrict mode); // Open file
FILE* freopen(const char* __restrict filename, const char* __restrict mode, FILE* __restrict stream); // Reopen stream with different file or mode
void  setbuf(FILE* __restrict stream, char* __restrict buffer); // Set stream buffer
int   setvbuf(FILE* __restrict stream, char* __restrict buffer, int mode, size_t size); // Change stream buffering
/** @} */


/** \addtogroup stdio_formatted_io Formatted input/output
 * @brief Printing and scanning
 * @{
 */
int fprintf(FILE* __restrict stream, const char* __restrict format, ...); // Write formatted data to stream
int fscanf(FILE* __restrict stream, const char* __restrict format, ...); // Read formatted data from stream
int printf(const char* __restrict format, ...); // Print formatted data to stdout
int scanf(const char* __restrict format, ...); // Read formatted data from stdin
int snprintf(char* __restrict s, size_t n, const char* __restrict format, ...); // Write formatted output to sized buffer
int sprintf(char* __restrict str, const char* __restrict format, ...); // Write formatted data to string
int sscanf(const char* __restrict s, const char* __restrict format, ...); // Read formatted data from string
int vfprintf(FILE* __restrict stream, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to stream
int vfscanf(FILE* __restrict stream, const char* __restrict format, va_list arg); // Read formatted data from stream into variable argument list
int vprintf(const char* __restrict format, va_list arg); // Print formatted data from variable argument list to stdout
int vscanf(const char* __restrict format, va_list arg); // Read formatted data into variable argument list
int vsnprintf(char* __restrict s, size_t n, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to sized buffer
int vsprintf(char* __restrict s, const char* __restrict format, va_list arg); // Write formatted data from variable argument list to string
int vsscanf(const char* __restrict s, const char* __restrict format, va_list arg); // Read formatted data from string into variable argument list
/** @} */


/** \addtogroup stdio_character_io Character input/output
 * @brief Get and put
 * @{
 */
int   fgetc(FILE* stream); // Get character from stream
char* fgets(char* __restrict str, int num, FILE* __restrict stream); // Get string from stream
int   fputc(int character, FILE* stream); // Write character to stream
int   fputs(const char* __restrict str, FILE* __restrict stream); // Write string to stream
int   getc(FILE* stream); // Get character from stream
int   getchar(void); // Get character from stdin
// char* gets(char* str); // NOTE: Officially removed from ISO C11
int   putc(int character, FILE* stream); // Write character to stream
int   putchar(int character); // Write character to stdout
int   puts(const char* str); // Write string to stdout
int   ungetc(int character, FILE* stream); // Unget character from stream
/** @} */


/** \addtogroup stdio_direct_io Direct input/output
 * @brief Read and write
 * @{
 */
size_t fread(void* __restrict ptr, size_t size, size_t count, FILE* __restrict stream); // Read block of data from stream
size_t fwrite(const void* __restrict ptr, size_t size, size_t count, FILE* __restrict stream); // Write block of data to stream
/** @} */


/** \addtogroup stdio_file_positioning File positioning
 * @brief Seek and tell
 * @{
 */
int      fgetpos(FILE* __restrict stream, fpos_t* __restrict pos); // Get current position in stream
int      fseek(FILE* stream, long int offset, int origin); // Reposition stream position indicator
int      fsetpos(FILE* stream, const fpos_t* pos); // Set position indicator of stream // TODO: restrict?
long int ftell(FILE* stream); // Get current position in stream
void     rewind(FILE* stream); // Set position of stream to the beginning
/** @} */


/** \addtogroup stdio_error_handling Error-handling
 * @brief Print, check, and clear errors
 * @{
 */
void clearerr(FILE* stream); // Clear error indicators
int  feof(FILE* stream); // Check end-of-file indicator
int  ferror(FILE* stream); // Check error indicator
void perror(const char* str); // Print error message
/** @} */

#ifdef __cplusplus
}
#endif

#endif

/** @} */
