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

// To prevent swipe inside the status bar. Must end swiping atlease some distance away from status bar
const int SWIPE_THRESHOLD = 30;

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

bool StatusArea::sceneEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
        if (mouseEvent) {
            firstPos = mouseEvent->pos();
        }
        return true;
    }
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
        if (mouseEvent) {
            lastPos = mouseEvent->pos();
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        if (firstPos.y() + SWIPE_THRESHOLD < lastPos.y()) {
            showStatusIndicatorMenu();
            lastPos = QPointF();
            return true;
        }
        break;
    }
    default:
        break;
    }
    //Event not handled here, so return false
    return false;
}

void StatusArea::showStatusIndicatorMenu()
{
    statusIndicatorMenuWindow->makeVisible();
}
