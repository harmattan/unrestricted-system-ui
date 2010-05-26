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
  virtual QString dbusService();
  virtual QString dbusPath();
  virtual void dbusError(QDBusConnection &connection, bool abortProgram);
  virtual LockScreenBusinessLogic *getLockScreenLogic();
}; 

// 2. IMPLEMENT STUB
void SysUidRequestStub::SysUidRequestConstructor() {

}
QString SysUidRequestStub::dbusService() {
  stubMethodEntered("dbusService");
  return stubReturnValue<QString>("dbusService");
}

QString SysUidRequestStub::dbusPath() {
  stubMethodEntered("dbusPath");
  return stubReturnValue<QString>("dbusPath");
}

LockScreenBusinessLogic *SysUidRequestStub::getLockScreenLogic()
{
  stubMethodEntered("getLockScreenLogic");
  return 0;
}

void SysUidRequestStub::dbusError(QDBusConnection &connection, bool abortProgram) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QDBusConnection & >(connection));
  params.append( new Parameter<bool >(abortProgram));
  stubMethodEntered("dbusError",params);
}



// 3. CREATE A STUB INSTANCE
SysUidRequestStub gDefaultSysUidRequestStub;
SysUidRequestStub* gSysUidRequestStub = &gDefaultSysUidRequestStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
SysUidRequest::SysUidRequest() {
  gSysUidRequestStub->SysUidRequestConstructor();
}

QString SysUidRequest::dbusService() {
  return gSysUidRequestStub->dbusService();
}

QString SysUidRequest::dbusPath() {
  return gSysUidRequestStub->dbusPath();
}

void SysUidRequest::dbusError(QDBusConnection &connection, bool abortProgram) {
  gSysUidRequestStub->dbusError(connection, abortProgram);
}

LockScreenBusinessLogic *SysUidRequest::getLockScreenLogic()
{
  return gSysUidRequestStub->getLockScreenLogic();
}

#endif
