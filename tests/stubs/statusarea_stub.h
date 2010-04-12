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
#ifndef STATUSAREASTUB_H
#define STATUSAREASTUB_H

#include <stubbase.h>
#include <statusarea.h>

class StatusAreaStub : public StubBase
{
public:
    virtual void statusAreaConstructor(MWidget *parent = NULL, StatusAreaWindow *statusAreaWindow = NULL);
    virtual void showStatusIndicatorMenu();
    virtual bool sceneEvent(QEvent *event);
};

void StatusAreaStub::statusAreaConstructor(MWidget *parent, StatusAreaWindow *statusAreaWindow)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget *>(parent));
    params.append(new Parameter<StatusAreaWindow *>(statusAreaWindow));
    stubMethodEntered("statusAreaConstructor", params);
}

void StatusAreaStub::showStatusIndicatorMenu()
{
    stubMethodEntered("showStatusIndicatorMenu");
}

bool StatusAreaStub::sceneEvent(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent *>(event));
    stubMethodEntered("sceneEvent", params);
    return stubReturnValue<bool>("sceneEvent");
}

StatusAreaStub gDefaultStatusAreaStub;
StatusAreaStub *gStatusAreaStub = &gDefaultStatusAreaStub;

StatusArea::StatusArea(MWidget *parent, StatusAreaWindow *statusAreaWindow)
{
    gStatusAreaStub->statusAreaConstructor(parent, statusAreaWindow);
}

void StatusArea::showStatusIndicatorMenu()
{
    gStatusAreaStub->showStatusIndicatorMenu();
}

bool StatusArea::sceneEvent(QEvent *event)
{
    return gStatusAreaStub->sceneEvent(event);
}

#endif
