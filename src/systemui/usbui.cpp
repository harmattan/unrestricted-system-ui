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
#include <MBasicListItem>
#include <MLabel>
#include <MLocale>
#include <QTimer>
#include <QDateTime>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

UsbUi::UsbUi(QObject *parent) : QObject(parent),
#ifdef HAVE_QMSYSTEM
    usbMode(new MeeGo::QmUSBMode(this)),
    requestedUSBMode(MeeGo::QmUSBMode::Undefined),
#endif
    notification(0),
    dialog(0)
{
#ifdef HAVE_QMSYSTEM
    connect(usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode)));
    connect(usbMode, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));

    // Lazy initialize to improve startup time
    QTimer::singleShot(5000, this, SLOT(applyCurrentUSBMode()));
#endif
}

UsbUi::~UsbUi()
{
}

#ifdef HAVE_QMSYSTEM
void UsbUi::applyCurrentUSBMode()
{
    applyUSBMode(usbMode->getMode());
}
#endif

void UsbUi::showDialog()
{
    if (dialog == NULL) {
        // Create dialog content buttons and put them into a central widget
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

        //% "Current state: Charging only"
        MLabel *label = new MLabel(qtTrId("qtn_usb_charging"));
        label->setStyleName("CommonBodyTextInverted");
        label->setAlignment(Qt::AlignCenter);
        layout->addItem(label);

        MBasicListItem *item = new MBasicListItem(MBasicListItem::SingleTitle);
        item->setStyleName("CommonBasicListItemInverted");
        //% "Mass Storage mode"
        item->setTitle(qtTrId("qtn_usb_mass_storage"));
        connect(item, SIGNAL(clicked()), this, SLOT(setMassStorageMode()));
        layout->addItem(item);

        item = new MBasicListItem(MBasicListItem::SingleTitle);
        item->setStyleName("CommonBasicListItemInverted");
        //% "Ovi Suite mode"
        item->setTitle(qtTrId("qtn_usb_ovi_suite"));
        QObject::connect(item, SIGNAL(clicked()), this, SLOT(setOviSuiteMode()));
        layout->addItem(item);

        MWidget *centralWidget = new MWidget;
        centralWidget->setLayout(layout);

        // Create the dialog
        dialog = new MDialog;
        dialog->setModal(false);
        dialog->setSystem(true);
        dialog->setButtonBoxVisible(false);
        dialog->setCentralWidget(centralWidget);
        //% "Connected to USB device"
        dialog->setTitle(qtTrId("qtn_usb_connected"));
    }

    // System dialogs always create a new top level window and a scene manager so no need to worry about registering to a specific scene manager here
    dialog->appear(MSceneWindow::DestroyWhenDone);

    // Tell interested parties that the dialog is now being shown
    emit dialogShown();
}

void UsbUi::hideDialog(bool accept)
{
    if (dialog != NULL) {
        if (accept) {
            dialog->accept();
        } else {
            dialog->reject();
        }
        dialog->disappear();
    }
}

void UsbUi::setOviSuiteMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    requestedUSBMode = MeeGo::QmUSBMode::OviSuite;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}

void UsbUi::setMassStorageMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    requestedUSBMode = MeeGo::QmUSBMode::MassStorage;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}

#ifdef HAVE_QMSYSTEM
void UsbUi::setRequestedUSBMode()
{
    if (requestedUSBMode != MeeGo::QmUSBMode::Undefined) {
        usbMode->setMode(requestedUSBMode);
        requestedUSBMode = MeeGo::QmUSBMode::Undefined;
    }
}

void UsbUi::applyUSBMode(MeeGo::QmUSBMode::Mode mode)
{
    switch (mode) {
    case MeeGo::QmUSBMode::Ask:
    case MeeGo::QmUSBMode::ModeRequest:
        showDialog();
        break;
    case MeeGo::QmUSBMode::Disconnected:
        // remove the previous notification
        hideNotification();

        // Hide the mode-selection dialog
        if (dialog && dialog->isVisible()) {
            hideDialog(false);
        }
        break;
    case MeeGo::QmUSBMode::OviSuite:
    case MeeGo::QmUSBMode::MassStorage:
        showNotification((int)mode);
        break;
    case MeeGo::QmUSBMode::ChargingOnly:
        // no-op
        break;
    default:
        break;
    }
}
#endif

void UsbUi::showNotification(int id)
{
    // Remove previous notification
    hideNotification();

    QString mode_text;
    switch (id) {
#ifdef HAVE_QMSYSTEM
    case MeeGo::QmUSBMode::OviSuite:
        //% "Ovi Suite mode"
        mode_text = qtTrId("qtn_usb_ovi_suite");
        break;
    case MeeGo::QmUSBMode::MassStorage:
        //% "Mass Storage mode"
        mode_text = qtTrId("qtn_usb_mass_storage");
        break;
#endif
    default:
        return;
    }

    //% "USB connected.<br />%1"
    notification = new MNotification(MNotification::DeviceAddedEvent, "", qtTrId("qtn_usb_info_connected").arg(mode_text));
    notification->publish();
}

void UsbUi::hideNotification()
{
    if (notification != NULL) {
        notification->remove();
        delete notification;
        notification = NULL;
    }
}

void UsbUi::showError(const QString &error)
{
    // Remove previous notification
    hideNotification();

    //% "USB connection error occurred"
    notification = new MNotification(MNotification::DeviceErrorEvent, "", qtTrId(error.toUtf8().constData()));
    notification->publish();
}
