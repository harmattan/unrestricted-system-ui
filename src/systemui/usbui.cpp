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

UsbUi::UsbUi (QObject *parent) : QObject (parent),
    m_notification (0),
    m_dialog (0),
    m_disabled (false),
    m_showdialog (false)
{
    /*
     * Initialize a little bit later...
     * we don't want to slow-down the sysuid startup
     */
#ifndef UNIT_TEST
    QTimer::singleShot (3000, this, SLOT (initialize ()));
#else
    initialize ();
#endif
}

UsbUi::~UsbUi ()
{
}

void
UsbUi::initialize ()
{
#ifdef HAVE_QMSYSTEM
    m_logic = new MeeGo::QmUSBMode (this);

    connect (m_logic, SIGNAL (modeChanged (MeeGo::QmUSBMode::Mode)),
             this, SLOT (currentModeChanged (MeeGo::QmUSBMode::Mode)));

    currentModeChanged (m_logic->getMode ());
#endif
}

void
UsbUi::setDisabled (bool disable)
{
    m_disabled = disable;

    if (disable == true)
    {
        // Hide the mode-selection dialog
        if (m_dialog && m_dialog->isVisible ())
        {
            m_showdialog = true;
            m_dialog->reject ();
            m_dialog->disappear ();
        }
    }
    else // (enable == true)
    {
        if (m_showdialog == true)
        {
            m_showdialog = false;
            ShowDialog ();
        }
    }
}

// Showing the mode selection dialog
void
UsbUi::ShowDialog ()
{
    MWidget       *centralwidget;
    MContentItem  *button;
    MLabel        *label;

    if (m_disabled == true)
    {
        // Do not show the dialog when it is disabled
        m_showdialog = true;
        return;
    }

    if (m_dialog)
    {
        m_dialog->appear (MSceneWindow::DestroyWhenDone);
        return;
    }

    m_dialog = new MDialog;

    //% "Connected to USB device"
    m_dialog->setTitle (qtTrId ("qtn_usb_connected"));
    m_dialog->setModal (false);
    m_dialog->setSystem (true);

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

    /*
     * System dialogs always create a new top level window and a scene manager
     * so no need to worry about registering to a specific scene manager here
     */
    m_dialog->appear (MSceneWindow::DestroyWhenDone);
}

void
UsbUi::OviSuiteSelected ()
{
    if (m_dialog)
    {
        m_dialog->accept ();
        m_dialog->disappear ();
    }

#ifdef HAVE_QMSYSTEM
    m_logic->setMode (MeeGo::QmUSBMode::OviSuite);
#endif
}

void
UsbUi::MassStorageSelected ()
{
    if (m_dialog)
    {
        m_dialog->accept ();
        m_dialog->disappear ();
    }

#ifdef HAVE_QMSYSTEM
    m_logic->setMode (MeeGo::QmUSBMode::MassStorage);
#endif
}

#ifdef HAVE_QMSYSTEM
void
UsbUi::currentModeChanged (MeeGo::QmUSBMode::Mode mode)
{
    switch (mode)
    {
        case MeeGo::QmUSBMode::Ask:
        case MeeGo::QmUSBMode::ModeRequest:
            ShowDialog ();
            break;
        case MeeGo::QmUSBMode::Disconnected:
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
            {
                m_dialog->reject ();
                m_dialog->disappear ();
            }

            break;
        case MeeGo::QmUSBMode::OviSuite:
        case MeeGo::QmUSBMode::MassStorage:
            ShowNotification ((int) mode);
            break;
        case MeeGo::QmUSBMode::ChargingOnly:
            // no-op
            break;
        default:
            // doing nothing, no ui interaction specified here...
            break;
    }
}
#endif

// id should be an usb_modes enum value
void
UsbUi::ShowNotification (int id)
{
    QString *mode_text;

    // no-op when disabled
    if (m_disabled)
        return;

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
        case MeeGo::QmUSBMode::OviSuite:
            //% "Ovi Suite mode"
            mode_text = new QString (qtTrId ("qtn_usb_ovi_suite"));
            break;
        case MeeGo::QmUSBMode::MassStorage:
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
    m_notification->publish ();
}

