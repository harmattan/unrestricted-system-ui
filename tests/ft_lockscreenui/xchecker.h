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
#ifndef XCHECKER_H
#define XCHECKER_H

#include <QtTest/QtTest>
#include <QObject>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/*!
 * A handy tool to perform verious GUI tests in a real life situation. This
 * class supports the following operations:
 * 1) Checking if a window in the X Server exists, viewable, and full-screen.
 *    Done by window ID and by window name.
 * 2) A very usefull window dump from the server.
 * 3) Checking if various daemons are crashed (the PID is changed). This check
 *    is always performed.
 * 4) Turning on/off the touch screen.
 */
class XChecker : public QObject
{
    Q_OBJECT

public:
    XChecker ();

    typedef enum {
        CheckIsVisible,
        CheckIsInvisible,
        CheckIsFullscreen,
    } RequestCode;

    bool checkWindow (Window WindowID, XChecker::RequestCode OpCode);
    bool checkWindow (const QString &WMClass, XChecker::RequestCode OpCode);

    void debug_dump_windows(Window highlighted = None);
    void debugDumpNotifications ();

    bool turnOffDisplay ();
    bool turnOnDisplay ();

private:
    Display *display();

    bool check_window_rec (
            Display               *dpy,
            Window                 WindowID,
            const QString         &WMClass,
            XChecker::RequestCode  opCode);

    char *get_atom_prop (Display *dpy, Window w, Atom atom);
    Window get_win_prop(Display *dpy, Window w, Atom atom);
    unsigned long get_card_prop(Display *dpy, Window w, Atom atom);
    long get_int_prop(Display *dpy, Window w, Atom atom);
    char *get_str_prop(Display *dpy, Window w, Atom atom);
    char * get_utf8_prop ( Display *dpy, Window w, Atom atom);
    const char * get_map_state ( int state);
    void pr (
            Window          highlighted,
            Display        *dpy,
            Window          WindowID,
            unsigned int    level,
            unsigned int    nthWindow);

    int pidof (const char *program);
    bool checkPIDs ();

private:
    Atom class_atom, name_atom, name_atom2,
    utf8_string_atom;

    int                  m_CompositorPID;
    int                  m_SysuidPID;
};

#endif
