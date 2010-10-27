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

#ifndef TRANSFERSTATUSINDICATOR_STUB
#define TRANSFERSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class TransferStatusIndicatorStub : public StubBase {
  public:
  virtual void TransferStatusIndicatorConstructor(QGraphicsItem *parent);
  virtual void TransferStatusIndicatorDestructor();
  virtual void transferStateChanged(const QString &state);
};

// 2. IMPLEMENT STUB
void TransferStatusIndicatorStub::TransferStatusIndicatorConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void TransferStatusIndicatorStub::TransferStatusIndicatorDestructor() {

}
void TransferStatusIndicatorStub::transferStateChanged(const QString &state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(state));
  stubMethodEntered("transferStateChanged",params);
}



// 3. CREATE A STUB INSTANCE
TransferStatusIndicatorStub gDefaultTransferStatusIndicatorStub;
TransferStatusIndicatorStub* gTransferStatusIndicatorStub = &gDefaultTransferStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
TransferStatusIndicator::TransferStatusIndicator(QGraphicsItem *parent) : connectionSessionBus(QDBusConnection::sessionBus()) {
  gTransferStatusIndicatorStub->TransferStatusIndicatorConstructor(parent);
}

TransferStatusIndicator::~TransferStatusIndicator() {
  gTransferStatusIndicatorStub->TransferStatusIndicatorDestructor();
}

void TransferStatusIndicator::transferStateChanged(const QString &state) {
  gTransferStatusIndicatorStub->transferStateChanged(state);
}


#endif
