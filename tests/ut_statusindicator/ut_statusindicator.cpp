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
#include <MApplication>
#include <MOnDisplayChangeEvent>
#include <MLocale>
#include <MGConfItem>
#include "ut_statusindicator.h"
#include "statusindicator.h"
#include "statusindicatoranimationview.h"
#include "testcontextitem.h"

#ifdef HAVE_QMSYSTEM
#include <qmdevicemode_stub.h>
#endif

bool MWidget::isOnDisplay() const
{
    return true;
}

// QDBusConnection::connect stub
static QHash<QString, QString> qDBusConnectionConnectParameters;
bool QDBusConnection::connect ( const QString & service, const QString & path, const QString & interface, const QString & name, QObject * receiver, const char * slot )
{
    qDBusConnectionConnectParameters["service"]=service;
    qDBusConnectionConnectParameters["path"]= path;
    qDBusConnectionConnectParameters["interface"]= interface;
    qDBusConnectionConnectParameters["name"]= name;
    qDBusConnectionConnectParameters["receiver"]= receiver->metaObject()->className();
    qDBusConnectionConnectParameters["slot"]= slot;
    return true;
}

QHash<QString, TestContextItem *> testContextItems;

// Test context
class TestContext : public ApplicationContext
{
public:
    virtual ContextItem *createContextItem(const QString &key) {
        testContextItems[key] = new TestContextItem;

        return testContextItems[key];
    }
};

bool timerStarted = false;
void QTimer::start()
{
    timerStarted = true;
    id = 1;
}

void QTimer::stop()
{
    timerStarted = false;
    id = -1;
}

QString gMLocaleLanguage;
QString MLocale::language() const
{
    return gMLocaleLanguage;
}

// MGConfItem stubs
QVariant mGConfItemValueForDisplayLimitedServiceState;
QVariant MGConfItem::value(const QVariant &def) const
{
    if (key() == "/desktop/meego/status_area/display_limited_service_state") {
        return QVariant(mGConfItemValueForDisplayLimitedServiceState.isNull() ? def : mGConfItemValueForDisplayLimitedServiceState);
    }

    return QVariant();
}

void Ut_StatusIndicator::init()
{
    m_subject = NULL;
    testContext = new TestContext();
    testContextItems.clear();
    gMLocaleLanguage = "";
}

void Ut_StatusIndicator::cleanup()
{
    delete m_subject;
    delete testContext;
    mGConfItemValueForDisplayLimitedServiceState = QVariant();
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
    gContextItemStub->stubReset();
    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext, NULL);
    QSignalSpy spy(m_subject, SIGNAL(subscriptionMade()));

    testContextItems["Internet.NetworkType"]->setValue(QVariant());

    // When the application becomes not visible, the context item updates
    // should be unsubscribed from
    m_subject->exitDisplayEvent();
    QCOMPARE(gContextItemStub->stubCallCount("unsubscribe"), 5);

    testContextItems["Internet.NetworkType"]->setValue("WLAN");
    testContextItems["System.WlanEnabled"]->setValue(true);

    // When the application becomes not visible, the context item updates
    // should be subscribed to
    m_subject->enterDisplayEvent();
    QCOMPARE(gContextItemStub->stubCallCount("subscribe"), 5);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->styleName().contains("WLAN"), QBool(true));
}

