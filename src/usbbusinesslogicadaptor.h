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
#ifndef USBBUSINESSLOGICADAPTOR_H
#define USBBUSINESSLOGICADAPTOR_H

#include "usbbusinesslogic.h"

#include <QDBusAbstractAdaptor>

// Implements the DBus API interface for the USB UI (testing)
class UsbBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.usb")

public:
    UsbBusinessLogicAdaptor (
            QObject             *parent,
            UsbBusinessLogic    *usb);

public slots:
    // Possible parameter values available in the
    // usb_moded development package headers
    Q_NOREPLY void simulateUsbModedSignal (QString param);

private:
    UsbBusinessLogic   *m_usb;
};

#endif 
