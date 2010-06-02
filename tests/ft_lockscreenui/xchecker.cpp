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
#include "xchecker.h"
#include "QX11Info"

#include <MNotification>

#define SHORT_DEBUG
#define DEBUG
#include "../../src/debug.h"

XChecker::XChecker()
{
	Display *dpy = display();
    
    class_atom = XInternAtom(dpy, "WM_CLASS", False);
    name_atom = XInternAtom(dpy, "_NET_WM_NAME", False);
    name_atom2 = XInternAtom(dpy, "WM_NAME", False);
    pid_atom = XInternAtom(dpy, "_NET_WM_PID", False);
    trans_atom = XInternAtom(dpy, "WM_TRANSIENT_FOR", False);
    utf8_string_atom = XInternAtom(dpy, "UTF8_STRING", False);
    win_type_atom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    wm_state_atom = XInternAtom(dpy, "_NET_WM_STATE", False);
    hildon_stack_atom = XInternAtom(dpy, "_HILDON_STACKABLE_WINDOW", False);
    non_comp_atom = XInternAtom(dpy, "_HILDON_NON_COMPOSITED_WINDOW", False);
}

Display *
XChecker::display()
{
    Display *dpy;

    // Could be XOpenDisplay(NULL), but we already have a display.
    dpy = XOpenDisplay(NULL);
	//dpy = QX11Info::display (); 
    Q_ASSERT (dpy != NULL);
    
    return dpy;
}

char *
XChecker::get_atom_prop (
        Display     *dpy, 
        Window       w, 
        Atom         atom)
{ 
    Atom type;
    int format, rc;
    unsigned long items;
    unsigned long left;
    Atom *value;
	char *copy;

    rc = XGetWindowProperty (
            dpy, w, atom, 0, 1, False, XA_ATOM, &type, &format,
            &items, &left, (unsigned char**)&value);

    if (type != XA_ATOM || format == 0 || rc != Success) {
        copy = strdup("");
    } else {
        char *s = XGetAtomName(dpy, *value);
	    copy = strdup(s);
	    XFree(s);
    }
    
    return copy;
}

Window
XChecker::get_win_prop (
        Display *dpy, 
        Window   w, 
        Atom     atom)
{ 
    Atom type;
    int format, rc;
    unsigned long items;
    unsigned long left;
    Window *value;

    rc = XGetWindowProperty (dpy, w, atom, 0, 1, False, XA_WINDOW, &type, 
            &format, &items, &left, (unsigned char**)&value);

    if (type == None || rc != Success)
        return 0;

    return *value;
}

unsigned long 
XChecker::get_card_prop (
        Display *dpy, 
        Window w, 
        Atom atom)
{ 
    Atom type;
    int format, rc;
    unsigned long items;
    unsigned long left;
    unsigned long *value;

    rc = XGetWindowProperty (
            dpy, w, atom, 0, 1, False,
            XA_CARDINAL, &type, &format,
            &items, &left, (unsigned char**)&value);
    
    if (type == None || rc != Success)
        return 0;

    return *value;
}

long 
XChecker::get_int_prop (
        Display *dpy, 
        Window w, 
        Atom atom)
{ 
          Atom type;
          int format, rc;
          unsigned long items;
          unsigned long left;
          unsigned long *value;

          rc = XGetWindowProperty (dpy, w, atom, 0, 1, False,
                              XA_INTEGER, &type, &format,
                              &items, &left, (unsigned char**)&value);
          if (type == None || rc != Success)
            return -1;
          else
          {
            return *value;
          }
}


char *
XChecker::get_str_prop(
        Display  *dpy, 
        Window    w, 
        Atom      atom)
{ 
    Atom type;
    int format, rc;
    unsigned long items;
    unsigned long left;
    char *value;

    rc = XGetWindowProperty (
            dpy, w, atom, 0, 200, False, XA_STRING, &type, &format,
            &items, &left, (unsigned char**)&value);
    
    if (type == None || rc != Success) {
        return NULL;
    } else {
        char *s = strdup((const char*)value);
        XFree(value);
        return s;
    }
}

char *
XChecker::get_utf8_prop (
        Display *dpy, 
        Window w, 
        Atom atom)
{ 
          Atom type;
          int format, rc;
          unsigned long items;
          unsigned long left;
          char *value;

          rc = XGetWindowProperty (dpy, w, atom, 0, 200, False,
				   utf8_string_atom, &type, &format,
				   &items, &left, (unsigned char**)&value);
          if (type == None || rc != Success)
            return NULL;
          else
          {
            char *s = strdup((const char*)value);
	    XFree(value);
            return s;
          }
}

const char *
XChecker::get_map_state (
        int state)
{
	switch (state) {
		case IsUnmapped:
			return "IsUnmapped";
		case IsUnviewable:
			return "IsUnviewable";
		case IsViewable:
			return "IsViewable";
		default:
			return "";
	}
}

