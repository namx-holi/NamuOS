/**
 * @file time.h
 * @defgroup libc_time <time.h>
 * @brief Time/date utilities
 * @ingroup libc
 * 
 * @todo Detailed description
 * @todo See X documentation
 * 
 * @see [C documentation](https://en.cppreference.com/w/c/chrono)
 * for **Date and time utilities**
 * 
 * @{
*/

#ifndef _LIBC_TIME_H
#define _LIBC_TIME_H 1

#include <stddef.h> // size_t


/***********************************************/
/* Date And Times Utility Functions            */
/* Ref: https://en.cppreference.com/w/c/chrono */
/***********************************************/

/** @brief Number of processor clock ticks per second
 * 
 * Expands to an expression (not necessarily a compile-time constant) of type
 * @ref clock_t equal to the number of clock ticks per second, as returned by
 * @ref clock().
 * 
 * @note POSIX defines `CLOCKS_PER_SEC` as one million, regardless of the actual
 * precision of @ref clock.
 * @note Until standardised as `CLOCKS_PER_SEC` in C89, this macro was sometimes
 * known by its IEEE std 1003.1-1988 name `CLK_TCK`: that name was not included
 * in C89 and was removed from POSIX itself in 1966 over ambiguity with
 * `_SC_CLK_TCK`, which gives the number of clocks per second for the function
 * [`times()`](https://pubs.opengroup.org/onlinepubs/9699919799/functions/times.html).
 * 
 * @see @ref clock @copybrief clock
 * @see @ref clock_t @copybrief clock_t
*/
#define CLOCKS_PER_SECOND ((__clock_t) 1000000)
// TODO: Find out how to calculate this

// TODO: Documentation, Expands to a value suitable for use as the base argument of timespec_get
#define TIME_UTC /* implementation defined */
// TODO: Define TIME_UTC

/** @brief Calendar time type
 * 
 * Structure holding a calendar date and time broken down into its components.
 * 
 * @note The Standard mandates only the presence of the aforementioned members
 * in either order. The implementations usually add more data-members to this
 * structure.
 * @note The range of @ref tm_sec allows for a positive leap second. Two leap
 * seconds in the same minute are not allowed (the C89 range 0..61 was a defect)
 * 
 * @see @ref localtime, @ref localtime_r, @ref localtime_s, @copybrief localtime
 * @see @ref gmtime, @ref gmtime_r, @ref gmtime_s, @copybrief gmtime
*/
struct tm {
	int tm_sec; ///< Seconds after minute - [`0`, `60`]
	int tm_min; ///< Minutes after the hour - [`0`, `59`]
	int tm_hour; ///< Hours since midnight - [`0`, `23`]
	int tm_mday; ///< Day of the month - [`1`, `31`]
	int tm_mon; ///< Months since January - [`0`, `11`]
	int tm_year; ///< Years since 1900
	int tm_wday; ///< Days since Sunday - [`0`, `6`]
	int tm_yday; ///< Days since January 1 - [`0`, `365`]
	int tm_isdst; ///< Daylight Saving Time flag. The value is positive if DST is in effect, zero if not and negative if no information is available
	// TODO: long int tm_gmtoff, Seconds east of UTC
	// TODO: const char* tm_zone, Timezone abbreviation
};

/** @brief Calendar time since epoch type
 * 
 * Real arithmetic type capable of representing times.
 * 
 * Although not defined by the C standard, this is almost always an integral
 * value holding the number of seconds (not counting leap seconds) since 00:00,
 * Jan 1 1970 UTC, corresponding to POSIX time.
 * 
 * @note The standard uses the term *calendar time* when referring to a value of
 * type **time_t**.
 * 
 * @see @ref time @copybrief time
 * @see @ref localtime, @ref localtime_r, @ref localtime_s, @copybrief localtime
 * @see @ref gmtime, @ref gmtime_r, @ref gmtime_s, @copybrief gmtime
*/
typedef long time_t;

/** @brief Processor time since era type
 * 
 * Real type capable of representing the processor time used by a process. It
 * has implementation-defined range and precision.
 * 
 * @see @ref clock @copybrief clock
 * @see @ref CLOCKS_PER_SEC @ref CLOCKS_PER_SEC
*/
typedef long clock_t;

/** @brief Time in seconds and nanoseconds
 * 
 * Structure holding an interval broken down into seconds and nanoseconds.
 * 
 * @note The type of @ref tv_nsec is `long long` on some platforms, which is
 * conforming only since C23.
 * 
 * @see @ref timespec_get @copybrief timespec_get
 * @see @ref tm @copybrief tm
*/
struct timespec {
	time_t tv_sec; ///< Whole seconds (valid values are >= 0)
	long tv_nsec; ///< Nanoseconds (valid values are [0, 999999999])
};

// TODO: Documentation, Computes the difference between times
extern double difftime(time_t time_end, time_t time_beg);

// TODO: Documentation, Returns the current calendar time of the system as time since epoch
extern time_t time(time_t* arg);

// TODO: Documentation, Returns raw processor clock time since the program is started
extern clock_t clock(void);

// TODO: Documentation, Returns the calendar time in seconds and nanoseconds based on a given time base
extern int timespec_get(struct timespec* ts, int base);

// TODO: Documentation, Returns the resolution of calendar time based on a given time base
extern int timespec_getres(struct timespec* ts, int base);

// TODO: Documentation, Converts a @ref tm object to a textual representation
extern char* asctime(const struct tm* time_ptr) __attribute__((__deprecated__));
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for asctime_s
extern errno_t asctime_s(char* buf, rsize_t bufsz, const struct tm* time_ptr);
#endif

// TODO: Documentation, Converts a @ref time_t object to a textual representation
extern char* ctime(const time_t* timer) __attribute__((__deprecated__));
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for ctime_s
extern errno_t ctime_s(char* buf, rsize_t bufsz, const time_t* timer);
#endif

// TODO: Documentation, Converts a @ref tm object to a custom textual representation
extern size_t strftime(char* restrict str, size_t count, const char* restrict format, const struct tm* restrict tp);

// TODO: Documentation, Converts time since epoch to calendar time expressed as UTC
extern struct tm* gmtime(const time_t* timer);
// TODO: Documentation for gmtime_r
extern struct tm* gmtime_r(const time_t* timer, struct tm* buf);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for gmtime_s
extern struct tm* gmtime_s(const time_t* restrict timer, struct tm* restrict buf);
#endif

// TODO: Documentation, Converts time since epoch to calendar time expressed as local time
extern struct tm* localtime(const time_t* timer);
// TODO: Documentation for localtime_r
extern struct tm* localtime_r(const time_t* timer, struct tm* buf);
#ifdef __STDC_LIB_EXT1__ /* Bounds checking */
// TODO: Also check for __STDC_WANT_LIB_EXT1__
// TODO: Documentation for localtime_s
extern struct tm* localtime_s(const time_t* restrict timer, struct tm* restrict buf);
#endif

// TODO: Documentation, Converts calendar time to time since epoch
extern time_t mktime(struct tm* arg);

#endif

/** @} */
