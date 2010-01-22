/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/******************************************************************************
 * The first part of the header can be loaded and loaded again, so we can turn
 * on and off the debug facility for each source file.
 */

/*
 * If the debug facility is enabled we also enable all the warning messages.
 */
#if defined(DEBUG) && !defined(WARNING)
#  define WARNING
#endif

#if defined(WARNING) && !defined(CRITICAL)
#  define CRITICAL
#endif
/*
 * The SYS_DEBUG macro is used to print normal debug messages.
 */
#undef SYS_DEBUG
#ifdef DEBUG
#  define SYS_DEBUG(...) SysDebug::sysPrintMsg (\
        QtDebugMsg, \
        __PRETTY_FUNCTION__, \
        __VA_ARGS__)
#else
#  define SYS_DEBUG(...) { /* Nothing... */ }
#endif

/*
 * The SYS_WARNING is used to print warning messages.
 */
#undef SYS_WARNING
#ifdef WARNING
#  define SYS_WARNING(...) SysDebug::sysPrintMsg (\
        QtWarningMsg, \
        __PRETTY_FUNCTION__, \
        __VA_ARGS__)
#else
#  define SYS_WARNING(...) { /* Nothing... */ }
#endif

/*
 * The SYS_CRITICAL is used to print warning messages.
 */
#undef SYS_CRITICAL
#ifdef CRITICAL
#  define SYS_CRITICAL(...) SysDebug::sysPrintMsg (\
        QtCriticalMsg, \
        __PRETTY_FUNCTION__, \
        __VA_ARGS__)
#else
#  define SYS_CRITICAL(...) { /* Nothing... */ }
#endif

/******************************************************************************
 * Here we have those parts that can be loaded only once, so we protect them
 * with DEBUG_H. 
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <QtDebug>

namespace SysDebug
{
    void sysPrintMsg (
        QtMsgType     type,
        const char   *function,
        const char   *formatstring,
        ...);
};

#define SYS_STR(qstring) qstring.toLatin1().constData()
#define SYS_BOOL(boolean) (boolean ? "true" : "false")
#endif
