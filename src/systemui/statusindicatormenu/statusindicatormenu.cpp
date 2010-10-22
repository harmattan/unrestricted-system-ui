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
#include <MApplicationIfProxy>
#include <QGraphicsSceneMouseEvent>
#include <MSceneManager>
#include <MScene>
#include "statusindicatormenu.h"
#include "statusindicatormenudropdownview.h"

const QString StatusIndicatorMenu::CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";


StatusIndicatorMenu::StatusIndicatorMenu(QGraphicsItem *parent) :
    MSceneWindow(parent)
{
}

StatusIndicatorMenu::~StatusIndicatorMenu()
{
}

void StatusIndicatorMenu::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension)
{
    MStatusIndicatorMenuExtensionInterface *menuExtension = static_cast<MStatusIndicatorMenuExtensionInterface *>(extension);
    menuExtension->setStatusIndicatorMenuInterface(*this);
}

void StatusIndicatorMenu::launchControlPanelAndHide()
{
    MApplicationIfProxy mApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);

    if (mApplicationIfProxy.connection().isConnected()) {
        mApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
    }

    hideStatusIndicatorMenu();
}

void StatusIndicatorMenu::showStatusIndicatorMenu()
{
    emit showRequested();
}

void StatusIndicatorMenu::hideStatusIndicatorMenu()
{
    emit hideRequested();
}

void StatusIndicatorMenu::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    /* Ignore the event if this isn't the topmost item,
       otherwise the subsequent mouse events are passed to this item */
    if (sceneManager()->scene()->itemAt(event->scenePos()) != this) {
        event->ignore();
    }
}

void StatusIndicatorMenu::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event);

    /* Close the menu as the user pressed outside the content */
    emit hideRequested();
}
