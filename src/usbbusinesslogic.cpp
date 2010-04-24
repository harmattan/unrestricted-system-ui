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

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusInterface>

#include <QString>

#define DEBUG
#define WARNING
#include "debug.h"

#define USB_GCONF_KEY "/Meego/System/UsbMode"

UsbBusinessLogic::UsbBusinessLogic (QObject *parent) :
    QObject (parent),
    m_setting (0),
    m_usb_moded (0),
    m_active (false),
    m_connected (false)
{
    m_setting = new MGConfItem (USB_GCONF_KEY);

    m_usb_moded = new QDBusInterface
                      (QString (USB_MODE_SERVICE),
                       QString (USB_MODE_OBJECT),
                       QString (USB_MODE_INTERFACE),
                       QDBusConnection::systemBus ());

    connect (m_usb_moded, SIGNAL (sig_usb_state_ind (QString)),
             this, SLOT (usb_moded_handler (QString)));
}

UsbBusinessLogic::~UsbBusinessLogic ()
{
    delete m_setting;
    m_setting = 0;

    delete m_usb_moded;
    m_usb_moded = 0;
}

// This function will not set the gconf-key value,
// that one is only modifiable by usb-applet,
// this function will only set the actual mode
void
UsbBusinessLogic::setMode (usb_modes new_mode)
{
    switch (new_mode)
    {
        case USB_OVI_SUITE:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_MASS_STORAGE);

            // TODO: check the result...
            m_active = true;
            emit Active (true);
            break;
        case USB_MASS_STORAGE:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_OVI_SUITE);

            // TODO: check the result...
            m_active = true;
            emit Active (true);
            break;
        case USB_NOOP:
        default:
            m_usb_moded->call (USB_MODE_STATE_SET, MODE_CHARGING);

            m_active = false;
            /* Do nothing, ie. Charging only ... */
            break;
    }
}

void
UsbBusinessLogic::usb_moded_handler (QString mode)
{
    SYS_DEBUG ("mode = %s", SYS_STR (mode));
}

// Returns whether the device has an active usb connection
// (aka Ovi Suite or Mass Storage mode...)
bool
UsbBusinessLogic::isActive ()
{
    return m_active;
}

// Returns true when a peripheral usb cable connected to the device
bool
UsbBusinessLogic::isConnected ()
{
    return m_connected;
}

void
UsbBusinessLogic::emitConnected (bool connected)
{
    emit Connected (connected);
}

void
UsbBusinessLogic::emitShowDialog ()
{
    emit ShowDialog ();
}

