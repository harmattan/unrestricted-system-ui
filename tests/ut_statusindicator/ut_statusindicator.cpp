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

#include <MOnDisplayChangeEvent>
#include "ut_statusindicator.h"
#include "statusindicator.h"
#include "statusindicatoranimationview.h"
#include "testcontextitem.h"
#include "inputmethodstatusindicatoradaptor_stub.h"

QHash<QString, TestContextItem *> testContextItems;

// Test context
class TestContext : public ApplicationContext
{
public:
    virtual ContextItem *createContextItem(const QString &key) {
        Q_UNUSED(key)

        testContextItems[key] = new TestContextItem;

        return testContextItems[key];
    }
};

// Stubs for status indicator views

QVariant gModelValue;

TestStatusIndicatorIconView::TestStatusIndicatorIconView(StatusIndicator *controller) :
    StatusIndicatorIconView(controller)
{
}

void TestStatusIndicatorIconView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            gModelValue = model()->value();
        }
    }
}



void Ut_StatusIndicator::init()
{
    m_subject = NULL;
    testContext = new TestContext();
    testContextItems.clear();
    gModelValue.clear();
}

void Ut_StatusIndicator::cleanup()
{
    if (m_subject)
        delete m_subject;
    delete testContext;
}

void Ut_StatusIndicator::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicator";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicator::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_StatusIndicator::testModelUpdates()
{
    MOnDisplayChangeEvent exitDisplayEvent(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    MOnDisplayChangeEvent enterDisplayEvent(MOnDisplayChangeEvent::FullyOnDisplay, QRectF());
    m_subject = new StatusIndicator();

    // When the application is visible the model should be updated
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    m_subject->setValue(QVariant(true));
    QCOMPARE(m_subject->model()->value(), QVariant(1));

    // When the application is not visible the model should not be updated
    qApp->sendEvent(m_subject, &exitDisplayEvent);
    m_subject->setValue(QVariant(false));
    QCOMPARE(m_subject->model()->value(), QVariant(1)); 

    // When the application becomes visible the model should be updated
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QCOMPARE(m_subject->model()->value(), QVariant(0));
}

void Ut_StatusIndicator::testContextItemSubscribe()
{
    MOnDisplayChangeEvent exitDisplayEvent(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    MOnDisplayChangeEvent enterDisplayEvent(MOnDisplayChangeEvent::FullyOnDisplay, QRectF());

    gContextItemStub->stubReset();
    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext, NULL);

    // When the application becomes not visible, the context item updates
    // should be unsubscribed from
    qApp->sendEvent(m_subject, &exitDisplayEvent);
    QCOMPARE(gContextItemStub->stubCallCount("unsubscribe"), 2);

    // When the application becomes not visible, the context item updates
    // should be subscribed to
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QCOMPARE(gContextItemStub->stubCallCount("subscribe"), 2);
}

void Ut_StatusIndicator::testContextItemDeletion()
{
    gContextItemStub->stubReset();
    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext, NULL);

    // There should be a total of two items constructed using the
    // StatusIndicator::createContextItem() call
    QCOMPARE(gContextItemStub->stubCallCount("ContextItemConstructor"), 2);

    delete m_subject;
    m_subject = NULL;

    // There should be a total of two items deleted by the
    // StatusIndicator destructor
    QCOMPARE(gContextItemStub->stubCallCount("ContextItemDestructor"), 2);
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrength()
{
    PhoneNetworkSignalStrengthStatusIndicator* statusIndicator =
        new PhoneNetworkSignalStrengthStatusIndicator(*testContext);
    statusIndicator->setView(new TestStatusIndicatorIconView(statusIndicator));
    m_subject = statusIndicator;

    testContextItems["Cellular.SignalStrength"]->setValue(QVariant(100));

    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    QVERIFY(statusIndicator->objectName() == "");
    statusIndicator->setDisplay(true);
    QVERIFY(statusIndicator->objectName() != "");
}

void Ut_StatusIndicator::testPhoneNetworkType()
{
    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext);
    QSignalSpy spy(m_subject, SIGNAL(networkAvailabilityChanged(bool)));

    QVERIFY(m_subject->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("no-sim"));
    QVERIFY(m_subject->objectName().indexOf("NoSIM") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));
    QVERIFY(m_subject->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("forbidden"));
    QVERIFY(m_subject->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("home"));
    QVERIFY(m_subject->objectName().indexOf("NoNetwork") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("gprs"));
    QVERIFY(m_subject->objectName().indexOf("2G") >= 0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(true));
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("egprs"));
    QVERIFY(m_subject->objectName().indexOf("25G") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("umts"));
    QVERIFY(m_subject->objectName().indexOf("3G") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("hspa"));
    QVERIFY(m_subject->objectName().indexOf("35G") >= 0);
    spy.clear();
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(false));
}

