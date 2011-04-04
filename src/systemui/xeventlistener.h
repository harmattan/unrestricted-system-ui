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
#ifndef XEVENTLISTENER_H
#define XEVENTLISTENER_H

#include <QHash>
#include <QObject>
#include <QAbstractEventDispatcher>
#include <X11/Xlib.h>

/*!
 * An interface for classes registering to XEventListener.
 */
class XEventListenerFilterInterface
{
public:
    XEventListenerFilterInterface() {}
    virtual ~XEventListenerFilterInterface() {}

    virtual bool xEventFilter(const XEvent &event) = 0;
};

/*!
 * XEventListener filters the x events that client has selected with XSelectInput and calls
 * the filters for registered XEventListenerFilterInterface classes.
 *
 * Registered XEventListenerFilterInterface classes MUST always be unregistered on deletion.
 *
 * To assure correct QAbstractEventDispatcher event filter handling, the event listener is used through a
 * static QSharedPointer instance for which the instance is created on first registration and which is only
 * destroyd when application is destroyed.
 */
class XEventListener : public QObject
{
protected:
    XEventListener();

public:
    ~XEventListener();
    //! Register a filter to get x events with a given mask
    static void registerEventFilter(XEventListenerFilterInterface *filter, long eventMask);
    //! Unregister a filter from x events
    static void unregisterEventFilter(XEventListenerFilterInterface *filter);

    /*!
     * Calls the registered filters for the relevant events.
     * \param message The XEvent
     */
    static bool xEventFilter(void *message);

private:
    //! Setups the event mask table
    static void setupEventMasks();

    //! Setups event listener on first registration
    static void setupListener();

    static QHash<XEventListenerFilterInterface *, long> filterList;
    static QAbstractEventDispatcher::EventFilter previousEventFilter;

#ifdef UNIT_TEST
        friend class Ut_XEventListener;
#endif
};

#endif
