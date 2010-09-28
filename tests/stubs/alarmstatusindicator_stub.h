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
#ifndef ALARMSTATUSINDICATOR_STUB
#define ALARMSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class AlarmStatusIndicatorStub : public StubBase
{
public:
    virtual void AlarmStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void AlarmStatusIndicatorDestructor();
    virtual void alarmChanged();
};

// 2. IMPLEMENT STUB
void AlarmStatusIndicatorStub::AlarmStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void AlarmStatusIndicatorStub::AlarmStatusIndicatorDestructor()
{

}
void AlarmStatusIndicatorStub::alarmChanged()
{
    stubMethodEntered("alarmChanged");
}



// 3. CREATE A STUB INSTANCE
AlarmStatusIndicatorStub gDefaultAlarmStatusIndicatorStub;
AlarmStatusIndicatorStub *gAlarmStatusIndicatorStub = &gDefaultAlarmStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
AlarmStatusIndicator::AlarmStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gAlarmStatusIndicatorStub->AlarmStatusIndicatorConstructor(context, parent);
}

AlarmStatusIndicator::~AlarmStatusIndicator()
{
    gAlarmStatusIndicatorStub->AlarmStatusIndicatorDestructor();
}

void AlarmStatusIndicator::alarmChanged()
{
    gAlarmStatusIndicatorStub->alarmChanged();
}


#endif
