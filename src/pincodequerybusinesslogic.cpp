#include "pincodequerybusinesslogic.h"
#include "callhandler.h"
#ifndef UNIT_TEST
    #include "pincodequerydbusadaptor.h"
#endif // UNIT_TEST
#include <DuiLocale>
#include <DuiButton>
#include <DuiDialog>
#include <DuiTextEdit>
#include <DuiApplicationWindow>
#include <QRegExpValidator>

#include <QtDBus>
#include <ssc-dbus-names.h>
#include <EmergencyNumbers>

#include "sysuid.h"

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
 *
 *
 * TODOs:
 *  2. Logical name for string "SIM card not inserted."
 *
 */

namespace {
    QString SIMCardNotInserted = trid("NOT DOCUMENTED YET" , "SIM card not inserted.");
    QString SIMCardRejected = trid("qtn_cell_sim_rejected" , "SIM card rejected.");    

    QString EmergencyCallHeader = trid("qtn_cell_start_emergency_call" , "Start Emergency call?");
    QString HeaderEnterPinCode = trid("qtn_cell_enter_pin_code", "Enter PIN code");
    QString HeaderEnterPinCode1Left = trid("qtn_cell_enter_pin_code_1", "Enter PIN code. 1 attempts remaining.");
    QString HeaderEnterPinCode2Left = trid("qtn_cell_enter_pin_code_2", "Enter PIN code. 2 attempts remaining.");
    QString HeaderEnterPinCodeNew = trid("qtn_cell_enter_new_pin", "Enter new PIN code");
    QString HeaderEnterPinCodeReNew = trid("qtn_cell_reenter_new_pin", "Re-enter new PIN code");
    QString HeaderEnterPukCode = trid("qtn_cell_enter_puk_code", "Enter PUK code");
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

PinCodeQueryBusinessLogic::PinCodeQueryBusinessLogic(QObject* parent) :
        QObject(parent),
        subState(SubNothing),
        previousSimState(-1),
        queryLaunch(false),
        initialized(false),
        callUi(NULL)

{
    qDebug() << Q_FUNC_INFO;

    simSec = new SIMSecurity(this);
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
    connect(simSec, SIGNAL(pinQueryStateComplete(SIMSecurity::PINQuery, SIMError)),
            this, SLOT(simPinQueryStateComplete(SIMSecurity::PINQuery, SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(enablePINQueryComplete(SIMError)),
            this, SLOT(simEnablePINQueryComplete(SIMError)), Qt::QueuedConnection);
    connect(simSec, SIGNAL(disablePINQueryComplete(SIMError)),
            this, SLOT(simEnablePINQueryComplete(SIMError)), Qt::QueuedConnection);

    simLock = new SIMLock(this);
    connect(simLock, SIGNAL(simLockUnlockComplete(SIMLockError)),
            this, SLOT(simLockUnlockCodeVerified(SIMLockError)));

    sim = new SIM(this);
    // bootstrap the state machine
    connect(sim, SIGNAL(statusComplete(SIM::SIMStatus, SIMError)),
        this, SLOT(simStatusComplete(SIM::SIMStatus, SIMError)), Qt::QueuedConnection);
    connect(sim, SIGNAL(statusChanged(SIM::SIMStatus)),
        this, SLOT(simStatusChanged(SIM::SIMStatus)), Qt::QueuedConnection);

    dbus = new PinCodeQueryDBusAdaptor(this);

    callUi = new CallHandler(this);
    connect(callUi, SIGNAL(callStarted()), this, SLOT(callStarted()));
    connect(callUi, SIGNAL(callDone()), this, SLOT(callDone()));

    sim->status();
}

PinCodeQueryBusinessLogic::~PinCodeQueryBusinessLogic()
{    
    delete uiPin;
    uiPin = NULL;
}

void PinCodeQueryBusinessLogic::createUi(bool enableBack)
{
    qDebug() << Q_FUNC_INFO << "existing:" << static_cast<QObject*> (uiPin);
    PinCodeQueryUI::setWindowOnTop(!enableBack);

    if(!uiPin)
    {
        EmergencyNumbers emeNums;
        uiPin = new PinCodeQueryUI(emeNums.numbers());

        uiPin->setPannableAreaInteractive(false);
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
        connect(uiPin, SIGNAL(backButtonClicked()), this, SLOT(cancelQuery()));

        /* Unfortunately no effect with libdui-dev version 0.11.1-1+0rebuild1+0m6 */
        qDebug() << Q_FUNC_INFO << "created:" << static_cast<QObject*> (uiPin);
    }
    uiPin->setBackButtonEnabled(enableBack);
    uiPin->getEnterBtn()->setEnabled(false);

    DuiApplicationPage::DisplayMode mod = 0;
    if(enableBack){
        mod = DuiApplicationPage::EscapeButtonVisible;
    }
    uiPin->setDisplayMode( mod );
    PinCodeQueryUI::showWindow();
    uiPin->appearNow(/*DuiSceneWindow::DestroyWhenDone*/);
}

void PinCodeQueryBusinessLogic::closeUi()
{
    qDebug() << Q_FUNC_INFO;
    subState = SubNothing;
    oldPinCode = "";
    newPinCode = "";
    qDebug() << Q_FUNC_INFO << "uiPin->isVisible()" << (uiPin ? uiPin->isVisible() : (bool)0);
    if(uiPin)
    {
        uiPin->getCancelBtn()->setEnabled(true);
        uiPin->getCodeEntry()->setText("");
        uiPin->disappearNow();
    }
    PinCodeQueryUI::hideWindow();
}

void PinCodeQueryBusinessLogic::setUiHeader(QString headerText)
{
    if(uiPin)
        uiPin->setHeader(headerText);
}

void PinCodeQueryBusinessLogic::setPinHeader(int attemptsLeft)
{
    qDebug() << Q_FUNC_INFO << attemptsLeft;
    switch (attemptsLeft) {
    case 2:
        setUiHeader(HeaderEnterPinCode2Left);
        break;
    case 1:
        setUiHeader(HeaderEnterPinCode1Left);
        break;
    };
}

void PinCodeQueryBusinessLogic::doEmergencyCall()
{
    qDebug() << Q_FUNC_INFO;

    DuiDialog* dlg = new DuiDialog();
    dlg->setTitle(QString(trid("qtn_cell_start_emergency_call", "Start emergency call?")));
    DuiButton* callButton = dlg->addButton(QString(trid("qtn_cell_emergency_call_number", "Call")));
    dlg->addButton(DuiDialog::Cancel);

    qDebug() << "\n" << Q_FUNC_INFO
            << "****************** PIN Query: About to call to ("
            << CallHandler::envVar() << ") :"
            << getenv( CallHandler::envVar() ) <<  "***************\n";
    dlg->exec();

    if(dlg->clickedButton() == callButton){
        callUi->startCall();
    }

    dlg->deleteLater();
    qDebug() << Q_FUNC_INFO << "out";
}

void PinCodeQueryBusinessLogic::callStarted()
{
    qDebug() << Q_FUNC_INFO;
    // TODO: Possibly better solution is to un/set flag Qt::WindowStaysOnTopHint but
    PinCodeQueryUI::hideWindow();
}

void PinCodeQueryBusinessLogic::callDone()
{
    qDebug() << Q_FUNC_INFO;
    PinCodeQueryUI::showWindow();
    uiPin->appear();
}

// =======================================
// User Interface drivers by state machine
// =======================================

void PinCodeQueryBusinessLogic::ui2SIMLocked(bool showNote)
{    
    createUi();
    uiPin->getEnterBtn()->setEnabled(true);
    setUiHeader(HeaderEnterSimLockCode);
    QRegExp rx("\\d{0,15}", Qt::CaseInsensitive); // max 15 digits
    uiPin->getCodeEntry()->setValidator(new QRegExpValidator(rx, uiPin->getCodeEntry()));

    if(showNote) {
        emit showNotification(SIMLocked, NotificationType::error);
    }
}
void PinCodeQueryBusinessLogic::ui2PINQuery()
{
    subState = SubFirstTry;
    getCode(false, HeaderEnterPinCode);
    simSec->pinAttemptsLeft(SIMSecurity::PIN);
}

void PinCodeQueryBusinessLogic::ui2PINFailed(int attemptsLeft)
{
    qDebug() << Q_FUNC_INFO << attemptsLeft;
    switch(attemptsLeft){
    case -1:
        // nothing
        break;
    case 2:
        emit showNotification(PINCodeIncorrect2AttemptsLeft, NotificationType::error);
        break;
    case 1:
        emit showNotification(PINCodeIncorrect1AttemptLeft, NotificationType::error);
        break;
    default:
        emit showNotification(PINCodeIncorrectSettings, NotificationType::error);
    };
    uiPin->getEnterBtn()->setEnabled(false);
}

void PinCodeQueryBusinessLogic::ui2PUKQuery()
{
    getCode(false, HeaderEnterPukCode);
}

void PinCodeQueryBusinessLogic::ui2enterNewPin()
{
    qDebug() << Q_FUNC_INFO;
    bool enableCancel = (SubEnterOldPIN == subState);
    subState = SubEnterNewPIN;
    createUi(enableCancel);
    setUiHeader(HeaderEnterPinCodeNew);
    uiPin->getCancelBtn()->setEnabled(enableCancel);
}

void PinCodeQueryBusinessLogic::ui2disappear(bool operationOk)
{
    qDebug() << Q_FUNC_INFO << "queried:" << queryLaunch << "state:" << previousSimState;
    emitLaunchResponse(operationOk);
    closeUi();
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
    } else{
        uiPin->getEnterBtn()->setEnabled(true);
    }
    qDebug() << Q_FUNC_INFO << "state:" << previousSimState << "len:" << len <<
            "enter enabled:" << uiPin->getEnterBtn()->isEnabled();
}

void PinCodeQueryBusinessLogic::uiButtonReleased()
{
    QString code = uiPin->getCodeEntry()->text();
    const QString buttonName = this->sender()->objectName();

    if(buttonName == QString("emergencyCallButton")) {
        doEmergencyCall();
   }
    else if(buttonName == QString("enterButton")) {
        if(SubEnablePinQuery == subState){
            simSec->enablePINQuery(SIMSecurity::PIN, uiPin->getCodeEntry()->text());
        } else if(SubDisablePinQuery == subState){
            simSec->disablePINQuery(SIMSecurity::PIN, uiPin->getCodeEntry()->text());
        } else {
           switch(previousSimState) {
            case SIM::SIMLockRejected:
                simLockCode = uiPin->getCodeEntry()->text();
                simLock->simLockUnlock(SIMLock::LevelGlobal, simLockCode);
            break;
            case SIM::PINRequired:
                simSec->verifyPIN(SIMSecurity::PIN, uiPin->getCodeEntry()->text());
            break;
            case SIM::PUKRequired:
                oldPinCode = "1234";
                simSec->verifyPUK(SIMSecurity::PUK, uiPin->getCodeEntry()->text(), oldPinCode);
            break;
            case SIM::Ok:
               if (subState == SubEnterOldPIN) {
                    oldPinCode = uiPin->getCodeEntry()->text();
                    simSec->verifyPIN(SIMSecurity::PIN, oldPinCode);
                } else if (subState == SubEnterNewPIN) {
                    subState = SubReEnterNewPIN;
                    newPinCode = uiPin->getCodeEntry()->text();
                    uiPin->getEnterBtn()->setEnabled(false);
                    setUiHeader(HeaderEnterPinCodeReNew);
                } else if (subState == SubReEnterNewPIN) {
                    if (newPinCode == uiPin->getCodeEntry()->text()) {
                        simSec->changePIN(SIMSecurity::PIN, oldPinCode, newPinCode);
                        ui2disappear(true);
                        return;
                    } else {
                        subState = SubEnterNewPIN;
                        setUiHeader(HeaderEnterPinCodeNew);
                        emit showNotification(PINCodesDoNotMatch, NotificationType::error);
                    }
                }
            break;
            } // switch
       }
        uiPin->getCodeEntry()->setText("");
    }
    else if(buttonName == QString("cancelButton")) {
        cancelQuery();
    }
}

void PinCodeQueryBusinessLogic::simStatusChanged(SIM::SIMStatus next)
{
    qDebug() << Q_FUNC_INFO << "sim status changed to" << next << "from" << previousSimState << "subState" << subState;

    // clear simLockCode
    if (SIM::SIMLockRejected != next) {
        simLockCode = "";
    }

    /* removed when hot swapping not supported.
    // sim inserted
    if (SIM::NoSIM == previousSimState && SIM::NoSIM != next) {
        emit showNotification(trid("qtn_cell_sim_inserted", "SIM card inserted."));
    } */

    if(initialized){
        bool closeUi = stateOperation(next, previousSimState);
        if(closeUi){
            previousSimState = next;
            ui2disappear(true);
        }
    }

    previousSimState = next;
}

bool PinCodeQueryBusinessLogic::stateOperation(int status, int relationState)
{
    qDebug() << Q_FUNC_INFO << "(" << status << ", " << relationState << ")";

    bool closeUi = false;

    switch(status)
    {
        case (SIM::Ok):
            if(SIM::PUKRequired == relationState) {
                ui2enterNewPin();
            } else {
                closeUi = true;
            }
            break;
        case (SIM::PINRequired):
            ui2PINQuery();
            break;
        case (SIM::PUKRequired):
            if(SIM::PINRequired == relationState){
                emit showNotification(PINCodeIncorrectPUKCodeRequired, NotificationType::error);
            }
            ui2PUKQuery();
            break;
        case (SIM::NoSIM):
            // removed when hot swapping not supported.
            // emit showNotification(trid("qtn_cell_sim_removed" , "SIM card removed. Cellular network is not available."));
            closeUi = true;
            break;
        case (SIM::SIMLockRejected):
            ui2SIMLocked();
            break;
        case (SIM::Rejected):
            // notification??
        case (SIM::PermanentlyBlocked):
            emit showNotification(SIMCardRejected, NotificationType::error);
        case (SIM::UnknownStatus):
        case (SIM::NotReady):
        default:
            closeUi = true;
            break;
    }
    return closeUi;
}

// called only in bootstrap
void PinCodeQueryBusinessLogic::simStatusComplete(SIM::SIMStatus status, SIMError error)
{
    qDebug() << Q_FUNC_INFO << status;

    if (handleSIMError(error)) {
        simStatusChanged(status);
    }
}


void PinCodeQueryBusinessLogic::simPINCodeVerified(bool success, SIMError error)
{   
    qDebug() << Q_FUNC_INFO << "sim status completed" << success << error;
    handleSIMError(error);

    if(!success) {
        if (error == SIMErrorWrongPassword) {
            if(SubEnterOldPIN == subState) {
                emit showNotification(PINCodeIncorrectSettings, NotificationType::error);
                ui2disappear(false);
            } else {
                if(SubFirstTry == subState){
                    subState = SubFailedTry;
                }
                simSec->pinAttemptsLeft(SIMSecurity::PIN);
            }
        }
        // else if sim locked is handled in status change
    }
    else if(SubEnterOldPIN == subState) {
        ui2enterNewPin();
    }
    else {
        ui2disappear(true);
    }
}

void PinCodeQueryBusinessLogic::simPUKCodeVerified(bool success, SIMError error)
{
    qDebug() << Q_FUNC_INFO << success << error;

    handleSIMError(error);

    if(!success && error == SIMErrorWrongPassword) {
        simSec->pukAttemptsLeft(SIMSecurity::PUK);
    } else if (!success){
        ui2disappear(false);
    } else {
        // in success case we just emit the signal and we'll ask the new PIN code
        emitLaunchResponse(true);
    }
}

void PinCodeQueryBusinessLogic::simLockUnlockCodeVerified(SIMLockError error)
{
    if(handleSIMLockError(error)) {
        emit showNotification(SIMUnlocked, NotificationType::info);
        simLockCode = "";
        // should we emit launch done signal here?
    }else{
        //ui2disappear(false);
    }
    //what is the next SIMStatus?
}

void PinCodeQueryBusinessLogic::simPINAttemptsLeft(int attempts, SIMError error)
{
    qDebug() << Q_FUNC_INFO << attempts << error;
    if(handleSIMError(error)){
        setPinHeader(attempts);
        if(SubFailedTry == subState){
            ui2PINFailed(attempts);
        }
    } else {
        ui2disappear(false);
    }
}

void PinCodeQueryBusinessLogic::simPUKAttemptsLeft(int attempts, SIMError error)
{
    qDebug() << Q_FUNC_INFO << attempts << ":" << error;
    handleSIMError(error);

    // warnings...
    attempts++;
    if(attempts > 0) {//if no more attempts, showing only the rejected notification
        uiPin->getEnterBtn()->setEnabled(false);
        emit showNotification(PUKCodeIncorrect, NotificationType::error);
    }
}

void PinCodeQueryBusinessLogic::simPINCodeChanged(bool success, SIMError error)
{
    handleSIMError(error);

    if(success) {
        emit showNotification(PINCodeChanged, NotificationType::info);
        ui2disappear(true);
    }
}

void PinCodeQueryBusinessLogic::simPinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error)
{
    qDebug() << Q_FUNC_INFO << "at subState" << subState << "state:" << state << "error" << error;
    // just queried, forward info
    if(SubPinQueryState == subState){
        dbus->pinQueryStateCompletedResponse(state, error);
        return;
    }

    // response to query
    if( SIMErrorNone != error ||
       (SIMSecurity::Enabled == state && SubEnablePinQuery == subState) ||
       (SIMSecurity::Disabled == state && SubDisablePinQuery == subState) ){
        qDebug() << Q_FUNC_INFO << "return: despiting error or state already requested one";
        dbus->pinQueryEnabledResponse(state);
        subState = SubNothing;
    } else {
        qDebug() << Q_FUNC_INFO << "ask PIN to change code";
        // ask PIN to change code
        getCode(true, HeaderEnterPinCode);
        simSec->pinAttemptsLeft(SIMSecurity::PIN);
    }
}

void PinCodeQueryBusinessLogic::simEnablePINQueryComplete(SIMError error)
{
    SIMSecurity::PINQuery state = SIMSecurity::UnknownState;
    bool success = handleSIMError(error);
    if (success) {
        if( SubEnablePinQuery == subState ){
            state = SIMSecurity::Enabled;
        }
        else if (SubDisablePinQuery == subState){
            state = SIMSecurity::Disabled;
        }
    } else {
        if (error == SIMErrorWrongPassword) {
            emit showNotification(PINCodeIncorrectSettings, NotificationType::error);
        }
    }
    dbus->pinQueryEnabledResponse(state);
    this->ui2disappear(success);
    qDebug() << Q_FUNC_INFO << state;
}

bool PinCodeQueryBusinessLogic::handleSIMError(SIMError error)
{
    bool success = false;

    switch(error) {
    case SIMErrorNone:
        success = true;
        break;
    case SIMErrorNoSIM:
        emit showNotification(SIMCardNotInserted, NotificationType::error);
        break;        
    case SIMErrorPINDisabled:
    case SIMErrorCodeBlocked:
    case SIMErrorWrongPassword:
        // no need to show notification
        break;
    case SIMErrorSIMRejected:
        emit showNotification(SIMCardRejected, NotificationType::error);
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
        qDebug() << Q_FUNC_INFO << "(" << error << ")";
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
        emit showNotification(SIMLockIncorrect, NotificationType::error);
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
        qDebug() << Q_FUNC_INFO << ":" << error;
        informTechnicalProblem();
        break;
    }
    return success;
}

void PinCodeQueryBusinessLogic::informTechnicalProblem()
{    
    emit showConfirmation(TechnicalProblem, trid("qtn_cell_continue", "Continue"));
}

// Offers an option for user to try enter the unlock code again.
// Tapping the button should try to enter the already typed unlock code directly again.
void PinCodeQueryBusinessLogic::simLockRetry()
{    
    emit showConfirmation(SIMLockTooFast, trid("qtn_cell_try_again", "Try again"));
    if(uiPin)
        uiPin->getCodeEntry()->setText(simLockCode);
}

void PinCodeQueryBusinessLogic::cancelQuery()
{
    qDebug() << Q_FUNC_INFO ;    
    // regardless of the state - just exit.
    ui2disappear(false);
}

void PinCodeQueryBusinessLogic::resendSimLockCode()
{
    qDebug() << Q_FUNC_INFO << ":" << simLockCode;    
    ui2SIMLocked(false);
    uiPin->getCodeEntry()->setText(simLockCode);
    if(!simLockCode.isEmpty() && SIM::SIMLockRejected == previousSimState) {
        simLock->simLockUnlock(SIMLock::LevelGlobal, simLockCode);
    }
}

// PIN code can be changed onli if SIM state is ok.
void PinCodeQueryBusinessLogic::changePinCode()
{
    qDebug() << Q_FUNC_INFO << ":" << previousSimState;

    if(SIM::Ok == previousSimState) {
        subState = SubEnterOldPIN;
        getCode(true, HeaderEnterPinCode);
        simSec->pinAttemptsLeft(SIMSecurity::PIN);
    }
}

void PinCodeQueryBusinessLogic::enablePinQueryRequested(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    if(enabled){
        subState = SubEnablePinQuery;
    } else {
        subState = SubDisablePinQuery;
    }
    simSec->pinQueryState(SIMSecurity::PIN);
}

bool PinCodeQueryBusinessLogic::launchPinQuery(SIMSecurity::PINType pinType)
{
    Q_UNUSED(pinType);
    qDebug() << Q_FUNC_INFO << "state" << previousSimState << "subState" << subState;

    initialized = true;
    if(queryLaunch){
        return false;
    }
    startLaunch();
    qDebug() << Q_FUNC_INFO << "return:" << queryLaunch;
    return queryLaunch;
}

void PinCodeQueryBusinessLogic::startLaunch()
{
    if(!queryLaunch && (
       SIM::PINRequired == previousSimState ||
       SIM::PUKRequired == previousSimState ||
       SIM::SIMLockRejected == previousSimState) ){
        queryLaunch = true;
        stateOperation(previousSimState, previousSimState);
    }
}

void PinCodeQueryBusinessLogic::getCode(bool enableBack, QString header)
{
    createUi(enableBack);
    QRegExp rx("\\d{0,8}", Qt::CaseInsensitive); // max 8 digits
    uiPin->getCodeEntry()->setValidator(new QRegExpValidator(rx, uiPin->getCodeEntry()));
    setUiHeader(header);
}

void PinCodeQueryBusinessLogic::emitLaunchResponse(bool ok)
{
    qDebug() << Q_FUNC_INFO << "emit:" << queryLaunch << "Response:" << ok;
    if(queryLaunch){
        dbus->pinQueryDoneResponse(ok);
        queryLaunch = false;
    }
}

void PinCodeQueryBusinessLogic::pinQueryState(SIMSecurity::PINType pinType)
{
    subState = SubPinQueryState;
    simSec->pinQueryState(pinType);
}
