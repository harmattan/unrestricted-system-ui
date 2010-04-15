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
#ifndef STATUSAREAWINDOW_STUB_H
#define STATUSAREAWINDOW_STUB_H

#include <stubbase.h>
#include <statusarea.h>
#include <statusareawindow.h>

class StatusAreaWindowStub : public StubBase
{
public:
    virtual void StatusAreaWindowConstructor();
    virtual void StatusAreaWindowDestructor();
    virtual void sceneChanged(const QList<QRectF> &region);
};

void StatusAreaWindowStub::StatusAreaWindowConstructor()
{
    stubMethodEntered("StatusAreaWindowConstructor");
}

void StatusAreaWindowStub::StatusAreaWindowDestructor()
{
    stubMethodEntered("StatusAreaWindowDestructor");
}

void StatusAreaWindowStub::sceneChanged(const QList<QRectF> &region)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<QRectF> >(region));
    stubMethodEntered("sceneChanged", params);
}


StatusAreaWindowStub gDefaultStatusAreaWindowStub;
StatusAreaWindowStub *gStatusAreaWindowStub = &gDefaultStatusAreaWindowStub;

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    MWindow(parent),
    scene(0),
    statusArea_(0)
{
    gStatusAreaWindowStub->StatusAreaWindowConstructor();
}

StatusAreaWindow::~StatusAreaWindow()
{
    gStatusAreaWindowStub->StatusAreaWindowDestructor();
}

void StatusAreaWindow::sceneChanged(const QList<QRectF> &region)
{
    return gStatusAreaWindowStub->sceneChanged(region);
}

#endif
