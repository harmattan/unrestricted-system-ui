/***************************************************************************
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

#include "statusindicatormenubusinesslogic.h"
#include "statusindicatormenuwindow.h"
#include "closeeventeater.h"

StatusIndicatorMenuBusinessLogic::StatusIndicatorMenuBusinessLogic(QObject *parent) :
    QObject(parent),
    statusIndicatorMenuWindow(NULL)
{
}

StatusIndicatorMenuBusinessLogic::~StatusIndicatorMenuBusinessLogic()
{
    delete statusIndicatorMenuWindow;
}

void StatusIndicatorMenuBusinessLogic::showStatusIndicatorMenu()
{
    if (statusIndicatorMenuWindow == NULL) {
        statusIndicatorMenuWindow = new StatusIndicatorMenuWindow;
        statusIndicatorMenuWindow->installEventFilter(new CloseEventEater(this));
        connect(statusIndicatorMenuWindow, SIGNAL(visibilityChanged(bool)), this, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));
    }

    statusIndicatorMenuWindow->makeVisible();
}

bool StatusIndicatorMenuBusinessLogic::isStatusIndicatorMenuVisible() const
{
    return statusIndicatorMenuWindow != NULL ? statusIndicatorMenuWindow->isVisible() : false;
}
