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
#include <MLibrary>
#include "screenlockextension.h"
#include "lockscreen.h"

Q_EXPORT_PLUGIN2(sysuid-screenlock, ScreenLockExtension)

ScreenLockExtension *ScreenLockExtension::instance_ = NULL;

ScreenLockExtension::ScreenLockExtension() :
    lockScreen(NULL),
    notificationManager(NULL)
{
    instance_ = this;
}

ScreenLockExtension::~ScreenLockExtension()
{
    delete lockScreen;
    instance_ = NULL;
}

ScreenLockExtension *ScreenLockExtension::instance()
{
    return instance_;
}

void ScreenLockExtension::reset()
{
    if (lockScreen != NULL) {
        lockScreen->reset();
        lockScreen->update();
    }
}

void ScreenLockExtension::setNotificationManagerInterface(NotificationManagerInterface &notificationManager)
{
    ScreenLockExtension::notificationManager = &notificationManager;
}

QObject *ScreenLockExtension::qObject()
{
    return this;
}

void ScreenLockExtension::setMode(ScreenLockMode)
{
}

bool ScreenLockExtension::initialize(const QString &)
{
    lockScreen = new LockScreen;
    connect(lockScreen, SIGNAL(unlocked()), this, SIGNAL(unlocked()));
    return true;
}

QGraphicsWidget *ScreenLockExtension::widget()
{
    return lockScreen;
}

NotificationManagerInterface *ScreenLockExtension::notificationManagerInterface()
{
    return notificationManager;
}

M_LIBRARY
