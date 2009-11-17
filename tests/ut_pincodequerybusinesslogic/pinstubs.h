/*
 * This file overloads sim.h and simlock.h files.
 * This re-implementation contains stub classes as part of system UI unit tests to
 * replace interfaces to CellularQt
 * \sa SIM, SIMLock
 */

#ifndef PINSTUBS_H
#define PINSTUBS_H

#include <QObject>

namespace Cellular {

//! Possible SIM errors
enum SIMError { SIMErrorNone, /*!< Returned when no error happened and operation completed successfully */
                SIMErrorBusCommunication, /*!< Returned when a D-Bus communication error with the Cellular Services daemon occured */
                SIMErrorInvalidParameter,
                SIMErrorModemNotReady,
                SIMErrorServiceBusy,
                SIMErrorModem,
                SIMErrorDataNotAvailable,
                SIMErrorWrongPassword,
                SIMErrorPINDisabled,
                SIMErrorCodeBlocked,
                SIMErrorNoSIM,
                SIMErrorSIMRejected,
                SIMErrorSecurityCodeRequired,
                SIMErrorUnknown
  };

//! Provides access to SIM status and status change notification
/*! Note that all methods are asynchronous and have a corresponding completion/result signal. */
class SIM : public QObject
{
    Q_OBJECT

public:
    //! Possible SIM status list
    enum SIMStatus { UnknownStatus, Ok, NoSIM, PermanentlyBlocked,
        NotReady, PINRequired, PUKRequired, Rejected, SIMLockRejected };

    SIM(QObject *parent=0);
    ~SIM();

    void emitStatusChanged();

public slots:
    //! Request current status/state of SIM
    void status();

signals: // SIGNALS
    //! Emitted when SIM status changes
    void statusChanged(SIM::SIMStatus status);

    //! Emitted when status() call completes
    void statusComplete(SIM::SIMStatus status, SIMError error);

public:
    SIM::SIMStatus simStatus;
    SIMError error;
};

//! Provides SIM security related features, such as PIN code verification.
/*! Note that all methods are asynchronous and have a corresponding completion/result signal. */
class SIMSecurity: public QObject
{
    Q_OBJECT

public:
    //! Possible PIN code types
    enum PINType { PIN, PIN2 };
    //! Possible PUK code types
    enum PUKType { PUK, PUK2 };
    //! PIN or PIN2 query state: enabled, disabled or unknown
    enum PINQuery { Enabled, Disabled, UnknownState };

public:

public:
    SIMSecurity(QObject *parent=0);
    ~SIMSecurity();

    void emitVerifyPINRequired(SIMSecurity::PINType type);
    void emitVerifyPUKRequired(SIMSecurity::PUKType type);

public slots: // METHODS

    //! Changes PIN or PIN2 code
    void changePIN(SIMSecurity::PINType type, const QString &oldCode, const QString &newCode);

    //! Disables PIN or PIN2 code protection of SIM
    void disablePINQuery(SIMSecurity::PINType type, const QString &code);

    //! Enables PIN or PIN2 code protection of SIM.
    void enablePINQuery(SIMSecurity::PINType type, const QString &code);

    //! Requests PIN or PIN2 code protection state
    void pinQueryState(SIMSecurity::PINType type);

    //! Verifies PIN or PIN2 code.
    /*! Verifying the PIN code opens the SIM,
        verifying PIN2 code opens access to PIN2 protected services, for example
        FDN phonebook.*/
    void verifyPIN(SIMSecurity::PINType type, const QString &code);

    //! Unblocks PIN or PIN2 code by verifying PUK or PUK2 code respectively, sets the new PIN or PIN2 code
    void verifyPUK(SIMSecurity::PUKType type, const QString &PUKCode, const QString &newPINCode);

   //! Requests the number of PIN or PIN2 verification attempts left
    void pinAttemptsLeft(SIMSecurity::PINType type);

   //! Requests the number of PUK or PUK2 verification attempts left
    void pukAttemptsLeft(SIMSecurity::PUKType type);

signals: // SIGNALS
    //! Emitted when verification of PIN or PIN2 code is needed.
    void verifyPINRequired(SIMSecurity::PINType type);

    //! Emitted when verification of PUK or PUK2 code is needed.
    void verifyPUKRequired(SIMSecurity::PUKType type);

    //! Emitted when changePIN() call completes
    void changePINComplete(bool success, SIMError error);

    //! Emitted when disablePINQuery() call completes
    void disablePINQueryComplete(SIMError error);

