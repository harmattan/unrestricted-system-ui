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
#ifndef USBBUSINESSLOGIC_H
#define USBBUSINESSLOGIC_H

#include <QObject>
#include <QString>
#include <MGConfItem>
#include <QDBusPendingCallWatcher>
#include <QDBusInterface>
#include <QDBusMessage>

typedef enum {
    USB_OVI_SUITE = 10,
    USB_MASS_STORAGE,
    USB_NOOP,
    USB_AUTO
} usb_modes;

class UsbBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (usb_modes new_mode);
        usb_modes getModeSetting ();

        bool isActive ();
        bool isConnected ();

        void emitShowDialog ();
        // Its for testing:
        void emitConnected (bool connected);

    signals:
        void ShowDialog ();
        void Active (bool active);
        void Connected (bool connected);

    private slots:
        void usb_moded_handler (QString mode);

    private:
        MGConfItem      *m_setting;
        QDBusInterface  *m_usb_moded;
        bool             m_active;
        bool             m_connected;

};

#endif

