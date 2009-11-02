#include "ut_batterybusinesslogic.h"

#include <DuiApplication>
#include <DuiTheme>
#include <DuiLocale>

namespace {
    const QString PSMActivateText = trid("qtn_ener_aps", "Activate power save now");
    const QString PSMDeactivateText = trid("qtn_ener_dps", "Deactivate power save now");
}

void Ut_BatteryBusinessLogic::init()
{    
    systemUIGConf = new SystemUIGConf();    
    m_subject = new BatteryBusinessLogic(systemUIGConf);
}

void Ut_BatteryBusinessLogic::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete systemUIGConf;
    systemUIGConf = NULL;
}

DuiApplication *app;
void Ut_BatteryBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_batterybusinesslogic";
    app = new DuiApplication(argc, &app_name);
}

void Ut_BatteryBusinessLogic::cleanupTestCase()
{
    delete app;
}

void Ut_BatteryBusinessLogic::testSetPSMThreshold()
{
    const QString test = "10";
    m_subject->setPSMThreshold(test);
    QCOMPARE(m_subject->PSMThresholdValue(), test);
}

void Ut_BatteryBusinessLogic::testTogglePSM()
{    
    QSignalSpy spy(m_subject, SIGNAL(PSMAutoValueChanged(bool)));

    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));
    m_subject->togglePSM(PSMActivateText);
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), false);

    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));
    m_subject->togglePSM(PSMDeactivateText);
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), false);
}

void Ut_BatteryBusinessLogic::testPSMValue()
{
    //toggle the PSM on
    m_subject->togglePSM(PSMActivateText);
    QCOMPARE(m_subject->PSMValue(), PSMDeactivateText);

    //toggle the PSM off
    m_subject->togglePSM(PSMDeactivateText);
    QCOMPARE(m_subject->PSMValue(), PSMActivateText);
}

void Ut_BatteryBusinessLogic::testTogglePSMAuto()
{    
    //init the gconf value to true
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    //toggle the PSM on
    m_subject->togglePSM(PSMActivateText);
    QCOMPARE(m_subject->PSMValue(), PSMDeactivateText);

    //test togglePSMAuto with false toggle
    m_subject->togglePSMAuto(false);

    //check that the values have changed
    QCOMPARE(m_subject->PSMValue(), PSMActivateText);
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);    
}
/*
void Ut_BatteryBusinessLogic::testBatteryBarValue()
{
    QmBattery battery
    for(int i=100; i>-1; --i) {
        int index = m_subject->batteryBarValue(i);



if(percentage == -1)
        percentage = battery->getBatteryEnergyLevel();

    int index = 0;
    if(percentage >= 88)
        index = PSMThresholds.indexOf("100");
    else if(percentage < 88 && percentage >= 75)
        index = PSMThresholds.indexOf("85");
    else if(percentage < 75 && percentage >= 63)
        index = PSMThresholds.indexOf("75");
    else if(percentage < 63 && percentage >= 50)
        index = PSMThresholds.indexOf("60");
    else if(percentage < 50 && percentage >= 38)
        index = PSMThresholds.indexOf("50");
    else if(percentage < 38 && percentage >= 25)
        index = PSMThresholds.indexOf("35");
    else if(percentage < 25 && percentage >= 13)
        index = PSMThresholds.indexOf("25");
    else if(percentage < 13 && percentage >= 10)
        index = PSMThresholds.indexOf("15");
    else if(percentage < 10 && percentage >= 5)
        index = PSMThresholds.indexOf("10");
    else if(percentage < 5)
        index = PSMThresholds.indexOf("5");

    return index;
*/

QTEST_APPLESS_MAIN(Ut_BatteryBusinessLogic)
