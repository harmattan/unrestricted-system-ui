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
#include "usbui.h"
#include "usbbusinesslogic.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MNotification>
#include <MSceneWindow>
#include <MButton>
#include <MLabel>
#include <MDialog>
#include <MLocale>
#include <QTimer>

#define DEBUG
#define WARNING
#include "debug.h"

UsbUi::UsbUi (QObject *parent) : QObject (parent),
    m_logic (0),
    m_notification (0),
    m_dialog (0)
{
    m_logic = new UsbBusinessLogic (this);

    QObject::connect (m_logic, SIGNAL (Connected (bool)),
                      this, SLOT (UsbEvent (bool)));

    // It is for UsbBusinessLogicAdaptor:
    QObject::connect (m_logic, SIGNAL (ShowDialog ()),
                      this, SLOT (ShowDialog ()));
}

UsbUi::~UsbUi ()
{
    if (m_dialog)
    {
        delete m_dialog;
        m_dialog = 0;
    }
}

// Showing the mode selection dialog
// (when mode is usb-auto / "ask on connection")
void
UsbUi::ShowDialog ()
{
    SYS_DEBUG ("");
    MButton     *button;
    MLabel      *label;

    if (m_dialog)
    {
        m_dialog->appear (MSceneWindow::KeepWhenDone);
        m_dialog->setFocus ();
        return;
    }

    m_dialog = new MDialog;

    //% "Connected to USB device"
    m_dialog->setTitle (qtTrId ("qtn_usb_connected"));
    m_dialog->setSystemModal (true);
    m_dialog->setCloseButtonVisible (true);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout (Qt::Vertical);

    //% "Current state: Charging only"
    label = new MLabel (qtTrId ("qtn_usb_charging"));
    label->setAlignment (Qt::AlignLeft);

    layout->addItem (label);

    //% "Mass Storage mode"
    button = new MButton (qtTrId ("qtn_usb_mass_storage"));

    layout->addItem (button);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (MassStorageSelected ()));

    //% "Ovi Suite mode"
    button = new MButton (qtTrId ("qtn_usb_ovi_suite"));

    layout->addItem (button);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (OviSuiteSelected ()));

    m_dialog->setButtonBoxVisible (false);
    m_dialog->setLayout (layout);

    // Modal dialogs always create a new top level window and a scene manager
    // so no need to worry about registering to a specific scene manager here
    m_dialog->appear (MSceneWindow::KeepWhenDone);
    m_dialog->setFocus ();
}

void
UsbUi::OviSuiteSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (USB_OVI_SUITE);

    m_dialog->disappear ();

    show_notification (USB_OVI_SUITE);
}

void
UsbUi::MassStorageSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (USB_MASS_STORAGE);

    m_dialog->disappear ();

    show_notification (USB_MASS_STORAGE);
}

// Showing notification on connection/disconnection
// TODO: Drop this function
void
UsbUi::UsbEvent (bool connected)
{
    SYS_DEBUG ("");

    if (connected == false)
    {
        // remove previous one
        if (m_notification)
        {
            m_notification->remove ();
            delete m_notification;
            m_notification = 0;
        }

// FIXME: According to UI spec no need for usb disconnected message
#if 0
        m_notification = new MNotification (MNotification::DeviceRemovedEvent,
                                            "",
                                            //% "Usb disconnected"
                                            qtTrId ("qtn_usb_disconnected"));
        m_notification->publish();
#endif
        // Hide the mode-selection dialog
        if (m_dialog && m_dialog->isVisible ())
            m_dialog->disappear ();

        return;
    }

    usb_modes   config    = m_logic->getModeSetting ();
    // Activate the desired usb mode
    if (config != USB_AUTO)
    {
        m_logic->setMode (config);
    }
    else // Or show the mode-selection dialog
    {
        ShowDialog ();
        return;
    }
}

// for dbus adaptor
UsbBusinessLogic *
UsbUi::getLogic ()
{
    return m_logic;
}

// id should be an usb_modes enum value
// TODO: This should be a slot and
// logic should call this...
void
UsbUi::show_notification (int id)
{
    QString *mode_text;

    switch (id)
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite mode"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage mode"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
        case USB_NOOP:
        case USB_AUTO:
        default:
            // no notification should be shown...
            return;
            break;
    }

    // remove previous one
    if (m_notification)
    {
        m_notification->remove ();
        delete m_notification;
        m_notification = 0;
    }

    m_notification = new MNotification (MNotification::DeviceAddedEvent,
                                        "",
                                        //% "USB connected.<br />%1"
                                        qtTrId ("qtn_usb_info_connected").arg (*mode_text));
    m_notification->publish ();
}

