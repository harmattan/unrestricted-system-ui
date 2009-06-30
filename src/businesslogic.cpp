#include "businesslogic.h"

BusinessLogic::BusinessLogic(PinCodeQuery *p, Notifier *n) : QObject()
{
    uiPin = p;
    uiNotif = n;

    sim = new SIM();
    simId = new SIMIdentity();
    simSec = new SIMSecurity();
    //simPb = new SIMPhonebook();
    qRegisterMetaType<SIM::SIMStatus>("SIM::SIMStatus");
    qRegisterMetaType<SIMError>("SIMError");

    connect(sim, SIGNAL(StatusChanged(SIM::SIMStatus)),
            this, SLOT(simStatusChanged(SIM::SIMStatus)), Qt::QueuedConnection);
    connect(sim, SIGNAL(StatusComplete(SIM::SIMStatus, SIMError)),
            this, SLOT(simStatusComplete(SIM::SIMStatus, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(VerifyPINComplete(bool, SIMError)),
            this, SLOT(PINCodeVerified(bool, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(VerifyPUKComplete(bool, SIMError)),
            this, SLOT(PUKCodeVerified(bool, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(PINAttemptsLeftComplete(int, SIMError)),
            this, SLOT(PINAttemptsLeft(int, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(PUKAttemptsLeftComplete(int, SIMError)),
            this, SLOT(PUKAttemptsLeft(int, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(ChangePINComplete(bool, SIMError)),
            this, SLOT(PINCodeChanged(bool, SIMError)), Qt::QueuedConnection);

    connect(uiPin, SIGNAL(codeEntered(PinCodeQuery::UIState, QString)),
            this, SLOT(codeEntered(PinCodeQuery::UIState, QString)));

    previousSimState = -1;
    sim->Status();

}

BusinessLogic::~BusinessLogic()
{
}

// =====
// Slots
// ======

void BusinessLogic::simStatusChanged(SIM::SIMStatus next)
{
    qDebug() << "sim status changed" << next;

    switch (previousSimState) {
    case -1: { // bootstrap
    break;
    }
    case SIM::NotReady:
    case SIM::UnknownStatus: {
        if (next == SIM::PINRequired) {
            uiPin->changeUIState(PinCodeQuery::UIPINState);
            uiPin->appear();            
        }
        else if (next == SIM::PUKRequired) {
            uiPin->changeUIState(PinCodeQuery::UIPUKState);
            uiPin->appear();            
        }
        else if(next == SIM::Rejected || next == SIM::PermanentlyBlocked)
            uiNotif->showNotification(Notifier::SIMCardRejected);        
        break;
    }
    case SIM::Ok:
        if (next == SIM::NoSIM)
            uiNotif->showNotification(Notifier::SIMCardRemoved);
        break;
    case SIM::NoSIM:
        if (next == SIM::PINRequired) {            
            uiPin->changeUIState(PinCodeQuery::UIPINState);
            uiPin->appear();
        }
        else if (next == SIM::PUKRequired) {
            uiPin->changeUIState(PinCodeQuery::UIPUKState);
            uiPin->appear();
        }
        else if (next == SIM::Ok)
            uiNotif->showNotification(Notifier::SIMCardInserted);
        break;    
    case SIM::PINRequired:
        if (next == SIM::PUKRequired) {
            uiPin->changeUIState(PinCodeQuery::UIPUKState);
            uiPin->appear();
        }
        else if(next == SIM::Ok)
            uiPin->disappear();
        break;
    case SIM::PUKRequired:
        if(next == SIM::Ok )
            uiPin->changeUIState(PinCodeQuery::UIEnterNewPINState);
        else if(next == SIM::PermanentlyBlocked)
            uiPin->disappear();
        break;
    case SIM::Rejected:
    case SIM::PermanentlyBlocked:
        break;

    }
    previousSimState = next;
}

// called only in bootstrap
void BusinessLogic::simStatusComplete(SIM::SIMStatus status, SIMError error)
{
    qDebug() << "sim status completed" << status;
    simStatusChanged(status);
}

void BusinessLogic::codeEntered(PinCodeQuery::UIState uiState, QString code)
{
    qDebug() << "Pin Code entered";
    switch(uiState)
    {
    case PinCodeQuery::UIPINState:        
    case PinCodeQuery::UIPIN2AttemptsLeftState:
    case PinCodeQuery::UIPIN1AttemptLeftState:
        simSec->VerifyPIN(SIMSecurity::PIN, code);
        break;
    case PinCodeQuery::UIPUKState:
        simSec->VerifyPUK(SIMSecurity::PUK, code, "1234");
        break;
    case PinCodeQuery::UIEnterNewPINState:
        newPinCode = code;
        uiPin->changeUIState(PinCodeQuery::UIReEnterNewPINState);
        break;
    case PinCodeQuery::UIReEnterNewPINState:
        if(newPinCode == code) {//new pin code matches, we can save it
            simSec->ChangePIN(SIMSecurity::PIN, "1234", code);
            //now we need to catch the signal from SIMSecurity that the PIN is changed            
        }
        else {
            uiNotif->showNotification(Notifier::PINCodesDoNotMatch);
            uiPin->changeUIState(PinCodeQuery::UIEnterNewPINState);
        }
    default:
        break;
    }
}

void BusinessLogic::PINCodeVerified(bool success, SIMError error)
{
    qDebug() << "pin code verified ";

    if(!success) {
        Notifier::Notification notification = Notifier::Empty;
        mapSIMError(error, notification, PinCodeQuery::UIPINState);
        if(notification == Notifier::PINCodeIncorrect) {            
            simSec->PINAttemptsLeft(SIMSecurity::PIN);
            return; //No notificaton sent yet, will be sent in PINAttemptsLeft slot
        }
        if(notification != Notifier::Empty)
            uiNotif->showNotification(notification);
    }
    //If correct, we don't show any notification
}

void BusinessLogic::PUKCodeVerified(bool success, SIMError error)
{
    qDebug() << "puk code verified ";

    if(!success) {
        Notifier::Notification notification = Notifier::Empty;
        mapSIMError(error, notification, PinCodeQuery::UIPUKState);       
        if(notification == Notifier::PUKCodeIncorrect) {
            simSec->PUKAttemptsLeft(SIMSecurity::PUK);
            return; //No notificaton sent yet, will be sent in PUKAttemptsLeft slot
        }
        if(notification != Notifier::Empty)
            uiNotif->showNotification(notification);
    }
    //If correct, we don't show any notification
}

void BusinessLogic::PINAttemptsLeft(int attempts, SIMError error)
{
    qDebug() << "pin attempts left, attempts: " << attempts << "error: " << error;
    Notifier::Notification notification = Notifier::Empty;
    if(error == SIMErrorNone) {
        switch(attempts)
        {
        case 2:
            notification = Notifier::PINCodeIncorrect2AttemptsLeft;
            uiPin->changeUIState(PinCodeQuery::UIPIN2AttemptsLeftState);
            break;
        case 1:
            notification = Notifier::PINCodeIncorrect1AttemptLeft;
            uiPin->changeUIState(PinCodeQuery::UIPIN1AttemptLeftState);
            break;
        default: // == 0
            notification = Notifier::PINCodeIncorrectPUKCodeRequired;
        }
    }

    if(notification != Notifier::Empty)
        uiNotif->showNotification(notification);
}

void BusinessLogic::PUKAttemptsLeft(int attempts, SIMError error)
{
    Notifier::Notification notification = Notifier::PUKCodeIncorrect;
    if(error == SIMErrorNone && attempts == 0)
        notification = Notifier::PUKCodeIncorrectSIMCardPermanentlyBlocked;

    uiNotif->showNotification(notification);
}

void BusinessLogic::PINCodeChanged(bool success, SIMError error)
{
    if(success) {
        uiNotif->showNotification(Notifier::PINCodeChanged);
        uiPin->disappear();
    }
}

void BusinessLogic::mapSIMError(SIMError error, Notifier::Notification &notification, PinCodeQuery::UIState uiState)
{
    switch(error)
    {
    case SIMErrorNone:
        notification = Notifier::Empty;
        break;
    case SIMErrorNoSIM:
        notification = Notifier::SIMCardNotInserted;
        break;        
    case SIMErrorWrongPassword:
        if(uiState == PinCodeQuery::UIPINState)
            notification = Notifier::PINCodeIncorrect;
        else if(uiState == PinCodeQuery::UIPUKState)
            notification = Notifier::PUKCodeIncorrect;       
        break;
    case SIMErrorSIMRejected:
        notification = Notifier::SIMCardRejected;
        break;

    //TODO: What to do with these???
    case SIMErrorSecurityCodeRequired:
    case SIMErrorBusCommunication:
    case SIMErrorInvalidParameter:
    case SIMErrorModemNotReady:
    case SIMErrorServiceBusy:
    case SIMErrorModem:
    case SIMErrorDataNotAvailable:
    case SIMErrorPINDisabled:
    case SIMErrorCodeBlocked:
    case SIMErrorUnknown:
    default:
        notification = Notifier::TechnicalProblem; //not necessarily to be shown to end user
        break;
    }
}
