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
#include "x11wrapper.h"

EventEater::EventEater()
{
    setWindowTitle("EventEater");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);
    setObjectName("EventEater");
    setProperty("NoMStyle", true);
}

void EventEater::mousePressEvent(QMouseEvent *)
{
    emit inputEventReceived();
}

void EventEater::mouseReleaseEvent(QMouseEvent *)
{
    emit inputEventReceived();
}

void EventEater::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // Set the stacking layer
    Display *display = QX11Info::display();
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 6;
        X11Wrapper::XChangeProperty(display, internalWinId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &layer, 1);
    }
}

