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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <QString>
#include "debug.h"


#ifdef DEBUG_SUPPRESS_COLOR
#  define TERM_RED     ""
#  define TERM_YELLOW  ""
#  define TERM_GREEN   ""
#  define TERM_NORMAL  ""
#  define TERM_BOLD    ""
#else 
#  define TERM_RED     "\033[1;33m"
#  define TERM_YELLOW  "\033[1;31m"
#  define TERM_GREEN   "\033[1;32m"
#  define TERM_NORMAL  "\033[0;39m"
#  define TERM_BOLD    "\033[1m"
#endif

/*
 * This function is used to print debug and error messages, an enhanced version
 * of the SysDebug::sysMsg(). Please use this function anly through macros (like
 * SYS_DEBUG() for example) so the change in the function interface does not
 * mean the change of all the code calling it).
 */
void 
SysDebug::sysPrintMsg (
        QtMsgType     type,
        const char   *function,
        const char   *formatstring,
        ...)
{
    va_list args;

    va_start (args, formatstring);
    switch (type) {
        case QtDebugMsg:
            fprintf (stderr, "%s%s%s: ",
                    TERM_GREEN TERM_BOLD, function, TERM_NORMAL);
            vfprintf (stderr, formatstring, args);
            break;

        case QtWarningMsg:
            fprintf (stderr, "%s%s%s: ",
                    TERM_YELLOW, function, TERM_NORMAL);
            vfprintf (stderr, formatstring, args);
            break;

        case QtCriticalMsg:
            fprintf (stderr, "%s%s%s: ",
                    TERM_RED, function, TERM_NORMAL);
            vfprintf (stderr, formatstring, args);
            break;

        case QtFatalMsg:
            fprintf (stderr, "%s%s%s: ",
                    TERM_GREEN, function, TERM_NORMAL);
            vfprintf (stderr, formatstring, args);
            fprintf (stderr, "\n%s%s%s: Aborting program.",
                    TERM_RED TERM_BOLD, function, TERM_NORMAL);
            putchar('\n');
            fflush (stderr);
            abort();
    }
    va_end (args);

    fprintf (stderr, "\n");
    fflush (stderr);
}

