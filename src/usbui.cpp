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
#include <MDialog>
#include <MLocale>
#include <QTimer>
#include <QProcess>

#undef DEBUG
#define WARNING
#include "debug.h"

#ifdef UNIT_TEST
#define INIT_TIME 0
#else
#define INIT_TIME 5000
#endif

using namespace Maemo;

UsbUi::UsbUi (QObject *parent) : QObject (parent),
    m_logic (0),
    m_locks (0),
    m_notification (0),
    m_dialog (0),
    m_showdialog (false),
    m_process (new QProcess)
{
    m_logic = new QmUSBMode (this);
    m_locks = new QmLocks (this);

    QTimer::singleShot (INIT_TIME, this, SLOT (initialize ()));
}

UsbUi::~UsbUi ()
{
    if (m_dialog)
    {
        delete m_dialog;
        m_dialog = 0;
    }

    delete m_process;
}

void
UsbUi::initialize ()
{
    connect (m_logic, SIGNAL (modeChanged (Maemo::QmUSBMode::Mode)),
             this, SLOT (currentModeChanged (Maemo::QmUSBMode::Mode)));

    currentModeChanged (m_logic->getMode ());
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
        m_dialog->appear (MSceneWindow::KeepWhenDone);
        m_dialog->setFocus ();
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
    m_dialog->appear (MSceneWindow::KeepWhenDone);
    m_dialog->setFocus ();
}

void
UsbUi::OviSuiteSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (QmUSBMode::OviSuite);

    if (m_dialog)
        m_dialog->disappear ();

    // HACK: FIXME: Remove this...
    m_process->start ("/usr/bin/sudo /usr/bin/pcsuite-enable.sh");
    SYS_WARNING ("Work-around started: /usr/bin/pcsuite-enable.sh");
}

void
UsbUi::MassStorageSelected ()
{
    SYS_DEBUG ("");

    m_logic->setMode (QmUSBMode::MassStorage);

    if (m_dialog)
        m_dialog->disappear ();
}

void
UsbUi::currentModeChanged (Maemo::QmUSBMode::Mode mode)
{
    switch (mode)
    {
        case QmUSBMode::Ask:
        case QmUSBMode::ModeRequest:
            if ((m_locks->getState (QmLocks::Device) == QmLocks::Locked) ||
                (m_locks->getState (QmLocks::TouchAndKeyboard) == QmLocks::Locked))
                // Show the dialog once the device is unlocked
                m_showdialog = true;
            else
                ShowDialog ();
            break;
        case QmUSBMode::Disconnected:
            m_showdialog = false;

            // remove the previous notification
            if (m_notification)
            {
                m_notification->remove ();
                delete m_notification;
                m_notification = 0;
            }

            // Hide the mode-selection dialog
            if (m_dialog && m_dialog->isVisible ())
                m_dialog->disappear ();

            break;
        case QmUSBMode::OviSuite:
        case QmUSBMode::MassStorage:
            m_showdialog = false;

            ShowNotification ((int) mode);
            break;
        case QmUSBMode::ChargingOnly:
            SYS_DEBUG ("mode : Charging only");
            // no-op
            break;
        default:
            m_showdialog = false;

            SYS_DEBUG ("What about mode = %d?", mode);
            // doing nothing, no ui interaction specified here...
            break;
    }
}

void
UsbUi::locksChanged (Maemo::QmLocks::Lock what, Maemo::QmLocks::State how)
{
    Q_UNUSED (what);

    if (how == QmLocks::Unlocked)
    {
        m_showdialog = false;
        // Show the mode selection dialog on idle...
        QTimer::singleShot (10, this, SLOT (ShowDialog ()));
    }
}

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
        case QmUSBMode::OviSuite:
            //% "Ovi Suite mode"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case QmUSBMode::MassStorage:
            //% "Mass Storage mode"
            mode_text = new QString (qtTrId ("qtn_usb_mass_storage"));
            break;
        default:
            // no notification should be shown...
            return;
            break;
    }

    m_notification = new MNotification (MNotification::DeviceAddedEvent,
                                        "",
                                        //% "USB connected.<br />%1"
                                        qtTrId ("qtn_usb_info_connected").arg (*mode_text));
    m_notification->publish ();
}

