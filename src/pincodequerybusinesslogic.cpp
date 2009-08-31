#include "pincodequerybusinesslogic.h"
#include <SIM>
#include <SIMLock>
#include <DuiLocale>
#include <DuiButton>
#include <DuiTextEdit>
#include <DuiApplicationWindow>
#include <DuiMenu>
#include <DuiNavigationBar>
#include <call-ui/calluiserviceapi.h>

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
    QString SIMCardInserted = trid("NOT DOCUMENTED YET", "SIM card inserted.");
    QString SIMCardNotInserted = trid("NOT DOCUMENTED YET" , "SIM card not inserted.");
    QString SIMCardRemoved = trid("NOT DOCUMENTED YET" , "SIM card removed.");
    QString SIMCardRejected = trid("qtn_cell_sim_rejected" , "SIM card rejected.");    
    QString EmergencyCallStarting = trid("qtn_cell_emergency_start" , "Starting emergency call.");
    QString EmergencyCallEnded = trid("qtn_cell_emergency_end" , "Emergency call ended.");

    /* At the moment this contains several error case that may pop up from SIM library.
       It needs to be clarified, which of those and how are they going to be reported
       to end user. */
    QString TechnicalProblem = trid("NOT DOCUMENTED YET" , "Technical problem.");
    QString PINCodeIncorrect2AttemptsLeft = trid("qtn_cell_incorrect_pin_2" , "Incorrect PIN code. 2 attempts remaining.");
    QString PINCodeIncorrect1AttemptLeft = trid("qtn_cell_incorrect_pin_1" , "Incorrect PIN code. 1 attempt remaining.");
    QString PINCodeIncorrectPUKCodeRequired = trid("qtn_cell_incorrect_pin_0" , "Incorrect PIN code. SIM locked with PUK code.");
    QString PUKCodeIncorrect = trid("qtn_cell_incorrect_puk" , "Incorrect PUK code.");
    QString PINCodeChanged = trid("qtn_cell_pin_code_changed" , "PIN code changed.");
    QString PINCodesDoNotMatch = trid("qtn_cell_codes_not_match" , "Codes do not match.");
    QString SIMUnlocked = trid("qtn_cell_sim_unlocked" , "SIM card unlocked");
    QString SIMLocked = trid("qtn_cell_sim_lock_notification", "This SIM card can not be used in the device.");
}

PinCodeQueryBusinessLogic::PinCodeQueryBusinessLogic() : QObject()
{    
    qDebug() << "business logic";

    win = new DuiApplicationWindow();

    Qt::WindowFlags flags = 0;
	flags |= Qt::FramelessWindowHint;

    win->setWindowFlags(flags);
    
    win->menu()->disappear();
    win->navigationBar()->disappear();
    win->setWindowOpacity(0);
    win->setVisible(false);

    EmergencyNumbers emeNums;
    QStringList emergencyNumbers = emeNums.numbers();

    uiNotif = new Notifier();
    uiPin = new PinCodeQueryUI(emergencyNumbers);
    DuiButton *uiPinEmergency = uiPin->getEmergencyBtn();
    DuiButton *uiPinCancel = uiPin->getCancelBtn();
    DuiButton *uiPinEnter = uiPin->getEnterBtn();
    DuiTextEdit *uiPinEntry = uiPin->getCodeEntry();

    connect(uiPinEntry, SIGNAL(textChanged()), this, SLOT(uiCodeChanged()));
    connect(uiPinEmergency, SIGNAL(released()), this, SLOT(uiButtonReleased()));
    connect(uiPinEnter, SIGNAL(released()), this, SLOT(uiButtonReleased()));
    connect(uiPinCancel, SIGNAL(released()), this, SLOT(uiButtonReleased()));


    sim = new SIM();
    simId = new SIMIdentity();
    simSec = new SIMSecurity();        
    simLock = new SIMLock();

    qRegisterMetaType<SIM::SIMStatus>("SIM::SIMStatus");
    qRegisterMetaType<SIMError>("SIMError");

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
    delete uiNotif;
    uiNotif = NULL;
    delete uiPin;
    uiPin = NULL;
    delete win;
    win = NULL;
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
}



// =======================================
// User Interface drivers by state machine
// =======================================

void PinCodeQueryBusinessLogic::ui2SIMLocked()
{    
    uiPin->getCancelBtn()->setEnabled(true);
    win->show();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_unlock_code",
                          "Enter code for unlocking SIM card"));
    uiNotif->showNotification(SIMLocked);
}
void PinCodeQueryBusinessLogic::ui2firstPINAttempt()
{
    qDebug() << "ui2first...";
    if (SIMhotswapped)
        uiNotif->showNotification(SIMCardInserted);

    uiPin->getCancelBtn()->setEnabled(true);
    win->show();
    win->menu()->disappear();
    win->navigationBar()->disappear();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_pin_code", "Enter PIN code"));
    uiPin->getEmergencyBtn()->hide();
}
void PinCodeQueryBusinessLogic::ui2PINFailed(int attemptsLeft)
{
    switch (attemptsLeft) {
    case 2:
        uiNotif->showNotification(PINCodeIncorrect2AttemptsLeft);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_2",
                              "Enter PIN code. 2 attempts remaining."));
        break;
    case 1:
        uiNotif->showNotification(PINCodeIncorrect1AttemptLeft);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_1",
                              "Enter PIN code. 1 attempt remaining."));
        break;
    };
}
void PinCodeQueryBusinessLogic::ui2PINFailedNowPUK()
{
    uiNotif->showNotification(PINCodeIncorrectPUKCodeRequired);
    uiPin->setHeader(trid("qtn_cell_enter_puk_code", "Enter PUK code"));
}
void PinCodeQueryBusinessLogic::ui2firstPUKAttempt()
{
    if (SIMhotswapped)
        uiNotif->showNotification(SIMCardInserted);

    win->show();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_PUK_code", "Enter PUK code"));
    uiPin->getCancelBtn()->setEnabled(true);
}
void PinCodeQueryBusinessLogic::ui2PUKFailed(int attemptsLeft)
{
    attemptsLeft++; // warnings off
    uiNotif->showNotification(PUKCodeIncorrect);
}
void PinCodeQueryBusinessLogic::ui2PUKFailedPermanently()
{
    uiNotif->showNotification(SIMCardRejected);
    ui2disappear();
}
void PinCodeQueryBusinessLogic::ui2PUKOk()
{
    qDebug() << "PUK OK";
    // new pin needed!
    subState = SubEnterNewPIN;

    uiPin->setHeader(trid("qtn_cell_enter_new_pin", "Enter new PIN code"));
    uiPin->getCancelBtn()->setEnabled(false);
}