void 
XChecker::pr (
        Window   highlighted,
        Display *dpy, 
        Window   WindowID, 
        int      level,
        int      nthWindow)
{
    QString indent, indent1;

	unsigned int n_children = 0;
	Window *child_l = NULL;
	Window root_ret, parent_ret;
	int i;
	char               *wmclass;
    char               *wmname, *wmname2;
    char               *wmtype, *wmstate;
	Window trans_for;
	char buf[100];
	XWindowAttributes attrs = { 0 };
	long hildon_stack, non_comp;

    int x_return, y_return;
    unsigned int width_return, height_return;
    unsigned int border_width_return, depth_return;
    QString      windowName;

	XQueryTree(dpy, WindowID, &root_ret, &parent_ret, &child_l, &n_children);
    XGetGeometry (dpy, WindowID, &root_ret,
              &x_return, &y_return, &width_return,
              &height_return, &border_width_return,
              &depth_return);

	for (i = 0; i < level; ++i)
		indent += "  ";
    for (i = 3; i >= level; --i)
        indent1 += "  ";

	wmclass = get_str_prop(dpy, WindowID, class_atom);
	wmname = get_utf8_prop(dpy, WindowID, name_atom);
	wmname2 = get_str_prop(dpy, WindowID, name_atom2);
	wmtype = get_atom_prop(dpy, WindowID, win_type_atom);
	wmstate = get_atom_prop(dpy, WindowID, wm_state_atom);
	trans_for = get_win_prop(dpy, WindowID, trans_atom);
	hildon_stack = get_int_prop(dpy, WindowID, hildon_stack_atom);
    non_comp = get_int_prop(dpy, WindowID, non_comp_atom);
	XGetWindowAttributes(dpy, WindowID, &attrs);

	if (trans_for)
		snprintf(buf, 100, "(transient for 0x%lx)", trans_for);
	else
		buf[0] = '\0';

    if (wmname && !wmname2)
        windowName = wmname;
    else if (!wmname && wmname2)
        windowName = wmname2;
    else if (wmname && wmname2)
        windowName = QString(wmname) + "/" + wmname2;
    else 
        windowName = "(none)";

    bool        Highlight = highlighted != None && highlighted == WindowID;
    const char *HighlightStart = Highlight ? "\033[1;31m" : "";
    const char *HighlightEnd = Highlight ? "\033[0;39m" : "";

    SYS_DEBUG ("%03d %s%s0x%06lx%s%s %-12s  %3dx%-3d  %-16s %s", 
                nthWindow,
                SYS_STR(indent), 
                HighlightStart, WindowID, HighlightEnd,
                SYS_STR(indent1),
                get_map_state(attrs.map_state),
                width_return, height_return,
			    wmclass ? wmclass : "",
                SYS_STR(windowName));
    if (n_children == 0) {
        // Nothing
	} else {
        for (i = 0; i < n_children; ++i) {
            ++nthWindow;
			pr (highlighted, dpy, child_l[i], level + 1, nthWindow);
		}
		XFree(child_l);
	}

	free(wmclass);
	free(wmname);
	free(wmtype);
	free(wmstate);
}

/*!
 * \param WindowID The current window ID for the recursion.
 * \param WMName The "_NET_WM_NAME" value for the windows to check.
 */
bool
XChecker::check_window_rec (
        Display               *dpy, 
        Window                 WindowID, 
        const QString         &WMName,
        XChecker::RequestCode  opCode)
{
    Window            rootR;
    Window            parentR;
    unsigned int      n_children = 0;
    Window           *child_l = NULL;
    char             *wmname;
	XWindowAttributes attrs = { 0 };

    wmname = get_utf8_prop (dpy, WindowID, name_atom);
    XGetWindowAttributes(dpy, WindowID, &attrs);
   
    /*
     * With this opcode we check if the window stack has at least one window
     * with the given wmname that is trully visible. It is not a perfect test
     * but we know only the wmname and of course multiple windows might have
     * the same wmname.
     */
    if (opCode == CheckIsVisible) {
        /*
         * First we check if this window has the wmname in question and it is
         * trully visible.
         */
        if (WMName == wmname && attrs.map_state == IsViewable) {
            return true;
        }

        /*
         * If not we check all the child windows...
         */
        XQueryTree (dpy, WindowID, &rootR, &parentR, &child_l, &n_children);
        for (int i = 0; i < n_children; ++i) {
			if (check_window_rec (dpy, child_l[i], WMName, opCode))
                return true;
		}
    } else if (opCode == CheckIsInvisible) {
        /*
         * This case we check all the window with the given wmname, none of
         * them should be visible.
         */

        /*
         * First the current window.
         */
        if (WMName == wmname && attrs.map_state == IsViewable)
            return false;

        /*
         * Then all the child windows.
         */
        XQueryTree (dpy, WindowID, &rootR, &parentR, &child_l, &n_children);
        for (int i = 0; i < n_children; ++i) {
			if (!check_window_rec (dpy, child_l[i], WMName, opCode))
                return false;
		}

        /*
         * If none of the windows are visible only then we return true;
         */
        return true;
    } else {
        SYS_WARNING ("opCode %d not supported.", opCode);
    }

    return false;
}

