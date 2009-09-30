#include "pincodequerybusinesslogic.h"
#include <SIM>
#include <SIMLock>
#include <DuiLocale>
#include <DuiButton>
#include <DuiDialog>
#include <DuiTextEdit>
#include <DuiApplicationWindow>
#include <DuiApplication>
#include <call-ui/calluiserviceapi.h>

#include <QtDBus>
#include <ssc-dbus-names.h>

#include "sysuid.h"

using namespace CallUi;

/*! The pincode query business logic object is the glue object between
 * SIM and pincode query UI widgets. It knows something about both ends.
 * It knows how states are being changed in the SIM and drives pincode
 * query UI with that information.
 *
 * User interface is plain "view" in sense that it just shows things.
 * UI has minimal signalling build already, i.e. numpad signals code
 * entry and backspace button removes typed numbers.
 *
 * Other UI signals are connected on constructor and thus we get the
 * control into this one signle object.
 *
 *  'ui2' prefix is for state changes
 *  'sim' prefix is for SIM DBus callbacks.
 */

//Notification texts
namespace {
    QString SIMCardInserted = trid("qtn_cell_sim_inserted", "SIM card inserted.");
    QString SIMCardNotInserted = trid("NOT DOCUMENTED YET" , "SIM card not inserted.");
    QString SIMCardRemoved = trid("qtn_cell_sim_removed" , "SIM card removed. Cellular network is not available.");
    QString SIMCardRejected = trid("qtn_cell_sim_rejected" , "SIM card rejected.");    

