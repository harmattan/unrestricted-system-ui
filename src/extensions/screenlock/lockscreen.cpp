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
#include "lockscreen.h"
#include <MApplication>
#include <MWidgetCreator>

LockScreen::LockScreen(QGraphicsItem *parent) :
    MWidgetController(parent)
{
    connect(MApplication::instance(), SIGNAL(minimized()), this, SLOT(unlock()));
}

LockScreen::~LockScreen()
{
}

void LockScreen::unlock()
{
    // isOnDisplay() will be true only if the window is shown and 
    // the display is on
    if (isOnDisplay()) {
        emit unlocked();
    }
}

void LockScreen::reset()
{
    emit resetRequested();
}

M_REGISTER_WIDGET(LockScreen)
