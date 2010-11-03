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
#include "MTester.h"

#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsWidget>
#include <QString>

#define DEBUG
#include "debug.h"
#include "debug.cpp"

bool 
MTester::isLayoutContains (
        QGraphicsLayout     *layout,
        QGraphicsLayoutItem *item)
{
    bool found = false;

    #if 0
    SYS_DEBUG ("*** layout = %p", layout);
    SYS_DEBUG ("*** item   = %p", item);
    SYS_DEBUG ("*** count  = %d", layout->count());
    #endif
    for (int n = 0; n < layout->count(); ++n) {
        QGraphicsLayoutItem *child = layout->itemAt (n);

        if (item == child)
            found = true;

        #if 0
        QGraphicsWidget* widget    = (QGraphicsWidget*) child; 
        SYS_DEBUG ("**********************************************");
        SYS_DEBUG ("*** child   = %p", child);
        SYS_DEBUG ("*** widget  = %p", child->graphicsItem ());
        SYS_DEBUG ("*** name    = %s", SYS_STR(widget->objectName()));
        #endif
    }

    if (!found) {
        QGraphicsWidget* widget = (QGraphicsWidget*) item;
        SYS_WARNING ("Layout at %p should contain widget %p/%s.",
                layout, 
                item,   
                SYS_STR(widget->objectName()));
    }

    return found;
}

bool 
MTester::objectnameIs (
            QGraphicsWidget *widget,
            const QString   &name)
{
    bool retval = widget->objectName() == name;

    if (!retval) {
        QString realName = widget->objectName();
        SYS_WARNING ("Widget at %p should have the name %s, but it is %s.",
                widget, 
                SYS_STR(name),
                realName.isEmpty() ? "(empty)" : SYS_STR(realName));
    }

    return retval;
}
