#ifndef UT_PINCODEQUERYBUSINESSLOGIC_H
#define UT_PINCODEQUERYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

#include "pincodequerybusinesslogic.h"
#include "pinstubs.h"

using namespace CallUi;

typedef SIM SIMStub;
typedef SIMSecurity SIMSecurityStub;
typedef SIMLock SIMLockStub;
typedef CallUiServiceApi CallUiServiceApiStub;

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
    void testFailedLaunchPinQuery();
    void testLaunchPinQuery();
    void testUiControls_PIN();
    void testUiControls_PUK();
    void testUiControls_ChangePIN();
    void testEnablePinQuery();
    void testChangePinCode();
    void testPinToPuk();
    void testSimLock();
    // test notifications
    void testErrorAtPUKQuery();
    void testErrorAtPINQuery();
    void testTechnicalProblem();
    void testSIMLockRetry();
    void testEmergencyCall();

private:
    void doAndVerifyStateChange(SIM::SIMStatus);
    void doAndVerifyPinOk();
    void doAndVerifyPinNok(QString code, QString errorText);
    void emitButtonReleased(QString buttonName);

private:
    SIMStub *m_sim;
    SIMSecurityStub *m_simSec;
    SIMLockStub *m_simLock;
    PinCodeQueryDBusAdaptor *m_dbus;
    CallUiServiceApiStub *m_callApi;
};

#endif
