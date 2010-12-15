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

#include "eventeater.h"

// Event eater instance that listens input events
static EventEater *eventEaterListenerInstance = NULL;
// Previous event filter
QAbstractEventDispatcher::EventFilter  previousEventFilter = NULL;

static bool eventEaterEventFilter(void *message)
{
    bool handled = false;
    XEvent *event = static_cast<XEvent *>(message);
    handled = eventEaterListenerInstance->eventFilter(event);

    if (!handled && previousEventFilter && previousEventFilter != eventEaterEventFilter) {
        handled = previousEventFilter(message);
    }
    return handled;
}

EventEater::EventEater()
{
    if (previousEventFilter == NULL)
        previousEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventEaterEventFilter);

    Display *dpy = QX11Info::display();
    int scr = DefaultScreen(dpy);

    XSetWindowAttributes attr;
    attr.override_redirect = True;

    window = X11Wrapper::XCreateWindow(dpy, DefaultRootWindow(dpy),
                                       0, 0,
                                       DisplayWidth(dpy, scr), DisplayHeight(dpy, scr),
                                       0,
                                       CopyFromParent,
                                       InputOnly,
                                       CopyFromParent,
                                       CWOverrideRedirect,
                                       &attr);

    X11Wrapper::XSelectInput(dpy, window, ButtonPressMask|ButtonReleaseMask|KeyPressMask);
    X11Wrapper::XStoreName(dpy, window, const_cast<char*>("EventEater"));

    // Set the stacking layer
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(dpy, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 6;
        X11Wrapper::XChangeProperty(dpy, window, stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &layer, 1);
    }

    eventEaterListenerInstance = this;
}

EventEater::~EventEater()
{
    X11Wrapper::XDestroyWindow(QX11Info::display(), window);

    // If destroying effective instance, then set static instance and filter pointers to NULL and restore previous event filter
    if (this == eventEaterListenerInstance) {
        eventEaterListenerInstance = NULL;
        QAbstractEventDispatcher::instance()->setEventFilter(previousEventFilter);
        previousEventFilter = NULL;
    }
}

void EventEater::show()
{
    Display *dpy = QX11Info::display();

    X11Wrapper::XMapRaised(dpy, window);

    // Grab is released automatically at unmap
    X11Wrapper::XGrabKeyboard(dpy, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
}

void EventEater::hide()
{
    X11Wrapper::XUnmapWindow(QX11Info::display(), window);
}

bool EventEater::eventFilter(XEvent *event)
{
    bool handled = false;

    if ((event->xany.window == window) &&
        (event->type == ButtonPress || event->type == KeyPress)) {

        handled = true;
        emit inputEventReceived();
    }

    return handled;
}
