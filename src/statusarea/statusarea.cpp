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

#include "statusarea.h"
#include "statusareawindow.h"
#include <MApplicationIfProxy>
#include <QGraphicsSceneMouseEvent>
#include "statusindicatormenuwindow.h"
#include "statusindicatormenuadaptor.h"

const QString StatusArea::STATUS_INDICATOR_MENU_SERVICE_NAME = "com.nokia.mstatusindicatormenu";

StatusArea::StatusArea(MWidget *parent, StatusAreaWindow *statusAreaWindow) :
    MWidgetController(parent),
    statusIndicatorMenuWindow(new StatusIndicatorMenuWindow)
{
    if (statusAreaWindow != NULL) {
        connect(statusIndicatorMenuWindow.data(), SIGNAL(visibilityChanged(bool)), statusAreaWindow, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));
    }

    // Register status indicator menu object on DBus
    new StatusIndicatorMenuAdaptor(statusIndicatorMenuWindow.data());
    QDBusConnection::sessionBus().registerService("com.meego.core.MStatusIndicatorMenu");
    QDBusConnection::sessionBus().registerObject("/statusindicatormenu", statusIndicatorMenuWindow.data());
}

StatusArea::~StatusArea()
{
}

void StatusArea::showStatusIndicatorMenu()
{
    statusIndicatorMenuWindow->makeVisible();
}