void Ut_StatusIndicator::testContextItemDeletion()
{
    gContextItemStub->stubReset();
    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext, NULL);

    // There should be a total of four items constructed using the
    // StatusIndicator::createContextItem() call
    QCOMPARE(gContextItemStub->stubCallCount("ContextItemConstructor"), 5);

    delete m_subject;
    m_subject = NULL;

    // There should be a total of four items deleted by the
    // StatusIndicator destructor
    QCOMPARE(gContextItemStub->stubCallCount("ContextItemDestructor"), 5);
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrength()
{
    PhoneNetworkSignalStrengthStatusIndicator* statusIndicator = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);
    m_subject = statusIndicator;

    testContextItems["Cellular.SignalBars"]->setValue(QVariant(5));
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("home"));

    QCOMPARE(statusIndicator->model()->value().type(), QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    QCOMPARE(statusIndicator->styleName(), QString("PhoneNetworkSignalStrengthStatusIndicator"));
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrenghtStyleName_data()
{
    QTest::addColumn<bool>("SystemOfflineMode");
    QTest::addColumn<QString>("CellularRegistrationStatus");
    QTest::addColumn<QString>("ExpectedObjectNamePostfix");

    QTest::newRow("Offline mode") << true << QString("") << QString("Offline");
    QTest::newRow("No-sim registration status") << false << QString("no-sim") << QString("NoSIM");
    QTest::newRow("Empty registration status") << false << QString("") << QString("NoNetwork");
    QTest::newRow("Offline registration status") << false << QString("offline") << QString("NoNetwork");
    QTest::newRow("Forbidden registration status") << false << QString("forbidden") << QString("NoNetwork");
    QTest::newRow("No-service registration status") << false << QString("no-service") << QString("NoNetwork");
    QTest::newRow("Home registration status") << false << QString("home") << QString("");
    QTest::newRow("Roam registration status") << false << QString("roam") << QString("");
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrenghtStyleName()
{
    QFETCH(bool, SystemOfflineMode);
    QFETCH(QString, CellularRegistrationStatus);
    QFETCH(QString, ExpectedObjectNamePostfix);

    m_subject = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);

    testContextItems["System.OfflineMode"]->setValue(SystemOfflineMode);
    testContextItems["Cellular.RegistrationStatus"]->setValue(CellularRegistrationStatus);

    QCOMPARE(m_subject->styleName().contains(ExpectedObjectNamePostfix), QBool(true));
}

void Ut_StatusIndicator::testWhenPhoneNetworkBecomesAvailableThenSignalIsEmitted()
{
    m_subject = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);

    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));

    QSignalSpy spy(m_subject, SIGNAL(networkAvailabilityChanged(bool)));
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("home"));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(true));
}

void Ut_StatusIndicator::testWhenPhoneNetworkBecomesUnavailableThenSignalIsEmitted()
{
    m_subject = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);

    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("home"));

    QSignalSpy spy(m_subject, SIGNAL(networkAvailabilityChanged(bool)));
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant("offline"));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0)[0], QVariant(false));
}

