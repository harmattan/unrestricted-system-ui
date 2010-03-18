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
    virtual void excludeFromTaskBar();
};

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowConstructor");
}

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowDestructor");
}

void StatusIndicatorMenuWindowStub::excludeFromTaskBar()
{
    stubMethodEntered("excludeFromTaskBar");
}


StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub *gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    DuiWindow(parent),
    applicationPage(0),
    escapeButtonPanel(0)
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::excludeFromTaskBar()
{
    gStatusIndicatorMenuWindowStub->excludeFromTaskBar();
}


#endif
