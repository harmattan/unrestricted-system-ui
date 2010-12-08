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
#include "callplugin.h"
#include "call.h"

Q_EXPORT_PLUGIN2(call, CallPlugin)

CallPlugin::CallPlugin() : statusIndicatorMenu(0), call(0)
{
}

void CallPlugin::setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface)
{
    statusIndicatorMenu = &menuInterface;
}

MStatusIndicatorMenuInterface *CallPlugin::statusIndicatorMenuInterface() const
{
    return statusIndicatorMenu;
}

// Methods derived from MApplicationExtensionInterface
bool CallPlugin::initialize(const QString &)
{
    call = new Call(this);

    return true;
}

QGraphicsWidget *CallPlugin::widget()
{
    return call;
}

M_LIBRARY
