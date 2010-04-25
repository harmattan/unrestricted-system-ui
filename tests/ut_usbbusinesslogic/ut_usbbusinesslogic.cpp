/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_usbbusinesslogic.h"
#include <usbbusinesslogic.h>
#include <usbbusinesslogicadaptor.h>

#include <MApplication>
#include <QList>

#include <usb_moded-modes.h>
#include <usb_moded-dbus.h>

#define DEBUG
#include <debug.h>

void 
Ut_UsbBusinessLogic::init()
{
}

void 
Ut_UsbBusinessLogic::cleanup()
{
}

void 
Ut_UsbBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_UsbBusinessLogic";

    m_app = new MApplication (argc, &app_name);
    m_logic = new UsbBusinessLogic;
}

void 
Ut_UsbBusinessLogic::cleanupTestCase()
{
    delete m_app;
    delete m_logic;
}

void
Ut_UsbBusinessLogic::testUsbModedRelation ()
{
    QSignalSpy spy (m_logic, SIGNAL (currentModeChanged (usb_modes)));

    // Simulate some usb_moded signals:
    m_logic->testModeChanged (QString (MODE_MASS_STORAGE));
    m_logic->testModeChanged (QString (USB_DISCONNECTED));
    m_logic->testModeChanged (QString (MODE_OVI_SUITE));
    m_logic->testModeChanged (QString (USB_CONNECTED));
    m_logic->testModeChanged (QString (DATA_IN_USE));
    m_logic->testModeChanged (QString (USB_CONNECTED_DIALOG_SHOW));

    QTest::qWait (150);

    SYS_DEBUG ("count = %d", spy.count ());
    QVERIFY (spy.count () == 6);

    // Do some checks here...
    QList<QVariant> arguments = spy.takeAt (0);
    usb_modes value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_MASS_STORAGE);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_NOTCONNECTED);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_OVI_SUITE);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_NOOP);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_DATA_IN_USE);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_AUTO);
}

void
Ut_UsbBusinessLogic::testDbusAdaptor ()
{
    m_adaptor = new UsbBusinessLogicAdaptor (this, m_logic);

    QSignalSpy spy (m_logic, SIGNAL (currentModeChanged (usb_modes)));

    m_adaptor->simulateUsbModedSignal (QString (USB_DISCONNECTED));
    m_adaptor->simulateUsbModedSignal (QString (USB_CONNECTED_DIALOG_SHOW));
    m_adaptor->simulateUsbModedSignal (QString (DATA_IN_USE));

    QTest::qWait (100);

    QVERIFY (spy.count () == 3);

    // Do some checks here...
    QList<QVariant> arguments = spy.takeAt (0);
    usb_modes value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_NOTCONNECTED);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_AUTO);

    arguments = spy.takeAt (0);
    value = arguments.at (0).value<usb_modes> ();
    QVERIFY (value == USB_DATA_IN_USE);

    delete m_adaptor;
}

QTEST_APPLESS_MAIN(Ut_UsbBusinessLogic)

