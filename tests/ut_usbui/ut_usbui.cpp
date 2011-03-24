/***************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (directui@nokia.com)
 **
 ** This file is part of system-ui.
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
#include <MApplication>
#include <MSceneWindow>
#include <MNotification>
#include <MDialog>
#include <QtTest/QtTest>
#include <usbui.h>

#include "ut_usbui.h"
#include "usbmode_stub.h"
#include "qmlocks_stub.h"

bool MNotification::publish()
{
    return true;
}

bool MNotification::isPublished() const
{
    return true;
}

bool MNotification::remove()
{
    return true;
}

int argc = 1;
char *argv[] = { (char *) "./ut_usbui", NULL };

void Ut_UsbUi::initTestCase()
{
    m_App = new MApplication(argc, argv);
}

void Ut_UsbUi::cleanupTestCase()
{
    m_App->deleteLater();
}

void Ut_UsbUi::init()
{
    m_subject = new UsbUi;
#ifdef HAVE_QMSYSTEM
    m_subject->usbMode->setMode(MeeGo::QmUSBMode::Undefined);
#endif
}

void Ut_UsbUi::cleanup()
{
    delete m_subject;
}

static bool dialog_visible;

void MDialog::appear(MSceneWindow::DeletionPolicy policy)
{
    Q_UNUSED (policy);
    dialog_visible = true;
}

void MSceneWindow::disappear()
{
    dialog_visible = false;
}

#ifdef HAVE_QMSYSTEM
void Ut_UsbUi::testConnections()
{
    QVERIFY(disconnect(m_subject->usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), m_subject, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode))));
}

void Ut_UsbUi::testShowHideDialog()
{
    QSignalSpy spy(m_subject, SIGNAL(dialogShown()));

    dialog_visible = false;

    // Set mode to ask
    m_subject->usbMode->setDefaultMode(MeeGo::QmUSBMode::Ask);

    // Emit the ask signal...
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Ask);
    QCOMPARE(dialog_visible, true);
    QVERIFY(!m_subject->dialog->isModal());
    QVERIFY(m_subject->dialog->isSystem());
    QCOMPARE(spy.count(), 1);

    // Emit the disconnect signal
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Disconnected);
    QCOMPARE(dialog_visible, false);

    // Emit the moderequest signal
    m_subject->applyUSBMode(MeeGo::QmUSBMode::ModeRequest);
    QCOMPARE(dialog_visible, true);
    QCOMPARE(spy.count(), 2);

    // Emit the disconnect signal
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Disconnected);
    QCOMPARE(dialog_visible, false);
}

void Ut_UsbUi::testUSBNotifications()
{
    // Init to some known state ...
    m_subject->applyUSBMode(MeeGo::QmUSBMode::OviSuite);

    QVERIFY(m_subject->notification != NULL);
    QCOMPARE(m_subject->notification->body(), qtTrId("qtn_usb_info_connected").arg(qtTrId("qtn_usb_ovi_suite")));
    QCOMPARE(m_subject->notification->isPublished(), true);

    m_subject->applyUSBMode(MeeGo::QmUSBMode::MassStorage);

    QVERIFY(m_subject->notification != NULL);
    QCOMPARE(m_subject->notification->body(), qtTrId("qtn_usb_info_connected").arg(qtTrId("qtn_usb_mass_storage")));
    QCOMPARE(m_subject->notification->isPublished(), true);
}

void Ut_UsbUi::testDialogButtons()
{
    // Go to connected state...
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Connected);

    // Call the Ovi Suite callback function...
    m_subject->setOviSuiteMode();
    m_subject->setRequestedUSBMode();

    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::OviSuite);

    // Call the Mass Storage callback function...
    m_subject->setMassStorageMode();
    m_subject->setRequestedUSBMode();

    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::MassStorage);
}
#endif

void Ut_UsbUi::testShowError()
{
    m_subject->showError("test");
    QVERIFY(m_subject->notification != NULL);
    QCOMPARE(m_subject->notification->body(), qtTrId("test"));
    QCOMPARE(m_subject->notification->isPublished(), true);
}

QTEST_APPLESS_MAIN (Ut_UsbUi)

