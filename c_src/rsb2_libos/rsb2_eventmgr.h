/** Module rsb2_eventmgr - Interface.
 * @file rsb2_eventmgr.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_eventmgr Event manager
 * @ingroup rsb2_libcore
 * @{
 */
#ifndef RSB2_EVENTMGR_H
#define RSB2_EVENTMGR_H

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Event handler function type.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param name event name
 * @param descr event arguments
 */
typedef void rsb2_Event_handler(const char *func, const char *file, int line,
		const char *name, const char *descr);

/** Inject an event handler.
 * Reinstall the default event handler if fHandler is null.
 * @param fHandler event handler function or NULL
 */
void rsb2_eventmgr_setHandler(rsb2_Event_handler *fHandler);

/** Notify an event.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param name event name
 * @param fmt event arguments format, followed by argument values
 */
void rsb2_eventmgr_notify(const char *func, const char *file, int line,
		const char *name, const char *fmt, ...)
		__attribute__((format(printf, 5, 6)));

/** Notify an error.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param name event name
 * @param fmt event arguments format, followed by argument values
 */
void rsb2_eventmgr_error(const char *func, const char *file, int line,
		const char *name, const char *fmt, ...)
		__attribute__((format(printf, 5, 6)));

/** Notify a non-zero errno.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param fcn called function name
 * @param err errno value
 * @param fmt error arguments format, followed by argument values
 */
void rsb2_eventmgr_errno(const char *func, const char *file, int line,
		const char *fcn, int err, const char *fmt, ...)
		__attribute__((format(printf, 6, 7)));

/** Notify an error return.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param fcn called function name
 * @param ret returned value
 * @param fmt error arguments format, followed by argument values
 */
void rsb2_eventmgr_errret(const char *func, const char *file, int line,
		const char *fcn, int ret, const char *fmt, ...)
		__attribute__((format(printf, 6, 7)));

/** Notify an event.
 * @param name event name
 * @param fmt event arguments format, followed by argument values
 */
#define RSB2_NOTIFY(name, fmt, ...) \
		rsb2_eventmgr_notify(__func__, __FILE__, __LINE__, \
				name, fmt, ##__VA_ARGS__)

/** Notify an error.
 * @param name event name
 * @param fmt event arguments format, followed by argument values
 */
#define RSB2_ERROR(name, fmt, ...) \
		rsb2_eventmgr_error(__func__, __FILE__, __LINE__, \
				name, fmt, ##__VA_ARGS__)

/** Notify a non-zero errno set by an external function.
 * @param name event name
 * @param fcn called function name
 * @param fmt event arguments format, followed by argument values
 */
#define RSB2_ERRNO(fcn, fmt, ...) \
		rsb2_eventmgr_errno(__func__, __FILE__, __LINE__, \
				fcn, errno, fmt, ##__VA_ARGS__)

/** Notify an error return by an external function.
 * @param name event name
 * @param fcn called function name
 * @param ret returned value
 * @param fmt event arguments format, followed by argument values
 */
#define RSB2_ERRRET(fcn, ret, fmt, ...) \
		rsb2_eventmgr_errret(__func__, __FILE__, __LINE__, \
				fcn, ret, fmt, ##__VA_ARGS__)

/** Notify an error return from an internal function.
 */
#define RSB2_ERRTRACE() \
		rsb2_eventmgr_error(__func__, __FILE__, __LINE__, \
				"error_trace", NULL)

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_EVENTMGR_H */
