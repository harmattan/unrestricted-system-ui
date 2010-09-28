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

#include "notificationstatusindicator.h"
#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "inputmethodstatusindicatoradaptor.h"
#include "applicationcontext.h"
#include "sysuid.h"
#include "notificationmanager.h"
#include "notifiernotificationsink.h"
#include "mcompositornotificationsink.h"

NotificationStatusIndicator::NotificationStatusIndicator(NotifierNotificationSink *notifierSink, QGraphicsItem *parent) :
    StatusIndicator(parent),
    notifierSink(notifierSink)
{
    setObjectName(QString(metaObject()->className()));

    NotificationManager *notificationManager = &Sysuid::instance()->notificationManager();

    // Connect notification signals
    if (notifierSink != NULL) {
        connect(&Sysuid::instance()->compositorNotificationSink(), SIGNAL(notificationAdded(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
        connect(notificationManager, SIGNAL(notificationRemoved(uint)), notifierSink, SLOT(removeNotification(uint)));
        connect(notifierSink, SIGNAL(notifierSinkActive(bool)), this, SLOT(setActive(bool)));
        connect(notificationManager, SIGNAL(notificationRestored(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
        connect(notifierSink, SIGNAL(notifierSinkActive(bool)), notificationManager, SLOT(removeUnseenFlags(bool)));
    }
}

NotificationStatusIndicator::~NotificationStatusIndicator()
{
}

void NotificationStatusIndicator::setActive(bool active)
{
    if (active) {
        setObjectName(QString(metaObject()->className()) + "On");
    } else {
        setObjectName(QString(metaObject()->className()));
    }
}
