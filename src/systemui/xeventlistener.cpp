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
#include "xeventlistener.h"

#include <QSharedPointer>
#include <X11/X.h>

static long maskMap[LASTEvent];

static QSharedPointer<XEventListener> eventListenerInstance;
QHash<XEventListenerFilterInterface *, long> XEventListener::filterList;
QAbstractEventDispatcher::EventFilter XEventListener::previousEventFilter = NULL;

XEventListener::XEventListener()
{
}

XEventListener::~XEventListener()
{
    if (previousEventFilter) {
        QAbstractEventDispatcher *dispatcher = QAbstractEventDispatcher::instance();
        if (dispatcher) {
            dispatcher->setEventFilter(previousEventFilter);
        }
        previousEventFilter = NULL;
    }
}

void XEventListener::setupEventMasks()
{
    maskMap[KeyPress] = KeyPressMask;
    maskMap[KeyRelease] = KeyReleaseMask;
    maskMap[ButtonPress] = ButtonPressMask;
    maskMap[ButtonRelease] = ButtonReleaseMask;
    maskMap[MotionNotify] = PointerMotionMask;
    maskMap[VisibilityNotify] = VisibilityChangeMask;
    maskMap[CreateNotify] = StructureNotifyMask|SubstructureNotifyMask;
    maskMap[DestroyNotify] = StructureNotifyMask|SubstructureNotifyMask;
    maskMap[UnmapNotify] = StructureNotifyMask|SubstructureNotifyMask;
    maskMap[MapNotify] = StructureNotifyMask|SubstructureNotifyMask;
    maskMap[PropertyNotify] = PropertyChangeMask;
    maskMap[ClientMessage] = 0xffffffff; // ClientMessages can't be masked
}

void XEventListener::setupListener()
{
    // Initialize event listener on first registration
    if (eventListenerInstance.isNull()) {
        setupEventMasks();
        previousEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(xEventFilter);
        eventListenerInstance = QSharedPointer<XEventListener>(new XEventListener);
    }
}

void XEventListener::registerEventFilter(XEventListenerFilterInterface *filter, long eventMask)
{
    setupListener();

    filterList.insert(filter, eventMask);
}

void XEventListener::unregisterEventFilter(XEventListenerFilterInterface *filter)
{
    filterList.remove(filter);
}

bool XEventListener::xEventFilter(void *message)
{
    XEvent *xevent = static_cast<XEvent*>(message);
    bool eventHandled = false;

    if (!filterList.isEmpty()) {
        QHashIterator<XEventListenerFilterInterface *, long> iterator(filterList);
        while(iterator.hasNext()) {
            iterator.next();
            if (maskMap[xevent->type] & iterator.value()) {
                if (eventHandled |= iterator.key()->xEventFilter(*xevent)) {
                    break;
                }
            }
        }
    }

    if (!eventHandled && previousEventFilter) {
        eventHandled = previousEventFilter(message);
    }

    return eventHandled;
}


