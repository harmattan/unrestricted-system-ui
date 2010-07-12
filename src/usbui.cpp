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

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MNotification>
#include <MSceneWindow>
#include <MWidget>
#include <MContentItem>
#include <MLabel>
#include <MLocale>
#include <QTimer>

#undef DEBUG
#define WARNING
#include "debug.h"

UsbUi::UsbUi (QObject *parent) : QObject (parent),
    m_notification (0),
    m_dialog (0)
{
#ifdef HAVE_QMSYSTEM
    m_logic = new Maemo::QmUSBMode (this);
#endif

    initialize ();
}

UsbUi::~UsbUi ()
{
}

void
UsbUi::initialize ()
{
#ifdef HAVE_QMSYSTEM
    connect (m_logic, SIGNAL (modeChanged (Maemo::QmUSBMode::Mode)),
             this, SLOT (currentModeChanged (Maemo::QmUSBMode::Mode)));

    currentModeChanged (m_logic->getMode ());
#endif
}

void
UsbUi::setDisabled (bool disable)
{
    if (disable == true)
    {
        // Disconnect from logic
        m_logic->disconnect ();

        // remove the previous notification
        if (m_notification)
        {
            m_notification->remove ();
            delete m_notification;
            m_notification = 0;
        }

        // Hide the mode-selection dialog
        if (m_dialog && m_dialog->isVisible ())
        {
            m_dialog->reject ();
            m_dialog->disappear ();
        }
    }
    else // (enable == true)
    {
        // reinitialize, and connect to businesslogic
        initialize ();
    }
}

// Showing the mode selection dialog
void
UsbUi::ShowDialog ()
{
    SYS_DEBUG ("");
    MWidget       *centralwidget;
    MContentItem  *button;
    MLabel        *label;

    if (m_dialog)
    {
        m_dialog->appear (MSceneWindow::DestroyWhenDone);
        return;
    }

    m_dialog = new MDialog;

    //% "Connected to USB device"
    m_dialog->setTitle (qtTrId ("qtn_usb_connected"));
    m_dialog->setSystemModal (true);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout (Qt::Vertical);

    //% "Current state: Charging only"
    label = new MLabel (qtTrId ("qtn_usb_charging"));
    label->setAlignment (Qt::AlignLeft);

    layout->addItem (label);

    button = new MContentItem (MContentItem::SingleTextLabel);
    //% "Mass Storage mode"
    button->setTitle (qtTrId ("qtn_usb_mass_storage"));

    layout->addItem (button);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (MassStorageSelected ()));

    button = new MContentItem (MContentItem::SingleTextLabel);
    //% "Ovi Suite mode"
    button->setTitle (qtTrId ("qtn_usb_ovi_suite"));

    layout->addItem (button);

    QObject::connect (button, SIGNAL (clicked ()),
                      this, SLOT (OviSuiteSelected ()));

    centralwidget = new MWidget;
    centralwidget->setLayout (layout);

    m_dialog->setButtonBoxVisible (false);
    m_dialog->setCentralWidget (centralwidget);

    // Modal dialogs always create a new top level window and a scene manager
    // so no need to worry about registering to a specific scene manager here
    m_dialog->appear (MSceneWindow::DestroyWhenDone);
}

void
UsbUi::OviSuiteSelected ()
{
    SYS_DEBUG ("");
    if (m_dialog)
    {
        m_dialog->accept ();
        m_dialog->disappear ();
    }

#ifdef HAVE_QMSYSTEM
    m_logic->setMode (Maemo::QmUSBMode::OviSuite);
#endif
}

void
UsbUi::MassStorageSelected ()
{
    SYS_DEBUG ("");
    if (m_dialog)
    {
        m_dialog->accept ();
        m_dialog->disappear ();
    }

#ifdef HAVE_QMSYSTEM
    m_logic->setMode (Maemo::QmUSBMode::MassStorage);
#endif
}

#ifdef HAVE_QMSYSTEM
void
UsbUi::currentModeChanged (Maemo::QmUSBMode::Mode mode)
{
    switch (mode)
    {
        case Maemo::QmUSBMode::Ask:
        case Maemo::QmUSBMode::ModeRequest:
            ShowDialog ();
            break;
        case Maemo::QmUSBMode::Disconnected:
            // remove the previous notification
            if (m_notification)
            {
                m_notification->remove ();
                delete m_notification;
                m_notification = 0;
            }

            // Hide the mode-selection dialog
            if (m_dialog && m_dialog->isVisible ())
            {
                m_dialog->reject ();
                m_dialog->disappear ();
            }

            break;
        case Maemo::QmUSBMode::OviSuite:
        case Maemo::QmUSBMode::MassStorage:
            ShowNotification ((int) mode);
            break;
        case Maemo::QmUSBMode::ChargingOnly:
            SYS_DEBUG ("mode : Charging only");
            // no-op
            break;
        default:
            SYS_DEBUG ("What about mode = %d?", mode);
            // doing nothing, no ui interaction specified here...
            break;
    }
}
#endif

// id should be an usb_modes enum value
void
UsbUi::ShowNotification (int id)
{
    SYS_DEBUG ("");
    QString *mode_text;

    // remove previous one
    if (m_notification)
    {
        m_notification->remove ();
        delete m_notification;
        m_notification = 0;
    }

    switch (id)
    {
#ifdef HAVE_QMSYSTEM
        case Maemo::QmUSBMode::OviSuite:
            //% "Ovi Suite mode"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case Maemo::QmUSBMode::MassStorage:
            //% "Mass Storage mode"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
#endif
        default:
            // no notification should be shown...
            return;
            break;
    }

    //% "USB connected.<br />%1"
    m_notification =
        new MNotification (MNotification::DeviceAddedEvent, "",
                           qtTrId ("qtn_usb_info_connected").arg (*mode_text));
    m_notification->setImage ("icon-m-common-usb");
    m_notification->publish ();
}

