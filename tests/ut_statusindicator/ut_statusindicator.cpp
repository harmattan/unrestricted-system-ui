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

#include <QtTest/QtTest>
#include <MOnDisplayChangeEvent>
#include "ut_statusindicator.h"
#include "statusindicator.h"
#include "statusindicatoranimationview.h"
#include "testcontextitem.h"
#include "inputmethodstatusindicatoradaptor_stub.h"


#ifdef HAVE_QMSYSTEM
#include <qmdevicemode_stub.h>
#endif

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


bool timerStarted = false;
void QTimer::start()
{
    timerStarted = true;
}

void QTimer::stop()
{
    timerStarted = false;
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

    testContextItems["Cellular.SignalBars"]->setValue(QVariant(5));

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

    // testing full battery
    QList<QVariant> values;
    values << QVariant(8) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 100);

    // testing empty battery
    values.clear();
    values << QVariant(0) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 0);

    // testing battery somewhere between empty and full
    values.clear();
    values << QVariant(6) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 75);

    // testing battery with invalid charge bar values
    values.clear();
    values << QVariant(-1) << QVariant(0);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("Level") >= 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("Charging") >= 0);

    // testing battery level when charging
    values.clear();
    values << QVariant(6) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 100), 75);

    // battery not charging and power save off
    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("Level") >= 0);

    // battery not charging, but power save on
    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("PowerSave") >= 0);
    QVERIFY(m_subject->objectName().indexOf("Charging") < 0);

    // battery charging, but power save off
    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(false));
    QVERIFY(m_subject->objectName().indexOf("Charging") >= 0);
    QVERIFY(m_subject->objectName().indexOf("PowerSave") < 0);

    // battery charging and power save on
    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(true));
    QVERIFY(m_subject->objectName().indexOf("PowerSaveCharging") >= 0);
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

void Ut_StatusIndicator::testPhoneNetwork_data() {
    QTest::addColumn<QString>("netstring");
    QTest::addColumn<QString>("home");
    QTest::addColumn<QString>("visitor");

    QTest::newRow("NoRoaming") << "F!o" << "F!o" << QString();
    QTest::newRow("RoamingIdeal") << "foo (bar)" << "foo" << "bar";
    QTest::newRow("MissingVisitor") << "F!o()" << "F!o" << QString();
    QTest::newRow("MissingBoth") << QString() << QString() << QString();
    QTest::newRow("HomeAndVisitorSame") << "foo (foo)" << "foo" << "foo";
    QTest::newRow("NoStartDelimiter") << "foo bar)" << "foo bar)" << QString();
    QTest::newRow("NoEndDelimiter") << "FoO(BaR1!" << "FoO" << QString();
    QTest::newRow("RoamingExtraSpaces") << " f1o (bar) " << "f1o" << "bar";
    QTest::newRow("RoamingExtraDelimiters") << "foo (()bar())" << "foo" << "()bar()";
}

void Ut_StatusIndicator::testPhoneNetwork()
{
    QFETCH(QString, netstring);
    QFETCH(QString, home);
    QFETCH(QString, visitor);

    StatusIndicator *m_subject = new PhoneNetworkStatusIndicator(*testContext);

    testContextItems["Cellular.NetworkName"]->setValue(QVariant(netstring));
    PhoneNetworkStatusIndicator* indicator = qobject_cast<PhoneNetworkStatusIndicator*>(m_subject);

    // reconnect timer timeout and check that reconnection fails i.e. is already connected
    bool disconnectSuccess = disconnect(&indicator->networkChangeShowVisitorTimer, SIGNAL(timeout()), indicator, SLOT(showVisitorNetworkName()));
    QCOMPARE(disconnectSuccess, true);

    // test methods
    QCOMPARE(indicator->homeNetwork(), home);
    QCOMPARE(indicator->visitorNetwork(), visitor);

    // check if home network is visible at first
    QCOMPARE(m_subject->model()->value(), QVariant(QString(home)));

    if (home.isEmpty() || visitor.isEmpty() || (home == visitor)) {
        // check if home or visitor empty or same, timer is not started
        QCOMPARE(timerStarted, false);
    } else {
        // check if timer has been started
        QCOMPARE(timerStarted, true);
        // else check for correct interval
        QCOMPARE(indicator->networkChangeShowVisitorTimer.interval(), 3000);
        // "triggers" timeout and shows visitor network name
        indicator->showVisitorNetworkName();
        // stops timer to reset it's value to not started
        indicator->networkChangeShowVisitorTimer.stop();
    }

    // check that visitor is showing only if home and visitor are non-empty and not same
    // and else home network is showing
    if (!home.isEmpty() && !visitor.isEmpty() && (home != visitor)) {
        QCOMPARE(m_subject->model()->value(), QVariant(QString(visitor)));
    } else {
        QCOMPARE(m_subject->model()->value(), QVariant(QString(home)));
    }
}

