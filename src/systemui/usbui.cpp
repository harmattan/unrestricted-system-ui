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
#include <MGConfItem>
#include <QTimer>
#include <QDateTime>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

UsbUi::UsbUi(QObject *parent) : MDialog(),
#ifdef HAVE_QMSYSTEM
    usbMode(new MeeGo::QmUSBMode(this)),
    requestedUSBMode(MeeGo::QmUSBMode::Undefined),
    locks(new MeeGo::QmLocks(this)),
#endif
    developerMode(new MGConfItem("/Meego/System/DeveloperMode", this)),
    chargingLabel(new MLabel),
    massStorageItem(new MBasicListItem(MBasicListItem::SingleTitle)),
    oviSuiteItem(new MBasicListItem(MBasicListItem::SingleTitle)),
    sdkItem(NULL)
{
    setParent(parent);
    setModal(false);
    setSystem(true);
    setButtonBoxVisible(false);

    // Create dialog content buttons and put them into a central widget
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

    chargingLabel->setStyleName("CommonBodyTextInverted");
    chargingLabel->setAlignment(Qt::AlignCenter);
    layout->addItem(chargingLabel);

    massStorageItem->setStyleName("CommonBasicListItemInverted");
    connect(massStorageItem, SIGNAL(clicked()), this, SLOT(setMassStorageMode()));
    layout->addItem(massStorageItem);

    oviSuiteItem->setStyleName("CommonBasicListItemInverted");
    connect(oviSuiteItem, SIGNAL(clicked()), this, SLOT(setOviSuiteMode()));
    layout->addItem(oviSuiteItem);

    if (developerMode->value().toBool()) {
        // Developer mode is enabled, so show the SDK option
        sdkItem = new MBasicListItem(MBasicListItem::SingleTitle);
        sdkItem->setStyleName("CommonBasicListItemInverted");
        connect(sdkItem, SIGNAL(clicked()), this, SLOT(setSDKMode()));
        layout->addItem(sdkItem);
    }

    MWidget *centralWidget = new MWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(qApp, SIGNAL(localeSettingsChanged()), this, SLOT(retranslateUi()));
    retranslateUi();

#ifdef HAVE_QMSYSTEM
    connect(usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode)));
    connect(usbMode, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));

    // Lazy initialize to improve startup time
    QTimer::singleShot(2000, this, SLOT(applyCurrentUSBMode()));
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
    // System dialogs always create a new top level window and a scene manager so no need to worry about registering to a specific scene manager here
    appear(MSceneWindow::KeepWhenDone);

    // Tell interested parties that the dialog is now being shown
    emit dialogShown();
}

void UsbUi::hideDialog(bool acceptDialog)
{
    if (isVisible()) {
        if (acceptDialog) {
            accept();
        } else {
            reject();
        }
        disappear();
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

void UsbUi::setSDKMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    requestedUSBMode = MeeGo::QmUSBMode::SDK;
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
    case MeeGo::QmUSBMode::Connected:
        if (locks->getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked) {
            // When the device lock is on and USB is connected, always pretend that the USB mode selection dialog is shown to unlock the touch screen lock
            emit dialogShown();
        }
        break;
    case MeeGo::QmUSBMode::Ask:
    case MeeGo::QmUSBMode::ModeRequest:
        showDialog();
        break;
    case MeeGo::QmUSBMode::DataInUse:
        // Hide the mode selection dialog and show an error notification
        hideDialog(false);
        showNotification(Error, mode);
        break;
    case MeeGo::QmUSBMode::Disconnected:
    case MeeGo::QmUSBMode::OviSuite:
    case MeeGo::QmUSBMode::MassStorage:
    case MeeGo::QmUSBMode::SDK:
        // Hide the mode selection dialog and show a mode notification
        hideDialog(false);
        showNotification(Mode, mode);
        break;
    case MeeGo::QmUSBMode::ChargingOnly:
        // no-op
        break;
    default:
        break;
    }
}

void UsbUi::showNotification(NotificationCategory category, MeeGo::QmUSBMode::Mode mode)
{
    // Remove previous notification from the category
    hideNotification(category);

    QString body;
    switch (mode) {
    case MeeGo::QmUSBMode::DataInUse:
        //% "Cannot export the filesystem over USB since the filesystem is in use."
        body = qtTrId("qtn_usb_filessystem_inuse");
        break;
    case MeeGo::QmUSBMode::OviSuite:
        //% "Sync and connect in use"
        body = qtTrId("qtn_usb_sync_active");
        break;
    case MeeGo::QmUSBMode::MassStorage:
        //% "Mass storage in use"
        body = qtTrId("qtn_usb_storage_active");
        break;
    case MeeGo::QmUSBMode::SDK:
        // TODO: should this be localizable?
        body = "SDK mode in use";
        break;
    case MeeGo::QmUSBMode::Disconnected:
        //% "USB cable disconnected"
        body = qtTrId("qtn_usb_disconnected");
        break;
    default:
        return;
    }

    MNotification notification(MNotification::DeviceAddedEvent, "", body);
    notification.publish();
    notifications.insert(category, notification);
}
#endif

void UsbUi::hideNotification(NotificationCategory category)
{
    if (notifications.contains(category)) {
        notifications[category].remove();
        notifications.remove(category);
    }
}

void UsbUi::showError(const QString &error)
{
    // Remove previous notification
    hideNotification(Error);

    //% "USB connection error occurred"
    MNotification notification(MNotification::DeviceErrorEvent, "", qtTrId(error.toUtf8().constData()));
    notification.publish();
    notifications.insert(Error, notification);
}

void UsbUi::retranslateUi()
{
    //% "Connected to USB device"
    setTitle(qtTrId("qtn_usb_connected"));

    //% "Current state: Charging only"
    chargingLabel->setText(qtTrId("qtn_usb_charging"));
    //% "Mass Storage mode"
    massStorageItem->setTitle(qtTrId("qtn_usb_mass_storage"));
    //% "Ovi Suite mode"
    oviSuiteItem->setTitle(qtTrId("qtn_usb_ovi_suite"));

    if (sdkItem != NULL) {
        // TODO: should this be localizable?
        sdkItem->setTitle("SDK");
    }
}