void Ut_StatusIndicator::testPhoneNetworkTypeStyleNameWhenSystemOnlineName_data()
{
    QTest::addColumn<bool>("PacketDataActive");
    QTest::addColumn<bool>("WlanEnabled");
    QTest::addColumn<QString>("CellularDataTechnology");
    QTest::addColumn<QString>("InternetConnectionType");
    QTest::addColumn<QString>("InternetConnectionState");
    QTest::addColumn<QString>("ExpectedObjectNamePostfix");

    QTest::newRow("gprs connection type and 2g tech not active, wlan disabled") << false << false << QString("gprs") << QString("GPRS") << QString("connected") << QString("2G");
    QTest::newRow("gprs connection type and 2g tech connecting, wlan disabled") << false << false << QString("gprs") << QString("GPRS") << QString("connecting") << QString("2GConnecting");
    QTest::newRow("gprs connection type and 2g tech active, wlan disabled") << true << false << QString("gprs") << QString("GPRS") << QString("connected") << QString("2GActive");
    QTest::newRow("gprs connection type and 2g tech active, internet disconnected, wlan disabled") << true << false << QString("gprs") << QString("GPRS") << QString("disconnected") << QString("2GActive");
    QTest::newRow("gprs connection type and 2g tech not active, internet disconnected, wlan disabled") << false << false << QString("gprs") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 25g tech not active, wlan disabled") << false << false << QString("egprs") << QString("GPRS") << QString("connected") << QString("25G");
    QTest::newRow("gprs connection type and 25g tech connecting, wlan disabled") << false << false << QString("egprs") << QString("GPRS") << QString("connecting") << QString("25GConnecting");
    QTest::newRow("gprs connection type and 25g tech active, wlan disabled") << true << false << QString("egprs") << QString("GPRS") << QString("connected") << QString("25GActive");
    QTest::newRow("gprs connection type and 25g tech active, internet disconnected, wlan disabled") << true << false << QString("egprs") << QString("GPRS") << QString("disconnected") << QString("25GActive");
    QTest::newRow("gprs connection type and 25g tech not active, internet disconnected, wlan disabled") << false << false << QString("egprs") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 3g tech not active, wlan disabled") << false << false << QString("umts") << QString("GPRS") << QString("connected") << QString("3G");
    QTest::newRow("gprs connection type and 3g tech connecting, wlan disabled") << false << false << QString("umts") << QString("GPRS") << QString("connecting") << QString("3GConnecting");
    QTest::newRow("gprs connection type and 3g tech active, wlan disabled") << true << false << QString("umts") << QString("GPRS") << QString("connected") << QString("3GActive");
    QTest::newRow("gprs connection type and 3g tech active, internet disconnected, wlan disabled") << true << false << QString("umts") << QString("GPRS") << QString("disconnected") << QString("3GActive");
    QTest::newRow("gprs connection type and 3g tech not active, internet disconnected, wlan disabled") << false << false << QString("umts") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 35g tech not active, wlan disabled") << false << false << QString("hspa") << QString("GPRS") << QString("connected") << QString("35G");
    QTest::newRow("gprs connection type and 35g tech connecting, wlan disabled") << false << false << QString("hspa") << QString("GPRS") << QString("connecting") << QString("35GConnecting");
    QTest::newRow("gprs connection type and 35g tech active, wlan disabled") << true << false << QString("hspa") << QString("GPRS") << QString("connected") << QString("35GActive");
    QTest::newRow("gprs connection type and 35g tech active, internet disconnected, wlan disabled") << true << false << QString("hspa") << QString("GPRS") << QString("disconnected") << QString("35GActive");
    QTest::newRow("gprs connection type and 35g tech not active, internet disconnected, wlan disabled") << false << false << QString("hspa") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("wlan connection type and wlan tech not active, wlan disabled") << false << false << QString("") << QString("WLAN") << QString("connected") << QString("");
    QTest::newRow("wlan connection type and wlan tech connecting, wlan disabled") << false << false << QString("") << QString("WLAN") << QString("connecting") << QString("");
    QTest::newRow("wlan connected and 2g packet data active, wlan disabled") << true << false << QString("gprs") << QString("WLAN") << QString("connected") << QString("2GActive");
    QTest::newRow("wlan connected and 25g packet data active, wlan disabled") << true << false << QString("egprs") << QString("WLAN") << QString("connected") << QString("25GActive");
    QTest::newRow("wlan connected and 3g packet data active, wlan disabled") << true << false << QString("umts") << QString("WLAN") << QString("connected") << QString("3GActive");
    QTest::newRow("wlan connected and 35g packet data active, wlan disabled") << true << false << QString("hspa") << QString("WLAN") << QString("connected") << QString("35GActive");

    QTest::newRow("gprs connection type and 2g tech not active, wlan enabled") << false << true << QString("gprs") << QString("GPRS") << QString("connected") << QString("2G");
    QTest::newRow("gprs connection type and 2g tech connecting, wlan enabled") << false << true << QString("gprs") << QString("GPRS") << QString("connecting") << QString("2GConnecting");
    QTest::newRow("gprs connection type and 2g tech active, wlan enabled") << true << true << QString("gprs") << QString("GPRS") << QString("connected") << QString("2GActive");
    QTest::newRow("gprs connection type and 2g tech active, internet disconnected, wlan enabled") << true << true << QString("gprs") << QString("GPRS") << QString("disconnected") << QString("2GActive");
    QTest::newRow("gprs connection type and 2g tech not active, internet disconnected, wlan enabled") << false << true << QString("gprs") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 25g tech not active, wlan enabled") << false << true << QString("egprs") << QString("GPRS") << QString("connected") << QString("25G");
    QTest::newRow("gprs connection type and 25g tech connecting, wlan enabled") << false << true << QString("egprs") << QString("GPRS") << QString("connecting") << QString("25GConnecting");
    QTest::newRow("gprs connection type and 25g tech active, wlan enabled") << true << true << QString("egprs") << QString("GPRS") << QString("connected") << QString("25GActive");
    QTest::newRow("gprs connection type and 25g tech active, internet disconnected, wlan enabled") << true << true << QString("egprs") << QString("GPRS") << QString("disconnected") << QString("25GActive");
    QTest::newRow("gprs connection type and 25g tech not active, internet disconnected, wlan enabled") << false << true << QString("egprs") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 3g tech not active, wlan enabled") << false << true << QString("umts") << QString("GPRS") << QString("connected") << QString("3G");
    QTest::newRow("gprs connection type and 3g tech connecting, wlan enabled") << false << true << QString("umts") << QString("GPRS") << QString("connecting") << QString("3GConnecting");
    QTest::newRow("gprs connection type and 3g tech active, wlan enabled") << true << true << QString("umts") << QString("GPRS") << QString("connected") << QString("3GActive");
    QTest::newRow("gprs connection type and 3g tech active, internet disconnected, wlan enabled") << true << true << QString("umts") << QString("GPRS") << QString("disconnected") << QString("3GActive");
    QTest::newRow("gprs connection type and 3g tech not active, internet disconnected, wlan enabled") << false << true << QString("umts") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("gprs connection type and 35g tech not active, wlan enabled") << false << true << QString("hspa") << QString("GPRS") << QString("connected") << QString("35G");
    QTest::newRow("gprs connection type and 35g tech connecting, wlan enabled") << false << true << QString("hspa") << QString("GPRS") << QString("connecting") << QString("35GConnecting");
    QTest::newRow("gprs connection type and 35g tech active, wlan enabled") << true << true << QString("hspa") << QString("GPRS") << QString("connected") << QString("35GActive");
    QTest::newRow("gprs connection type and 35g tech active, internet disconnected, wlan enabled") << true << true << QString("hspa") << QString("GPRS") << QString("disconnected") << QString("35GActive");
    QTest::newRow("gprs connection type and 35g tech not active, internet disconnected, wlan enabled") << false << true << QString("hspa") << QString("GPRS") << QString("disconnected") << QString("");
    QTest::newRow("wlan connection type and wlan tech not active, wlan enabled") << false << true << QString("") << QString("WLAN") << QString("connected") << QString("WLAN");
    QTest::newRow("wlan connection type and wlan tech connecting, wlan enabled") << false << true << QString("") << QString("WLAN") << QString("connecting") << QString("WLANConnecting");
    QTest::newRow("wlan connected and 2g packet data active, wlan enabled") << true << true << QString("gprs") << QString("WLAN") << QString("connected") << QString("WLAN2GActive");
    QTest::newRow("wlan connected and 25g packet data active, wlan enabled") << true << true << QString("egprs") << QString("WLAN") << QString("connected") << QString("WLAN25GActive");
    QTest::newRow("wlan connected and 3g packet data active, wlan enabled") << true << true << QString("umts") << QString("WLAN") << QString("connected") << QString("WLAN3GActive");
    QTest::newRow("wlan connected and 35g packet data active, wlan enabled") << true << true << QString("hspa") << QString("WLAN") << QString("connected") << QString("WLAN35GActive");
}

