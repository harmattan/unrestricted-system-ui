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
#include "notifier.h"
#include "notifiernotificationsink.h"
#include "sysuid.h"
#include "mcompositornotificationsink.h"

Notifier::Notifier(MWidget *parent) : MLabel("0", parent),
 notifierSink(new NotifierNotificationSink)
{
    NotificationManager *notificationManager = &Sysuid::sysuid()->notificationManager();

    // Connect notification signals
    connect(&Sysuid::sysuid()->compositorNotificationSink(), SIGNAL(notificationAdded(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notifierSink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), notifierSink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(notificationManager, SIGNAL(groupRemoved(uint)), notifierSink, SLOT(removeGroup(uint)));
    connect(notifierSink, SIGNAL(notificationCountChanged(uint)), this, SLOT(notificationCountChanged(uint)));

    // Set the text of the label in centrally aligned mode
    setAlignment(Qt::AlignCenter);
    hide();
}


Notifier::~Notifier()
{
}

void Notifier::notificationCountChanged(uint count)
{
    if (count > 0) {
        setText(QString().sprintf("%u", count));
        show();
    } else {
        hide();
        setText(QString().sprintf("%u", count));
    }
}
