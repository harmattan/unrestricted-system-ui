#include "ut_pincodequerybusinesslogic.h"
#include "pincodequeryui.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
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
    m_subject = new PinCodeQueryBusinessLogic(this);
    connect(this, SIGNAL(uiButtonReleased()), m_subject, SLOT(uiButtonReleased()));

    m_sim = m_subject->sim;
    m_simSec = m_subject->simSec;
    m_simLock = m_subject->simLock;
    m_dbus = m_subject->dbus;
}

void Ut_PinCodeQueryBusinessLogic::cleanup()
{
    delete m_subject;
    m_subject = NULL;

    m_sim = NULL;
    m_simSec = NULL;
    m_simLock = NULL;
    m_dbus = NULL;
}

DuiApplication *app;
DuiApplicationWindow *win;
void Ut_PinCodeQueryBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_pincodequerybusinesslogic";
    app = new DuiApplication(argc, &app_name);

    win = new DuiApplicationWindow();
    Qt::WindowFlags flags = 0;
        flags |= Qt::FramelessWindowHint;// | Qt::WindowStaysOnTopHint;
    win->setWindowOpacity(0);
    win->setWindowFlags(flags);

    qRegisterMetaType<NotificationType::Type>("NotificationType::Type");
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

void Ut_PinCodeQueryBusinessLogic::emitButtonReleased(QString buttonName)
{
    QString tmp = this->objectName();
    this->setObjectName(buttonName);
    emit uiButtonReleased();
    this->setObjectName(tmp);
}

void Ut_PinCodeQueryBusinessLogic::testUiClosed()
{
    QCOMPARE(m_subject->subState, PinCodeQueryBusinessLogic::SubNothing);
    QCOMPARE(m_subject->uiPin->isVisible(), false);
}

///////////////// TESTS ///////////////////

void Ut_PinCodeQueryBusinessLogic::testStateChange()
{
    qDebug() << Q_FUNC_INFO << "m_sim" << (QObject*) m_sim;

    // test initial value; m_sim initialized to SIM::NotReady
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));

    stateChange(SIM::UnknownStatus);

    SIM::SIMStatus newState = SIM::NotReady;

    // test error case
    m_subject->simStatusComplete(newState, SIMErrorBusCommunication);
    QCOMPARE(m_subject->previousSimState, (int)(m_sim->simStatus));

    // test ok case
    m_subject->simStatusComplete(newState, SIMErrorNone);
    QCOMPARE(m_subject->previousSimState, (int)newState);
}

void Ut_PinCodeQueryBusinessLogic::testUiControls()
{
    // enable UI operations
    m_subject->initialized = true;

    // check button enability
    stateChange(SIM::PINRequired);
    QCOMPARE(m_subject->uiPin.isNull(), false);
    DuiButton *enter = m_subject->uiPin.data()->getEnterBtn();
    DuiButton *cancel = m_subject->uiPin.data()->getCancelBtn();

    // test enter button enablity
    QString pin("12");
    m_subject->uiPin->getCodeEntry()->setText(pin);
    QCOMPARE(enter->isEnabled(), false);
    QCOMPARE(cancel->isEnabled(), true);

    m_subject->uiPin->getCodeEntry()->setText(pin+pin);
    QCOMPARE(enter->isEnabled(), true);
    QCOMPARE(cancel->isEnabled(), true);

    // clean UI
    emitButtonReleased("cancelButton");

    stateChange(SIM::Ok);
    m_subject->changePinCode();

    QCOMPARE(m_subject->uiPin.isNull(), false);
    enter = m_subject->uiPin.data()->getEnterBtn();
    cancel = m_subject->uiPin.data()->getCancelBtn();
    QCOMPARE(enter->isEnabled(), false);
    QCOMPARE(cancel->isEnabled(), true);

    m_subject->uiPin->getCodeEntry()->setText(m_simSec->pin);
    emitButtonReleased("enterButton");

    QCOMPARE(enter->isEnabled(), false);
    QCOMPARE(cancel->isEnabled(), false);

    m_subject->uiPin->getCodeEntry()->setText(pin);
    QCOMPARE(enter->isEnabled(), false);
    QCOMPARE(cancel->isEnabled(), false);

    m_subject->uiPin->getCodeEntry()->setText(pin+pin);
    QCOMPARE(enter->isEnabled(), true);
    QCOMPARE(cancel->isEnabled(), false);
}