void PinCodeQueryBusinessLogic::ui2disappear()
{
    //win->hide();
    //uiPin->hide();    
    uiPin->disappear();
}
void PinCodeQueryBusinessLogic::ui2disappearWithNotification(QString notifText)
{
    ui2disappear();
    uiNotif->showNotification(notifText);
}




// ==========================
// Slots
// ==========================

void PinCodeQueryBusinessLogic::uiCodeChanged()
{
    uiPin->getEnterBtn()->setEnabled(true);
    if (previousSimState == SIM::PINRequired
        || previousSimState == SIM::Ok) { // new pin or re-enter new pin
        uiPin->getEnterBtn()->setEnabled(
            uiPin->getCodeEntry()->text().length() >= 4);
    }
}

void PinCodeQueryBusinessLogic::uiButtonReleased()
{
    QString code = uiPin->getCodeEntry()->text();

    DuiButton* button = static_cast<DuiButton*>(this->sender());
    if(button->objectName() == QString("emergencyCallButton")) {
        CallUiServiceApi* callUi = new CallUiServiceApi();
        callUi->Call(NULL, code);
        delete callUi;
    }
    else if(button->objectName() == QString("enterButton")) {
        switch(previousSimState) {
        case SIM::SIMLockRejected:
            simLock->simLockUnlock(SIMLock::LevelGlobal, uiPin->getCodeEntry()->text());
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
                } else {
                    subState = SubEnterNewPIN;
                    uiPin->setHeader(trid("qtn_cell_enter_new_pin",
                                          "Enter new PIN code"));
                    uiNotif->showNotification(PINCodesDoNotMatch);
                }
            }
        break;
        }

        uiPin->getCodeEntry()->setText("");
    }
    else if(button->objectName() == QString("cancelButton")) {
        // regardless of the state - just exit.
        uiPin->disappear();
    }
}




void PinCodeQueryBusinessLogic::simStatusChanged(SIM::SIMStatus next)
{
    qDebug() << "sim status changed" << next;

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
            subState = SubFirstTry;
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
            uiNotif->showNotification(SIMCardRemoved);
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
    if(handleSIMLockError(error))
        uiNotif->showNotification(SIMUnlocked);

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
        uiNotif->showNotification(PUKCodeIncorrect);
}

void PinCodeQueryBusinessLogic::simPINCodeChanged(bool success, SIMError error)
{
    handleSIMError(error);

    if(success) {
        uiNotif->showNotification(PINCodeChanged);
        uiPin->disappear();
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
        uiNotif->showNotification(SIMCardNotInserted);
        break;        
    case SIMErrorPINDisabled:
    case SIMErrorCodeBlocked:
    case SIMErrorWrongPassword:
        // no need to show notification
        break;
    case SIMErrorSIMRejected:
        uiNotif->showNotification(SIMCardRejected);
        break;

    //TODO: What to do with these???
    case SIMErrorSecurityCodeRequired:
    case SIMErrorBusCommunication:
    case SIMErrorInvalidParameter:
    case SIMErrorModemNotReady:
    case SIMErrorServiceBusy:
    case SIMErrorModem:
    case SIMErrorDataNotAvailable:
    case SIMErrorUnknown:
    default:
        //not necessarily to be shown to end user
        uiNotif->showNotification(TechnicalProblem);
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
    case SIMLockErrorWrongPassword: /*!< Incorrect unlock code */

        // NOTIFICATION NEEDED. VAPPU WILL PROVIDE.

        break;
    case SIMLockErrorBusCommunication: /*!< Returned when a D-Bus communication error with the Cellular Services daemon occured */
    case SIMLockErrorInvalidParameter: /*!< Invalid parameter values */
    case SIMLockErrorModemNotReady: /*!< Cellular modem not ready */
    case SIMLockErrorWrongLevel: /*!< Requested level not found */
    case SIMLockErrorNotAllowed: /*!< Action not allowed. E.g. the lock does not exist */
    case SIMLockErrorTimerNotElapsed: /*!< Unlocking timer not elapsed (too fast retry) */
    case SIMLockErrorUnknown: /*!< General SIM lock unlocking failure */
    default:
        //not necessarily to be shown to end user
        uiNotif->showNotification(TechnicalProblem);
        break;
    }
    return success;
}