bool
XChecker::checkWindow (
        const QString         &WMName,
        XChecker::RequestCode  OpCode)
{
    Display *dpy = display();
    bool     retval = false;
    Window   root;

    root = XDefaultRootWindow (dpy);

    retval = check_window_rec (dpy, root, WMName, OpCode);
    if (!retval && OpCode == CheckIsVisible) {
        SYS_WARNING ("A window with name set to %s should be visible.",
                SYS_STR(WMName));
    } else if (!retval && OpCode == CheckIsInvisible) {
        SYS_WARNING ("None of the windows with name set to %s should be "
                "visible.", SYS_STR(WMName));
    }

    if (!retval) 
        debug_dump_windows ();

    return retval;
}


bool
XChecker::checkWindow (
        Window                 WindowID,
        XChecker::RequestCode  OpCode)
{
    Display          *Display = QX11Info::display ();
    bool              retval = false;
    XWindowAttributes attrs = { 0 };
    int               x_return, y_return;
    unsigned int      width_return, height_return;
    unsigned int      border_width_return, depth_return;
    Window            root_ret;


    SYS_DEBUG ("*** WindowID = 0x%lx", WindowID);

    if (WindowID == None) {
        switch (OpCode) {
            case CheckIsVisible:
                SYS_WARNING ("Window 0x%lx should be visible but it is 'None'.",
                        WindowID);
                retval = false;
                break;
            case CheckIsFullscreen:
                SYS_WARNING ("Window 0x%lx should be fullscreen but it is 'None'.",
                        WindowID);
                retval = false;
                break;

            case CheckIsInvisible:
                retval = true;
                break;
        }

        goto finalize;
    }

	if (!XGetWindowAttributes (Display, WindowID, &attrs)) {
        switch (OpCode) {
            case CheckIsFullscreen:
            case CheckIsVisible:
                SYS_WARNING ("Window 0x%lx does not exists.", WindowID);
                retval = false;
                break;

            case CheckIsInvisible:
                retval = true;
                break;
        }

        goto finalize;
    }

    if (!XGetGeometry (Display, WindowID, &root_ret,
              &x_return, &y_return, &width_return,
              &height_return, &border_width_return,
              &depth_return)) {
        switch (OpCode) {
            case CheckIsFullscreen:
            case CheckIsVisible:
                SYS_WARNING ("Window 0x%lx does not exists.", WindowID);
                retval = false;
                break;

            case CheckIsInvisible:
                retval = true;
                break;
        }

        goto finalize;
    }

    switch (OpCode) {
        case CheckIsVisible:
            if (attrs.map_state == IsViewable) {
                retval = true;
            } else {
                SYS_WARNING ("Window 0x%lx should be visible, but it is not.",
                        WindowID);
            }
            break;

        case CheckIsInvisible:
            if (attrs.map_state != IsViewable) {
                retval = true;
            } else {
                SYS_WARNING ("Window 0x%lx should not be visible, but it is.",
                        WindowID);
            }
            break;

        case CheckIsFullscreen:
            if (width_return == 864 && height_return == 480)
                retval = true;

            if (width_return == 480 && height_return == 864)
                retval = true;

            if (!retval) {
                SYS_WARNING ("Window 0x%lx should not be fullscreen, but it "
                        "has size %dx%d",
                        WindowID,
                        width_return, height_return);
            }
            break;
    }

finalize:
    if (!retval) 
        debug_dump_windows (WindowID);
    return retval;
}

void 
XChecker::debug_dump_windows(
        Window highlighted)
{
	Display *dpy = display();
	Window root;

    SYS_DEBUG (
" #  Window           MapState      Size     WMclass          Window name");
    SYS_DEBUG (
"-----------------------------------------------------------------------------");
	
    root = XDefaultRootWindow(dpy);
	pr (highlighted, dpy, root, 0, 0);
}


void 
XChecker::debugDumpNotifications ()
{
    QList<MNotification *> notifications;
    int                    n = 0;

    notifications = MNotification::notifications();
    foreach (MNotification *notification, notifications) {
        QString  body;
        QString  image;
        QString  summary;

        body = notification->body ();
        image = notification->image ();
        summary = notification->summary ();
        SYS_DEBUG ("************ Notification %d *****************", n);
        SYS_DEBUG ("*** summary     = %s", SYS_STR(summary));
        SYS_DEBUG ("*** body        = %s", SYS_STR(body));
        SYS_DEBUG ("*** image       = %s", SYS_STR(image));
        SYS_DEBUG ("*** isPublished = %s", 
                SYS_BOOL(notification->isPublished()));

        ++n;
    }
}


