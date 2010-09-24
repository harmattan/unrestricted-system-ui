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
#undef HANDLE_CRASH
#ifdef HANDLE_CRASH
#  define BACKTRACE_SIZE 128
#  include <execinfo.h>
#endif

#include "sysuid.h"

#include <MApplication>
#include <MApplicationService>
#include <QObject>

#undef DEBUG
#include "debug.h"

#include "signal.h"

MApplication *exitPtr;

void sysuid_exit (int sig)
{
    Q_UNUSED (sig);
    if (exitPtr) {
        exitPtr->quit ();
        exitPtr = NULL;
    }
}

#ifdef HANDLE_CRASH
void
some_crash_happened (
        void)
{
    void     *backtrace_array [BACKTRACE_SIZE];
    char    **backtrace_strings;
    size_t    backtrace_size;

    SYS_WARNING ("Crash...");
    backtrace_size = backtrace (backtrace_array, BACKTRACE_SIZE);
    backtrace_strings = backtrace_symbols (backtrace_array, backtrace_size);

    /*
     * Let's print the backtrace from the stack.
     */
    fprintf (stderr, "++++++++++ Crash backtrace of sysuid ++++++++++\n");
    for (size_t i = 0; i < backtrace_size; i++) {
        fprintf (stderr, "+++ %03u %-20s +++\n", i, backtrace_strings[i]);
    }
    fprintf (stderr, "+++++++++++++++++++++++++++++++++++++++++++++++\n");
    fflush (stderr);
}

void
termination_signal_handler (
        int signum)
{
    SYS_DEBUG ("*** signum = %d", signum);
    switch (signum) {
        case SIGINT:
		    //sysuid_exit (signum);
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
            /*
             * These are the signals that are not caused by any bug in the code.
             * We can do something with these, but now we just exit.
             */
            some_crash_happened ();
            exit (0);
            break;
       
        case SIGILL:
        case SIGSEGV:
        case SIGBUS:
        case SIGABRT:
        case SIGFPE:
            /*
             * And here are those that are caused by errors.
             */
            some_crash_happened ();
            break;
    }

    /*
     * If we discovered an applet crash we raise the same signal so that the
     * parent will know that we need to be restarted.
     */
    signal (signum, SIG_DFL);
    raise (signum);
}
#endif

class SystemUIservice : public MApplicationService
{

public:
    SystemUIservice (QObject *parent = 0) :
        MApplicationService ("com.nokia.sysuid", parent)
    {
    }

    void launch ()
    {
        SYS_DEBUG ("");
        /*
         * No operation, we must not let meegotouch to
         * raise/activate some hidden sysuid window.
         */
    }
};

int
main (int argc, char** argv)
{
    SYS_DEBUG ("- System-UI start");
    #ifdef DEBUG
    SYS_DEBUG ("+++ Arguments:");
    for (int n = 0; n < argc; ++n) {
        SYS_DEBUG ("*arg[%d] = %s", n, argv[n]);
    }
    SYS_DEBUG ("+++ Environtment:");
    system ("set");
    #endif

    #ifdef HANDLE_CRASH
    signal (SIGTERM, termination_signal_handler);
    signal (SIGHUP,  termination_signal_handler);
    signal (SIGINT,  termination_signal_handler);
    signal (SIGQUIT,  termination_signal_handler);

    signal (SIGILL,  termination_signal_handler);
    signal (SIGSEGV, termination_signal_handler);
    signal (SIGBUS, termination_signal_handler);
    signal (SIGABRT, termination_signal_handler);
    signal (SIGFPE,  termination_signal_handler);
    #else
    signal (SIGINT, sysuid_exit);
    #endif

    MApplication app (argc, argv, new SystemUIservice);
    exitPtr = &app;

    app.setQuitOnLastWindowClosed (false);

    signal (SIGINT, sysuid_exit);

    Sysuid daemon(&app);

    SYS_DEBUG ("- System-UI daemon initialized");

    return app.exec ();
}

