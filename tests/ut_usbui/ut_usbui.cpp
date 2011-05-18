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

static bool dialog_visible = false;

void MDialog::appear(MSceneWindow::DeletionPolicy)
{
    dialog_visible = true;
}

void MSceneWindow::disappear()
{
    dialog_visible = false;
}

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
    dialog_visible = false;
}

#ifdef HAVE_QMSYSTEM
void Ut_UsbUi::testConnections()
{
    QVERIFY(disconnect(m_subject->usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), m_subject, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode))));
}

void Ut_UsbUi::testShowDialog_data()
{
    QTest::addColumn<int>("mode");

    QTest::newRow("Ask") << (int)MeeGo::QmUSBMode::Ask;
    QTest::newRow("Mode request") << (int)MeeGo::QmUSBMode::ModeRequest;
}

void Ut_UsbUi::testShowDialog()
{
    QFETCH(int, mode);

    QSignalSpy spy(m_subject, SIGNAL(dialogShown()));
    m_subject->usbMode->setDefaultMode((MeeGo::QmUSBMode::Mode)mode);
    m_subject->applyUSBMode((MeeGo::QmUSBMode::Mode)mode);
    QCOMPARE(dialog_visible, true);
    QVERIFY(!m_subject->dialog->isModal());
    QVERIFY(m_subject->dialog->isSystem());
    QCOMPARE(spy.count(), 1);
}

void Ut_UsbUi::testHideDialog_data()
{
    QTest::addColumn<int>("mode");

    QTest::newRow("Disconnected") << (int)MeeGo::QmUSBMode::Disconnected;
    QTest::newRow("Ovi Suite") << (int)MeeGo::QmUSBMode::OviSuite;
    QTest::newRow("Mass Storage") << (int)MeeGo::QmUSBMode::MassStorage;
    QTest::newRow("SDK") << (int)MeeGo::QmUSBMode::SDK;
}

void Ut_UsbUi::testHideDialog()
{
    QFETCH(int, mode);

    m_subject->usbMode->setDefaultMode(MeeGo::QmUSBMode::Ask);
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Ask);
    m_subject->applyUSBMode((MeeGo::QmUSBMode::Mode)mode);
    QCOMPARE(dialog_visible, false);
}

void Ut_UsbUi::testUSBNotifications_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<QString>("body");

    QTest::newRow("Ovi Suite") << (int)MeeGo::QmUSBMode::OviSuite << qtTrId("qtn_usb_sync_active");
    QTest::newRow("Mass Storage") << (int)MeeGo::QmUSBMode::MassStorage << qtTrId("qtn_usb_storage_active");
    QTest::newRow("SDK") << (int)MeeGo::QmUSBMode::SDK << "SDK mode in use";
}

void Ut_UsbUi::testUSBNotifications()
{
    QFETCH(int, mode);
    QFETCH(QString, body);

    m_subject->applyUSBMode((MeeGo::QmUSBMode::Mode)mode);
    QVERIFY(m_subject->notification != NULL);
    QCOMPARE(m_subject->notification->body(), body);
    QCOMPARE(m_subject->notification->isPublished(), true);
}

void Ut_UsbUi::testDialogButtons()
{
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Connected);

    m_subject->setOviSuiteMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::OviSuite);

    m_subject->setMassStorageMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::MassStorage);

    m_subject->setSDKMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::SDK);
}

void Ut_UsbUi::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown_data()
{
    QTest::addColumn<int>("deviceLocked");
    QTest::addColumn<int>("dialogShownCount");
    QTest::newRow("Device locked") << (int)MeeGo::QmLocks::Locked << 1;
    QTest::newRow("Device not locked") << (int)MeeGo::QmLocks::Unlocked << 0;
}

void Ut_UsbUi::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown()
{
    QFETCH(int, deviceLocked);
    QFETCH(int, dialogShownCount);

    QSignalSpy spy(m_subject, SIGNAL(dialogShown()));
    gQmLocksStub->stubSetReturnValue("getState", (MeeGo::QmLocks::State)deviceLocked);
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Connected);
    QCOMPARE(spy.count(), dialogShownCount);
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