void Ut_StatusIndicator::testPhoneNetworkTypeStyleNameWhenSystemOnlineName()
{
    QFETCH(bool, PacketDataActive);
    QFETCH(bool, WlanEnabled);
    QFETCH(QString, CellularDataTechnology);
    QFETCH(QString, InternetConnectionType);
    QFETCH(QString, InternetConnectionState);
    QFETCH(QString, ExpectedObjectNamePostfix);

    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext);

    testContextItems["Cellular.PacketData"]->setValue(PacketDataActive);
    testContextItems["Cellular.DataTechnology"]->setValue(CellularDataTechnology);
    testContextItems["Internet.NetworkType"]->setValue(InternetConnectionType);
    testContextItems["Internet.NetworkState"]->setValue(InternetConnectionState);
    testContextItems["System.WlanEnabled"]->setValue(WlanEnabled);

    if (!ExpectedObjectNamePostfix.isEmpty()) {
        QCOMPARE(m_subject->styleName(), QString(m_subject->metaObject()->className() + ExpectedObjectNamePostfix));
    } else {
        QCOMPARE(m_subject->styleName(), QString(m_subject->metaObject()->className()));
    }
}

void Ut_StatusIndicator::testPhoneNetworkAvailabilityChanged_data()
{
    QTest::addColumn<QString>("NetworkState");
    QTest::addColumn<bool>("WlanEnabled");
    QTest::addColumn<bool>("NetworkAvailability");
    QTest::addColumn<bool>("StyleNameEmpty");

    QTest::newRow("network disconnected, wlan enabled, network changed unavailable") << QString("disconnected") << true << false << true;
    QTest::newRow("network connected, wlan enabled, network changed unavailable") << QString("connected") << true << false << false;
    QTest::newRow("network connecting, wlan enabled, network changed unavailable") << QString("connecting") << true << false << false;
    QTest::newRow("network disconnected, wlan enabled, network changed available") << QString("disconnected") << true << true << true;
    QTest::newRow("network connected, wlan enabled, network changed available") << QString("connected") << true << true << false;
    QTest::newRow("network connecting, wlan enabled, network changed available") << QString("connecting") << true << true << false;

    QTest::newRow("network disconnected, wlan disabled, network changed unavailable") << QString("disconnected") << false << false << true;
    QTest::newRow("network connected, wlan disabled, network changed unavailable") << QString("connected") << false << false << true;
    QTest::newRow("network connecting, wlan disabled, network changed unavailable") << QString("connecting") << false << false << true;
    QTest::newRow("network disconnected, wlan disabled, network changed available") << QString("disconnected") << false << true << true;
    QTest::newRow("network connected, wlan disabled, network changed available") << QString("connected") << false << true << true;
    QTest::newRow("network connecting, wlan disabled, network changed available") << QString("connecting") << false << true << true;
}

