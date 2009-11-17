#include "ut_networkbusinesslogic.h"
#include "networkbusinesslogic.h"

#include <DuiApplication>
#include <DuiTheme>
#include <DuiLocale>

#include <QSignalSpy>
#include <QVariant>
#include <QDebug>

namespace {
    const int NetworkTechnologyCheckInterval = 500;
}

void Ut_NetworkBusinessLogic::init()
{
    m_subject = new NetworkBusinessLogic(true);
    m_subject->networkRegistration->init(m_subject->networkOperator);
}

void Ut_NetworkBusinessLogic::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

DuiApplication *app;
void Ut_NetworkBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_NetworkBusinessLogic";
    app = new DuiApplication(argc, &app_name);
}

void Ut_NetworkBusinessLogic::cleanupTestCase()
{
    delete app;
}

void Ut_NetworkBusinessLogic::testNetworkIcon()
{    
    // Test 2G icon
    m_subject->radioAccess->setTechnology(RadioAccess::GSM);
    m_subject->technology->networkCell->setServices(NetworkCell::GPRSSupport);
    QTest::qWait(NetworkTechnologyCheckInterval + 50);    
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-gsm"));

    // Test 2.5G icon
    m_subject->technology->networkCell->setServices(NetworkCell::EGPRSSupport);
    QTest::qWait(NetworkTechnologyCheckInterval - 10);
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-gsm")); //test that the icon is not changed ...
    QTest::qWait(10 + 50);
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-25g")); // until now

    // Test 3G icon
    m_subject->radioAccess->setTechnology(RadioAccess::UMTS);
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    m_subject->technology->networkCell->setServices(NetworkCell::GPRSSupport);  //restart the timer
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-25g")); //test that the icon is not changed ...
    QTest::qWait(10 + 50);
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-3g")); // until now

    // Test 3.5G icon
    m_subject->technology->networkCell->setServices(NetworkCell::HSDPASupport);
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    m_subject->radioAccess->setState(RadioAccess::AllocatedHSDPA); //restart the timer
    QTest::qWait(NetworkTechnologyCheckInterval - 50);    
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-3g")); //test that the icon is not changed ...
    QTest::qWait(10 + 50);
    QCOMPARE(m_subject->networkIcon(), QString("icon-s-35g")); // until now
}

void Ut_NetworkBusinessLogic::testQueryAvailableOperators()
{
    QSignalSpy spy(m_subject, SIGNAL(availableNetworkOperators(int,QStringList,bool)));
    QStringList names = addOperators();

    m_subject->networkRegistration->setQuerySuccess(false); // make the query to fail
    m_subject->queryAvailableOperators();

    // test that  the query failed
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).type() == QVariant::Int);
    QVERIFY(arguments.at(0).toInt() == -1);
    QVERIFY(arguments.at(1).type() == QVariant::StringList);
    QVERIFY(arguments.at(1).toStringList().size() == 0);
    QVERIFY(arguments.at(2).type() == QVariant::Bool);
    QVERIFY(arguments.at(2).toBool() == true);

    m_subject->networkRegistration->setQuerySuccess(true); // make the query succeeds
    m_subject->queryAvailableOperators();

    // test that  the query succeeded
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).type() == QVariant::Int);
    QVERIFY(arguments.at(0).toInt() != -1);
    QVERIFY(arguments.at(1).type() == QVariant::StringList);
    QVERIFY(arguments.at(1).toStringList().size() > 0);
    QVERIFY(arguments.at(2).type() == QVariant::Bool);
    QVERIFY(arguments.at(2).toBool() == true);

    // test that the operator names are sorted
    operatorNames = arguments.at(1).toStringList();
    names.removeOne("Sonera");
    names.sort();
    for(int i=0; i<operatorNames.size(); ++i) {
        QVERIFY(operatorNames.at(i) == names.at(i));
        qDebug() << "operatorName: " << operatorNames.at(i) << " " << names.at(i);
    }

    // test that the operators are in same order than in business logics QMap
    // and that Radiolinja(2) is the selected operator
    QMapIterator<QString, QStringList> iter(m_subject->operators);
    int index = 0;
    while (iter.hasNext()) {
        iter.next();
        QVERIFY(iter.key() == operatorNames.at(index));
        if(iter.value().at(0) == "RadiolinjaMNC2") {
            qDebug() << "index: " << index;
            QVERIFY(index == arguments.at(0).toInt());
        }
        ++index;
    }

}

void Ut_NetworkBusinessLogic::testSelectOperator()
{
    addOperators();
    m_subject->networkRegistration->setQuerySuccess(true); // make the query succeeds
    m_subject->queryAvailableOperators();

    for(int i=0; i<operatorNames.size(); ++i) {    
        m_subject->selectOperator(i);
        QVERIFY(m_subject->networkOperator->name() == operatorNames.at(i));
    }
}

void Ut_NetworkBusinessLogic::testSignalStrengthIcon()
{
    QSignalSpy spy(m_subject, SIGNAL(signalStrengthIconChanged(QString)));    
    int count;

    for(int i=0; i<=100; ++i) {
        m_subject->signalStrength->setBars(i);
        count = (((i%20 == 1 && i > 20) || i == 0) ? 1 : 0); //0, 21, 41, 61, 81
        QCOMPARE(spy.count(), count);
        if(count == 1) {
            QList<QVariant> arguments = spy.takeFirst();
            QVERIFY(arguments.at(0).type() == QVariant::String);
            QVERIFY(arguments.at(0).toString() == QString("icon-s-network-%1").arg(i<100 ? i/20*20 + 20 : 100));
        }
    }
}

QStringList Ut_NetworkBusinessLogic::addOperators()
{    
    QStringList names;
    names << "Tele" << "Radiolinja" << "DNA" << "Radiolinja" << "Sonera";
    QStringList mncs;
    mncs << "TeleMNC" << "RadiolinjaMNC" << "DNAMNC" << "RadiolinjaMNC2" << "SoneraMNC";
    QStringList mccs;
    mccs << "TeleMCC" << "RadiolinjaMCC" << "DNAMCC" << "RadiolinjaMCC2" << "SoneraMCC";
    QList<AvailableOperator::Availability> availabilities;
    availabilities << AvailableOperator::Available << AvailableOperator::Available << AvailableOperator::Available
            << AvailableOperator::Current << AvailableOperator::NotAvailable;

    for(int i=0; i<names.size(); ++i)
        m_subject->networkRegistration->addOperator(names.at(i), mncs.at(i), mccs.at(i), availabilities.at(i));
    return names;
}

QTEST_APPLESS_MAIN(Ut_NetworkBusinessLogic);
