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
#include <QGraphicsLinearLayout>
#include <MViewCreator>
#include "lockscreenwithoutpadlockview.h"
#include "lockscreen.h"
#include "notificationarea.h"

LockScreenWithoutPadlockView::LockScreenWithoutPadlockView(MSceneWindow* controller) :
    LockScreenView(controller)
{
    notificationArea = new NotificationArea(controller, false);
    notificationArea->setObjectName("LockScreenNotificationArea");
    notificationArea->setHonorPrivacySetting(true);
    layout->addStretch();
    layout->addItem(notificationArea);
}

LockScreenWithoutPadlockView::~LockScreenWithoutPadlockView()
{
    delete notificationArea;
}

M_REGISTER_VIEW_NEW(LockScreenWithoutPadlockView, LockScreen)