void Ut_StatusIndicator::testPhoneNetworkAvailabilityChanged()
{
    QFETCH(QString, NetworkState);
    QFETCH(bool, WlanEnabled);
    QFETCH(bool, NetworkAvailability);
    QFETCH(bool, StyleNameEmpty);

    m_subject = new PhoneNetworkTypeStatusIndicator(*testContext);

    PhoneNetworkTypeStatusIndicator* networkTypeIndicator = qobject_cast<PhoneNetworkTypeStatusIndicator*>(m_subject);

    testContextItems["Internet.NetworkState"]->setValue(QVariant(NetworkState));
    testContextItems["System.WlanEnabled"]->setValue(QVariant(WlanEnabled));
    testContextItems["Internet.NetworkType"]->setValue(QVariant("WLAN"));
    networkTypeIndicator->setNetworkAvailability(NetworkAvailability);
    QVERIFY((networkTypeIndicator->styleName() == networkTypeIndicator->metaObject()->className()) == StyleNameEmpty);
}

void Ut_StatusIndicator::testBattery()
{
    m_subject = new BatteryStatusIndicator(*testContext);

    // testing full battery
    QList<QVariant> values;
    values << QVariant(8) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 9);

    // testing empty battery
    values.clear();
    values << QVariant(0) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 0);

    // testing first non-empty battery level
    values.clear();
    values << QVariant(1) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 2);

    // testing battery somewhere between empty and full
    values.clear();
    values << QVariant(6) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 7);

    // testing battery with invalid charge bar values
    values.clear();
    values << QVariant(-1) << QVariant(0);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("Level") >= 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("Charging") >= 0);

    // testing battery level when charging and battery empty
    values.clear();
    values << QVariant(0) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 1);

    // testing battery level when charging and battery non-empty
    values.clear();
    values << QVariant(6) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 7);

    // testing battery level when charging and remaining bars equal to maximum bars
    values.clear();
    values << QVariant(8) << QVariant(8);
    testContextItems["Battery.ChargeBars"]->setValue(QVariant(values));
    QVERIFY(m_subject->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(m_subject->model()->value().toDouble() * 10), 8);

    // battery not charging and power save off
    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("Level") >= 0);

    // battery not charging, but power save on
    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("PowerSave") >= 0);
    QVERIFY(m_subject->styleName().indexOf("Charging") < 0);

    // battery charging, but power save off
    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("Charging") >= 0);
    QVERIFY(m_subject->styleName().indexOf("PowerSave") < 0);

    // battery charging and power save on
    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    testContextItems["System.PowerSaveMode"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("PowerSaveCharging") >= 0);
}

void Ut_StatusIndicator::testAlarm()
{
    m_subject = new AlarmStatusIndicator(*testContext);

    testContextItems["Alarm.Present"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("Set") < 0);

    testContextItems["Alarm.Present"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("Set") >= 0);
}

