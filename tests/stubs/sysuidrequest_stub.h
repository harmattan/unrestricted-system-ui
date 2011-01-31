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
#ifndef SYSUIDREQUEST_STUB
#define SYSUIDREQUEST_STUB

#include "sysuidrequest.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class SysUidRequestStub : public StubBase {
  public:
  virtual void SysUidRequestConstructor();
  virtual ScreenLockBusinessLogic *screenLockBusinessLogic();
}; 

// 2. IMPLEMENT STUB
void SysUidRequestStub::SysUidRequestConstructor() {

}

ScreenLockBusinessLogic *SysUidRequestStub::screenLockBusinessLogic()
{
  stubMethodEntered("screenLockBusinessLogic");
  return stubReturnValue<ScreenLockBusinessLogic *>("screenLockBusinessLogic");
}



// 3. CREATE A STUB INSTANCE
SysUidRequestStub gDefaultSysUidRequestStub;
SysUidRequestStub* gSysUidRequestStub = &gDefaultSysUidRequestStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
SysUidRequest::SysUidRequest() {
  gSysUidRequestStub->SysUidRequestConstructor();
}

ScreenLockBusinessLogic *SysUidRequest::screenLockBusinessLogic()
{
  return gSysUidRequestStub->screenLockBusinessLogic();
}

#endif
