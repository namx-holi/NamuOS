/**
 * @file errno.h
 * @defgroup libc_errno <errno.h>
 * @brief TODO
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see @ref math_errhandling @copybrief math_errhandling
 * @see @ref MATH_ERRNO @copybrief MATH_ERRNO
 * @see @ref MATH_ERREXCEPT @copybrief MATH_ERREXCEPT
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/error)
 * for **Error handling**
 * 
 * @todo Implementation for Bounds checking?
 * 
 * @{
*/

// Ref: https://en.cppreference.com/w/c/error

#ifndef _LIBC_ERRNO_H
#define _LIBC_ERRNO_H 1

/** @brief Macro which expands to POSIX-compatible thread-local error number variable
 * 
 * `errno` is a preprocessor macro (but see note below) that expands to a
 * thread-local modifiable lvalue of type `int`. Several standard library
 * functions indicate errors by writing positive integers to `errno`. Typically,
 * the value of `errno` is set to one of the error codes listed in
 * @ref libc_errno as macro constants beginning with the letter E followed by
 * uppercase letters or digits.
 * 
 * The value of `errno` is `0` at program startup, and although library
 * functions are allowed to write positive integers to `errno` whether or not an
 * error occured, library functions never store `0` in `errno`.
 * 
 * Library functions @ref perror and @ref strerror can be used to obtain
 * textual descriptions of the error conditions that correspond to the current
 * `errno` value.
 * 
 * @note Until C11, the C standards had contradictory requirements, in that they
 * said that `errno` is a macro but *also* that "it is unspecified whether
 * `errno` is a macro or an identifier declared with external linkage". C11
 * fixes this, requiring that it be defined as a macro (see also WG14
 * [N1338](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1338.htm)).
 * 
 * @todo Needs an implementation
 * 
 * @see @ref libc_errno_codes @copybrief libc_errno_codes
 * @see @ref perror @copybrief perror
 * @see @ref strerror @copybrief strerror
 * @see @ref strerror_s @copybrief strerror_s
 * @see @ref strerrorlen_s @copybrief strerrorlen_s
 * @see @ref math_errhandling @copybrief math_errhandling
 * @see @ref MATH_ERRNO @copybrief MATH_ERRNO
 * @see @ref MATH_ERREXCEPT @copybrief MATH_ERREXCEPT
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/error/errno)
 * for `errno`
*/
#define errno /* implementation defined */
// TODO: Implement errno

