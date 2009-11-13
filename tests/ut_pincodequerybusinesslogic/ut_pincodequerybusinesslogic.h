#ifndef UT_PINCODEQUERYBUSINESSLOGIC_H
#define UT_PINCODEQUERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "pincodequerybusinesslogic.h"
#include "pinstubs.h"

typedef SIM SIMStub;
typedef SIMSecurity SIMSecurityStub;
typedef SIMLock SIMlockStub;

class Ut_PinCodeQueryBusinessLogic : public QObject
{
    Q_OBJECT

public:
    PinCodeQueryBusinessLogic* m_subject;

signals:
    void uiButtonReleased();

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
    void testPuk();
    void testSimLock();

private:
    void stateChange(SIM::SIMStatus);
    void testPinOk();
    void testPinNok(QString code, QString errorText);
    void testPukOk();
    void testPukNok(QString code, QString errorText);
    void testSimLockOk();
    void testSimLockNok(QString code, QString errorText);
    void emitButtonReleased(QString buttonName);
    void testUiClosed();

private:
    SIMStub *m_sim;
    SIMSecurityStub *m_simSec;
    SIMLock *m_simLock;
    PinCodeQueryDBusAdaptor *m_dbus;
};

#endif
