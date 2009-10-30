#include "ut_lowbatterynotifier.h"
#include "batterybusinesslogic.h"
#include "displaystatestub.h"

#include <DuiApplication>
#include <DuiTheme>

#include <QTime>
#include <QThread>

namespace {
    const int Act = 1000;
    const int Inact = 2000;
}

void LowBatteryHelper::notificationShown()
{
    qDebug() << "PERSE LowBatteryHelper::notificationShown(): " << count;
    ++count;
}

void Ut_LowBatteryNotifier::init()
{            
    m_subject = new LowBatteryNotifier();
}

void Ut_LowBatteryNotifier::cleanup()
{
    delete m_subject;
    m_subject = NULL;    
}

DuiApplication *app;
void Ut_LowBatteryNotifier::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_lowbatterynotifier";
    app = new DuiApplication(argc, &app_name);
}

void Ut_LowBatteryNotifier::cleanupTestCase()
{
    delete app;
}

void Ut_LowBatteryNotifier::testShowNotification()
{
    m_helper = new LowBatteryHelper();
    connect(m_subject, SIGNAL(showNotification(QString)), m_helper, SLOT(notificationShown()));
    QSignalSpy spy(m_subject, SIGNAL(showNotification(QString)));
    m_subject->display->set(Maemo::QmDisplayState::On);
    m_subject->activeInterval = Act;
    m_subject->inactiveInterval = Inact;

    time.start();    
    m_subject->showLowBatteryNotification();
    QCOMPARE(m_helper->count, 1);
    QTest::qWait(Act/2);
    QCOMPARE(m_helper->count, 1);
    qDebug() << "time elapsed: " << m_subject->time.elapsed();
    QTest::qWait(Act/2 + 10);
    QCOMPARE(m_helper->count, 2);
    qDebug() << "time elapsed: " << m_subject->time.elapsed();
    QTest::qWait(Act/2);
    QCOMPARE(m_helper->count, 2);
    qDebug() << "time elapsed: " << m_subject->time.elapsed();
    QTest::qWait(Act/2);
    QCOMPARE(m_helper->count, 3);
    qDebug() << "time elapsed: " << m_subject->time.elapsed();


    // TO BE CONTINUED...

}


QTEST_APPLESS_MAIN(Ut_LowBatteryNotifier)