void Ut_StatusIndicator::testBluetooth()
{
    m_subject = new BluetoothStatusIndicator(*testContext);

    // !enabled && !connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("On") < 0);
    QVERIFY(m_subject->styleName().indexOf("Active") < 0);

    // enabled && !connected == BluetoothStatusIndicatorOn
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("On") >= 0);
    QVERIFY(m_subject->styleName().indexOf("Active") < 0);

    // enabled && connected == BluetoothStatusIndicatorActice
    testContextItems["Bluetooth.Connected"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("On") < 0);
    QVERIFY(m_subject->styleName().indexOf("Active") >= 0);

    // !enabled && connected == BluetoothStatusIndicator
    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("On") < 0);
    QVERIFY(m_subject->styleName().indexOf("Active") < 0);
}

void Ut_StatusIndicator::testPresence()
{
    m_subject = new PresenceStatusIndicator(*testContext);

    // Offline mode by default
    QVERIFY(m_subject->styleName().indexOf("Busy") < 0);
    QVERIFY(m_subject->styleName().indexOf("Available") < 0);

    // Busy
    testContextItems["Presence.State"]->setValue(QVariant("busy"));
    QVERIFY(m_subject->styleName().indexOf("Busy") >= 0);
    QVERIFY(m_subject->styleName().indexOf("Available") < 0);

    // Available
    testContextItems["Presence.State"]->setValue(QVariant("available"));
    QVERIFY(m_subject->styleName().indexOf("Busy") < 0);
    QVERIFY(m_subject->styleName().indexOf("Available") >= 0);

    // Offline (explicitly)
    testContextItems["Presence.State"]->setValue(QVariant("offline"));
    QVERIFY(m_subject->styleName().indexOf("Busy") < 0);
    QVERIFY(m_subject->styleName().indexOf("Available") < 0);
}

void Ut_StatusIndicator::testAnimation()
{
    StatusIndicator *m_subject = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    m_subject->exitDisplayEvent();
    QCOMPARE(m_subject->model()->animate(), false);

    m_subject->enterDisplayEvent();
    QCOMPARE(m_subject->model()->animate(), true);
    delete m_subject;
}

void Ut_StatusIndicator::testPhoneNetwork_data() {
    QTest::addColumn<QString>("netstring");
    QTest::addColumn<QString>("home");
    QTest::addColumn<QString>("visitor");
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("extnetstring");
    QTest::addColumn<QString>("servicestatus");
    QTest::addColumn<QString>("registrationstatus");
    QTest::addColumn<QVariant>("displayLimitedServiceState");
    QTest::addColumn<QString>("stylePostfix");

    QTest::newRow("NoRoaming") << "F!o" << "F!o" << QString() << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("RoamingIdeal") << "foo (bar)" << "foo" << "bar" << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("MissingVisitor") << "F!o()" << "F!o" << QString() << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("MissingBoth") << QString() << QString() << QString() << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("HomeAndVisitorSame") << "foo (foo)" << "foo" << "foo" << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("NoStartDelimiter") << "foo bar)" << "foo bar)" << QString() << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("NoEndDelimiter") << "FoO(BaR1!" << "FoO" << QString() << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("RoamingExtraSpaces") << " f1o (bar) " << "f1o" << "bar" << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("RoamingExtraDelimiters") << "foo (()bar())" << "foo" << "()bar()" << "" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("ChineseLocale") << "foo" << "bar" << "" << "zh" << "bar" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("ChineseLocaleNoExtNetString") << "foo" << "foo" << "" << "zh" << "" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("ChineseLocaleVisitor") << "foo" << "bar" << "meh" << "zh" << "bar (meh)" << "full-service" << "home" << QVariant(true) << QString();
    QTest::newRow("EmptyServiceStatus") << "F!o" << "F!o" << QString() << "" << "" << "" << "home" << QVariant(true) << QString();
    QTest::newRow("LimitedServiceServiceStatusLimitedServiceStateDisplayEnabled") << "foo (bar)" << qtTrId("qtn_cell_emergency_calls_only") << QString() << "" << "" << "limited-service" << "home" << QVariant(true) << "LimitedService";
    QTest::newRow("LimitedServiceServiceStatusLimitedServiceStateDisplayNotSet") << "foo (bar)" << qtTrId("qtn_cell_emergency_calls_only") << QString() << "" << "" << "limited-service" << "home" << QVariant() << "LimitedService";
    QTest::newRow("LimitedServiceServiceStatusLimitedServiceStateDisplayDisabled") << "F!o" << "F!o" << QString() << "" << "" << "limited-service" << "home" << QVariant(false) << QString();
    QTest::newRow("NoCoverageServiceStatusAndOfflineRegistrationStatus") << "F!o" << "F!o" << QString() << "" << "" << "no-coverage" << "offline" << QVariant(true) << QString();
    QTest::newRow("NoCoverageServiceStatusAndOnlineRegistrationStatusLimitedServiceStateDisplayEnabled") << "foo (bar)" << qtTrId("qtn_stat_no_coverage") << QString() << "" << "" << "no-coverage" << "home" << QVariant(true) << "LimitedService";
    QTest::newRow("NoCoverageServiceStatusAndOnlineRegistrationStatusLimitedServiceStateDisplayNotSet") << "foo (bar)" << qtTrId("qtn_stat_no_coverage") << QString() << "" << "" << "no-coverage" << "home" << QVariant() << "LimitedService";
    QTest::newRow("NoCoverageServiceStatusAndOnlineRegistrationStatusLimitedServiceStateDisplayDisabled") << "F!o" << "F!o" << QString() << "" << "" << "no-coverage" << "home" << QVariant(false) << QString();
}

