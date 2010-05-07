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
    testContext = new TestContext();
    testContextItems.clear();
    gModelValue.clear();
}

void Ut_StatusIndicator::cleanup()
{
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
    StatusIndicator *statusIndicator = new BluetoothStatusIndicator(*testContext);

    // When the application is visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(true));
    QCOMPARE(statusIndicator->model()->value(), QVariant(1));

    // When the application is not visible the model should not be updated
    qApp->sendEvent(statusIndicator, &exitDisplayEvent);
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    QCOMPARE(statusIndicator->model()->value(), QVariant(1));

    // When the application becomes visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    delete statusIndicator;
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrength()
{
    PhoneNetworkSignalStrengthStatusIndicator *statusIndicator = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);
    statusIndicator->setView(new TestStatusIndicatorIconView(statusIndicator));

    testContextItems["Cellular.SignalStrength"]->setValue(QVariant(100));

    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    QVERIFY(statusIndicator->objectName() == "");
    statusIndicator->setDisplay(true);
    QVERIFY(statusIndicator->objectName() != "");

    delete statusIndicator;
}

void Ut_StatusIndicator::testPhoneNetworkType()
{
    StatusIndicator *statusIndicator = new PhoneNetworkTypeStatusIndicator(*testContext);
    QSignalSpy spy(statusIndicator, SIGNAL(networkAvailabilityChanged(bool)));

    QVERIFY(statusIndicator->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("no-sim"));
    QVERIFY(statusIndicator->objectName().indexOf("NoSIM") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));
    QVERIFY(statusIndicator->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("forbidden"));
    QVERIFY(statusIndicator->objectName().indexOf("Offline") >= 0);
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("home"));
    QVERIFY(statusIndicator->objectName().indexOf("NoNetwork") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("gprs"));
    QVERIFY(statusIndicator->objectName().indexOf("2G") >= 0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(true));
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("egprs"));
    QVERIFY(statusIndicator->objectName().indexOf("25G") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("umts"));
    QVERIFY(statusIndicator->objectName().indexOf("3G") >= 0);
    testContextItems["Cellular.DataTechnology"]->setValue(QVariant("hspa"));
    QVERIFY(statusIndicator->objectName().indexOf("35G") >= 0);
    spy.clear();
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(false));
}

void Ut_StatusIndicator::testBattery()
{
    StatusIndicator *statusIndicator = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.ChargePercentage"]->setValue(QVariant(100));
    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    QVERIFY(statusIndicator->objectName().indexOf("Level") >= 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("Charging") >= 0);

    delete statusIndicator;
}

void Ut_StatusIndicator::testAlarm()
{
    StatusIndicator *statusIndicator = new ClockAlarmStatusIndicator(*testContext);

    testContextItems["UserAlarm.Present"]->setValue(QVariant(false));
    QVERIFY(statusIndicator->objectName().indexOf("NoAlarm") >= 0);

    testContextItems["UserAlarm.Present"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("NoAlarm") == -1);

    delete statusIndicator;
}

void Ut_StatusIndicator::testBluetooth()
{
    StatusIndicator *statusIndicator = new BluetoothStatusIndicator(*testContext);

    // !enabled && !connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(false));
    QVERIFY(statusIndicator->objectName().indexOf("On") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Active") < 0);

    // enabled && !connected == BluetoothStatusIndicatorOn
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("On") >= 0);
    QVERIFY(statusIndicator->objectName().indexOf("Active") < 0);

    // enabled && connected == BluetoothStatusIndicatorActice
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("On") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Active") >= 0);

    // !enabled && connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    QVERIFY(statusIndicator->objectName().indexOf("On") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Active") < 0);

    delete statusIndicator;
}

