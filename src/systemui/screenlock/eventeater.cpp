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
#include <QMouseEvent>
#include <QX11Info>
#include <QAbstractEventDispatcher>
#include "x11wrapper.h"

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
    setWindowTitle("EventEater");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);
    setObjectName("EventEater");
    setProperty("NoMStyle", true);

    if (previousEventFilter == NULL)
        previousEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventEaterEventFilter);

    eventEaterListenerInstance = this;
}

EventEater::~EventEater()
{
    // If destroying effective instance, then set static instance and filter pointers to NULL and restore previous event filter
    if (this == eventEaterListenerInstance) {
        eventEaterListenerInstance = NULL;
        QAbstractEventDispatcher::instance()->setEventFilter(previousEventFilter);
        previousEventFilter = NULL;
    }
}

void EventEater::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // Set the stacking layer
    Display *display = QX11Info::display();
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 6;
        X11Wrapper::XChangeProperty(display, winId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &layer, 1);
    }
}

bool EventEater::eventFilter(XEvent *event)
{
    bool handled = false;
    if (isVisible() && event->xany.window == winId() && (event->type == ButtonPress || event->type == ButtonRelease)) {
        handled = true;
        emit inputEventReceived();
    }

    return handled;
}
