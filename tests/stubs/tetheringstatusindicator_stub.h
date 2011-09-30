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
#ifndef TETHERINGSTATUSINDICATOR_STUB
#define TETHERINGSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class TetheringStatusIndicatorStub : public StubBase
{
public:
    virtual void TetheringStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void TetheringStatusIndicatorDestructor();
    virtual void tetheringChanged();
};

// 2. IMPLEMENT STUB
void TetheringStatusIndicatorStub::TetheringStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);
}

void TetheringStatusIndicatorStub::TetheringStatusIndicatorDestructor()
{
}

void TetheringStatusIndicatorStub::tetheringChanged()
{
    stubMethodEntered("tetheringChanged");
}

// 3. CREATE A STUB INSTANCE
TetheringStatusIndicatorStub gDefaultTetheringStatusIndicatorStub;
TetheringStatusIndicatorStub *gTetheringStatusIndicatorStub = &gDefaultTetheringStatusIndicatorStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
TetheringStatusIndicator::TetheringStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gTetheringStatusIndicatorStub->TetheringStatusIndicatorConstructor(context, parent);
}

TetheringStatusIndicator::~TetheringStatusIndicator()
{
    gTetheringStatusIndicatorStub->TetheringStatusIndicatorDestructor();
}

void TetheringStatusIndicator::tetheringChanged()
{
    gTetheringStatusIndicatorStub->tetheringChanged();
}

#endif

