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
#include "ledstub.h"
#include <QDebug>

namespace Maemo
{

QmLED::QmLED(QObject *parent) : QObject(parent)
{
}

QmLED::~QmLED()
{
}

bool QmLED::activate(const QString &pattern)
{
    qDebug() << pattern;
    return true;
}

bool QmLED::deactivate(const QString &pattern)
{
    qDebug() << pattern;
    return false;
}

bool QmLED::enable()
{
    return true;
}

bool QmLED::disable()
{
    return false;
}

}
