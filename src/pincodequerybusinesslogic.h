#ifndef PINCODEQUERYBUSINESSLOGIC_H
#define PINCODEQUERYBUSINESSLOGIC_H

#include "pincodequeryui.h"
#include "notificationtype.h"

#include <QObject>
#include <QPointer>
#include <SIM>
#include <SIMLock>

class DuiApplicationWindow;
class PinCodeQueryDBusAdaptor;
class QTimer;

using namespace Cellular;

class PinCodeQueryBusinessLogic : public QObject
{
    Q_OBJECT
public:

    /** Sub states are mini states to keep user interface going while
     * SIM state does not change.
     * enum SIMStatus { UnknownStatus, Ok, NoSIM, PermanentlyBlocked,
     *                  NotReady, PINRequired, PUKRequired, Rejected, SIMLockRejected };
     */
    enum SubStates {
        SubNothing,
        // State: -1 (bootstrap)
        // State: UnknownStatus,
        // State: Ok,
        SubEnterOldPIN,
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
        // any state
        SubEnablePinQuery,
        SubDisablePinQuery,
        /*
        UIPINState,
        UIPIN2AttemptsLeftState,
        UIPIN1AttemptLeftState,
        UIPUKState,
        */
    };

    explicit PinCodeQueryBusinessLogic(QObject* parent = 0);
    virtual ~PinCodeQueryBusinessLogic();

    void enablePinQueryRequested(bool enabled);
    bool launchPinQuery(SIMSecurity::PINType pinType);
    void changePinCode();

public slots:
    void cancelQuery();
    void resendSimLockCode();

signals:
    void showNotification(const QString &text, NotificationType::Type type);
    void showConfirmation(const QString &text, const QString &buttonText);

private: // attributes
    QPointer<PinCodeQueryUI> uiPin;    
    PinCodeQueryDBusAdaptor* dbus;
    QString newPinCode;
    QString oldPinCode;
    QString simLockCode;
    QTimer *launchResponseTimer;

    SubStates subState;
    int previousSimState;
    bool queryLaunch;
    bool initialized;

    enum LaunchState {
        None,
        Requested,
        Ongoing,
        DoneWithError,
        DoneWithOk
    };
//    LaunchState queryState;

    SIM* sim;
    SIMIdentity* simId;
    SIMSecurity* simSec;
    SIMLock *simLock;

private: // methods
    bool handleSIMError(SIMError error);
    bool handleSIMLockError(SIMLockError error);

    void createUi(bool enableBack = false);
    void closeUi();
    void setUiHeader(QString headerText);
    bool stateOperation(int status, int relationState);
    void startLaunch();
    void getCode(bool enableBack, QString header);

    void doEmergencyCall();
    void informTechnicalProblem();
    void simLockRetry();

    //  states
    void ui2SIMLocked(bool showNote = true);
    void ui2PINQuery();
    void ui2PINFailed(int attemptsLeft);

    void ui2PUKQuery();
    void ui2PUKFailed(int attemptsLeft);
    void ui2PUKFailedPermanently();
    void ui2enterNewPin();
    void ui2reenterPIN();    
    void ui2disappear();

private slots:
    void uiCodeChanged();
    void uiButtonReleased();
    void startLaunchResponseTimer();
    void emitLaunchResponse(bool ok);
    void emitLaunchResponse();

    void simStatusChanged(SIM::SIMStatus status);
    void simStatusComplete(SIM::SIMStatus status, SIMError error);
    void simPINCodeVerified(bool success, SIMError error);
    void simPUKCodeVerified(bool success, SIMError error);
    void simPINAttemptsLeft(int attempts, SIMError error);
    void simPUKAttemptsLeft(int attempts, SIMError error);
    void simPINCodeChanged(bool success, SIMError error);
    void simLockUnlockCodeVerified(SIMLockError error);
    void simEnablePINQueryComplete(SIMError error);
    void simPinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error);
};

#endif // PINCODEQUERYBUSINESSLOGIC_H
