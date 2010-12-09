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
#ifndef LOCKSCREENHEADER_STUB
#define LOCKSCREENHEADER_STUB

#include "lockscreenheader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LockScreenHeaderStub : public StubBase {
  public:
  virtual void LockScreenHeaderConstructor();
  virtual void LockScreenHeaderDestructor();
}; 

// 2. IMPLEMENT STUB
void LockScreenHeaderStub::LockScreenHeaderConstructor() {
}
void LockScreenHeaderStub::LockScreenHeaderDestructor() {
}

// 3. CREATE A STUB INSTANCE
LockScreenHeaderStub gDefaultLockScreenHeaderStub;
LockScreenHeaderStub* gLockScreenHeaderStub = &gDefaultLockScreenHeaderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LockScreenHeader::LockScreenHeader(QGraphicsItem *)
{
  gLockScreenHeaderStub->LockScreenHeaderConstructor();
}

LockScreenHeader::~LockScreenHeader() {
  gLockScreenHeaderStub->LockScreenHeaderDestructor();
}

#endif
