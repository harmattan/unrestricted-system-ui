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

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

#define SHORT_DEBUG
#define DEBUG
#include "debug.h"
#include "debug.cpp"

XChecker::XChecker()
{
    Display *dpy = display();

    class_atom = XInternAtom(dpy, "WM_CLASS", False);
    name_atom = XInternAtom(dpy, "_NET_WM_NAME", False);
    name_atom2 = XInternAtom(dpy, "WM_NAME", False);
    utf8_string_atom = XInternAtom(dpy, "UTF8_STRING", False);

    Q_ASSERT (class_atom != None);
    Q_ASSERT (name_atom != None);
    Q_ASSERT (name_atom2 != None);
    Q_ASSERT (utf8_string_atom != None);

    m_CompositorPID = pidof ("mcompositor");
    SYS_DEBUG ("pidof mcompositor = %d", m_CompositorPID);

    m_SysuidPID = pidof ("sysuid");
    SYS_DEBUG ("pidof sysuid      = %d", m_SysuidPID);
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
            return 0;
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
        unsigned int level,
        unsigned int nthWindow)
{
    QString indent, indent1;

    unsigned int n_children = 0;
    Window *child_l = NULL;
    Window root_ret, parent_ret;
    unsigned int        i;
    char               *wmclass;
    char               *wmname, *wmname2;
    XWindowAttributes attrs;
    attrs.map_state = IsUnmapped;

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

    for (i = 4; i > level; --i)
        indent1 += "  ";

    wmclass = get_str_prop(dpy, WindowID, class_atom);
    wmname = get_utf8_prop(dpy, WindowID, name_atom);
    wmname2 = get_str_prop(dpy, WindowID, name_atom2);
    XGetWindowAttributes(dpy, WindowID, &attrs);

    if (wmname && !wmname2)
        windowName = wmname;
    else if (!wmname && wmname2)
        windowName = wmname2;
    else if (wmname && wmname2)
        windowName = QString(wmname) + "/" + wmname2;
    else
        windowName = "(none)";

    bool        Highlight = highlighted != None && highlighted == WindowID;
    const char *HighlightStart = Highlight ? TERM_RED : "";
    const char *HighlightEnd = Highlight ? TERM_NORMAL : "";

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
}

/*!
 * \param WindowID The current window ID for the recursion.
 * \param WMName The "_NET_WM_NAME" or "WM_NAME" value for the windows to check.
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
    XWindowAttributes attrs;
    attrs.map_state = IsUnmapped;

    wmname = get_utf8_prop (dpy, WindowID, name_atom);

    if (!wmname)
        wmname = get_str_prop (dpy, WindowID, name_atom2);

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
        if (WMName == wmname && attrs.map_state == IsViewable)
        {
            if (wmname != 0)
                delete[] wmname;

            return true;
        }

        if (wmname != 0)
        {
            delete[] wmname;
            wmname = 0;
        }

        /*
         * If not we check all the child windows...
         */
        XQueryTree (dpy, WindowID, &rootR, &parentR, &child_l, &n_children);
        for (unsigned int i = 0; i < n_children; ++i) {
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
        {
            if (wmname != 0)
                delete[] wmname;

            return false;
        }

        if (wmname != 0)
        {
            delete[] wmname;
            wmname = 0;
        }

        /*
         * Then all the child windows.
         */
        XQueryTree (dpy, WindowID, &rootR, &parentR, &child_l, &n_children);
        for (unsigned int i = 0; i < n_children; ++i) {
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

    if (wmname != 0)
        delete[] wmname;

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

    /*
     * Here are some stuff that we want to check always.
     */
    checkPIDs ();


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
    XWindowAttributes attrs;
    int               x_return, y_return;
    unsigned int      width_return, height_return;
    unsigned int      border_width_return, depth_return;
    Window            root_ret;

    attrs.map_state = IsUnmapped;

    /*
     * Here are some stuff that we want to check always.
     */
    checkPIDs ();

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
            if (width_return == 854 && height_return == 480)
                retval = true;

            if (width_return == 480 && height_return == 854)
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

int
XChecker::pidof (
        const char *program)
{
    char    commandLine[256];
    FILE   *pipe;
    char    line[256];
    char   *part;

    snprintf (commandLine, 256, "ps axu | grep %s | grep -v grep", program);
    pipe = popen (commandLine, "r");
    if (pipe == NULL) {
        SYS_WARNING ("popen() failed");
        return -1;
    }

    line[0] = '\0';
    if (fgets(line, 255, pipe) == NULL) {
        pclose (pipe);
        return -1;
    }

    part = line;
    while (*part != 0 && *part != ' ')
        ++part;
    while (*part != 0 && *part == ' ')
        ++part;

    pclose (pipe);


    return atoi (part);
}

bool
XChecker::checkPIDs ()
{
    bool     retval = true;
    int      pid;

    pid = pidof ("mcompositor");
    if (pid != m_CompositorPID) {
        SYS_WARNING (
" \n\n\n"
"----------------------------------------------------------------------------\n"
"                          MCompositor crash warning \n"
" WARNING: The PID of the mcompositor process has been changed. This probably\n"
" means that the mcompositor has been crashed. The old PID was %d, the new\n"
" PID is %d.\n"
" Please note that the crash of the mcompositor usually causes problems, so\n"
" the showing of certain windows will not work. If this test fails showing\n"
" a window (e.g. the lockscreenUI or the EventEater) it is most probably\n"
" because of the mcompositor crash.\n"
"----------------------------------------------------------------------------\n"
" \n",
        m_CompositorPID, pid);
        m_CompositorPID = pid;
        retval = false;
    }

    pid = pidof ("sysuid");
    if (pid != m_SysuidPID) {
        SYS_WARNING (
" \n\n\n"
"----------------------------------------------------------------------------\n"
"                             Sysuid crash warning \n"
" WARNING: The PID of the sysuid has been changed. This probably means that \n"
" the sysuid daemon has been crashed. The old PID was %d, the PID now is \n"
"----------------------------------------------------------------------------\n"
" %d \n",
        m_SysuidPID, pid);
        m_SysuidPID = pid;
        retval = false;
    }

    if (!retval) {
        SYS_DEBUG ("Content of /home/user/MyDocs/core-dumps/:");
        system ("ls -lh /home/user/MyDocs/core-dumps/");
    }

    return retval;
}

bool
XChecker::turnOffDisplay ()
{
    #if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    MeeGo::QmDisplayState  display;
    bool                   success;

    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Turning off the touch screen ******************");
    SYS_DEBUG ("***************************************************");
    success = display.set (MeeGo::QmDisplayState::Off);
    if (! success) {
        SYS_WARNING ("Turning off the display failed!");
    }

    return success;
    #else
    return true;
    #endif
}

bool
XChecker::turnOnDisplay ()
{
    #if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    MeeGo::QmDisplayState  display;
    bool                   success;

    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Turning on the screen *************************");
    SYS_DEBUG ("***************************************************");
    success = display.set (MeeGo::QmDisplayState::On);
    if (! success) {
        SYS_WARNING ("Turning on the display failed!");
    }

    return success;
    #else
    return true;
    #endif
}


