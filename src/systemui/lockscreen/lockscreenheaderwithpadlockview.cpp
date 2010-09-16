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

#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include "lockscreenheaderwithpadlockview.h"

LockScreenHeaderWithPadlockView::LockScreenHeaderWithPadlockView(MWidgetController *controller) :
        LockScreenHeaderView(controller)
{
    padlockImageWidget = new MImageWidget;
    padlockImageWidget->setVisible (true);
    padlockImageWidget->setImage ("icon-m-common-locked", QSize (32, 32));
    padlockImageWidget->setZoomFactor (1.0);
    padlockImageWidget->setObjectName ("lockscreenIconLocked");

    layout->addItem(padlockImageWidget);
    layout->setAlignment (padlockImageWidget, Qt::AlignRight | Qt::AlignVCenter);
}

LockScreenHeaderWithPadlockView::~LockScreenHeaderWithPadlockView()
{
}

M_REGISTER_VIEW_NEW(LockScreenHeaderWithPadlockView, MWidgetController)
