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
#include "locksstub.h"

#include <QDebug>

namespace MeeGo
{

QmLocks::QmLocks(QObject *parent) :
    QObject(parent)
{
}

QmLocks::~QmLocks()
{
}

QmLocks::State QmLocks::getState(QmLocks::Lock what) const
{
    qDebug() << "QmLocks::getState(" << what << ")";
    return Unlocked;
}

bool QmLocks::setState(QmLocks::Lock what, QmLocks::State how)
{
    qDebug() << "QmLocks::setState(" << what << "," << how << ")";
    return false;
}

bool QmLocks::setDeviceAutolockTime(int seconds) 
{
    qDebug() << "QmLocks::setDeviceAutolockTime(" << seconds << ")";
    return false;
}

int QmLocks::getDeviceAutolockTime()
{
    qDebug() << "QmLocks::getDeviceAutolockTime()";
    return -1;
}

}