void Ut_StatusIndicator::testPhoneNetwork()
{
    QFETCH(QString, netstring);
    QFETCH(QString, home);
    QFETCH(QString, visitor);
    QFETCH(QString, language);
    QFETCH(QString, extnetstring);
    QFETCH(QString, servicestatus);
    QFETCH(QString, registrationstatus);
    QFETCH(QVariant, displayLimitedServiceState);
    QFETCH(QString, stylePostfix);

    gMLocaleLanguage = language;
    mGConfItemValueForDisplayLimitedServiceState = displayLimitedServiceState;

    StatusIndicator *m_subject = new PhoneNetworkStatusIndicator(*testContext);

    testContextItems["Cellular.NetworkName"]->setValue(QVariant(netstring));
    testContextItems["Cellular.ExtendedNetworkName"]->setValue(QVariant(extnetstring));
    testContextItems["Cellular.ServiceStatus"]->setValue(QVariant(servicestatus));
    testContextItems["Cellular.RegistrationStatus"]->setValue(QVariant(registrationstatus));
    PhoneNetworkStatusIndicator* indicator = qobject_cast<PhoneNetworkStatusIndicator*>(m_subject);

    // Check that the network change timer is connected
    QCOMPARE(disconnect(&indicator->networkChangeShowVisitorTimer, SIGNAL(timeout()), indicator, SLOT(showVisitorNetworkName())), true);

    // Check that registration status changes are interesting if limited service state display is enabled
    QCOMPARE(disconnect(indicator->cellularServiceStatus, SIGNAL(contentsChanged()), indicator, SLOT(phoneNetworkChanged())), displayLimitedServiceState.isNull() ? true : displayLimitedServiceState.toBool());

    // Check style postfix
    QCOMPARE(m_subject->model()->stylePostfix(), stylePostfix);

    // check if home network is visible at first
    QCOMPARE(m_subject->model()->value(), QVariant(QString(home)));

    QCOMPARE(((bool)m_subject->styleName().contains("Disabled")), home.isEmpty() && visitor.isEmpty());

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
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(true));

    statusIndicator->setIconID("test");
    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("test"));
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(false));

    statusIndicator->setIconID(QString());
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(true));
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
    QCOMPARE((bool)m_subject->styleName().contains("Ringing"),
             ringingStyleEnabled);

    QFETCH(bool, ongoingStyleEnabled);
    QCOMPARE((bool)m_subject->styleName().contains("Ongoing"),
             ongoingStyleEnabled);
}

