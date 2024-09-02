/**
 * @file ctype.h
 * @defgroup libc_ctype <ctype.h>
 * @brief Functions to determine the type contained in character data
 * @ingroup libc
 * 
 * @todo Detailed description
 * 
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte)
 * for `Null`-terminated byte strings
 * 
 * @{
*/

// Ref: https://en.cppreference.com/w/c/string/byte

#ifndef _LIBC_CTYPE_H
#define _LIBC_CTYPE_H 1

/** @brief Checks if a character is alphanumeric
 * 
 * Checks if the given digit is an alphanumeric character as classified by the
 * current C locale. In the default locale, the following characters are
 * alphanumeric:
 * 
 * - Digits (`0123456789`),
 * - Uppercase letters (`ABCDEFGHIJKLMNOPQRSTUVWXYZ`),
 * - Lowercase letters (`abcdefghijklmnopqrstuvwxyz`)
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is an alphanumeric character, `0`
 * otherwise.
 * 
 * @see @ref iswalnum @copybrief iswalnum
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isalnum)
 * for `isalnum`
*/
extern int isalnum(int ch);

/** @brief Checks if a character is alphabetic
 * 
 * Checks if the given character is an alphanumeric character, i.e. either an
 * upercase letter (`ABCDEFGHIJKLMNOPQRSTUVWXYZ`) or a lowercase letter
 * (`abcdefghijklmnopqrstuvwxyz`).
 * 
 * In locales other than `"C"`, an alphanumeric character is a character for
 * which @ref isupper or @ref islower returns `true` or any other character
 * considered alphabetic by the locale. In any case, @ref iscntrl, @ref isdigit,
 * @ref ispunct, and @ref isspace will return `false` for this character.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is an alphabetic character, zero
 * otherwise.
 * 
 * @see @ref iswalpha @copybrief iswalpha
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isalpha)
 * for `isalpha`
*/
extern int isalpha(int ch);

/** @brief Checks if a character is lowercase
 * 
 * Checks if the given character is classified as a lowercase character
 * according to the current C locale. In the default "C" locale, `islower`
 * returns true only for the lowercase letters (`abcdefghijklmnopqrstuvwxyz`).
 * 
 * If `islower` returns `true`, it is guaranteed that @ref iscntrl,
 * @ref isdigit, @ref ispunct, and @ref isspace return `false` for the same
 * character in the same C locale.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a lowercase letter, zero
 * otherwise.
 * 
 * @see @ref iswlower @copybrief iswlower
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/islower)
 * for `islower`
*/
extern int islower(int ch);

/** @brief Checks if a character is uppercase
 * 
 * Checks if the given character is an uppercase character according to the
 * current C locale. In the default "C" locale, `isupper` returns true only for
 * the uppercase letters (`ABCDEFGHIJKLMNOPQRSTUVWXYZ`).
 * 
 * If `isupper` returns `true`, it is guaranteed that @ref iscntrl,
 * @ref isdigit, @ref ispunct, and @ref isspace return `false` for the same
 * character in the same C locale.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is an uppercase letter, zero
 * otherwise.
 * 
 * @see @ref iswupper @copybrief iswupper
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isupper)
 * for `isupper`
*/
extern int isupper(int ch);

/** @brief Checks if a character is a digit
 * 
 * Checks if the given character is a numeric character (`0123456789`).
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a numeric character, zero
 * otherwise.
 * 
 * @note `isdigit` and @ref isxdigit are the only standard narrow character
 * classifications that are not affected by the currently installed C locale,
 * although some implementations (e.g. Microsoft in
 * [1252 codepage](https://en.wikipedia.org/wiki/Windows-1252)) may classify
 * additional single-byte characters as digits.
 * 
 * @see @ref iswdigit @copybrief iswdigit
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isdigit)
 * for `isdigit`
*/
extern int isdigit(int ch);

/** @brief Checks if a character is a hexadecimal character
 * 
 * Checks if the given character is a hexadecimal numeric character
 * (`0123456789abcdefABCDEF`) or is classified as a hexadecimal character.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a hexadecimal numeric character,
 * zero otherwise.
 * 
 * @note @ref isdigit and `isxdigit` are the only standard narrow character
 * classifications that are not affected by the currently installed C locale,
 * although some implementations (e.g. Microsoft in
 * [1252 codepage](https://en.wikipedia.org/wiki/Windows-1252)) may classify
 * additional single-byte characters as digits.
 * 
 * @see @ref iswxdigit @copybrief iswxdigit
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isxdigit)
 * for `isxdigit`
*/
extern int isxdigit(int ch);

/** @brief Checks if a character is a control character
 * 
 * Checks if the given character is a control character, i.e. codes `0x00-0x1F`
 * and `0x7F`.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a control character, zero
 * otherwise.
 * 
 * @see @ref iswcntrl @copybrief iswcntrl
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/iscntrl)
 * for `iscntrl`
*/
extern int iscntrl(int ch);

