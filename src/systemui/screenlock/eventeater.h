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
#ifndef EVENTEATER_H
#define EVENTEATER_H

#include "xeventlistener.h"
#include <QObject>
#include <QAbstractEventDispatcher>
#include <QX11Info>
#include "x11wrapper.h"

/*!
 * An EventEater window interrupts X pointer and key press events. On event received EventEater hides its window.
 */
class EventEater : public QObject, XEventListenerFilterInterface {
    Q_OBJECT
public:

    //! Creates a new EventEater instance and its window.
    EventEater();

    //! Destroys the EventEater and its window.
    virtual ~EventEater();

    /*! Show the event eater window */
    void show();

    /*! Hide the event eater window */
    void hide();

    //! Handles x input events
    bool xEventFilter(const XEvent &event);

private:

    Window window;

signals:
    //! Emitted when input event is received for the window.
    void inputEventReceived();

#ifdef UNIT_TEST
    friend class Ut_LockScreenUI;
    friend class Ut_EventEater;
    friend class Ft_LockScreenUI;
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

#endif
