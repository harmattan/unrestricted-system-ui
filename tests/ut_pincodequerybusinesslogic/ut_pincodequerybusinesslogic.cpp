#include "ut_pincodequerybusinesslogic.h"
#include "pincodequeryui.h"
#include "pincodequerydbusadaptor.h"

#include <DuiApplication>
#include <DuiTextEdit>
#include <DuiButton>
#include <DuiLocale>

#include <QVariant>
#include <QDebug>

#include "notificationtype.h"

using namespace Cellular;

namespace {
    QString SIMCardNotInserted = trid("NOT DOCUMENTED YET" , "SIM card not inserted.");
    QString SIMCardRejected = trid("qtn_cell_sim_rejected" , "SIM card rejected.");

    QString EmergencyCallHeader = trid("qtn_cell_start_emergency_call" , "Start Emergency call?");
    QString HeaderEnterPinCode = trid("qtn_cell_enter_pin_code", "Enter PIN code");
    QString HeaderEnterPinCode1Left = trid("qtn_cell_enter_pin_code_1", "Enter PIN code. 1 attempts remaining.");
    QString HeaderEnterPinCode2Left = trid("qtn_cell_enter_pin_code_2", "Enter PIN code. 2 attempts remaining.");
    QString HeaderEnterPinCodeNew = trid("qtn_cell_enter_new_pin", "Enter new PIN code");
    QString HeaderEnterPinCodeReNew = trid("qtn_cell_reenter_new_pin", "Re-enter new PIN code");
    QString HeaderEnterPukCode = trid("qtn_cell_enter_PUK_code", "Enter PUK code");
    QString HeaderEnterSimLockCode = trid("qtn_cell_enter_unlock_code", "Enter code for unlocking SIM card");

    /* At the moment this contains several error case that may pop up from SIM library.
       It needs to be clarified, how are they going to be reported to end user. */
    QString TechnicalProblem = trid("qtn_cell_general_sim_error", "Error with SIM card. Continue without SIM?");;
    QString PINCodeIncorrectSettings = trid("qtn_cell_incorrect_pin_settings" , "Incorrect PIN code.");

    QString PINCodeIncorrect2AttemptsLeft = trid("qtn_cell_incorrect_pin_2" , "Incorrect PIN code. 2 attempts remaining.");
    QString PINCodeIncorrect1AttemptLeft = trid("qtn_cell_incorrect_pin_1" , "Incorrect PIN code. 1 attempt remaining.");
    QString PINCodeIncorrectPUKCodeRequired = trid("qtn_cell_incorrect_pin_0" , "Incorrect PIN code. SIM locked with PUK code.");
    QString PUKCodeIncorrect = trid("qtn_cell_incorrect_puk" , "Incorrect PUK code.");
    QString PINCodeChanged = trid("qtn_cell_pin_code_changed" , "PIN code changed.");
    QString PINCodesDoNotMatch = trid("qtn_cell_codes_not_match" , "Codes do not match.");

    QString SIMUnlocked = trid("qtn_cell_sim_unlocked" , "SIM card unlocked");
    QString SIMLocked = trid("qtn_cell_sim_lock_notification", "This SIM card can not be used in the device.");
    QString SIMLockIncorrect = trid("qtn_cell_sim_lock_incorrect", "Incorrect unlock code.");
    QString SIMLockTooFast = trid("qtn_cell_too_fast_entry", "Error with unlock code. Trying again might help.");

}


void Ut_PinCodeQueryBusinessLogic::init()
{    
    m_sim = new SIMStub(this);
    m_simSec = new SIMSecurityStub(this);
    m_simLock = new SIMLock(this);

    m_subject = new PinCodeQueryBusinessLogic(m_sim, m_simSec, m_simLock, this);
}

void Ut_PinCodeQueryBusinessLogic::cleanup()
{
    delete m_subject;
    m_subject = NULL;

    delete m_sim;
    m_sim = NULL;
    delete m_simSec;
    m_simSec = NULL;
    delete m_simLock;
    m_simLock = NULL;
}

DuiApplication *app;
void Ut_PinCodeQueryBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_pincodequerybusinesslogic";
    app = new DuiApplication(argc, &app_name);

    m_simSec->pinAttempts = 4;
    m_simSec->pin = QString("1234");
}

void Ut_PinCodeQueryBusinessLogic::cleanupTestCase()
{
    delete app;
}

///////////////// HELPERS ///////////////////