/** @brief Checks if a character is a graphical character
 * 
 * Checks if the given character has a graphical representation, i.e. it is
 * either a number (`0123456789`), an uppercase letter
 * (`ABCDEFGHIJKLMNOPQRSTUVWXYZ`), a lowercase letter
 * (`abcdefghijklmnopqrstuvwxyz`), or a punctuation character
 * (<tt>!"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~</tt>), or any graphical character specific to
 * the current C locale.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character has a graphical representation
 * character, zero otherwise.
 * 
 * @see @ref iswgraph @copybrief iswgraph
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isgraph)
 * for `isgraph`
*/
extern int isgraph(int ch);

/** @brief Checks if a character is a space character
 * 
 * Checks if the given character is either:
 * - A standard white-space character:
 *   - Space (`0x20`, <tt>' '</tt>),
 *   - Form feed (`0x0c`, <tt>'\\f'</tt>),
 *   - Line feed (`0x0a`, <tt>'\\n'</tt>),
 *   - Carriage return (`0x0d`, <tt>'\\r'</tt>),
 *   - Horizontal tab (`0x09`, <tt>'\\t'</tt>),
 *   - Vertical tab (`0x0b`, <tt>'\\v'</tt>),
 * - Or a locale-specific white-space character.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a whitespace character, zero
 * otherwise.
 * 
 * @see @ref iswspace @copybrief iswspace
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isspace)
 * for `isspace`
*/
extern int isspace(int ch);

/** @brief Checks if a character is a blank character
 * 
 * Checks if the given character is a blank character in the current locale. In
 * the default C locale, only space (`0x20`) and horizontal tab (`0x09`) are
 * classified as blank.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a blank character, zero
 * otherwise.
 * 
 * @see @ref iswblank @copybrief iswblank
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isblank)
 * for `isblank`
*/
extern int isblank(int ch);

/** @brief Checks if a character is a printing character
 * 
 * Checks if the given character can be printed, i.e. it is either a number
 * (`0123456789`), an uppercase letter (`ABCDEFGHIJKLMNOPQRSTUVWXYZ`), a
 * lowercase letter (`abcdefghijklmnopqrstuvwxyz`), a punctuation character
 * (<tt>!"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~</tt>), or space, or any character
 * classified as printable by the current C locale.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character can be printed, zero otherwise.
 * 
 * @see @ref iswprint @copybrief iswprint
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/isprint)
 * for `isprint`
*/
extern int isprint(int ch);

/** @brief Checks if a character is a punctuation character
 * 
 * Checks if the given character is a punctuation character in the current C
 * locale. The default C locale classifies the characters
 * <tt>!"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~</tt> as punctuation.
 * 
 * The behaviour is undefined if the value of `ch` is not representable as
 * `unsigned char` and is not equal to `EOF`.
 * 
 * @param ch Character to classify
 * 
 * @returns Non-zero value if the character is a punctuation character, zero
 * otherwise.
 * 
 * @see @ref iswpunct @copybrief iswpunct
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/ispunct)
 * for `ispunct`
*/
extern int ispunct(int ch);

/** @brief Converts a character to lowercase
 * 
 * Converts the given character to lowercase according to the character
 * conversion rules defined by the currently installed C locale.
 * 
 * In the default "C" locale, the following uppercase letters
 * `ABCDEFGHIJKLMNOPQRSTUVWXYZ` are replaced with respective lowercase letters
 * `abcdefghijklmnopqrstuvwxyz`.
 * 
 * @param ch Character to be converted. If the value of `ch` is not
 * representable as `unsigned char` and does not equal `EOF`, the behaviour is
 * undefined.
 * 
 * @returns Lowercase version of `ch` or unmodified `ch` if no lowercase version
 * is listed in the current C locale.
 * 
 * @see @ref toupper @copybrief toupper
 * @see @ref towlower @copybrief towlower
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/tolower)
 * for `tolower`
*/
extern int tolower(int ch);

/** @brief Converts a character to uppercase
 * 
 * Converts the given character to uppercase according to the character
 * conversion rules defined by the currently installed C locale.
 * 
 * In the default "C" locale, the following lowercase letters
 * `abcdefghijklmnopqrstuvwxyz` are replaced with respective uppercase letters
 * `ABCDEFGHIJKLMNOPQRSTUVWXYZ`.
 * 
 * @param ch Character to be converted. If the value of `ch` is not
 * representable as `unsigned char` and does not equal `EOF`, the behaviour is
 * undefined.
 * 
 * @returns Uppercase version of `ch` or unmodified `ch` if no uppercase version
 * is listed in the current C locale.
 * 
 * @see @ref tolower @copybrief tolower
 * @see @ref towupper @copybrief towupper
 * @see [C++ documentation](https://en.cppreference.com/w/cpp/string/byte/toupper)
 * for `toupper`
*/
extern int toupper(int ch);

#endif

/** @} */
