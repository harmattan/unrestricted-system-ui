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
#include <unlockwidgets.h>
#include <stubbase.h>

// 1. DECLARE STUB
class UnlockHeaderStub : public StubBase {
public:
    virtual void UnlockHeaderConstructor();
    virtual void UnlockHeaderDestructor();
    virtual void updateDateTime();
    virtual void setActive(bool active);
};

void UnlockHeaderStub::UnlockHeaderConstructor()
{
}

void UnlockHeaderStub::UnlockHeaderDestructor()
{
}

void UnlockHeaderStub::updateDateTime()
{
}

void UnlockHeaderStub::setActive(bool active)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(active));
    stubMethodEntered("setActive", params);
}

class UnlockAreaStub : public StubBase {
public:
    virtual void UnlockAreaConstructor();
    virtual void UnlockAreaDestructor();
    virtual void setEnabled (bool enabled);
    virtual void setActive(bool active);
};

void UnlockAreaStub::UnlockAreaConstructor()
{
}

void UnlockAreaStub::UnlockAreaDestructor()
{
}

void UnlockAreaStub::setEnabled(bool enabled)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(enabled));
    stubMethodEntered("setEnabled", params);
}

void UnlockAreaStub::setActive(bool active)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(active));
    stubMethodEntered("setActive", params);
}


// 3. CREATE A STUB INSTANCE
UnlockHeaderStub gDefaultUnlockHeaderStub;
UnlockHeaderStub* gUnlockHeaderStub = &gDefaultUnlockHeaderStub;

UnlockAreaStub gDefaultUnlockAreaStub;
UnlockAreaStub* gUnlockAreaStub = &gDefaultUnlockAreaStub;

UnlockHeader::UnlockHeader()
{
    gUnlockHeaderStub->UnlockHeaderConstructor();
}

UnlockHeader::~UnlockHeader()
{
    gUnlockHeaderStub->UnlockHeaderDestructor();
}

void UnlockHeader::updateDateTime()
{
    gUnlockHeaderStub->updateDateTime();
}

#ifdef HAVE_QMSYSTEM
void UnlockHeader::timeSettingsChanged (Maemo::QmTimeWhatChanged what) {
    Q_UNUSED (what);
}
#endif

void UnlockHeader::setActive (bool active)
{
    gUnlockHeaderStub->setActive(active);
}

UnlockArea::UnlockArea()
{
    gUnlockAreaStub->UnlockAreaConstructor();
}

UnlockArea::~UnlockArea()
{
    gUnlockAreaStub->UnlockAreaDestructor();
}

void UnlockArea::setEnabled (bool enabled)
{
    gUnlockAreaStub->setEnabled(enabled);
}

void UnlockArea::setActive(bool active)
{
    gUnlockAreaStub->setActive(active);
}

