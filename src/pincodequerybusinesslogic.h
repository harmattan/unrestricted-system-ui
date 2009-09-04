#ifndef PINCODEQUERYBUSINESSLOGIC_H
#define PINCODEQUERYBUSINESSLOGIC_H

#include "pincodequeryui.h"
#include "notifier.h"

#include <QObject>
#include <SIM>
#include <SIMLock>
#include <EmergencyNumbers>

class DuiApplicationWindow;

using namespace Cellular;

class PinCodeQueryBusinessLogic : public QObject
{
    Q_OBJECT
public:

    /** Sub states are mini states to keep user interface going while
     * SIM state does not change.
     */
    enum SubStates {
        SubNothing,
        // State: -1 (bootstrap)
        // State: UnknownStatus,
        // State: Ok,
        SubEnterNewPIN,
        SubReEnterNewPIN,
        // State: NoSIM,
        // State: PermanentlyBlocked,
        // State: NotReady,
        // State: PINRequired,
        SubFirstTry,
        SubFailedTry,
        // State: PUKRequired,
        // State: Rejected
        // State: SIMLockReject
        SubUnlocked, //kytkykauppa
        /*
        UIPINState,
        UIPIN2AttemptsLeftState,
        UIPIN1AttemptLeftState,
        UIPUKState,
        */
    };


    explicit PinCodeQueryBusinessLogic();
    virtual ~PinCodeQueryBusinessLogic();

private: // attributes
    PinCodeQueryUI *uiPin;
    Notifier *uiNotif;
    QString newPinCode;

    bool SIMhotswapped;
    int subState;
    int previousSimState;

    SIM* sim;
    SIMIdentity* simId;
    SIMSecurity* simSec;
    SIMLock *simLock;

private: // methods
    bool handleSIMError(SIMError error);
    bool handleSIMLockError(SIMLockError error);

    // "empty" state changes.
    void nothing();
    void doEmergencyCall();

    void ui2SIMLocked();
    void ui2firstPINAttempt();
    void ui2PINFailedNowPUK();
    void ui2PINFailed(int attemptsLeft);
    void ui2firstPUKAttempt();
    void ui2PUKFailed(int attemptsLeft);
    void ui2PUKFailedPermanently();
    void ui2PUKOk();
    void ui2disappear();
    void ui2disappearWithNotification(QString notifText);
    void ui2reenterPIN();    


private slots:

    void uiCodeChanged();
    void uiButtonReleased();

    void simStatusChanged(SIM::SIMStatus status);
    void simStatusComplete(SIM::SIMStatus status, SIMError error);
    void simPINCodeVerified(bool success, SIMError error);
    void simPUKCodeVerified(bool success, SIMError error);
    void simPINAttemptsLeft(int attempts, SIMError error);
    void simPUKAttemptsLeft(int attempts, SIMError error);
    void simPINCodeChanged(bool success, SIMError error);
    void simLockUnlockCodeVerified(SIMLockError error);

};

#endif // PINCODEQUERYBUSINESSLOGIC_H
