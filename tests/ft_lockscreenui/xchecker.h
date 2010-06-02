/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
 * A class to contact the X Server and check if certain things are going as they
 * should. This class is used in the real life for we discovered some problems.
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
            Window   highlighted,
            Display *dpy, 
            Window   WindowID, 
            int      level, 
            int      nthWindow);

private:
    Atom class_atom, name_atom, name_atom2, pid_atom,
    trans_atom, hildon_stack_atom, utf8_string_atom,
    win_type_atom, wm_state_atom, non_comp_atom;
};

#endif