void Ut_PinCodeQueryBusinessLogic::stateChange(SIM::SIMStatus status)
{
    m_sim->simStatus = status;
    m_sim->emitStatusChanged();
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));
}

///////////////// TESTS ///////////////////

void Ut_PinCodeQueryBusinessLogic::testStateChange()
{
    qDebug() << Q_FUNC_INFO;

    // test initial value; m_sim initialized to SIM::NotReady
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));

    m_sim->simStatus = SIM::UnknownStatus;
    m_sim->emitStatusChanged();
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));

    SIM::SIMStatus newState = SIM::NotReady;

    // test error case
    m_subject->simStatusComplete(newState, SIMErrorBusCommunication);
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));

    // test ok case
    m_subject->simStatusComplete(newState, SIMErrorNone);
    QCOMPARE(m_subject->previousSimState, (int)newState);
}

void Ut_PinCodeQueryBusinessLogic::testLaunchPinQuery()
{
    m_simSec->pinAttempts = 4;
    m_simSec->pin = QString("1234");

    // Test no start
    stateChange(SIM::NotReady);
    bool resp = m_subject->launchPinQuery(SIMSecurity::PIN);
    QCOMPARE(resp, false);

    // Test start
    stateChange(SIM::PINRequired);
    resp = m_subject->launchPinQuery(SIMSecurity::PIN);
    QCOMPARE(resp, true);

    // Test nok signal
    testPinNok(QString("4321"), PINCodeIncorrectSettings);

    m_subject->uiPin->getCancelBtn()->click();

    QSignalSpy spyNok(m_subject, SIGNAL(pinQueryDoneResponse(bool)));

    QCOMPARE(spyNok.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spyNok.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toBool() == false); // verify the first argument

    // Test ok signal
    testPinOk(QString("1234"));

    m_subject->uiPin->getCancelBtn()->click();

    QSignalSpy spyOk(m_subject, SIGNAL(pinQueryDoneResponse(bool)));

    QCOMPARE(spyOk.count(), 1); // make sure the signal was emitted exactly one time
    arguments = spyOk.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toBool() == false); // verify the first argument
}

void Ut_PinCodeQueryBusinessLogic::testUiControls()
{
    stateChange(SIM::PINRequired);

    QCOMPARE(m_subject->uiPin.isNull(), false);
    DuiButton *enter = m_subject->uiPin.data()->getEnterBtn();

    // init test data
    m_simSec->pinAttempts = 4;
    m_simSec->pin = QString("1234");

    // test enter button enablity
    QString pin("12");
    m_subject->uiPin->getCodeEntry()->setText(pin);
    QCOMPARE(enter->isEnabled(), false);

    m_subject->uiPin->getCodeEntry()->setText(pin+pin);
    QCOMPARE(enter->isEnabled(), true);

    // test error note for wrong PIN
    enter->click();
    QSignalSpy spy(m_subject, SLOT(showNotification(QString, NotificationType::Type)));

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toString() == PINCodeIncorrectSettings); // verify the first argument

    m_subject->uiPin->getCodeEntry()->setText(pin+pin);
}

void Ut_PinCodeQueryBusinessLogic::testEnablePinQuery()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testPinQueryStateChanges()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testChangePinCode()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}


void Ut_PinCodeQueryBusinessLogic::testNotifications()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}


void Ut_PinCodeQueryBusinessLogic::testPin()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testPinOk(QString code)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code;
    m_simSec->pin = QString("1234");

}

void Ut_PinCodeQueryBusinessLogic::testPinNok(QString code, QString errorText)
{
    m_subject->uiPin->getCodeEntry()->setText(code);
    m_subject->uiPin.data()->getEnterBtn()->click();
    QSignalSpy spy(m_subject, SLOT(showNotification(QString, NotificationType::Type)));

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toString() == errorText); // verify the first argument
}

void Ut_PinCodeQueryBusinessLogic::testPuk()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testPukOk(QString code)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code;
}

void Ut_PinCodeQueryBusinessLogic::testPukNok(QString code, QString errorText)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code << errorText;
}

void Ut_PinCodeQueryBusinessLogic::testSimLock()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testSimLockOk(QString code)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code;
}

void Ut_PinCodeQueryBusinessLogic::testSimLockNok(QString code, QString errorText)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code << errorText;
}

QTEST_APPLESS_MAIN(Ut_PinCodeQueryBusinessLogic)
