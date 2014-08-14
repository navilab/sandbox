/** Module rsb2_tracer - Interface.
 * @file rsb2_tracer.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_tracer Tracer
 * @ingroup rsb2_libcore
 * @{
 */
#ifndef RSB2_TRACER_H
#define RSB2_TRACER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Trace groups. */
typedef enum rsb2_TraceGroup {
	RSB2_TRACEGROUP_FUNC		= 1,	/**< Function call and return. */
	RSB2_TRACEGROUP_DATA		= 2,	/**< Incoming and outgoing messages. */
	RSB2_TRACEGROUP_CTRL		= 4,	/**< Control operations. */
	RSB2_TRACEGROUP_APPL		= 8,	/**< Application trace. */
} rsb2_TraceGroup;

/** Notify a trace event.
 * @param trace event group
 * @param fmt message format, followed by variables
 */
#define RSB2_TRACE(group, fmt, ...) \
		rsb2_module_trace(g_module, __func__, __FILE__, __LINE__, \
				group, fmt, ##__VA_ARGS__)

/** Trace a function call.
 * @param fcn called function name
 */
#define RSB2_TRACE_CALL(fcn) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_call %s", fcn)

/** Trace return from a function call.
 * @param fcn called function name
 */
#define RSB2_TRACE_RETURN(fcn) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_return %s", fcn)

/** Trace entry in the current function. */
#define RSB2_TRACE_ENTRY() \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_entry")

/** Trace entry in the current function. */
#define RSB2_TRACE_ARGS(fmt, ...) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_entry " fmt, ##__VA_ARGS__)

/** Trace exit from the current function.*/
#define RSB2_TRACE_EXIT() \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit")

/** Trace exit from the current function, returning a boolean.
 * @param b returned value
 */
#define RSB2_TRACE_EXIT_BOOL(b) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%s", (b)? "true": "false")

/** Trace exit from the current function, returning an integer.
 * @param i returned value
 */
#define RSB2_TRACE_EXIT_INT(i) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%d", i)

/** Trace exit from the current function, returning a long.
 * @param l returned value
 */
#define RSB2_TRACE_EXIT_LONG(l) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%ld", l)

/** Trace exit from the current function, returning a size.
 * @param n returned value
 */
#define RSB2_TRACE_EXIT_SIZE(n) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%zu", n)

/** Trace exit from the current function, returning a double.
 * @param d returned value
 */
#define RSB2_TRACE_EXIT_DBL(d) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%f", d)

/** Trace exit from the current function, returning a pointer.
 * @param p returned value
 */
#define RSB2_TRACE_EXIT_PTR(p) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret=%p", p)

/** Trace exit from the current function, returning a string.
 * @param s returned value
 */
#define RSB2_TRACE_EXIT_STR(s) \
		RSB2_TRACE(RSB2_TRACEGROUP_FUNC, \
				"func_exit ret='%s'", s)

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_TRACER_H */
