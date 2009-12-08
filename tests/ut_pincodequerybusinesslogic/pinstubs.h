/*
 * This file overloads sim.h and simlock.h files.
 * This re-implementation contains stub classes as part of system UI unit tests to
 * replace interfaces to CellularQt
 * \sa SIM, SIMLock
 */

#ifndef PINSTUBS_H
#define PINSTUBS_H

#include <QObject>
#include <QStringList>

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

class EmergencyNumbers
{
public:
    inline QStringList numbers(){QStringList l; l << QString("911") << QString("112"); return l;}
};

} // namespace Cellular

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

/*****************************************************************/
/*    CallUi */
/*****************************************************************/
namespace CallUi
{
enum CallDirection {
    Outgoing, //!< Outgoing call.
    Incoming  //!< Incoming call.
};

enum EndReason {
    UserEnded,   //!< The call was ended locally.
    RemoteEnded, //!< The call was ended by the remote end.
    Error,       //!< The call was ended bacause of an error.
    Unknown      //!< The call was ended but we failed to collect the reason.
};

class PendingCallRequest : public QObject
{
    Q_OBJECT

public: // API
    PendingCallRequest(QObject *parent);
    virtual ~PendingCallRequest();
    inline QString callId() const {return m_callId;}
    inline bool isFinished() const {return m_finished;}
    inline bool isValid() const {return m_valid;}
    inline bool isError() const {return m_error;}
    inline QString errorName() const {return m_eName;}
    inline QString errorMessage() const {return m_eMsg;}

Q_SIGNALS:
    void finished(CallUi::PendingCallRequest *request);

public:
    inline void emitFinished() {emit finished(this);}

public:
    QString m_callId;
    bool m_finished;
    bool m_valid;
    bool m_error;
    QString m_eName;
    QString m_eMsg;
};


class CallUiServiceApi : public QObject
{
    Q_OBJECT

public:
    CallUiServiceApi(QObject *parent=NULL);
    virtual ~CallUiServiceApi();

public Q_SLOTS:
    bool Call(const QString &accountPath, const QString &contactId) const;

public Q_SLOTS:
    CallUi::PendingCallRequest * RequestCall( const QString &accountPath,
                                              const QString &contactId,
                                              bool requestVideo=false );

    CallUi::PendingCallRequest * RequestCellularCall(
            const QString &phoneNumber );

    CallUi::PendingCallRequest * RequestEmergencyCall();

Q_SIGNALS:
    void NewCall(QString uid, int direction, QString contactId) const;
    void CallEnded(QString uid, int reason, int duration, QString message) const;

public:
    void emitNewCall(QString uid, int direction, QString contactId);
    void emitCallEnded(QString uid, int reason, int duration, QString message);

public:
    PendingCallRequest *m_request;
    QString m_uid;
    int m_direction;
    QString m_contactId;
    int m_reason;
    int m_duration;
    QString m_msg;

    // input
    bool m_callStart;
    QString m_number;
};

} // namespace CallUi

#endif // PINSTUBS_H