void Ut_StatusIndicator::testBattery()
{
    m_subject = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.ChargePercentage"]->setValue(QVariant(100));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 100);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("Level") >= 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("Charging") >= 0);
}

void Ut_StatusIndicator::testAlarm()
{
    m_subject = new AlarmStatusIndicator(*testContext);

    testContextItems["Alarm.Present"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("Set") < 0);

    testContextItems["Alarm.Present"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("Set") >= 0);
}

void Ut_StatusIndicator::testBluetooth()
{
    m_subject = new BluetoothStatusIndicator(*testContext);

    // !enabled && !connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("On") < 0);
    QVERIFY(m_subject->objectName().indexOf("Active") < 0);

    // enabled && !connected == BluetoothStatusIndicatorOn
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("On") >= 0);
    QVERIFY(m_subject->objectName().indexOf("Active") < 0);

    // enabled && connected == BluetoothStatusIndicatorActice
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("On") < 0);
    QVERIFY(m_subject->objectName().indexOf("Active") >= 0);

    // !enabled && connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("On") < 0);
    QVERIFY(m_subject->objectName().indexOf("Active") < 0);
}

void Ut_StatusIndicator::testPresence()
{
    m_subject = new PresenceStatusIndicator(*testContext);

    // Offline mode by default
    QVERIFY(m_subject->objectName().indexOf("Busy") < 0);
    QVERIFY(m_subject->objectName().indexOf("Available") < 0);
    QVERIFY(m_subject->objectName().indexOf("Away") < 0);

    // Busy
    testContextItems["Presence.State"]->setValue(QVariant("busy"));
    QVERIFY(m_subject->objectName().indexOf("Busy") >= 0);
    QVERIFY(m_subject->objectName().indexOf("Available") < 0);
    QVERIFY(m_subject->objectName().indexOf("Away") < 0);

    // Available
    testContextItems["Presence.State"]->setValue(QVariant("available"));
    QVERIFY(m_subject->objectName().indexOf("Busy") < 0);
    QVERIFY(m_subject->objectName().indexOf("Available") >= 0);
    QVERIFY(m_subject->objectName().indexOf("Away") < 0);

    // Away
    testContextItems["Presence.State"]->setValue(QVariant("away"));
    QVERIFY(m_subject->objectName().indexOf("Busy") < 0);
    QVERIFY(m_subject->objectName().indexOf("Available") < 0);
    QVERIFY(m_subject->objectName().indexOf("Away") >= 0);

    // Offline (explicitly)
    testContextItems["Presence.State"]->setValue(QVariant("offline"));
    QVERIFY(m_subject->objectName().indexOf("Busy") < 0);
    QVERIFY(m_subject->objectName().indexOf("Available") < 0);
    QVERIFY(m_subject->objectName().indexOf("Away") < 0);
}

void Ut_StatusIndicator::testInternetConnection()
{
    m_subject = new InternetConnectionStatusIndicator(*testContext);

    QVERIFY(m_subject->objectName().indexOf("WLAN") < 0);
    QVERIFY(m_subject->objectName().indexOf("PacketData") < 0);
    QVERIFY(m_subject->objectName().indexOf("Connecting") < 0);
    QVERIFY(m_subject->objectName().indexOf("Active") < 0);

    testContextItems["Internet.NetworkType"]->setValue(QVariant("WLAN"));
    testContextItems["Internet.NetworkState"]->setValue(QVariant("connecting"));
    QVERIFY(m_subject->objectName().indexOf("WLANConnecting") >= 0);

    testContextItems["Internet.NetworkType"]->setValue(QVariant("GPRS"));
    QVERIFY(m_subject->objectName().indexOf("PacketDataConnecting") >= 0);

    testContextItems["Internet.NetworkState"]->setValue(QVariant("connected"));
    QVERIFY(m_subject->objectName().indexOf("PacketData") >= 0);

    testContextItems["Internet.TrafficIn"]->setValue(QVariant(1));
    QVERIFY(m_subject->objectName().indexOf("PacketDataActive") >= 0);

    testContextItems["Internet.TrafficOut"]->setValue(QVariant(1));
    QVERIFY(m_subject->objectName().indexOf("PacketDataActive") >= 0);

    testContextItems["Internet.TrafficIn"]->setValue(QVariant(0));
    QVERIFY(m_subject->objectName().indexOf("PacketDataActive") >= 0);

    testContextItems["Internet.NetworkType"]->setValue(QVariant("WLAN"));
    QVERIFY(m_subject->objectName().indexOf("WLAN") >= 0);

    testContextItems["Internet.NetworkState"]->setValue(QVariant("disconnected"));
    QVERIFY(m_subject->objectName().indexOf("WLAN") < 0);
    QVERIFY(m_subject->objectName().indexOf("PacketData") < 0);
    QVERIFY(m_subject->objectName().indexOf("Connecting") < 0);
    QVERIFY(m_subject->objectName().indexOf("Active") < 0);
}