    QString EmergencyCallHeader = trid("qtn_cell_start_emergency_call" , "Start Emergency call?");

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

PinCodeQueryBusinessLogic::PinCodeQueryBusinessLogic() : QObject()
{    
    qDebug() << "PinCodeQueryBusinessLogic()";

    uiNotif = Sysuid::notifier();
    qDebug() << "PinCodeQueryBusinessLogic::PinCodeQueryBusinessLogic connected to " << uiNotif;


    sim = new SIM();
    simId = new SIMIdentity();
    simSec = new SIMSecurity();        
    simLock = new SIMLock();

    connect(sim, SIGNAL(statusChanged(SIM::SIMStatus)),
            this, SLOT(simStatusChanged(SIM::SIMStatus)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(verifyPINComplete(bool, SIMError)),
            this, SLOT(simPINCodeVerified(bool, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(verifyPUKComplete(bool, SIMError)),
            this, SLOT(simPUKCodeVerified(bool, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(pinAttemptsLeftComplete(int, SIMError)),
            this, SLOT(simPINAttemptsLeft(int, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(pukAttemptsLeftComplete(int, SIMError)),
            this, SLOT(simPUKAttemptsLeft(int, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(changePINComplete(bool, SIMError)),
            this, SLOT(simPINCodeChanged(bool, SIMError)), Qt::QueuedConnection);
    connect(simLock, SIGNAL(simLockUnlockComplete(SIMLockError)),
            this, SLOT(simLockUnlockCodeVerified(SIMLockError)));

    // bootstrap the state machine
    previousSimState = -1;
    connect(sim, SIGNAL(statusComplete(SIM::SIMStatus, SIMError)),
            this, SLOT(simStatusComplete(SIM::SIMStatus, SIMError)),
            Qt::QueuedConnection);
    sim->status();

    SIMhotswapped = false;
}

PinCodeQueryBusinessLogic::~PinCodeQueryBusinessLogic()
{
    uiNotif = NULL; // don't delete, not owned!
    delete uiPin;
    uiPin = NULL;
    delete sim;
    sim = NULL;
    delete simId;
    simId = NULL;
    delete simSec;
    simSec = NULL;
    delete simLock;
    simLock = NULL;
}

void PinCodeQueryBusinessLogic::nothing()
{
    qDebug() << "nothing()";
}

void PinCodeQueryBusinessLogic::createUi()
{
    qDebug() << "createUi() existing:" << static_cast<QObject*> (uiPin);
    if(!uiPin)
    {
        uiPin = new PinCodeQueryUI();
        DuiButton *uiPinEmergency = uiPin->getEmergencyBtn();
        DuiButton *uiPinCancel = uiPin->getCancelBtn();
        uiPinCancel->setEnabled(true);
        DuiButton *uiPinEnter = uiPin->getEnterBtn();
        uiPinEnter->setEnabled(false);
        DuiTextEdit *uiPinEntry = uiPin->getCodeEntry();

        connect(uiPinEntry, SIGNAL(textChanged()), this, SLOT(uiCodeChanged()));
        connect(uiPinEmergency, SIGNAL(released()), this, SLOT(uiButtonReleased()));
        connect(uiPinEnter, SIGNAL(released()), this, SLOT(uiButtonReleased()));
        connect(uiPinCancel, SIGNAL(released()), this, SLOT(uiButtonReleased()));

        DuiApplication::instance()->applicationWindow()->show();
        uiPin->setPannableAreaInteractive(false);
        uiPin->appearNow(/*DuiSceneWindow::DestroyWhenDone*/);
        qDebug() << "createUi() created:" << static_cast<QObject*> (uiPin);
    }
    // temporary else here until the flag DuiSceneWindow::DestroyWhenDone can be taken in use at
    // DuiApplicationPage::appearNow(). See bug #137469
    else
    {
        qDebug() << "PinCodeQueryBusinessLogic::createUi() win isHidden():" << DuiApplication::instance()->applicationWindow()->isHidden();
        if(DuiApplication::instance()->applicationWindow()->isHidden()) {
            DuiApplication::instance()->applicationWindow()->show();
        }
        qDebug() << "PinCodeQueryBusinessLogic::createUi() uiPin->isVisible():" << uiPin->isVisible();
        if(!uiPin->isVisible()) {
            uiPin->appearNow(/*DuiSceneWindow::DestroyWhenDone*/);
        }
        // update button enablity
        uiCodeChanged();
        uiPin->getCancelBtn()->setEnabled(true);
    }
}

void PinCodeQueryBusinessLogic::doEmergencyCall()
{
    DuiDialog* dlg = new DuiDialog();
    dlg->setTitle(QString(trid("qtn_cell_start_emergency_call", "Start emergency call?")));
    DuiButton* callButton = dlg->addButton(QString(trid("qtn_cell_emergency_call_number", "Call")));
    dlg->addButton(DuiDialog::Cancel);
    dlg->exec();

    if(dlg->clickedButton() == callButton)
    {
        dlg->accept();

        // do call
        CallUiServiceApi* callUi = new CallUiServiceApi();
        callUi->Call(NULL, NULL);
        delete callUi;
    } else {
        dlg->reject();
    }

    dlg->deleteLater();
    dlg = NULL;
}

// =======================================
// User Interface drivers by state machine
// =======================================

void PinCodeQueryBusinessLogic::ui2SIMLocked(bool showNote)
{    
    createUi();
    uiPin->setHeader(trid("qtn_cell_enter_unlock_code",
                          "Enter code for unlocking SIM card"));
    if(showNote) {
        uiNotif->showNotification(SIMLocked, Notifier::error);
    }
}

void PinCodeQueryBusinessLogic::ui2firstPINAttempt()
{
    qDebug() << "ui2first...";
    if (SIMhotswapped)
        uiNotif->showNotification(SIMCardInserted, Notifier::info);

    subState = SubFirstTry;
    createUi();
    uiPin->setHeader(trid("qtn_cell_enter_pin_code", "Enter PIN code"));
}
void PinCodeQueryBusinessLogic::ui2PINFailed(int attemptsLeft)
{
    switch (attemptsLeft) {
    case 2:
        uiNotif->showNotification(PINCodeIncorrect2AttemptsLeft, Notifier::error);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_2",
                              "Enter PIN code. 2 attempts remaining."));
        break;
    case 1:
        uiNotif->showNotification(PINCodeIncorrect1AttemptLeft, Notifier::error);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_1",
                              "Enter PIN code. 1 attempt remaining."));
        break;
    };
}
void PinCodeQueryBusinessLogic::ui2PINFailedNowPUK()
{
    uiNotif->showNotification(PINCodeIncorrectPUKCodeRequired, Notifier::error);
    uiPin->setHeader(trid("qtn_cell_enter_puk_code", "Enter PUK code"));
}
void PinCodeQueryBusinessLogic::ui2firstPUKAttempt()
{
    if (SIMhotswapped)
        uiNotif->showNotification(SIMCardInserted, Notifier::info);

    createUi();
    uiPin->setHeader(trid("qtn_cell_enter_PUK_code", "Enter PUK code"));
}
void PinCodeQueryBusinessLogic::ui2PUKFailed(int attemptsLeft)
{
    attemptsLeft++; // warnings off
    uiNotif->showNotification(PUKCodeIncorrect, Notifier::error);
}
void PinCodeQueryBusinessLogic::ui2PUKFailedPermanently()
{
    uiNotif->showNotification(SIMCardRejected, Notifier::error);
    ui2disappear();
}
void PinCodeQueryBusinessLogic::ui2PUKOk()
{
    qDebug() << "PUK OK";
    // new pin needed!
    subState = SubEnterNewPIN;

    createUi();
    uiPin->setHeader(trid("qtn_cell_enter_new_pin", "Enter new PIN code"));
    uiPin->getCancelBtn()->setEnabled(false);
}

void PinCodeQueryBusinessLogic::ui2disappear()
{
    subState = SubNothing;
    if(uiPin)
    {
        qDebug() << "PinCodeQueryBusinessLogic::ui2disappear()";
        uiPin->getCodeEntry()->setText("");
        uiPin->disappearNow();
        DuiApplication::instance()->applicationWindow()->hide();
    }
        qDebug() << "PinCodeQueryBusinessLogic::ui2disappear() win isHidden():" << DuiApplication::instance()->applicationWindow()->isHidden();
}
void PinCodeQueryBusinessLogic::ui2disappearWithNotification(QString notifText)
{
    ui2disappear();
    uiNotif->showNotification(notifText, Notifier::info);
}




// ==========================
// Slots
// ==========================

void PinCodeQueryBusinessLogic::uiCodeChanged()
{
    int len = uiPin->getCodeEntry()->text().length();
    if (previousSimState == SIM::PINRequired
        || previousSimState == SIM::PUKRequired
        || previousSimState == SIM::Ok) { // new pin or re-enter new pin
        uiPin->getEnterBtn()->setEnabled( (len >= 4 && len <= 8) );
    } else {
        uiPin->getEnterBtn()->setEnabled(len > 0);
    }

}

void PinCodeQueryBusinessLogic::uiButtonReleased()
{
    QString code = uiPin->getCodeEntry()->text();

    DuiButton* button = static_cast<DuiButton*>(this->sender());
    if(button->objectName() == QString("emergencyCallButton")) {
        doEmergencyCall();
   }
    else if(button->objectName() == QString("enterButton")) {
        switch(previousSimState) {
        case SIM::SIMLockRejected:
            simLockCode = uiPin->getCodeEntry()->text();
            simLock->simLockUnlock(SIMLock::LevelGlobal, simLockCode);
        break;
        case SIM::PINRequired:            
            simSec->verifyPIN(SIMSecurity::PIN, uiPin->getCodeEntry()->text());
        break;
        case SIM::PUKRequired:
            simSec->verifyPUK(SIMSecurity::PUK, uiPin->getCodeEntry()->text(), "1234");
        break;
        case SIM::Ok:
            if (subState == SubEnterNewPIN) {
                subState = SubReEnterNewPIN;
                newPinCode = uiPin->getCodeEntry()->text();

                uiPin->setHeader(trid("qtn_cell_reenter_new_pin",
                                      "Re-enter new PIN code"));
            } else if (subState == SubReEnterNewPIN) {
                if (newPinCode == uiPin->getCodeEntry()->text()) {
                    subState = SubNothing;
                    simSec->changePIN(SIMSecurity::PIN, "1234", newPinCode);
                    ui2disappear();
                    // no need for cleanings anymore, just close
                    return;
                } else {
                    subState = SubEnterNewPIN;
                    uiPin->setHeader(trid("qtn_cell_enter_new_pin",
                                          "Enter new PIN code"));
                    uiNotif->showNotification(PINCodesDoNotMatch, Notifier::error);
                }
            }
        break;
        }

        uiPin->getCodeEntry()->setText("");
    }
    else if(button->objectName() == QString("cancelButton")) {
        cancelQuery();
    }
}




void PinCodeQueryBusinessLogic::simStatusChanged(SIM::SIMStatus next)
{
    qDebug() << "sim status changed" << next << "from" << previousSimState << "subState" << subState;

    // clear simLockCode
    if (next != SIM::SIMLockRejected) {
        simLockCode = "";
    }

    switch (previousSimState) {
    case -1: { // bootstrap
      /* When UI is launched up we may start in any state.
       */
        if (next == SIM::Ok)
            ; // XXX
        else if (next == SIM::NotReady)
            nothing();
        else if (next == SIM::SIMLockRejected)
            ui2SIMLocked();
        else if (next == SIM::PINRequired) {
            ui2firstPINAttempt();
        } else if (next == SIM::PUKRequired)
            ui2firstPUKAttempt();
        /*
        else if (next == SIM::NoSIM)
            ; // XXX (notification?)
        else if (next == SIM::Rejected)
            ; // XXX (notification?)
        else if (next == SIM::PermanentlyBlocked)
            ; // XXX (notification?)
        */
        break;
    }
    case SIM::UnknownStatus: {            
        if (next == SIM::NotReady)
            nothing();
        else if (next == SIM::SIMLockRejected)
            ui2SIMLocked();
        else if(next == SIM::NoSIM)
            nothing();
        break;
    }
    case SIM::NotReady: {
        if (next == SIM::Ok)
            ui2disappear();
        else if (next == SIM::SIMLockRejected)
            ui2SIMLocked();
        else if (next == SIM::PINRequired)
            ui2firstPINAttempt();
        else if (next == SIM::PUKRequired)
            ui2firstPUKAttempt();
        else if (next == SIM::NoSIM)
            nothing(); // XXX (notification?)
        /*
        else if (next == SIM::Rejected)
            ; // XXX (notification?)
        else if (next == SIM::PermanentlyBlocked)
            ; // XXX (notification?)
        */
        break;
    }
    case SIM::NoSIM:

        SIMhotswapped = true;

        if (next == SIM::PINRequired)
            ui2firstPINAttempt();
        else if (next == SIM::PUKRequired)
            ui2firstPUKAttempt();
        else if (next == SIM::SIMLockRejected)
            ui2SIMLocked();
        else if (next == SIM::Ok)
            ui2disappearWithNotification(SIMCardInserted);
        break;
    case SIM::PINRequired:
        if (next == SIM::PUKRequired)
            ui2PINFailedNowPUK();
        else if(next == SIM::Ok)
            ui2disappear();
        else if (next == SIM::NoSIM)
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::PUKRequired:
        if(next == SIM::Ok )
            ui2PUKOk();
        else if(next == SIM::PermanentlyBlocked)
            ui2PUKFailedPermanently();
        else if (next == SIM::NoSIM)
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::PermanentlyBlocked:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::SIMLockRejected:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            nothing();
        break;
    case SIM::Rejected:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::Ok:
        if (next == SIM::NoSIM)
            // if new PIN after PUK asked the UI is open here
            ui2disappearWithNotification(SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;

    }
    previousSimState = next;
}

// called only in bootstrap
void PinCodeQueryBusinessLogic::simStatusComplete(SIM::SIMStatus status, SIMError error)
{
    qDebug() << "sim status completed" << status;

    if (handleSIMError(error))
        simStatusChanged(status);
}


void PinCodeQueryBusinessLogic::simPINCodeVerified(bool success, SIMError error)
{   
    handleSIMError(error);

    if(!success) {
        if (error == SIMErrorWrongPassword)
            simSec->pinAttemptsLeft(SIMSecurity::PIN);
        // else if sim locked is handled in status change
    }
    else
        ui2disappear();
}

void PinCodeQueryBusinessLogic::simPUKCodeVerified(bool success, SIMError error)
{
    qDebug() << "puk code verified ";

    handleSIMError(error);

    if(!success && error == SIMErrorWrongPassword) {
        simSec->pukAttemptsLeft(SIMSecurity::PUK);
    }  
}

void PinCodeQueryBusinessLogic::simLockUnlockCodeVerified(SIMLockError error)
{
    if(handleSIMLockError(error)) {
        uiNotif->showNotification(SIMUnlocked, Notifier::info);
        simLockCode = "";
    }

    //what is the next SIMStatus?
}

void PinCodeQueryBusinessLogic::simPINAttemptsLeft(int attempts, SIMError error)
{
    if(handleSIMError(error))
        ui2PINFailed(attempts);
    //else
    //    ; // XXX
}

void PinCodeQueryBusinessLogic::simPUKAttemptsLeft(int attempts, SIMError error)
{
    handleSIMError(error);

    // warnings...
    attempts++;
    if(attempts > 0) //if no more attempts, showing only the rejected notification
        uiNotif->showNotification(PUKCodeIncorrect, Notifier::error);
}

void PinCodeQueryBusinessLogic::simPINCodeChanged(bool success, SIMError error)
{
    handleSIMError(error);

    if(success) {
        uiNotif->showNotification(PINCodeChanged, Notifier::info);
        ui2disappear();
    }
}

bool PinCodeQueryBusinessLogic::handleSIMError(SIMError error)
{
    bool success = false;

    switch(error) {
    case SIMErrorNone:
        success = true;
        break;
    case SIMErrorNoSIM:
        uiNotif->showNotification(SIMCardNotInserted, Notifier::error);
        break;        
    case SIMErrorPINDisabled:
    case SIMErrorCodeBlocked:
    case SIMErrorWrongPassword:
        // no need to show notification
        break;
    case SIMErrorSIMRejected:
        uiNotif->showNotification(SIMCardRejected, Notifier::error);
        break;

    case SIMErrorSecurityCodeRequired:
    case SIMErrorBusCommunication:
    case SIMErrorInvalidParameter:
    case SIMErrorModemNotReady:
    case SIMErrorServiceBusy:
    case SIMErrorModem:
    case SIMErrorDataNotAvailable:
    case SIMErrorUnknown:
    default:
        qDebug() << "PinCodeQueryBusinessLogic::handleSIMError(" << error << ")";
        informTechnicalProblem();
        break;
    }
    return success;
}

bool PinCodeQueryBusinessLogic::handleSIMLockError(SIMLockError error)
{
    bool success = false;

    switch(error) {
    case SIMLockErrorNone: /*!< Returned when no error happened and operation completed successfully */
    case SIMLockErrorAlreadyOpened: /*!< SIMlock already opened */
        success = true;
        break;
    case SIMLockErrorWrongPassword:
        uiNotif->showNotification(SIMLockIncorrect, Notifier::error);
        break;
    case SIMLockErrorTimerNotElapsed: /*!< Returned when a D-Bus communication error with the Cellular Services daemon occured */
        simLockRetry();
        break;
    case SIMLockErrorBusCommunication: /*!< Returned when a D-Bus communication error with the Cellular Services daemon occured */
    case SIMLockErrorInvalidParameter: /*!< Invalid parameter values */
    case SIMLockErrorModemNotReady: /*!< Cellular modem not ready */
    case SIMLockErrorWrongLevel: /*!< Requested level not found */
    case SIMLockErrorNotAllowed: /*!< Action not allowed. E.g. the lock does not exist */
    case SIMLockErrorUnknown: /*!< General SIM lock unlocking failure */
    default:
        qDebug() << "PinCodeQueryBusinessLogic::handleSIMLockError(" << error << ")";
        informTechnicalProblem();
        break;
    }
    return success;
}

void PinCodeQueryBusinessLogic::informTechnicalProblem()
{
    connect(uiNotif->responseObject(), SIGNAL(pinQueryCanceled()), this, SLOT(cancelQuery()));
    uiNotif->showConfirmation(TechnicalProblem, trid("qtn_cell_continue", "Continue"));
}

// Offers an option for user to try enter the unlock code again.
// Tapping the button should try to enter the already typed unlock code directly again.
void PinCodeQueryBusinessLogic::simLockRetry()
{
    connect(uiNotif->responseObject(), SIGNAL(doSimLockRetry()), this, SLOT(resendSimLockCode()));
    uiNotif->showConfirmation(SIMLockTooFast, trid("qtn_cell_try_again", "Try again"));
    if(uiPin)
        uiPin->getCodeEntry()->setText(simLockCode);
}

void PinCodeQueryBusinessLogic::cancelQuery()
{
    qDebug() << "PinCodeQueryBusinessLogic::cancelQuery()";
    disconnect(uiNotif->responseObject(), SIGNAL(pinQueryCanceled()), this, SLOT(cancelQuery()));
    // regardless of the state - just exit.
    ui2disappear();
    // Well, let's put the radio and cellular off.
    QDBusInterface ssc(SSC_DBUS_NAME, SSC_DBUS_PATH, SSC_DBUS_IFACE,
                       QDBusConnection::systemBus());
    ssc.call(QDBus::NoBlock, QString(SSC_DBUS_METHOD_SET_RADIO), QString("off"));
    // just in case before the uiPin is not deleted when hidden
    if(uiPin)
    {
        uiPin->getCodeEntry()->setText("");
    }
}

void PinCodeQueryBusinessLogic::resendSimLockCode()
{
    qDebug() << "PinCodeQueryBusinessLogic::resendSimLockCode()";
    disconnect(uiNotif->responseObject(), SIGNAL(doSimLockRetry()), this, SLOT(resendSimLockCode()));
    ui2SIMLocked(false);
    uiPin->getCodeEntry()->setText(simLockCode);
    if(!simLockCode.isEmpty() && SIM::SIMLockRejected == previousSimState) {
        simLock->simLockUnlock(SIMLock::LevelGlobal, simLockCode);
    }
}
