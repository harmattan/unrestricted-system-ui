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
#include <QDBusError>
#include <QMetaType>

enum usb_modes {
    USB_OVI_SUITE = 10,
    USB_MASS_STORAGE,
    USB_NOOP,
    USB_AUTO,
    USB_NOTCONNECTED,
    USB_DATA_IN_USE
};

Q_DECLARE_METATYPE(usb_modes);

class QDBusInterface;

class UsbBusinessLogic : public QObject
{
    Q_OBJECT
    Q_ENUMS(usb_modes)

    public:
        UsbBusinessLogic (QObject *parent = 0);
        ~UsbBusinessLogic ();

        void setMode (usb_modes new_mode);
        usb_modes getCurrentMode ();

        // For UI testing only:
        void testModeChanged (QString mode);

    signals:
        void currentModeChanged (usb_modes mode);

    private slots:
        void usb_moded_handler (QString mode);
        void dbusError (QDBusError error);

    private:
        QDBusInterface  *m_usb_moded;
        usb_modes        m_currentMode;
};

#endif

