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
#ifndef STATUSINDICATORMENUADAPTOR_STUB_H
#define STATUSINDICATORMENUADAPTOR_STUB_H

#include <stubbase.h>
#include <statusindicatormenuadaptor.h>

class StatusIndicatorMenuAdaptorStub : public StubBase
{
public:
    virtual void StatusIndicatorMenuAdaptorConstructor();
    virtual void open();
};

void StatusIndicatorMenuAdaptorStub::StatusIndicatorMenuAdaptorConstructor()
{
    stubMethodEntered("StatusIndicatorMenuAdaptorConstructor");
}

void StatusIndicatorMenuAdaptorStub::open()
{
    stubMethodEntered("open");
}


StatusIndicatorMenuAdaptorStub gDefaultStatusIndicatorMenuAdaptorStub;
StatusIndicatorMenuAdaptorStub *gStatusIndicatorMenuAdaptorStub = &gDefaultStatusIndicatorMenuAdaptorStub;


StatusIndicatorMenuAdaptor::StatusIndicatorMenuAdaptor(StatusIndicatorMenuBusinessLogic *parent) :
    QDBusAbstractAdaptor(parent)
{
    gStatusIndicatorMenuAdaptorStub->StatusIndicatorMenuAdaptorConstructor();
}

void StatusIndicatorMenuAdaptor::open()
{
    gStatusIndicatorMenuAdaptorStub->open();
}

#endif
