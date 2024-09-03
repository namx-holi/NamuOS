/**
 * @file locale.h
 * @defgroup libc_locale <locale.h>
 * @brief Localisation utilities
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/locale)
 * for **Localisation library**
 * 
 * @{
*/

#ifndef _LIBC_LOCALE_H
#define _LIBC_LOCALE_H

/** @brief Formatting details, returned by @ref localeconv
 * 
 * The struct `lconv` contains numeric and monetary formatting rules as defined
 * by a C locale. Objects of this struct  may be obtained with @ref localeconv.
 * The members of `lconv` are values of type `char` and of type `char*`. Each
 * `char*` member except @ref decimal_point may be pointing at a null character
 * (that is, at an empty C-string). The members of each type `char` are all
 * non-negative numbers, any of which may be @ref CHAR_MAX if the corresponding
 * value is not available in the current C locale.
 * 
 * The characters of the C-strings pointed to by @ref grouping and
 * @ref mon_grouping are interpreted according to their numeric values. When the
 * terminating `'\0'` is encountered, the last value seen is assumed to repeat
 * for the remainder of digits. If @ref CHAR_MAX is encountered, no further
 * digits are grouped. The typical grouping of three digits at a time is
 * `"\003"`.
 * 
 * The values of @ref p_sep_by_space, @ref n_sep_by_space,
 * @ref int_p_sep_by_space, @ref int_n_sep_by_space are interpreted as follows:
 * 
 * - `0`: No space separates the currency symbol and the value
 * - `1`: Sign sticks to the currency symbol, value is separated by a space
 * - `2`: Sign sticks to the value. Currency symbol is separated by a space
 * 
 * The values of @ref p_sign_posn, @ref n_sign_posn, @ref int_p_sign_posn,
 * @ref int_n_sign_posn are interpreted as follows:
 * 
 * - `0`: Parentheses around the value and the currency symbol are used to represent the sign
 * - `1`: Sign before the value and the currency symbol
 * - `2`: Sign after the value and the currency symbol
 * - `3`: Sign before the currency symbol
 * - `4`: Sign after the currency symbol
 * 
 * @todo Copy briefs of each field as detailed description
 * @todo Make sure mentions of fields are ref'd
 * 
 * @see @ref localeconv @copybrief localeconv
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/locale/lconv) for `lconv`
*/
struct lconv {
	// Non-monetary numeric formatting parameters
	char* decimal_point; ///< The character used as the decimal point
	char* thousands_sep; ///< The character used to separate groups of digits before the decimal point
	char* grouping; ///< A string whose elements indicate the sizes of digit groups

	// Monetary numeric formatting parameters
	char* mon_decimal_point; ///< The character used as the decimal point
	char* mon_thousands_sep; ///< The character used to separate groups of digits before the decimal point
	char* mon_grouping; ///< A string whose elements indicate the sizes of digit groups
	char* positive_sign; ///< A string used to indicate non-negative monetary quantity
	char* negative_sign; ///< A string used to indicate negative monetary quantity

	// Local monetary numeric formatting parameters
	char* currency_symbol; ///< The symbol used for currency in the current C locale
	char frac_digits; ///< The number of digits after the decimal point to display in a monetary quantity
	char p_cs_precedes; ///< `1` if `currency_symbol` is placed before non-negative value, `0` if after
	char n_cs_precedes; ///< `1` if `currency_symbol` is placed before negative value, `0` if after
	char p_sep_by_space; ///< Indicates the separation of `currency_symbol`, `positive_sign`, and the non-negative monetary value
	char n_sep_by_space; ///< Indicates the separation of `currency_symbol`, `negative_sign`, and the negative monetary value
	char p_sign_posn; ///< Indicates the position of `positive_sign` in a non-negative monetary value
	char n_sign_posn; ///< Indicates the position of `negative_sign` in a negative monetary value

	// International monetary numeric formatting parameters
	char* int_curr_symbol; ///< The string used as international currency name in the current C locale
	char int_frac_digits; ///< The number of digits after the decimal point to display in an international monetary quantity
	char int_p_cs_precedes; ///< `1` if `int_curr_symbol` is placed before non-negative international monetary value, `0` if after
	char int_n_cs_precedes; ///< `1` if `int_curr_symbol` is placed before negative international monetary value, `0` if after
	char int_p_sep_by_space; ///< Indicates the separation of `int_curr_symbol`, `positive_sign`, and the non-negative international monetary value
	char int_n_sep_by_space; ///< Indicates the separation of `int_curr_symbol`, `negative_sign`, and the negative international monetary value
	char int_p_sign_posn; ///< Indicates the position of `positive_sign` in a non-negative international monetary value
	char int_n_sign_posn; ///< Indicates the position of `negative_sign` in a negative international monetary value
};

