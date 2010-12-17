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

#include <DcpWidget>
#include "privatemodeapplet.h"
#include "privatemodebrief.h"

Q_EXPORT_PLUGIN2(privatemodeapplet, PrivateModeApplet)

void PrivateModeApplet::init()
{
};

DcpWidget* PrivateModeApplet::constructWidget(int widgetId)
{
    Q_UNUSED(widgetId);
    return 0;
}

QVector<MAction*> PrivateModeApplet::viewMenuItems()
{
    QVector<MAction*> vector(0);
    return vector;
}

DcpBrief* PrivateModeApplet::constructBrief(int)
{
    return new PrivateModeBrief();
}

