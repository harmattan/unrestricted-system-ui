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
#ifndef DISKSPACENOTIFIER_STUB
#define DISKSPACENOTIFIER_STUB

#include "diskspacenotifier.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DiskSpaceNotifierStub : public StubBase {
  public:
  virtual void DiskSpaceNotifierConstructor(QObject *parent);
  virtual void DiskSpaceNotifierDestructor();
  virtual void handleDiskSpaceChange(const QString &path, int percentage);
}; 

// 2. IMPLEMENT STUB
void DiskSpaceNotifierStub::DiskSpaceNotifierConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void DiskSpaceNotifierStub::DiskSpaceNotifierDestructor() {

}
void DiskSpaceNotifierStub::handleDiskSpaceChange(const QString &path, int percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  params.append( new Parameter<int >(percentage));
  stubMethodEntered("handleDiskSpaceChange",params);
}



// 3. CREATE A STUB INSTANCE
DiskSpaceNotifierStub gDefaultDiskSpaceNotifierStub;
DiskSpaceNotifierStub* gDiskSpaceNotifierStub = &gDefaultDiskSpaceNotifierStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DiskSpaceNotifier::DiskSpaceNotifier(QObject *parent) {
  gDiskSpaceNotifierStub->DiskSpaceNotifierConstructor(parent);
}

DiskSpaceNotifier::~DiskSpaceNotifier() {
  gDiskSpaceNotifierStub->DiskSpaceNotifierDestructor();
}

void DiskSpaceNotifier::handleDiskSpaceChange(const QString &path, int percentage) {
  gDiskSpaceNotifierStub->handleDiskSpaceChange(path, percentage);
}


#endif
