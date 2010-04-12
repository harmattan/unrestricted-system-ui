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
#ifndef STATUSINDICATORMENUWINDOW_STUB_H
#define STATUSINDICATORMENUWINDOW_STUB_H

#include <stubbase.h>
#include <statusindicatormenuwindow.h>

class StatusIndicatorMenuWindowStub : public StubBase
{
public:
    virtual void StatusIndicatorMenuWindowConstructor();
    virtual void StatusIndicatorMenuWindowDestructor();
    virtual void makeVisible();
    virtual void excludeFromTaskBar();
    virtual void displayActive();
    virtual void displayInActive();
};

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowConstructor");
}

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowDestructor");
}

void StatusIndicatorMenuWindowStub::makeVisible()
{
    stubMethodEntered("makeVisible");
}

void StatusIndicatorMenuWindowStub::excludeFromTaskBar()
{
    stubMethodEntered("excludeFromTaskBar");
}

void StatusIndicatorMenuWindowStub::displayActive()
{
    stubMethodEntered("displayActive");
}

void StatusIndicatorMenuWindowStub::displayInActive()
{
    stubMethodEntered("displayInActive");
}

StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub *gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    applicationPage(0),
    escapeButtonPanel(0)
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::makeVisible()
{
    gStatusIndicatorMenuWindowStub->makeVisible();
}

void StatusIndicatorMenuWindow::excludeFromTaskBar()
{
    gStatusIndicatorMenuWindowStub->excludeFromTaskBar();
}

void StatusIndicatorMenuWindow::displayActive()
{
    gStatusIndicatorMenuWindowStub->displayActive();
}

void StatusIndicatorMenuWindow::displayInActive()
{
    gStatusIndicatorMenuWindowStub->displayInActive();
}

#endif
