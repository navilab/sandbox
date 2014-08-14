/** Module rsb2_eventmgr - Implementation.
 * @file rsb2_eventmgr.c
 * @author jp.tranvouez@navilab.com
 */
#include "rsb2_eventmgr.h"
#include "rsb2_assert.h"

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void rsb2_eventmgr_handle(const char *func, const char *file, int line,
		const char *name, const char *descr)
{
	char loc[256];
	snprintf(loc, sizeof(loc), "%-16.16s %4d %s", file, line, func);
	if (descr &&*descr) {
		fprintf(stderr, "event    %-50.50s|%s,%s\n", loc, name, descr);
	} else {
		fprintf(stderr, "event    %-50.50s|%s\n", loc, name);
	}
}

static rsb2_Event_handler *g_fHandler = rsb2_eventmgr_handle;

void rsb2_eventmgr_setHandler(rsb2_Event_handler *fHandler)
{
	g_fHandler = fHandler? fHandler: rsb2_eventmgr_handle;
}

void rsb2_eventmgr_notify(const char *func, const char *file, int line,
		const char *name, const char *fmt, ...)
{
	if (fmt && *fmt) {
		va_list ap;
		va_start(ap, fmt);
		char descr[256];
		vsnprintf(descr, sizeof(descr), fmt, ap);
		va_end(ap);
		g_fHandler(func, file, line, name, descr);
	} else {
		g_fHandler(func, file, line, name, NULL);
	}
}

void rsb2_eventmgr_error(const char *func, const char *file, int line,
		const char *name, const char *fmt, ...)
{
	if (fmt && *fmt) {
		va_list ap;
		va_start(ap, fmt);
		char descr[256];
		vsnprintf(descr, sizeof(descr), fmt, ap);
		va_end(ap);
		g_fHandler(func, file, line, name, descr);
	} else {
		g_fHandler(func, file, line, name, NULL);
	}
}

void rsb2_eventmgr_errno(const char *func, const char *file, int line,
		const char *fcn, int err, const char *fmt, ...)
{
	char descr[256];
	if (fmt && *fmt) {
		va_list ap;
		va_start(ap, fmt);
		char buf[256];
		vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);
		snprintf(descr, sizeof(descr), "fcn=%s,errno=%d,err=%s,%s",
				fcn, err, strerror(err), buf);
	} else {
		snprintf(descr, sizeof(descr), "fcn=%s,errno=%d,err=%s",
				fcn, err, strerror(err));
	}
	g_fHandler(func, file, line, "errno_set", descr);
}

void rsb2_eventmgr_errret(const char *func, const char *file, int line,
		const char *fcn, int ret, const char *fmt, ...)
{
	char descr[256];
	if (fmt && *fmt) {
		va_list ap;
		va_start(ap, fmt);
		char buf[256] = "";
		vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);
		snprintf(descr, sizeof(descr), "fcn=%s,ret=%d,%s",
				fcn, ret, buf);
	} else {
		snprintf(descr, sizeof(descr), "fcn=%s,ret=%d",
				fcn, ret);
	}
	g_fHandler(func, file, line, "error_return", descr);
}

/*END*/
