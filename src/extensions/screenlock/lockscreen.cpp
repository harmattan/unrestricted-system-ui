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

LockScreen::LockScreen(QGraphicsItem *parent) :
    MWidgetController(parent),
    displayOn(true)
{
    connect(this, SIGNAL(displayExited()), this, SLOT(unlock()));

#ifdef HAVE_QMSYSTEM
    connect(&qmDisplayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(updateDisplayState(MeeGo::QmDisplayState::DisplayState)));

    // Update the display state from the current QmSystem value
    updateDisplayState(qmDisplayState.get());
#endif
}

LockScreen::~LockScreen()
{
}

#ifdef HAVE_QMSYSTEM
void LockScreen::updateDisplayState(MeeGo::QmDisplayState::DisplayState state)
{
    displayOn = state == MeeGo::QmDisplayState::On;
}
#endif

void LockScreen::unlock()
{
    if (displayOn) {
        emit unlocked();
    }
}

void LockScreen::reset()
{
    emit resetRequested();
}
