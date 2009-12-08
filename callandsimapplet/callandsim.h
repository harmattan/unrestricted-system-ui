#ifndef CALLANDSIM_H
#define CALLANDSIM_H

#include "dcpcallandsim.h"


#include <cellular-qt/CallWaiting>
#include <cellular-qt/SIM>
//#include <cellular-qt/NetworkRegistration>

#include <QObject>

class QDBusInterface;

using namespace Cellular;

class CallAndSim : public QObject
{
    Q_OBJECT

public:
    CallAndSim(QObject* parent = 0);

signals:
    void callerIdSendingComplete(int);
    void callWaitingComplete(bool);
    void pinRequestComplete(bool);
    void requestFailed(DcpCallAndSim::Data data);

public slots:
    void setCallerIdSending(int value);
    void setCallWaiting(bool enabled);
    void setPinRequest(bool enabled);
    void changePinCode();
    void requestData(DcpCallAndSim::Data data);

private slots:
    void waitingActivateComplete(CallWaiting::WaitingError);
    void waitingCancelComplete(CallWaiting::WaitingError);
    void waitingCheckComplete(bool active, CallWaiting::WaitingError);

    void pinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error = SIMErrorNone);
    void pinQueryEnabled(SIMSecurity::PINQuery queryState);

    void networkStatusChanges(int status);

    void DBusMessagingFailure();

private:
    CallWaiting* callWaiting;
    SIMSecurity* simSecurity;
    //NetworkRegistration *networkRegistration;
    QDBusInterface* dbusPinIf;
};

#endif // CALLANDSIM_H
