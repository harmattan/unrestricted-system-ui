#ifndef UT_BATTERYBUSINESSLOGIC_H
#define UT_BATTERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "batterybusinesslogic.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "ledstub.h"

class Ut_BatteryBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testSetPSMThreshold();
    void testTogglePSM();
    void testTogglePSMAuto();

private:
    SystemUIGConf *systemUIGConf;
    BatteryBusinessLogic* m_subject;
};

#endif
