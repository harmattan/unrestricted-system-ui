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
#include "displaystatestub.h"

namespace Maemo
{

QmDisplayState::QmDisplayState(QObject *parent) :
        QObject(parent),
        state(QmDisplayState::On),
        brightness(2),
        blankTimeout(35),
        dimTimeout(30),
        blankingWhenCharging(true)
{
}

QmDisplayState::~QmDisplayState()
{
}

QmDisplayState::DisplayState QmDisplayState::get() const
{
    return state;
}

bool QmDisplayState::set(DisplayState state)
{
    this->state = state;
    emit displayStateChanged(state);
    return true;
}

int QmDisplayState::getMaxDisplayBrightnessValue()
{
    return 5;
}

int QmDisplayState::getDisplayBrightnessValue()
{
    return brightness;
}

void QmDisplayState::setDisplayBrightnessValue(int value)
{
    brightness = value;
}

int QmDisplayState::getDisplayBlankTimeout()
{
    return blankTimeout;
}

int QmDisplayState::getDisplayDimTimeout()
{
    return dimTimeout;
}

void QmDisplayState::setDisplayBlankTimeout(int value)
{
    blankTimeout = value;
}

void QmDisplayState::setDisplayDimTimeout(int value)
{
    dimTimeout = value;
}

void QmDisplayState::setBlankingWhenCharging(bool value)
{
    blankingWhenCharging = value;
}

bool QmDisplayState::getBlankingWhenCharging()
{
    return blankingWhenCharging;
}

}