void Ut_StatusIndicator::testPresence()
{
    StatusIndicator *statusIndicator = new PresenceStatusIndicator(*testContext);

    // Offline mode by default
    QVERIFY(statusIndicator->objectName().indexOf("Busy") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Available") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Away") < 0);
    // Busy
    testContextItems["Presence.State"]->setValue(QVariant("busy"));
    QVERIFY(statusIndicator->objectName().indexOf("Busy") >= 0);
    QVERIFY(statusIndicator->objectName().indexOf("Available") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Away") < 0);
    // Available
    testContextItems["Presence.State"]->setValue(QVariant("available"));
    QVERIFY(statusIndicator->objectName().indexOf("Busy") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Available") >= 0);
    QVERIFY(statusIndicator->objectName().indexOf("Away") < 0);
    // Away
    testContextItems["Presence.State"]->setValue(QVariant("away"));
    QVERIFY(statusIndicator->objectName().indexOf("Busy") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Available") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Away") >= 0);
    // Offline (explicitly)
    testContextItems["Presence.State"]->setValue(QVariant("offline"));
    QVERIFY(statusIndicator->objectName().indexOf("Busy") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Available") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Away") < 0);

    delete statusIndicator;
}

void Ut_StatusIndicator::testInternetConnection()
{
    StatusIndicator *statusIndicator = new InternetConnectionStatusIndicator(*testContext);

    testContextItems["Internet.SignalStrength"]->setValue(QVariant(100));
    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    delete statusIndicator;
}

void Ut_StatusIndicator::testAnimation()
{
    MOnDisplayChangeEvent exitDisplayEvent(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    MOnDisplayChangeEvent enterDisplayEvent(MOnDisplayChangeEvent::FullyOnDisplay, QRectF());
    StatusIndicator *statusIndicator = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    qApp->sendEvent(statusIndicator, &exitDisplayEvent);
    QCOMPARE(statusIndicator->model()->animate(), false);

    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    QCOMPARE(statusIndicator->model()->animate(), true);
    delete statusIndicator;
}

void Ut_StatusIndicator::testPhoneNetwork()
{
    StatusIndicator *statusIndicator = new PhoneNetworkStatusIndicator(*testContext);
    testContextItems["Cellular.NetworkName"]->setValue(QVariant("foobarbarabush"));

    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("foobarbarabus"));

    delete statusIndicator;
}

void Ut_StatusIndicator::testInputMethod()
{
    InputMethodStatusIndicator *statusIndicator = new InputMethodStatusIndicator;
    statusIndicator->setIconID("test");
    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("test"));

    delete statusIndicator;
}

void Ut_StatusIndicator::testCall()
{
    StatusIndicator *statusIndicator = new CallStatusIndicator(*testContext);
    testContextItems["Phone.Call"]->setValue(QVariant("inactive"));
    QVERIFY(statusIndicator->model()->value().type() == QVariant::Int);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Ongoing") < 0);

    testContextItems["Phone.Call"]->setValue(QVariant("ringing"));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("active"));
    QVERIFY(statusIndicator->objectName().indexOf("Ongoing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("knocking"));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("inactive"));
    testContextItems["Phone.Muted"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") < 0);
    QVERIFY(statusIndicator->objectName().indexOf("Ongoing") < 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("ringing"));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    testContextItems["Phone.Call"]->setValue(QVariant("active"));
    QVERIFY(statusIndicator->objectName().indexOf("Ongoing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(1));

    testContextItems["Phone.Call"]->setValue(QVariant("knocking"));
    QVERIFY(statusIndicator->objectName().indexOf("Ringing") >= 0);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));
}

void Ut_StatusIndicator::testProfile()
{
    StatusIndicator *statusIndicator = new ProfileStatusIndicator(*testContext);
    testContextItems["Profile.Name"]->setValue(QVariant("silent"));
    QVERIFY(statusIndicator->objectName().indexOf("Silent") >= 0);
    testContextItems["Profile.Name"]->setValue(QVariant("default"));
    QVERIFY(statusIndicator->objectName().indexOf("Silent") < 0);

    delete statusIndicator;
}

void Ut_StatusIndicator::testGPS()
{
    StatusIndicator *gpsIndicator = new GPSStatusIndicator(*testContext);
    testContextItems["Location.SatPositioningState"]->setValue(QVariant("on"));
    QVERIFY(gpsIndicator->objectName().indexOf("On") >= 0);
    testContextItems["Location.SatPositioningState"]->setValue(QVariant("search"));
    QVERIFY(gpsIndicator->objectName().indexOf("Search") >= 0);
    QVERIFY(gpsIndicator->objectName().indexOf("On") < 0);

    delete gpsIndicator;
}

QTEST_APPLESS_MAIN(Ut_StatusIndicator)
