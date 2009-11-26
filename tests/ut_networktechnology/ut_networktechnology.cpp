#include "ut_networktechnology.h"

#include <DuiApplication>
#include <DuiTheme>

#include <QVariant>
#include <QDebug>

namespace {
    const int NetworkTechnologyCheckInterval = 500;
}

Q_DECLARE_METATYPE(NetworkTechnology::Technology);

void Ut_NetworkTechnology::init()
{
    radioAccessStub = new RadioAccess();
    m_subject = new NetworkTechnology(radioAccessStub);
    spy = new QSignalSpy(m_subject, SIGNAL(technologyChanged(NetworkTechnology::Technology)));
}

void Ut_NetworkTechnology::cleanup()
{    
    delete m_subject;
    m_subject = NULL;
    delete radioAccessStub;
    radioAccessStub = NULL;
    delete spy;
    spy = NULL;
}

DuiApplication *app;
void Ut_NetworkTechnology::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_NetworkTechnology";
    app = new DuiApplication(argc, &app_name);
    qRegisterMetaType<NetworkTechnology::Technology>("NetworkTechnology::Technology");
}

void Ut_NetworkTechnology::cleanupTestCase()
{
    delete app;
}

void Ut_NetworkTechnology::checkSpy(int count, NetworkTechnology::Technology technology)
{
    qDebug() << "count: " << count << ", technology: " << technology;
    QCOMPARE(spy->count(), count); // make sure the signal was emitted 'count' time(s)
    if(count > 0) {
        QList<QVariant> arguments = spy->takeFirst(); // take the first signal        
        qDebug() << "arg: " << arguments.at(0).value<NetworkTechnology::Technology>();
        QVERIFY(arguments.at(0).value<NetworkTechnology::Technology>() == technology);
    }
}

void Ut_NetworkTechnology::testNetworkIcon()
{
    // Test 2G icon
    radioAccessStub->setTechnology(RadioAccess::GSM);
    m_subject->networkCell->setServices(NetworkCell::GPRSSupport);
    QTest::qWait(NetworkTechnologyCheckInterval + 50);
    checkSpy(1, NetworkTechnology::TwoG);

    // Test 2.5G icon
    m_subject->networkCell->setServices(NetworkCell::EGPRSSupport);
    QTest::qWait(NetworkTechnologyCheckInterval - 10);
    checkSpy(); // test that no signal sent yet
    QTest::qWait(10 + 50);
    checkSpy(1, NetworkTechnology::TwoPointFiveG);

    // Test 3G icon
    radioAccessStub->setTechnology(RadioAccess::UMTS);
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    m_subject->networkCell->setServices(NetworkCell::GPRSSupport); //make timer to restart
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    checkSpy(); // test that no signal sent yet
    QTest::qWait(10 + 50);
    checkSpy(1, NetworkTechnology::ThreeG);

    // Test 3.5G icon
    m_subject->networkCell->setServices(NetworkCell::HSDPASupport);
    QTest::qWait(NetworkTechnologyCheckInterval - 50);
    radioAccessStub->setState(RadioAccess::AllocatedHSDPA); //make timer to restart
    QTest::qWait(NetworkTechnologyCheckInterval - 50);    
    checkSpy(); // test that no signal sent yet
    QTest::qWait(10 + 50);
    checkSpy(1, NetworkTechnology::ThreePointFiveG);
}

QTEST_APPLESS_MAIN(Ut_NetworkTechnology);
