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
#include <DuiContainer>
#include <DuiNotification>
#include <DuiSceneWindow>
#include <DuiButton>
#include <DuiDialog>
#include <DuiLocale>
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
    DuiButton     *button;
    DuiContainer  *hbox;

    if (m_dialog)
    {
        m_dialog->appear (DuiSceneWindow::KeepWhenDone);
        m_dialog->setFocus ();
        return;
    }

    m_dialog = new DuiDialog;

    //% "Usb connected"
    m_dialog->setTitle (qtTrId ("qtn_usb_connected_title"));
    m_dialog->setSystemModal (true);
    m_dialog->setCloseButtonVisible (false);
    //FIXME: seems dialog should hide itself from task-selector too ^^^

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;

    //% "Ovi Suite"
    button = new DuiButton (qtTrId ("qtn_usb_ovi_suite"));

    layout->addItem (button);
    layout->setStretchFactor (button, 1);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (OviSuiteSelected ()));

    //% "Mass Storage"
    button = new DuiButton (qtTrId ("qtn_usb_mass_storage"));

    layout->addItem (button);
    layout->setStretchFactor (button, 1);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (MassStorageSelected ()));

    hbox = new DuiContainer;
    hbox->setHeaderVisible (false);
    hbox->setLayout (layout);

    m_dialog->setCentralWidget (hbox);
    m_dialog->setButtonBoxVisible (false);

    // Modal dialogs always create a new top level window and a scene manager
    // so no need to worry about registering to a specific scene manager here
    m_dialog->appear (DuiSceneWindow::KeepWhenDone);
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

        //% "Usb disconnected"
        m_notification = new DuiNotification (DuiNotification::DeviceRemovedEvent, "", qtTrId ("qtn_usb_disconnected"));
        m_notification->publish();

        // Hide the mode-selection dialog
        if (m_dialog)
            m_dialog->disappear ();

        return;
    }

    usb_modes   config    = m_logic->getModeSetting ();
    // Activate the desired usb mode
    if (config != USB_AUTO)
    {
        m_logic->setMode (config);
        // TODO: result checking?
    }
    else // Or show the mode-selection dialog
    {
        ShowDialog ();
        return;
    }

    show_notification ((int) config);
}

// for dbus adaptor
UsbBusinessLogic *
UsbUi::getLogic ()
{
    return m_logic;
}

// id should be an usb_modes enum value
void
UsbUi::show_notification (int id)
{
    QString *mode_text;

    switch (id)
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
        case USB_NOOP:
            //% "Do nothing"
            mode_text = new QString (qtTrId ("qtn_usb_do_nothing"));
            break;
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

    //% "<b>Usb connected</b><br />Selected mode: <b>%1</b>"
    m_notification = new DuiNotification (DuiNotification::DeviceAddedEvent, "", qtTrId ("qtn_usb_connected_%1").arg (*mode_text));
    m_notification->publish();
}