/**
 * @defgroup libc_errno_codes E2BIG, EACCES, ..., EXDEV
 * @brief Macros for standard POSIX-compatible error conditions
 * @ingroup libc_errno
 * 
 * Each of the macros defined in @ref libc_errno expands to an integer constant
 * expression with type `int` and with a unique positive value. The following
 * constants are defined by ISO C. The implementation may define more, as long
 * as they begin with <tt>'E'</tt> followed by digits or uppercase letters.
 * 
 * @note Many additional errno constants are
 * [defined by POSIX](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html)
 * and by the
 * [C++ standard library](https://en.cppreference.com/w/cpp/error/errno_macros),
 * and individual implementations may define even more, e.g. `errno(3)` on
 * Linux or `intro(2)` on BSD and OS X.
 * 
 * @see @ref errno @copybrief errno
 * @see @ref perror @copybrief perror
 * @see @ref strerror @copybrief strerror
 * @see @ref strerror_s @copybrief strerror_s
 * @see @ref strerrorlen_s @copybrief strerrorlen_s
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/error/errno_macros)
 * for **Error numbers**
 * 
 * @{
*/
#define E2BIG 1 ///< Argument list too long
#define EACCES 2 ///< Permission denied
#define EADDRINUSE 3 ///< Address in use
#define EADDRNOTAVAIL 4 ///< Address not available
#define EAFNOSUPPORT 5 ///< Address family not supported
#define EAGAIN 6 ///< Resource unavailable, try again
#define EALREADY 7 ///< Connection already in progress
#define EBADF 8 ///< Bad file descriptor
#define EBADMSG 9 ///< Bad message
#define EBUSY 10 ///< Device or resource busy
#define ECANCELED 11 ///< Operation canceled
#define ECHILD 12 ///< No child processes
#define ECONNABORTED 13 ///< Connection aborted
#define ECONNREFUSED 14 ///< Connection refused
#define ECONNRESET 15 ///< Connection reset
#define EDEADLK 16 ///< Resource deadlock would occur
#define EDESTADDRREQ 17 ///< Desgination address required
#define EDOM 18 ///< Mathematics argument out of domain of function
#define EDQUOT 19 ///< Reserved
#define EEXIST 20 ///< File exists
#define EFAULT 21 ///< Bad address
#define EFBIG 22 ///< File too large
#define EHOSTUNREACH 23 ///< Host is unreachable
#define EIDRM 24 ///< Identifier removed
#define EILSEQ 25 ///< Illegal byte sequence
#define EINPROGRESS 26 ///< Operation in progress
#define EINTR 27 ///< Interrupted function
#define EINVAL 28 ///< Invalid argument
#define EIO 29 ///< I/O error
#define EISCONN 30 ///< Socket is connected
#define EISDIR 31 ///< Is a directory
#define ELOOP 32 ///< Too many levels of symbolic links
#define EMFILE 33 ///< File descriptor value too large
#define EMLINK 34 ///< Too many links
#define EMSGSIZE 35 ///< Message too large
#define EMULTIHOP 36 ///< Reserved
#define ENAMETOOLONG 37 ///< Filename too long
#define ENETDOWN 38 ///< Network is down
#define ENETRESET 39 ///< Connection aborted by network
#define ENETUNREACH 40 ///< Network unreachable
#define ENFILE 41 ///< Too many files open in system
#define ENOBUFS 42 ///< No buffer space available
#define ENODATA 43 ///< No message is available on the STREAM head read queue
#define ENODEV 44 ///< No such device
#define ENOENT 45 ///< No such file or directory
#define ENOEXEC 46 ///< Executable file format error
#define ENOLCK 47 ///< No locks available
#define ENOLINK 48 ///< Link has been severed
#define ENOMEM 49 ///< Not enough space
#define ENOMSG 50 ///< No message of the desired type
#define ENOPROTOOPT 51 ///< Protocol not available
#define ENOSPC 52 ///< No space left on device
#define ENOSR 53 ///< No STREAM resources
#define ENOSTR 54 ///< Not a STREAM
#define ENOSYS 55 ///< Functionality not supported
#define ENOTCONN 56 ///< The socket is not connected
#define ENOTDIR 57 ///< Not a directory or a symbolic link to a directory
#define ENOTEMPTY 58 ///< Directory not empty
#define ENOTRECOVERABLE 59 ///< State not recoverable
#define ENOTSOCK 60 ///< Not a socket
#define ENOTSUP 61 ///< Not supported
#define ENOTTY 62 ///< Inappropriate I/O control operation
#define ENXIO 63 ///< No such device or address
#define EOPNOTSUPP 64 ///< Operation not supported on socket
#define EOVERFLOW 65 ///< Value too large to be stored in data type
#define EOWNERDEAD 66 ///< Previous owner died
#define EPERM 67 ///< Operation not permitted
#define EPIPE 68 ///< Broken pipe
#define EPROTO 69 ///< Protocol error
#define EPROTONOSUPPORT 70 ///< Protocol not supported
#define EPROTOTYPE 71 ///< Protocol wrong type for socket
#define ERANGE 72 ///< Result too large
#define EROFS 73 ///< Read-only file system
#define ESPIPE 74 ///< Invalid seek
#define ESRCH 75 ///< No such process
#define ESTALE 76 ///< Reserved?
#define ETIME 77 ///< Stream @ref ioctl timeout
#define ETIMEDOUT 78 ///< Connection timed out
#define ETXTBSY 79 ///< Text file busy
#define EWOULDBLOCK 80 ///< Operation would block
#define EXDEV 81 ///< Cross-device link
/** @} */

#endif 

/** @} */