void Ut_StatusIndicator::testAnimation()
{
    MOnDisplayChangeEvent exitDisplayEvent(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    MOnDisplayChangeEvent enterDisplayEvent(MOnDisplayChangeEvent::FullyOnDisplay, QRectF());
    StatusIndicator *m_subject = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    qApp->sendEvent(m_subject, &exitDisplayEvent);
    QCOMPARE(m_subject->model()->animate(), false);

    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QCOMPARE(m_subject->model()->animate(), true);
    delete m_subject;
}

void Ut_StatusIndicator::testPhoneNetwork()
{
    StatusIndicator *m_subject = new PhoneNetworkStatusIndicator(*testContext);

    testContextItems["Cellular.NetworkName"]->setValue(QVariant("foobarbarabush"));
    QVERIFY(m_subject->model()->value().type() == QVariant::String);
    QCOMPARE(m_subject->model()->value(), QVariant("foobarbarabus"));
}

void Ut_StatusIndicator::testInputMethod()
{
    InputMethodStatusIndicator *statusIndicator = new InputMethodStatusIndicator;
    m_subject = statusIndicator;

    statusIndicator->setIconID("test");
    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("test"));
}

void Ut_StatusIndicator::testCall()
{
    m_subject = new CallStatusIndicator(*testContext);

    testContextItems["Phone.Call"]->setValue(QVariant("inactive"));
    QVERIFY(m_subject->model()->value().type() == QVariant::Int);
    QCOMPARE(m_subject->model()->value(), QVariant(0));
    QVERIFY(m_subject->objectName().indexOf("Ringing") < 0);
    QVERIFY(m_subject->objectName().indexOf("Ongoing") < 0);

    testContextItems["Phone.Call"]->setValue(QVariant("ringing"));
    QVERIFY(m_subject->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("active"));
    QVERIFY(m_subject->objectName().indexOf("Ongoing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("knocking"));
    QVERIFY(m_subject->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("inactive"));
    testContextItems["Phone.Muted"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("Ringing") < 0);
    QVERIFY(m_subject->objectName().indexOf("Ongoing") < 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("ringing"));
    QVERIFY(m_subject->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("active"));
    QVERIFY(m_subject->objectName().indexOf("Ongoing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(1));

    testContextItems["Phone.Call"]->setValue(QVariant("knocking"));
    QVERIFY(m_subject->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(m_subject->model()->value(), QVariant(0));
}

void Ut_StatusIndicator::testProfile()
{
    m_subject = new ProfileStatusIndicator(*testContext);

    testContextItems["Profile.Name"]->setValue(QVariant("silent"));
    QVERIFY(m_subject->objectName().indexOf("Silent") >= 0);

    testContextItems["Profile.Name"]->setValue(QVariant("default"));
    QVERIFY(m_subject->objectName().indexOf("Silent") < 0);
}

void Ut_StatusIndicator::testGPS()
{
    m_subject = new GPSStatusIndicator(*testContext);

    testContextItems["Location.SatPositioningState"]->setValue(QVariant("on"));
    QVERIFY(m_subject->objectName().indexOf("On") >= 0);

    testContextItems["Location.SatPositioningState"]->setValue(QVariant("search"));
    QVERIFY(m_subject->objectName().indexOf("Search") >= 0);
    QVERIFY(m_subject->objectName().indexOf("On") < 0);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicator)
