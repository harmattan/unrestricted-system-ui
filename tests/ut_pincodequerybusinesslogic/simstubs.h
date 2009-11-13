/*
 * This file overloads sim.h and simlock.h files.
 * This re-implementation contains stub classes as part of system UI unit tests to
 * replace interfaces to CellularQt
 * \sa SIM, SIMLock
 */

#ifndef SIMSTUBS_H
#define SIMSTUBS_H

#include <SIM>
#include <SIMLock>

using namespace Cellular;

//! Provides access to SIM status and status change notification
/*! Note that all methods are asynchronous and have a corresponding completion/result signal. */
class SIMStub : public SIM
{
    Q_OBJECT
public:
    SIMStub(QObject *parent=0);
    ~SIMStub();

    void emitStatusChanged();

public slots:
    //! Request current status/state of SIM
    void status();

signals: // SIGNALS
    //! Emitted when SIM status changes
    void statusChanged(SIMStatus status);

    //! Emitted when status() call completes
    void statusComplete(SIM::SIMStatus status, SIMError error);

public:
    SIM::SIMStatus simStatus;
    SIMError error;
};

//! Provides SIM security related features, such as PIN code verification.
/*! Note that all methods are asynchronous and have a corresponding completion/result signal. */
class SIMSecurityStub: public SIMSecurity
{
    Q_OBJECT
public:
    SIMSecurityStub(QObject *parent=0);
    ~SIMSecurityStub();

    void emitVerifyPINRequired(SIMSecurity::PINType type);
    void emitVerifyPUKRequired(SIMSecurity::PUKType type);

public slots: // METHODS

    //! Changes PIN or PIN2 code
    void changePIN(PINType type, const QString &oldCode, const QString &newCode);

    //! Disables PIN or PIN2 code protection of SIM
    void disablePINQuery(PINType type, const QString &code);

    //! Enables PIN or PIN2 code protection of SIM.
    void enablePINQuery(PINType type, const QString &code);

    //! Requests PIN or PIN2 code protection state
    void pinQueryState(PINType type);

    //! Verifies PIN or PIN2 code.
    /*! Verifying the PIN code opens the SIM,
        verifying PIN2 code opens access to PIN2 protected services, for example
        FDN phonebook.*/
    void verifyPIN(PINType type, const QString &code);

    //! Unblocks PIN or PIN2 code by verifying PUK or PUK2 code respectively, sets the new PIN or PIN2 code
    void verifyPUK(PUKType type, const QString &PUKCode, const QString &newPINCode);

   //! Requests the number of PIN or PIN2 verification attempts left
    void pinAttemptsLeft(PINType type);

   //! Requests the number of PUK or PUK2 verification attempts left
    void pukAttemptsLeft(PUKType type);

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

class SIMLockStub: public SIMLock
{
    Q_OBJECT
public:
    SIMLockStub(QObject *parent=0);
    ~SIMLockStub();

public slots: // METHODS
    //! Request current SIMLock status
    void simLockStatus();

    //! unlocks SIM lock.
    void simLockUnlock(SIMLock::SIMLockLevel level, const QString &unlockCode);

signals: // SIGNALS
    //! Emitted when simLockStatus() call completes
    void simLockStatusComplete(SIMLock::SIMLockStatus status, SIMLockError error);

    //! Emitted when unlock() call completes
    void simLockUnlockComplete(SIMLockError error);

public:
    SIMLockStatus status;
    SIMLockLevel level;
    SIMLockError error;
    QString code;
};

#endif // SIMSTUBS_H
