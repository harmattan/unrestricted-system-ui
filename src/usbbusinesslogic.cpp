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
#include "usbbusinesslogic.h"

#include <usb_moded-modes.h>
#include <usb_moded-dbus.h>

#include <QDBusConnection>
#include <QDBusInterface>

#include <QString>

#define DEBUG
#define WARNING
#include "debug.h"

UsbBusinessLogic::UsbBusinessLogic (QObject *parent) :
    QObject (parent),
    m_usb_moded (0),
    m_currentMode (USB_NOOP)
{
    m_usb_moded = new QDBusInterface
                      (QString (USB_MODE_SERVICE),
                       QString (USB_MODE_OBJECT),
                       QString (USB_MODE_INTERFACE),
                       QDBusConnection::systemBus ());

    connect (m_usb_moded, SIGNAL (sig_usb_state_ind (QString)),
             this, SLOT (usb_moded_handler (QString)));

    // Request the current state from usb_moded
    m_usb_moded->callWithCallback (QString (USB_MODE_STATE_REQUEST),
                                   QList<QVariant> (), this,
                                   SLOT (usb_moded_handler (QString)),
                                   SLOT (dbusError (QDBusError)));

    qRegisterMetaType<usb_modes> ();
}

UsbBusinessLogic::~UsbBusinessLogic ()
{
    delete m_usb_moded;
    m_usb_moded = 0;
}

// Send the state change request to usb_moded:
void
UsbBusinessLogic::setMode (usb_modes new_mode)
{
    switch (new_mode)
    {
        case USB_OVI_SUITE:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_MASS_STORAGE);
            break;
        case USB_MASS_STORAGE:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_OVI_SUITE);
            break;
        case USB_NOOP:
        default:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_CHARGING);
            break;
    }
}

usb_modes
UsbBusinessLogic::getCurrentMode ()
{
    return m_currentMode;
}

void
UsbBusinessLogic::testModeChanged (QString mode)
{
    usb_moded_handler (mode);
}

void
UsbBusinessLogic::usb_moded_handler (QString mode)
{
    SYS_DEBUG ("usb_moded mode = %s", SYS_STR (mode));

    if (mode == USB_DISCONNECTED)
        m_currentMode = USB_NOTCONNECTED;
    else if (mode == MODE_MASS_STORAGE)
        m_currentMode = USB_MASS_STORAGE;
    else if (mode == MODE_OVI_SUITE)
        m_currentMode = USB_OVI_SUITE;
    else if (mode == USB_CONNECTED_DIALOG_SHOW)
        m_currentMode = USB_AUTO;
    else if (mode == MODE_ASK)
        m_currentMode = USB_AUTO;
    else if (mode == DATA_IN_USE)
        m_currentMode = USB_DATA_IN_USE;
    else
        m_currentMode = USB_NOOP;

    emit currentModeChanged (m_currentMode);
}

void
UsbBusinessLogic::dbusError (QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name ()),
                           SYS_STR (error.message ()));
}