    //! Emitted when enablePINQuery() call completes
    void enablePINQueryComplete(SIMError error);

    //! Emitted when pinQueryState() call completes
    void pinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error);

    //! Emitted when verifyPIN() call completes
    void verifyPINComplete(bool success, SIMError error);

    //! Emitted when verifyPUK() call completes
    void verifyPUKComplete(bool success, SIMError error);

    //! Emitted when pinAttemptsLeft() call completes
    /*! \param attempts number of PIN or PIN2 verification attempts left, -1 if unknown. */
    void pinAttemptsLeftComplete(int attempts, SIMError error);

    //! Emitted when pukAttemptsLeft() call completes
    /*! \param attempts number of PUK or PUK2 verification attempts left, -1 if unknown. */
    void pukAttemptsLeftComplete(int attempts, SIMError error);

public:
    QString pin;
    QString puk;
    PINQuery queryState;
    int pinAttempts;
    int pukAttempts;
    bool success;
    SIMError error;
};

//! Possible SIM errors
enum SIMLockError { SIMLockErrorNone, /*!< Returned when no error happened and operation completed successfully */
                    SIMLockErrorBusCommunication, /*!< Returned when a D-Bus communication error with the Cellular Services daemon occured */
                    SIMLockErrorInvalidParameter, /*!< Invalid parameter values */
                    SIMLockErrorModemNotReady, /*!< Cellular modem not ready */
                    SIMLockErrorWrongPassword, /*!< Incorrect unlock code */
                    SIMLockErrorAlreadyOpened, /*!< SIMlock already opened */
                    SIMLockErrorWrongLevel, /*!< Requested level not found */
                    SIMLockErrorNotAllowed, /*!< Action not allowed. E.g. the lock does not exist */
                    SIMLockErrorTimerNotElapsed, /*!< Unlocking timer not elapsed (too fast retry) */
                    SIMLockErrorUnknown /*!< General SIM lock unlocking failure */
  };

//! Provides SIMLock status and unlocking
/*! Note that all methods are asynchronous and have a corresponding completion/result signal. */
class SIMLock: public QObject
{
    Q_OBJECT

public:
    enum SIMLockStatus { SimLockStatusUnknown, /*!< status not available */
                         SimLockStatusOk, /*!< SIMLock accepts current (U)SIM card */
                         SimLockStatusRestricted, /*!< SIMLock rejects the (U)SIM card */
                         SimLockStatusRestrictionPending, /*!< Not all autolocks could clos on the current (U)SIM card. SIMLock rejects the card */
                         SimLockStatusRestrictionOn /*!< SIMLock autolocked to current (U)SIM card */
    };

    //! Level of SIM lock to be opened
    enum SIMLockLevel { LevelMccMnc=1, LevelGid1, LevelGid2, LevelImsi,
                        LevelMccMncGid1, LevelMccMncGid1Gid2, LevelGlobal};

public:
    SIMLock(QObject *parent=0);
    ~SIMLock();

public slots: // METHODS
    //! Request current SIMLock status
    void simLockStatus();

    //! unlocks SIM lock.
    void simLockUnlock(SIMLock::SIMLockLevel level, const QString &code);

signals: // SIGNALS
    //! Emitted when simLockStatus() call completes
    void simLockStatusComplete(SIMLock::SIMLockStatus status, SIMLockError error);

    //! Emitted when unlock() call completes
    void simLockUnlockComplete(SIMLockError error);

public:
    SIMLock::SIMLockStatus status;
    SIMLockLevel level;
    SIMLockError error;
    QString code;
};

} // namespace

//////////////////

using namespace Cellular;

class PinCodeQueryDBusAdaptor : public QObject
{
    Q_OBJECT

public:
    PinCodeQueryDBusAdaptor(QObject* parent);
    virtual ~PinCodeQueryDBusAdaptor();

    //! Response to \sa PinQueryState(bool enabled)
    void pinQueryStateCompletedResponse(SIMSecurity::PINQuery state, SIMError error);

    //! Response to \sa EnablePinQueryRequested(bool enabled)
    void pinQueryEnabledResponse(SIMSecurity::PINQuery state);

        //! Response to \sa LaunchPinQueryRequested(SIMSecurity::PINType pinType)
    void pinQueryDoneResponse(bool queryOk);

public:
    SIMSecurity::PINQuery state;
    SIMError error;
    bool queryDoneOk;
};


#endif // PINSTUBS_H
