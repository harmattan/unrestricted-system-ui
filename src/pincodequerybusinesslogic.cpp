#include "pincodequerybusinesslogic.h"
#include <DuiLocale>
#include <DuiButton>
#include <DuiTextEdit>
#include <DuiApplicationWindow>
#include <DuiMenu>
#include <DuiNavigationBar>

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


PinCodeQueryBusinessLogic::PinCodeQueryBusinessLogic() : QObject()
{    
    qDebug() << "business logic";

    win = new DuiApplicationWindow();
    win->menu()->disappear();
    win->navigationBar()->disappear();
    win->setWindowOpacity(0);
    win->setVisible(false);

    uiNotif = new Notifier();
    uiPin = new PinCodeQueryUI();
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
    //simPb = new SIMPhonebook();

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

}

void PinCodeQueryBusinessLogic::nothing()
{
}



// =======================================
// User Interface drivers by state machine
// =======================================

void PinCodeQueryBusinessLogic::ui2SIMLocked()
{
    if (SIMhotswapped)
        uiNotif->showNotification(Notifier::SIMCardInserted);

    uiPin->getCancelBtn()->setEnabled(true);
    win->show();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_unlock_code",
                          "Enter code for unlocking SIM card"));
}
void PinCodeQueryBusinessLogic::ui2firstPINAttempt()
{
    qDebug() << "ui2first...";
    if (SIMhotswapped)
        uiNotif->showNotification(Notifier::SIMCardInserted);

    uiPin->getCancelBtn()->setEnabled(true);
    win->show();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_pin_code", "Enter PIN code"));
}
void PinCodeQueryBusinessLogic::ui2PINFailed(int attemptsLeft)
{
    switch (attemptsLeft) {
    case 2:
        uiNotif->showNotification(Notifier::PINCodeIncorrect2AttemptsLeft);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_2",
                              "Enter PIN code. 2 attempts remaining."));
        break;
    case 1:
        uiNotif->showNotification(Notifier::PINCodeIncorrect1AttemptLeft);
        uiPin->setHeader(trid("qtn_cell_enter_pin_code_1",
                              "Enter PIN code. 1 attempt remaining."));
        break;
    };
}
void PinCodeQueryBusinessLogic::ui2PINFailedNowPUK()
{
    uiNotif->showNotification(Notifier::PINCodeIncorrectPUKCodeRequired);
    uiPin->setHeader(trid("qtn_cell_enter_puk_code", "Enter PUK code"));
}
void PinCodeQueryBusinessLogic::ui2firstPUKAttempt()
{
    if (SIMhotswapped)
        uiNotif->showNotification(Notifier::SIMCardInserted);

    win->show();
    uiPin->appear();
    uiPin->setHeader(trid("qtn_cell_enter_PUK_code", "Enter PUK code"));
    uiPin->getCancelBtn()->setEnabled(true);
}
void PinCodeQueryBusinessLogic::ui2PUKFailed(int attemptsLeft)
{
    attemptsLeft++; // warnings off
    uiNotif->showNotification(Notifier::PUKCodeIncorrect);
}
void PinCodeQueryBusinessLogic::ui2PUKFailedPermanently()
{
    uiNotif->showNotification(Notifier::SIMCardPermanentlyBlocked);
    ui2disappear();
}
void PinCodeQueryBusinessLogic::ui2PUKOk()
{
    // new pin needed!
    subState = SubEnterNewPIN;

    uiPin->setHeader(trid("qtn_cell_enter_new_pin", "Enter new PIN code"));
    uiPin->getCancelBtn()->setEnabled(false);
}

void PinCodeQueryBusinessLogic::ui2disappear()
{
    //win->hide();
    uiPin->disappear();
}
void PinCodeQueryBusinessLogic::ui2disappearWithNotification(Notifier::Notification n)
{
    ui2disappear();
    uiNotif->showNotification(n);
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
        uiNotif->showNotification(Notifier::EmergencyCallStarting);
    }
    else if(button->objectName() == QString("enterButton")) {
        switch(previousSimState) {
        case SIM::SIMLockRejected:
            ; // XXX simSec->
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
            ui2disappearWithNotification(Notifier::SIMCardInserted);
        break;
    case SIM::PINRequired:
        if (next == SIM::PUKRequired)
            ui2PINFailedNowPUK();
        else if(next == SIM::Ok)
            ui2disappear();
        else if (next == SIM::NoSIM)
            ui2disappearWithNotification(Notifier::SIMCardRemoved);
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
            ui2disappearWithNotification(Notifier::SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::PermanentlyBlocked:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(Notifier::SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::SIMLockRejected:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(Notifier::SIMCardRemoved);
        else if (next == SIM::NotReady)
            nothing();
        break;
    case SIM::Rejected:
        if (next == SIM::NoSIM)
            ui2disappearWithNotification(Notifier::SIMCardRemoved);
        else if (next == SIM::NotReady)
            // XXX technical problem?
            ui2disappear();
        break;
    case SIM::Ok:
        if (next == SIM::NoSIM)
            uiNotif->showNotification(Notifier::SIMCardRemoved);
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

    checkSIMError(error);

    if (error == SIMErrorNone)
        simStatusChanged(status);
}


void PinCodeQueryBusinessLogic::simPINCodeVerified(bool success, SIMError error)
{   
    checkSIMError(error);       

    if(!success) {
        if (error == SIMErrorWrongPassword)
            simSec->pinAttemptsLeft(SIMSecurity::PIN);
        // else if sim locked is handled in status change
    }
}

void PinCodeQueryBusinessLogic::simPUKCodeVerified(bool success, SIMError error)
{
    qDebug() << "puk code verified ";

    checkSIMError(error);

    if(!success && error == SIMErrorWrongPassword) {
        simSec->pukAttemptsLeft(SIMSecurity::PUK);
    }
}

void PinCodeQueryBusinessLogic::simPINAttemptsLeft(int attempts, SIMError error)
{
    checkSIMError(error);

    if(error == SIMErrorNone)
        ui2PINFailed(attempts);
    //else
    //    ; // XXX
}

void PinCodeQueryBusinessLogic::simPUKAttemptsLeft(int attempts, SIMError error)
{
    checkSIMError(error);

    // warnings...
    attempts++;

    uiNotif->showNotification(Notifier::PUKCodeIncorrect);
}

void PinCodeQueryBusinessLogic::simPINCodeChanged(bool success, SIMError error)
{
    checkSIMError(error);

    if(success) {
        uiNotif->showNotification(Notifier::PINCodeChanged);
        uiPin->disappear();
    }
}

void PinCodeQueryBusinessLogic::checkSIMError(SIMError error)
{
    switch(error) {
    case SIMErrorNone:
        break;
    case SIMErrorNoSIM:
        uiNotif->showNotification(Notifier::SIMCardNotInserted);
        break;        
    case SIMErrorPINDisabled:
    case SIMErrorCodeBlocked:
    case SIMErrorWrongPassword:
        // not a real error case.
        break;
    case SIMErrorSIMRejected:
        uiNotif->showNotification(Notifier::SIMCardRejected);
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
        uiNotif->showNotification(Notifier::TechnicalProblem);
        break;
    }
}
