/**
 * @file wctype.h
 * @defgroup libc_wctype <wctype.h>
 * @brief Functions to determine the type contained in wide character data
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/string/wide)
 * for **Null-terminated wide strings**
 * 
 * @{
*/

#ifndef _LIBC_WCTYPE_H
#define _LIBC_WCTYPE_H 1


/****************************************************/
/* Null-Terminated Wide String Functions            */
/* Ref: https://en.cppreference.com/w/c/string/wide */
/****************************************************/

// NOTE: Can be defined in <wchar.h> or <wctype.h>
#ifndef WEOF
/** @brief A non-character value of type @ref wint_t used to indicate errors */
#define WEOF (0xffffffffu)
#endif

// NOTE: Can be defined in <wchar.h> or <wctype.h>
#ifndef _WINT_T
#define _WINT_T 1
/** @brief Integer type that can hold any valid wide character and at least one more value */
typedef unsigned int wint_t;
#endif

/** @brief Scalar type that holds locale-specific character mapping */
typedef const signed int* wctrans_t;

/** @brief Scalar type that holds locale-specific character classification */
typedef unsigned long wctype_t;

// TODO: Documentation, Checks if a wide character is alphanumeric
extern int iswalnum(wint_t ch);

// TODO: Documentation, Checks if a wide character is alphabetic
extern int iswalpha(wint_t ch);

// TODO: Documentation, Checks if a wide character is a lowercase character
extern int iswlower(wint_t ch);

// TODO: Documentation, Checks if a wide character is an uppercase character
extern int iswupper(wint_t ch);

// TODO: Documentation, Checks if a wide character is a digit
extern int iswdigit(wint_t ch);

// TODO: Documentation, Checks if a wide character is a hexadecimal character
extern int iswxdigit(wint_t ch);

// TODO: Documentation, Checks if a wide character is a control character
extern int iswcntrl(wint_t ch);

// TODO: Documentation, Checks if a wide character is a graphical character
extern int iswgraph(wint_t ch);

// TODO: Documentation, Checks if a wide character is a space character
extern int iswspace(wint_t ch);

// TODO: Documentation, Checks if a wide character is a blank character
extern int iswblank(wint_t ch);

// TODO: Documentation, Checks if a wide character is a printing character
extern int iswprint(wint_t ch);

// TODO: Documentation, Checks if a wide character is a punctuation character
extern int iswpunct(wint_t ch);

// TODO: Documentation, Classifies a wide character according to the specified @ref LC_CTYPE category
extern int iswctype(wint_t wc, wctype_t desc);

// TODO: Documentation, Looks up a character classification category in the current C locale
extern wctype_t wctype(const char* str);

// TODO: Documentation, Converts a wide character to lowercase
extern wint_t towlower(wint_t wc);

// TODO: Documentation, Converts a wide character to uppercase
extern wint_t towupper(wint_t wc);

// TODO: Documentation, Performs a character mapping according to the specified @ref LC_CTYPE mapping category
extern wint_t towctrans(wint_t wc, wctrans_t desc);

// TODO: Documentation, Looks up a character mapping category in the current C locale
wctrans_t wctrans(const char* str);

#endif

/** @} */
