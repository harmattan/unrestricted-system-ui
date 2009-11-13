#ifndef UT_PINCODEQUERYBUSINESSLOGIC_H
#define UT_PINCODEQUERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "pincodequerybusinesslogic.h"
#include "simstubs.h"

class Ut_PinCodeQueryBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testStateChange();
    void testLaunchPinQuery();
    void testUiControls();
    void testEnablePinQuery();
    void testPinQueryStateChanges();
    void testChangePinCode();
    void testNotifications();
    void testPin();
    void testPinOk(QString code);
    void testPinNok(QString code, QString errorText);
    void testPuk();
    void testPukOk(QString code);
    void testPukNok(QString code, QString errorText);
    void testSimLock();
    void testSimLockOk(QString code);
    void testSimLockNok(QString code, QString errorText);

private:
    void stateChange(SIM::SIMStatus);

private:
    PinCodeQueryBusinessLogic* m_subject;
    SIMStub *m_sim;
    SIMSecurityStub *m_simSec;
    SIMLock *m_simLock;
};

#endif