void Ut_StatusIndicator::testInputMethod()
{
    InputMethodStatusIndicator *statusIndicator = new InputMethodStatusIndicator;
    m_subject = statusIndicator;

    statusIndicator->setIconID("test");
    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("test"));
}

// keep these in sync with the context framework!
static const QString CONTEXT_CALLSTATE_ALERTING = "alerting";
static const QString CONTEXT_CALLSTATE_KNOCKING = "knocking";
static const QString CONTEXT_CALLSTATE_ACTIVE = "active";
static const QString CONTEXT_CALLSTATE_INACTIVE = "inactive";

void Ut_StatusIndicator::testCall_data()
{
    QTest::addColumn<QVariant>("stateToSet");
    QTest::addColumn<QVariant>("modelValue");
    QTest::addColumn<bool>("ringingStyleEnabled");
    QTest::addColumn<bool>("ongoingStyleEnabled");
    QTest::addColumn<QVariant>("muted");

    QTest::newRow("inactive") << QVariant(CONTEXT_CALLSTATE_INACTIVE)
                              << QVariant(0)
                              << false << false << QVariant(false);

    QTest::newRow("alerting") << QVariant(CONTEXT_CALLSTATE_ALERTING)
                              << QVariant(0)
                              << true << false << QVariant(false);

    QTest::newRow("active") << QVariant(CONTEXT_CALLSTATE_ACTIVE)
                            << QVariant(0)
                            << false << true << QVariant(false);

    QTest::newRow("knocking") << QVariant(CONTEXT_CALLSTATE_KNOCKING)
                              << QVariant(0)
                              << true << false << QVariant(false);

    QTest::newRow("inactive_muted") << QVariant(CONTEXT_CALLSTATE_INACTIVE)
                                    << QVariant(0)
                                    << false << false << QVariant(true);

    QTest::newRow("alerting_muted") << QVariant(CONTEXT_CALLSTATE_ALERTING)
                                    << QVariant(0)
                                    << true << false << QVariant(true);

    QTest::newRow("active_muted") << QVariant(CONTEXT_CALLSTATE_ACTIVE)
                                  << QVariant(1)
                                  << false << true << QVariant(true);

    QTest::newRow("knocking_muted") << QVariant(CONTEXT_CALLSTATE_KNOCKING)
                                    << QVariant(0)
                                    << true << false << QVariant(true);
}

void Ut_StatusIndicator::testCall()
{
    m_subject = new CallStatusIndicator(*testContext);

    QFETCH(QVariant, stateToSet);
    testContextItems["Phone.Call"]->setValue(stateToSet);

    QFETCH(QVariant, muted);
    testContextItems["Phone.Muted"]->setValue(muted);

    QCOMPARE(m_subject->model()->value().type(), QVariant::Int);

    QFETCH(QVariant, modelValue);
    QCOMPARE(m_subject->model()->value(), modelValue);

    QFETCH(bool, ringingStyleEnabled);
    QCOMPARE((bool)m_subject->objectName().contains("Ringing"),
             ringingStyleEnabled);

    QFETCH(bool, ongoingStyleEnabled);
    QCOMPARE((bool)m_subject->objectName().contains("Ongoing"),
             ongoingStyleEnabled);
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
