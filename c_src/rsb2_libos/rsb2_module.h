/** Module rsb2_module - Interface.
 * @file rsb2_module.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_module Module Manager
 * @ingroup rsb2_libcore
 * @{
 */
#ifndef RSB2_MODULE_H
#define RSB2_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rsb2_assert.h"	/* convenience */
#include "rsb2_eventmgr.h"	/* convenience */
#include "rsb2_tracer.h"	/* convenience */

/** Trace handler function type.
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @parem ref module reference
 * @param group event group
 * @param descr event arguments
 */
typedef void rsb2_Module_tracer(const char *func, const char *file, int line,
		int ref, rsb2_TraceGroup group, const char *descr);

/** Inject a trace handler.
 * Reinstall the default trace handler if fHandler is null.
 * @param fTrace trace handler function or NULL
 */
void rsb2_module_setTracer(rsb2_Module_tracer *fTrace);

/** Return a module reference.
 * @param name module name
 * @return module reference
 * @retval -1 error
 */
int rsb2_module_ref(const char *name);

/** Destroy a module object.
 * @param ref module reference
 */
void rsb2_module_destroy(int ref);

/** Notify a trace event.
 * @parem ref module reference
 * @param func function name
 * @param file source file name
 * @param line soure line number
 * @param group event group
 * @param fmt event arguments format, followed by argument values
 */
void rsb2_module_trace(int ref, const char *func, const char *file, int line,
		rsb2_TraceGroup group, const char *fmt, ...)
		__attribute__((format(printf, 6, 7)));

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_MODULE_H */
