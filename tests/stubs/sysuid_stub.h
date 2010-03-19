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
#ifndef SYSUID_STUB
#define SYSUID_STUB

#include "sysuid.h"
#include <stubbase.h>
#include <DuiApplicationWindow>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class SysuidStub : public StubBase {
  public:
  virtual void SysuidConstructor();
  virtual void SysuidDestructor();
  virtual Sysuid * sysuid();
  virtual QString dbusService();
  virtual QString dbusPath();
  virtual NotificationManager & notificationManager();
  virtual DuiCompositorNotificationSink & compositorNotificationSink();
  virtual Dui::Orientation orientation() const;
  virtual Dui::OrientationAngle orientationAngle() const;
  virtual DuiApplicationWindow &applicationWindow();
  virtual void retranslate();
  virtual void applyUseMode();
}; 

// 2. IMPLEMENT STUB
void SysuidStub::SysuidConstructor() {

}
void SysuidStub::SysuidDestructor() {

}
Sysuid * SysuidStub::sysuid() {
  stubMethodEntered("sysuid");
  return stubReturnValue<Sysuid *>("sysuid");
}

QString SysuidStub::dbusService() {
  stubMethodEntered("dbusService");
  return stubReturnValue<QString>("dbusService");
}

QString SysuidStub::dbusPath() {
  stubMethodEntered("dbusPath");
  return stubReturnValue<QString>("dbusPath");
}

NotificationManager & SysuidStub::notificationManager() {
  stubMethodEntered("notificationManager");
  return stubReturnValue<NotificationManager &>("notificationManager");
}

DuiCompositorNotificationSink & SysuidStub::compositorNotificationSink() {
  stubMethodEntered("compositorNotificationSink");
  return stubReturnValue<DuiCompositorNotificationSink &>("compositorNotificationSink");
}

Dui::Orientation SysuidStub::orientation() const {
  stubMethodEntered("orientation");
  return stubReturnValue<Dui::Orientation>("orientation");
}

Dui::OrientationAngle SysuidStub::orientationAngle() const {
  stubMethodEntered("orientationAngle");
  return stubReturnValue<Dui::OrientationAngle>("orientationAngle");
}

DuiApplicationWindow &SysuidStub::applicationWindow() {
  static DuiApplicationWindow window;
  stubMethodEntered("applicationWindow");
  return window;
}

void SysuidStub::retranslate() {
  stubMethodEntered("retranslate");
}

void SysuidStub::applyUseMode() {
    stubMethodEntered("applyUseMode");
}


// 3. CREATE A STUB INSTANCE
SysuidStub gDefaultSysuidStub;
SysuidStub* gSysuidStub = &gDefaultSysuidStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Sysuid::Sysuid() {
  gSysuidStub->SysuidConstructor();
}

Sysuid::~Sysuid() {
  gSysuidStub->SysuidDestructor();
}

Sysuid * Sysuid::sysuid() {
  return gSysuidStub->sysuid();
}

QString Sysuid::dbusService() {
  return gSysuidStub->dbusService();
}

QString Sysuid::dbusPath() {
  return gSysuidStub->dbusPath();
}

NotificationManager & Sysuid::notificationManager() {
  return gSysuidStub->notificationManager();
}

DuiCompositorNotificationSink & Sysuid::compositorNotificationSink() {
  return gSysuidStub->compositorNotificationSink();
}

Dui::Orientation Sysuid::orientation() const {
  return gSysuidStub->orientation();
}

Dui::OrientationAngle Sysuid::orientationAngle() const {
  return gSysuidStub->orientationAngle();
}

DuiApplicationWindow &Sysuid::applicationWindow() {
  return gSysuidStub->applicationWindow();
}

void Sysuid::retranslate() {
  gSysuidStub->retranslate();
}

void Sysuid::applyUseMode() {
  gSysuidStub->applyUseMode();
}

#endif
