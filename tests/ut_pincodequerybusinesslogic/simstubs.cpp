#include "simstubs.h"
#include <QDebug>

/* ------------ SIM ------------------*/
SIMStub::SIMStub(QObject *parent) :
        SIM(parent),
        simStatus(SIM::NotReady),
        error(SIMErrorNone)
{
}

SIMStub::~SIMStub()
{
}

void SIMStub::status()
{
    qDebug() << Q_FUNC_INFO;
    emit statusComplete(simStatus, error);
}

void SIMStub::emitStatusChanged()
{
    emit statusChanged(simStatus);
}

/* ------------ SIM Security ------------------*/
SIMSecurityStub::SIMSecurityStub(QObject *parent) :
        SIMSecurity(parent),
        pin(QString("1234")),
        puk(QString("1234")),
        queryState(Enabled),
        pinAttempts(4),
        pukAttempts(10),
        success(true),
        error(SIMErrorNone)
{
}

SIMSecurityStub::~SIMSecurityStub()
{
}

void SIMSecurityStub::changePIN(PINType type, const QString &oldCode, const QString &newCode)
{
    Q_UNUSED(type);
    if(oldCode != pin){
        emit changePINComplete(false, SIMErrorWrongPassword);
    } else {
        emit changePINComplete(true, SIMErrorNone);
        pin = newCode;
    }
}

void SIMSecurityStub::disablePINQuery(PINType type, const QString &code)
{
    Q_UNUSED(type);
    if(code != pin){
        emit enablePINQueryComplete(SIMErrorWrongPassword);
    } else {
        emit enablePINQueryComplete(SIMErrorNone);
        queryState = SIMSecurity::Disabled;
    }
}

void SIMSecurityStub::enablePINQuery(PINType type, const QString &code)
{
    Q_UNUSED(type);
    if(code != pin){
        emit enablePINQueryComplete(SIMErrorWrongPassword);
    } else {
        emit enablePINQueryComplete(SIMErrorNone);
        queryState = SIMSecurity::Enabled;
    }
}

void SIMSecurityStub::pinQueryState(PINType type)
{
    Q_UNUSED(type);
    emit pinQueryStateComplete(queryState, error);
}

void SIMSecurityStub::verifyPIN(PINType type, const QString &code)
{
    Q_UNUSED(type);
    if(code != pin){
        emit verifyPINComplete(false, SIMErrorWrongPassword);
    } else {
        emit verifyPINComplete(true, SIMErrorNone);
    }
}

void SIMSecurityStub::verifyPUK(PUKType type, const QString &PUKCode, const QString &newPINCode)
{
    Q_UNUSED(type);

    if(PUKCode != puk){
        emit verifyPUKComplete(false, SIMErrorWrongPassword);
    } else {
        emit verifyPUKComplete(true, SIMErrorNone);
        pin = newPINCode;
    }
}

void SIMSecurityStub::pinAttemptsLeft(PINType type)
{
    Q_UNUSED(type);
    emit pinAttemptsLeftComplete(pinAttempts, error);
}

void SIMSecurityStub::pukAttemptsLeft(PUKType type)
{
    Q_UNUSED(type);
    emit pukAttemptsLeftComplete(pukAttempts, error);
}

void SIMSecurityStub::emitVerifyPINRequired(SIMSecurity::PINType type)
{
    emit verifyPINRequired(type);
}

void SIMSecurityStub::emitVerifyPUKRequired(SIMSecurity::PUKType type)
{
    emit verifyPUKRequired(type);
}


/* ------------ SIM Lock ------------------*/
SIMLockStub::SIMLockStub(QObject *parent) :
        SIMLock(parent),
        status(SimLockStatusOk),
        level(SIMLock::LevelGlobal),
        error(SIMLockErrorNone),
        code(QString("1234"))
{
}

SIMLockStub::~SIMLockStub()
{
}

void SIMLockStub::simLockStatus()
{
    emit simLockStatusComplete(status, error);
}

void SIMLockStub::simLockUnlock(SIMLock::SIMLockLevel level, const QString &unlockCode)
{
    Q_UNUSED(level);
    if(unlockCode != code){
        emit simLockUnlockComplete(SIMLockErrorWrongPassword);
    } else {
        emit simLockUnlockComplete(SIMLockErrorNone);
    }
}
