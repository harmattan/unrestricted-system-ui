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

#include <DcpWidgetTypes>
#include "privatemodebrief.h"

PrivateModeBrief::PrivateModeBrief():
    privateSetting("/desktop/meego/privacy/private_lockscreen_notifications")
{
}

bool PrivateModeBrief::toggle() const
{
    return privateSetting.value().toBool();
}

void PrivateModeBrief::setToggle (bool toggle)
{
    privateSetting.set(toggle);
    emit valuesChanged();
}

int PrivateModeBrief::widgetTypeID() const
{
    return DcpWidgetType::Toggle;
}

