/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/******************************************************************************
 * The first part of the header can be loaded and loaded again, so we can turn
 * on and off the debug facility for each source file.
 */
#undef DEBUG


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

#define SYS_STR(qstring) ("\033[1m" + qstring + "\033[0;39m").toLatin1().constData()
#define SYS_BOOL(boolean) (boolean ? \
        "\033[1mtrue\033[0;39m" : \
        "\033[1mfalse\033[0;39m")

#endif
