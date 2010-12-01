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
#include <cstdio>

Q_EXPORT_PLUGIN2(sysuid-screenlock, ScreenLockExtension)

ScreenLockExtension::ScreenLockExtension() : lockScreen(NULL)
{
    printf("SCREEN LOCK EXTENSION INITIALIZING\n");
}

ScreenLockExtension::~ScreenLockExtension()
{
    delete lockScreen;
}

void ScreenLockExtension::reset()
{
    if (lockScreen != NULL) {
        lockScreen->reset();
    }
}

bool ScreenLockExtension::initialize(const QString &)
{
    lockScreen = new LockScreen;

    return true;
}

QGraphicsWidget *ScreenLockExtension::widget()
{
    return lockScreen;
}

M_LIBRARY
