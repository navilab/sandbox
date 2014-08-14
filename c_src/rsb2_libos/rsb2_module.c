/** Module rsb2_module - Implementation.
 * @file rsb2_module.c
 * @author jp.tranvouez@navilab.com
 */
#include "rsb2_module.h"

#include <stdarg.h>
#include <stdio.h>

static int g_module = 0;				/* Module reference. */

int rsb2_module_ref(const char *name)
{
	RSB2_TRACE_ARGS("name=%s", name);
	int ref = 0;
	RSB2_TRACE_EXIT_INT(ref);
	return ref;
}

void rsb2_module_destroy(int ref)
{
	RSB2_TRACE_ENTRY();
	RSB2_TRACE_EXIT();
}

static void rsb2_module_tracer(const char *func, const char *file, int line,
		int ref, rsb2_TraceGroup group, const char *descr)
{
	char loc[256];
	snprintf(loc, sizeof(loc), "%-16.16s %4d %s", file, line, func);
	fprintf(stderr, "trace    %-50.50s|%s\n", loc, descr);
}

static rsb2_Module_tracer *g_fTrace = rsb2_module_tracer;

void rsb2_module_setTracer(rsb2_Module_tracer *fTrace)
{
	g_fTrace = fTrace? fTrace: rsb2_module_tracer;
}

void rsb2_module_trace(int ref, const char *func, const char *file, int line,
		rsb2_TraceGroup group, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char descr[256];
	vsnprintf(descr, sizeof(descr), fmt, ap);
	va_end(ap);
	g_fTrace(func, file, line, ref, group, descr);
}

/*END*/