void Ut_PinCodeQueryBusinessLogic::testLaunchPinQuery()
{
    m_simSec->pinAttempts = 4;
    m_simSec->pin = QString("1234");

    m_dbus->queryStarted = false;

    // Test no start
    stateChange(SIM::NotReady);
    bool resp = m_subject->launchPinQuery(SIMSecurity::PIN);
    QCOMPARE(resp, false);
    QCOMPARE(m_dbus->queryStarted, false);

    // Test start
    stateChange(SIM::PINRequired);
    resp = m_subject->launchPinQuery(SIMSecurity::PIN);
    QCOMPARE(resp, true);
    QCOMPARE(m_dbus->queryStarted, false);

    // Test nok signal
    testPinNok(QString("4321"), PINCodeIncorrectSettings);
    emitButtonReleased("cancelButton");
    QCOMPARE(m_dbus->queryStarted, false);

    // Test ok signal
    resp = m_subject->launchPinQuery(SIMSecurity::PIN);
    QCOMPARE(resp, true);
    QCOMPARE(m_dbus->queryStarted, false);

    testPinOk();
    testUiClosed();
    QCOMPARE(m_dbus->queryStarted, true);
}

void Ut_PinCodeQueryBusinessLogic::testEnablePinQuery()
{
    QSKIP("Not implemented", SkipSingle);
}

void Ut_PinCodeQueryBusinessLogic::testPinQueryStateChanges()
{
    QSKIP("Not implemented", SkipSingle);
}

void Ut_PinCodeQueryBusinessLogic::testChangePinCode()
{
    QSKIP("Not implemented", SkipSingle);

    stateChange(SIM::Ok);
    m_subject->changePinCode();

    m_subject->uiPin->getCodeEntry()->setText(m_simSec->pin);
    emitButtonReleased("enterButton");

    QString newPin("1111");
    m_subject->uiPin->getCodeEntry()->setText(newPin);
    QCOMPARE(m_simSec->pin, newPin);
}


void Ut_PinCodeQueryBusinessLogic::testNotifications()
{
    QSKIP("Not implemented", SkipSingle);
}


void Ut_PinCodeQueryBusinessLogic::testPin()
{
    QSKIP("Not implemented", SkipSingle);
}

void Ut_PinCodeQueryBusinessLogic::testPinOk()
{
    m_simSec->pin = QString("1234");
    if(SIM::PINRequired != m_subject->previousSimState){
        this->stateChange(SIM::PINRequired);
    }
    m_subject->initialized = true;

    m_subject->uiPin->getCodeEntry()->setText(m_simSec->pin);
    emitButtonReleased("enterButton");

    testUiClosed();
}

void Ut_PinCodeQueryBusinessLogic::testPinNok(QString code, QString errorText)
{
    QSignalSpy spy(m_subject, SIGNAL(showNotification(QString, NotificationType::Type)));

    if(SIM::PINRequired != m_subject->previousSimState){
        this->stateChange(SIM::PINRequired);
    }
    m_subject->initialized = true;

    m_subject->uiPin->getCodeEntry()->setText(code);
    emitButtonReleased("enterButton");

    qDebug() << Q_FUNC_INFO << "spy.count()" << spy.count();

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toString() == errorText); // verify the first argument
}

void Ut_PinCodeQueryBusinessLogic::testPuk()
{
    QSKIP("Not implemented", SkipSingle);
}

void Ut_PinCodeQueryBusinessLogic::testPukOk()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testPukNok(QString code, QString errorText)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code << errorText;
}

void Ut_PinCodeQueryBusinessLogic::testSimLock()
{
    QSKIP("Not implemented", SkipSingle);
}

void Ut_PinCodeQueryBusinessLogic::testSimLockOk()
{
    qDebug() << Q_FUNC_INFO << "dummy";
}

void Ut_PinCodeQueryBusinessLogic::testSimLockNok(QString code, QString errorText)
{
    qDebug() << Q_FUNC_INFO << "dummy" << code << errorText;
}

QTEST_APPLESS_MAIN(Ut_PinCodeQueryBusinessLogic)
