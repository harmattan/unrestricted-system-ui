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

NotificationStatusIndicator::NotificationStatusIndicator(MWidget *parent) :
        StatusIndicator(parent), notifierSink(new NotifierNotificationSink)
{
    setObjectName(QString(metaObject()->className()));

    NotificationManager *notificationManager = &Sysuid::instance()->notificationManager();

    // Connect notification signals
    connect(&Sysuid::instance()->compositorNotificationSink(), SIGNAL(notificationAdded(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notifierSink, SLOT(removeNotification(uint)));
    connect(notifierSink, SIGNAL(notifierSinkActive(bool)), this, SLOT(setActive(bool)));
    connect(notificationManager, SIGNAL(notificationRestored(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
    connect(notifierSink, SIGNAL(notifierSinkActive(bool)), notificationManager, SLOT(removeUnseenFlags(bool)));
}

NotificationStatusIndicator::~NotificationStatusIndicator()
{
}

void NotificationStatusIndicator::statusIndicatorMenuVisibilityChange(bool visible)
{
    // When status menu is opened remove all notifications from the sink
    if (visible) {
        notifierSink->clearSink();
    }
    // We need to enable/disable notifications from being added to notifier sink when status menu is unvisible/visible
    notifierSink->disableNotificationAdditions(visible);
}

void NotificationStatusIndicator::setActive(bool active)
{
    if (active) {
        setObjectName(QString(metaObject()->className()) + "On");
    } else {
        setObjectName(QString(metaObject()->className()));
    }
}
