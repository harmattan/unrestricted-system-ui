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
#include "radioaccessstub.h"
#include <QDebug>

namespace Cellular
{

RadioAccess::RadioAccess(QObject *parent) :
        QObject(parent),
        currentMode(RadioAccess::UMTSMode),
        currentTechnology(RadioAccess::UMTS),
        flags(RadioAccess::Idle)
{
}

RadioAccess::~RadioAccess()
{
}

RadioAccess::Mode RadioAccess::mode() const
{
    return currentMode;
}

RadioAccess::Technology RadioAccess::technology() const
{
    return currentTechnology;
}

RadioAccess::Flags RadioAccess::state() const
{
    return flags;
}

void RadioAccess::setMode(RadioAccess::Mode mode)
{
    currentMode = mode;
    emit modeChanged((int)mode);
}

void RadioAccess::setTechnology(RadioAccess::Technology technology)
{
    currentTechnology = technology;
    emit technologyChanged((int)technology);
}

void RadioAccess::setState(RadioAccess::State state)
{
    flags = state;
    emit statusChanged(flags);
}

}
