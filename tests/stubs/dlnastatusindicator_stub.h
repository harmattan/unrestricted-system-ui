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
#ifndef DLNASTATUSINDICATOR_STUB
#define DLNASTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DLNAStatusIndicatorStub : public StubBase
{
public:
    virtual void DLNAStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void DLNAStatusIndicatorDestructor();
    virtual void dlnaEnabledChanged();
};

// 2. IMPLEMENT STUB
void DLNAStatusIndicatorStub::DLNAStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void DLNAStatusIndicatorStub::DLNAStatusIndicatorDestructor()
{

}
void DLNAStatusIndicatorStub::dlnaEnabledChanged()
{
    stubMethodEntered("dlnaEnabledChanged");
}



// 3. CREATE A STUB INSTANCE
DLNAStatusIndicatorStub gDefaultDLNAStatusIndicatorStub;
DLNAStatusIndicatorStub *gDLNAStatusIndicatorStub = &gDefaultDLNAStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DLNAStatusIndicator::DLNAStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gDLNAStatusIndicatorStub->DLNAStatusIndicatorConstructor(context, parent);
}

DLNAStatusIndicator::~DLNAStatusIndicator()
{
    gDLNAStatusIndicatorStub->DLNAStatusIndicatorDestructor();
}

void DLNAStatusIndicator::dlnaEnabledChanged()
{
    gDLNAStatusIndicatorStub->dlnaEnabledChanged();
}


#endif
