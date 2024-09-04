/**
 * @file wchar.h
 * @defgroup libc_wchar <wchar.h>
 * @brief TODO
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/io)
 * for **File input/output**
 * 
 * @todo chrono ref
 * @todo multibyte ref
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/string/wide)
 * for **Null-terminated wide strings**
 * 
 * @{
*/

#ifndef _LIBC_WCHAR_H
#define _LIBC_WCHAR_H 1


/*******************************************/
/* File Input/Output Functions             */
/* Ref: https://en.cppreference.com/w/c/io */
/*******************************************/
// TODO: fwide
// TODO: fgetwc, getwc
// TODO: fgetws
// TODO: fputwc, putwc
// TODO: fputws
// TODO: getwchar
// TODO: putwchar
// TODO: ungetwc
// TODO: wscanf, fwscanf, swscanf, wscanf_s, fwscanf_s, swscanf_s
// TODO: vwscanf, vfwscanf, vswscanf, vwscanf_s, vfwscanf_s, vswscanf_s
// TODO: wprintf, fwprintf, swprintf, wprintf_s, fwprintf_s, swprintf_s, snwprintf_s
// TODO: vwprintf, vfwprintf, vswprintf, vfwprintf_s, vfwprintf_s, vswprintf_s, vsnwprintf_s


/***********************************************/
/* Date And Times Utility Functions            */
/* Ref: https://en.cppreference.com/w/c/chrono */
/***********************************************/
// TODO: wcsftime


/*********************************************************/
/* Null-Terminated Multibyte String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/multibyte */
/*********************************************************/
// TODO: mbsinit
// TODO: btowc
// TODO: wctob
// TODO: mbrlen
// TODO: mbrtowc
// TODO: wcrtomb, wcrtomb_s
// TODO: mbsrtowcs, mbsrtowcs_s
// TODO: mbstate_t


/****************************************************/
/* Null-Terminated Wide String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/wide */
/****************************************************/
// TODO: wcstol, wcstoll
// TODO: wcstoul, wcstoull
// TODO: wcstof, wcstod, wcstold
// TODO: wcscpy, wcscpy_s
// TODO: wcsncpy, wcsncpy_s
// TODO: wcscat, wcscat_s
// TODO: wcsncat, wcsncat_s
// TODO: wcsxfrm
// TODO: wcslen, wcsnlen_s
// TODO: wcscmp
// TODO: wcsncmp
// TODO: wcscoll
// TODO: wcschr
// TODO: wcsrchr
// TODO: wcsspn
// TODO: wcscspn
// TODO: wcspbrk
// TODO: wcstok, wcstok_s
// TODO: wmemcpy, wmemcpy_s
// TODO: wmemmove, wmemmove_s
// TODO: wmemcmp
// TODO: wmemchr
// TODO: wmemset
// TODO: wchar_t (in stdlib.h, wchar.h)
// TODO: wint_t (in wchar.h, wctype.h)
// TODO: WEOF (in wchar.h, wctype.h)
// TODO: WCHAR_MIN
// TODO: WCHAR_MAX

#endif

/** @} */
