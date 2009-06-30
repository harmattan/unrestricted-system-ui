#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include "pincodequery.h"
#include "notifier.h"
#include "sim.h"

using namespace Cellular;

class BusinessLogic : public QObject
{
    Q_OBJECT
public:
    BusinessLogic(PinCodeQuery *pin, Notifier *notif);
    virtual ~BusinessLogic();

private:
    PinCodeQuery *uiPin;
    Notifier *uiNotif;
    QString newPinCode;

    int previousSimState;

    SIM* sim;
    SIMIdentity* simId;
    SIMSecurity* simSec;
    //SIMPhonebook* simPb;

private:
    void mapSIMError(SIMError error, Notifier::Notification &notification, PinCodeQuery::UIState uiState);

private slots:
    void simStatusChanged(SIM::SIMStatus status);
    void simStatusComplete(SIM::SIMStatus status, SIMError error);
    void PINCodeVerified(bool success, SIMError error);
    void PUKCodeVerified(bool success, SIMError error);
    void codeEntered(PinCodeQuery::UIState uiState, QString code);
    void PINAttemptsLeft(int attempts, SIMError error);
    void PUKAttemptsLeft(int attempts, SIMError error);
    void PINCodeChanged(bool success, SIMError error);


};

#endif // BUSINESSLOGIC_H