void Ut_StatusIndicator::testProfile()
{
    m_subject = new ProfileStatusIndicator(*testContext);

    testContextItems["Profile.Name"]->setValue(QVariant("silent"));
    QVERIFY(m_subject->styleName().indexOf("Silent") >= 0);

    testContextItems["Profile.Name"]->setValue(QVariant("default"));
    QVERIFY(m_subject->styleName().indexOf("Silent") < 0);
}

void Ut_StatusIndicator::testGPS()
{
    m_subject = new GPSStatusIndicator(*testContext);

    testContextItems["Location.SatPositioningState"]->setValue(QVariant("on"));
    QVERIFY(m_subject->styleName().indexOf("On") >= 0);

    testContextItems["Location.SatPositioningState"]->setValue(QVariant("search"));
    QVERIFY(m_subject->styleName().indexOf("Search") >= 0);
    QVERIFY(m_subject->styleName().indexOf("On") < 0);
}

void Ut_StatusIndicator::testTransferStatusIndicatorDBusConnectionCreationInConstructor()
{
    QScopedPointer<TransferStatusIndicator> transferStatusIndicator(new TransferStatusIndicator());

    QCOMPARE(qDBusConnectionConnectParameters["service"], QString());
    QCOMPARE(qDBusConnectionConnectParameters["path"], transferStatusIndicator->TRANSFER_UI_DBUS_PATH);
    QCOMPARE(qDBusConnectionConnectParameters["interface"], transferStatusIndicator->TRANSFER_UI_DBUS_INTERFACE);
    QCOMPARE(qDBusConnectionConnectParameters["name"], transferStatusIndicator->TRANSFER_UI_DBUS_SIGNAL);
    QCOMPARE(qDBusConnectionConnectParameters["receiver"], QString(transferStatusIndicator->metaObject()->className()));
    QCOMPARE(qDBusConnectionConnectParameters["slot"].contains("transferStateChanged(const QString&)"), QBool(true));

}

void Ut_StatusIndicator::testTransferStatusStateChange()
{
    QScopedPointer<TransferStatusIndicator> transferStatusIndicator(new TransferStatusIndicator());

    QCOMPARE(transferStatusIndicator->styleName(), QString("TransferStatusIndicator"));

    transferStatusIndicator->transferStateChanged(transferStatusIndicator->TRANSFER_UI_STATE_LIVE);
    QCOMPARE(transferStatusIndicator->styleName(), QString("TransferStatusIndicator") + transferStatusIndicator->TRANSFER_UI_SUFFIX_LIVE);

    transferStatusIndicator->transferStateChanged(transferStatusIndicator->TRANSFER_UI_STATE_FAIL);
    QCOMPARE(transferStatusIndicator->styleName(), QString("TransferStatusIndicator") + transferStatusIndicator->TRANSFER_UI_SUFFIX_FAIL);

    transferStatusIndicator->transferStateChanged(transferStatusIndicator->TRANSFER_UI_STATE_IDLE);
    QCOMPARE(transferStatusIndicator->styleName(), QString("TransferStatusIndicator"));

    transferStatusIndicator->transferStateChanged(transferStatusIndicator->TRANSFER_UI_STATE_PENDING);
    QCOMPARE(transferStatusIndicator->styleName(), QString("TransferStatusIndicator") + transferStatusIndicator->TRANSFER_UI_SUFFIX_PENDING);
}

void Ut_StatusIndicator::testCallForwarding()
{
    m_subject = new CallForwardingStatusIndicator(*testContext);

    testContextItems["Cellular.CallForwardUncond"]->setValue(QVariant(false));
    QVERIFY(m_subject->styleName().indexOf("Set") < 0);

    testContextItems["Cellular.CallForwardUncond"]->setValue(QVariant(true));
    QVERIFY(m_subject->styleName().indexOf("Set") >= 0);
}

void Ut_StatusIndicator::testNotification()
{
    NotificationStatusIndicator *statusIndicator = new NotificationStatusIndicator;
    m_subject = statusIndicator;
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(true));

    statusIndicator->setIconID("test");
    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("test"));
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(false));

    statusIndicator->setIconID(QString());
    QCOMPARE(m_subject->styleName().contains("Disabled"), QBool(true));
}

QTEST_APPLESS_MAIN(Ut_StatusIndicator)