/** @brief Gets and sets the current C locale
 * 
 * The `setlocale` function installs the specified system locale or its portion
 * as the new C locale. The modifications remain in effect and influences the
 * execution of all locale-sensitive C library functions until the next call to
 * `setlocale`. If `locale` is a null pointer, `setlocale` queries the current C
 * locale without modifying it.
 * 
 * @param category Locale category identifier, one of the `LC_xxx` macros. May
 * be null.
 * @param locale System-specific locale identifier. Can be `""` for the
 * user-preferred locale or "C" for the minimal locale
 * 
 * @returns Pointer to a narrow null-terminated string identifying the C locale
 * after applying the changes, if any, or null pointer on failure.
 * @returns A copy of the returned string along with the category used in this
 * call to `setlocale` may be used later in the program to restore the locale
 * back to the state at the end of this call.
 * 
 * @note During program startup, the equivalent of `setlocale(LC_ALL, "C");` is
 * executed before any user code is run.
 * @note Although the return type is `char*`, modifying the pointed-to
 * characters is undefined behaviour.
 * @note Because `setlocale` modifies global state which affects execution of
 * locale-dependent functions, it is undefined behaviour to call it from one
 * thread, while another thread is executing any of the following functions:
 * @ref fprintf, @ref isprint, @ref iswdigit, @ref localeconv, @ref tolower,
 * @ref fscanf, @ref ispunct, @ref iswgraph, @ref mblen, @ref toupper,
 * @ref isalnum, @ref isspace, @ref iswlower, @ref mbstowcs, @ref towlower,
 * @ref isalpha, @ref isupper, @ref iswprint, @ref mbtowc, @ref towupper,
 * @ref isblank, @ref iswalnum, @ref iswpunct, @ref setlocale, @ref wcscoll,
 * @ref iscntrl, @ref iswalpha, @ref iswspace, @ref strcoll, @ref wcstod,
 * @ref isdigit, @ref iswblank, @ref iswupper, @ref strerror, @ref wcstombs,
 * @ref isgraph, @ref iswcntrl, @ref iswxdigit, @ref strtod, @ref wcsxfrm,
 * @ref islower, @ref iswctype, @ref isxdigit.
 * @note POSIX also defines a locale named "POSIX", which is always accessible
 * and is exactly equivalent to the default minimal "C" locale.
 * @note POSIX also specifies that the returned pointer, not just the contents
 * of the pointed-to string, may be invalidated by subsequent calls to
 * `setlocale`.
 * 
 * @see @ref libc_locale_categories @copybrief libc_locale_categories
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/locale/setlocale) for `setlocale`
*/
extern char* setlocale(int category, const char* locale);

/** @brief Queries numeric and monetary formatting details of the current locale
 * 
 * The `localeconv` function obtains a pointer to a static object of type
 * @ref lconv, which represents numeric and monetary formatting rules of the
 * current C locale.
 * 
 * @returns Pointer to the current @ref lconv object.
 * 
 * @note Modifying the object references through the returned pointer is
 * undefined behaviour.
 * @note `localeconv` modifies a static object, calling it from different
 * threads without synchronisation is undefined behaviour.
 * 
 * @see @ref setlocale @copybrief setlocale
 * @see @ref lconv @copybrief lconv
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/locale/localeconv) for `localeconv`
*/
struct lconv* localeconv(void);

/**
 * @defgroup libc_locale_categories LC_ALL, LC_COLLATE, ..., LC_TIME 
 * @brief Locale categories for @ref setlocale
 * @ingroup libc_locale
 * 
 * Each of these macro constants expand to integer constant expressions with
 * distinct values that are suitable for use as the first argument of
 * @ref setlocale.
 * 
 * Additional macro constants, with names that begin with `LC_` followed by at
 * least one uppercase letter, may be defined in @ref libc_locale. For example,
 * the POSIX specification requires `LC_MESSAGES` (which controls, among other
 * things, @ref perror and @ref stderror), ISO/IEC 30112:2014
 * ([2014 draft](https://www.open-std.org/JTC1/SC35/WG5/docs/30112d10.pdf))
 * additionally defines `LC_IDENTIFICATION`, `LC_XLITERATE`, `LC_NAME`,
 * `LC_ADDRESS`, `LC_TELEPHONE`, `LC_PAPER`, `LC_MEASUREMENT`, and
 * `LC_KEYBOARD`, which are supported by the GNU C library (except for
 * `LC_XLITERATE`).
 * 
 * @see @ref setlocale @copybrief setlocale
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/locale/LC_categories)
 * for **locale categories**
 * 
 * @todo Implementations for categories. Could be just digits
 * 
 * @{
 */
// TODO: Implement locale categories
#define LC_ALL 1 ///< Selects the entire C locale
#define LC_COLLATE 2 ///< Selects the collation category of the C locale
#define LC_CTYPE 3 ///< Selects the character classification category of the C locale
#define LC_MONETARY 4 ///< Selects the monetary formatting category of the C locale
#define LC_NUMERIC 5 ///< Selects the numeric formatting category of the C locale
#define LC_TIME 6 ///< Selects the time formatting category of the C locale
/** @} */

#endif

/** @} */
