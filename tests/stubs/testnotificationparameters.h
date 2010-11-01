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
#ifndef TESTNOTIFICATIONPARAMETERS
#define TESTNOTIFICATIONPARAMETERS

#include "notificationwidgetparameterfactory.h"
#include "notificationparameters.h"

class TestNotificationParameters : public NotificationParameters
{
public:
    TestNotificationParameters(QString summary = "", QString body = "", QString iconId = "", QString action = "", uint timestamp = 0);
};

TestNotificationParameters::TestNotificationParameters(QString summary, QString body, QString iconId, QString action, uint timestamp)
{
    if (!summary.isEmpty()) {
        add(NotificationWidgetParameterFactory::createSummaryParameter(summary));
    }
    if (!body.isEmpty()) {
        add(NotificationWidgetParameterFactory::createBodyParameter(body));
    }
    if (!iconId.isEmpty()) {
        add(NotificationWidgetParameterFactory::createIconIdParameter(iconId));
    }
    if (!action.isEmpty()) {
        add(NotificationWidgetParameterFactory::createActionParameter(action));
    }
    if (timestamp != 0) {
        add(NotificationParameter("timestamp", timestamp));
    }
}

#endif
